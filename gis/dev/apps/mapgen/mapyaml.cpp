#include "mapyaml.h"
#include <fstream>
#include "utlstring.h"
#include "geolayer.h"
#include "geoaerial.h"
#include "geowords.h"
#include "utlqt.h"

//============================================================================
//============================================================================
MapYaml::MapYaml()
{
}

//============================================================================
//============================================================================
void MapYaml::initSearchPath(const std::string &yamlfile)
{
    std::string path =  UtlString::GetPath(yamlfile.c_str());
    if (path.size())
    _searchPath.push_back(path);
}

//============================================================================
//============================================================================
bool MapYaml::load(const std::string &text, GisSys *pgis)
{
    const char *func = "MapYaml::load() - ";

    YAML::Node doc;
    try
    {
        clear();
        doc = YAML::Load(text);
    }
    catch (...)//catch (YAML::ParserException &e)
    {
        LogError("%s Unexpected Exception loading yaml string: %s", func, text.c_str());
        return false;
    }

    if (!load(doc, pgis))
    {
        LogError("%s Unexpected Exception reading yaml string: %s", func, text.c_str());
        return false;
    }

    return true;
}

//============================================================================
//============================================================================
bool MapYaml::loadFile(const char *file, GisSys *pgis)
{
    const char *func = "MapYaml::loadFile() - ";

    YAML::Node doc;

    try
    {
        clear();
        // check if it is a file to avoid yaml ifstream exception that is causing issues
        std::string yamlpath;
        if (!fileExists(file, &yamlpath))
        {
            LogError("%s file doesn't exist: %s", func, file);
            return false;
        }

        _yamlfile = yamlpath;
        initSearchPath(yamlpath);


        doc = YAML::LoadFile(yamlpath);
    }
    catch (...)//catch (YAML::ParserException &e)
    {
        LogError("%s Unexpected Exception loading yaml file: %s", func, file);
        return false;
    }

    if (!load(doc, pgis))
    {
        LogError("%s Unexpected Exception reading yaml file: %s", func, file);
        return false;
    }


    return true;
}

//============================================================================
//============================================================================
bool MapYaml::load(YAML::Node &doc, GisSys *pgis)
{
    const char *func = "MapYaml::load() - ";

    // check if it is a file to avoid yaml ifstream exception that is causing issues

    try
    {
        // load output
        if (doc["output"])
        {
           loadOutput(doc["output"]);
        }

        // load colorramps (data objects depend on them)
        if (doc["colorramps"])
        {
            loadColorRamps(doc["colorramps"]);
        }

        // styles need dataobjects for masks
        // load dataobjs
        if(doc["dataobjs"])
        {
            loadDataObjs(doc["dataobjs"]);
        }


        // load styles
        if (doc["styles"])
        {
            loadStyles(doc["styles"]);
        }

        // load compute objs
        if(doc["computeobjs"])
        {
            loadComputeObjs(doc["computeobjs"]);
        }

        // load mapobjs
        if(doc["mapobjs"])
        {
            loadMapObjs(doc["mapobjs"]);
        }
    }
    catch (...)//catch (YAML::ParserException &e)
    {
        LogError("%s Unexpected Exception parsing yaml", func);
        return false;
    }


    // init
    pgis->insert(&_mapObjs);
    initCompute(pgis);
    initAerial(pgis);
    pgis->setConfig(_cfg);
    pgis->initLabels();
    pgis->zoomExtents();


    return true;
}

//============================================================================
//============================================================================
void MapYaml::initAerial(GisSys *pgis)
{
    box3d bbox = pgis->computeBox();
    Extents ext(bbox.vmin.x, bbox.vmax.y, bbox.vmax.x, bbox.vmin.y);

    // find an load any aerial
    std::map<std::string, PGlObj>::iterator it = _dataObjMap.begin();
    while(it != _dataObjMap.end())
    {
        GlObj *pobj = it->second.get();
        it++;

        GeoAerial *aerial = dynamic_cast<GeoAerial*>(pobj);
        if (aerial)
        {
            ext.expand(aerial->_extExtra);
            if (!pgis->loadAerial(aerial, ext))
            {
                LogError("%s failed to load aerial");
            }
        }
    }
}

//============================================================================
//============================================================================
void MapYaml::initCompute(GisSys *pgis)
{
    // find an load any aerial
    std::map<std::string, PGeoCompute>::iterator it = _computeObjMap.begin();
    while(it != _computeObjMap.end())
    {
        PGeoCompute pobj = it->second;
        it++;

        pobj->compute();
    }

}

//============================================================================
//============================================================================
void MapYaml::loadOutput(const YAML::Node& node)
{
    _cfg.reset(new Config());

    _cfg->mode( getString(node, "mode", _cfg->mode().c_str()) );
    _cfg->width( getInt(node, "width", _cfg->width()) );
    _cfg->height( getInt(node, "height", _cfg->height()) );
    _cfg->imgFile( getString(node, "file", _cfg->imgFile()) );
    _cfg->outType( getString(node, "outtype", _cfg->outType()) );
    _cfg->dataFile( getString(node, "datafile", _cfg->dataFile()) );

    _cfg->imgFile( validateOutfile(_cfg->imgFile()) );
    /*
    std::string path = UtlString::GetPath(_cfg->mapfile.c_str(), false);
    if (!path.size())
    {
        path = UtlString::GetPath(_yamlfile.c_str(), true);
        _cfg->mapfile = path + _cfg->mapfile;
    }
    */
}

//============================================================================
//============================================================================
void MapYaml::loadStyles(const YAML::Node& node)
{
    for (std::size_t i=0;i<node.size();i++)
    {
        PDrawAttr style = loadStyle(node[i]);
        if (style) _styleMap[style->_name] = style;
    }
}

//============================================================================
//============================================================================
PDrawAttr MapYaml::loadStyle(const YAML::Node& node)
{
    PDrawAttr attr(new DrawAttr());

    attr->_name = getString(node, "name", "");

    if (node["polymask"])
    {
        const YAML::Node& an = node["polymask"];

        attr->_drawPolyMasked = getBool(an, "draw");
        std::string maskstr  = getString(an, "masks");

        std::vector<std::string> vmasks;
        UtlString::explode(maskstr, std::string(","), &vmasks);

        for (unsigned int i=0; i<vmasks.size(); i++)
        {
            PGlObj mask = getDataObj(vmasks[i]);
            if (mask)
            {
                attr->_maskPolyList.push_back(mask);
            }
        }
    }

    if (node["polyfill"])
    {
        const YAML::Node& an = node["polyfill"];

        attr->_drawPolyFill = getBool(an, "draw");
        attr->_colorPolyFill = getColorRgbf(an, "color");
    }

    if (node["polyoutline"])
    {
        const YAML::Node& an = node["polyoutline"];

        attr->_drawPolyOutline = getBool(an, "draw");
        attr->_colorPolyOutline = getColorRgbf(an, "color");

        // if (an["linewidth"])
    }

    if (node["labels"])
    {
        const YAML::Node& an = node["labels"];

        attr->_drawLabels = getBool(an, "draw");
        attr->_colorLabels = getColorRgbf(an, "color");
        attr->_feature =  getString(an, "feature");

        // TODO: figure out how to configure fonts folder, and search path
        int fontsize = getInt(an, "fontsize", 36);
        attr->_font.initFontTexture("C:/Windows/Fonts/ARIAL.TTF", fontsize);
    }

    return attr;
}


//============================================================================
//============================================================================
void MapYaml::loadColorRamps(const YAML::Node& node)
{
    for (std::size_t i=0;i<node.size();i++)
    {
        PColorRamp ramp = loadColorRamp(node[i]);
        if (ramp) _colorRampMap[ramp->name] = ramp;
    }
}

//============================================================================
//============================================================================
MapYaml::PColorRamp MapYaml::loadColorRamp(const YAML::Node& node)
{
    PColorRamp ramp(new ColorRamp());

    ramp->name = getString(node, "name", "");

    std::string min, mid, max;
    if (node["min"]) min = node["min"].as<std::string>();
    if (node["mid"]) mid = node["mid"].as<std::string>();
    if (node["max"]) max = node["max"].as<std::string>();

    if (min.size() && mid.size() && max.size())
    {
        min = "#" + min;
        mid = "#" + mid;
        max = "#" + max;
        ramp->picker.reset(new GradientPicker(QColor(min.c_str()), QColor(mid.c_str()), QColor(max.c_str())));
        return ramp;
    }
    else if (min.size() && max.size())
    {
        min = "#" + min;
        max = "#" + max;
        ramp->picker.reset(new GradientPicker(QColor(min.c_str()), QColor(max.c_str())));
        return ramp;
    }

    return PColorRamp();

}

//============================================================================
//============================================================================
void MapYaml::loadDataObjs(const YAML::Node& node)
{
    for (std::size_t i=0;i<node.size();i++)
    {
        PGlObj obj = loadDataObj(node[i]);
        if (obj) _dataObjMap[obj->_name] = obj;
    }
}

//============================================================================
//============================================================================
PGlObj MapYaml::loadDataObj(const YAML::Node& node)
{
    const char *func = "MapYaml::loadDataObj( - )";

    std::string type = getString(node, "type");   
    std::string name = getString(node, "name");

    if (type == "geojson")
    {
        std::string file;
        if (!getFile(node, &file))
        {
            LogError("%s - unable to load json for %s", func, name.c_str());
            return PGlObj();
        }

        GeoJson json;
        if (!json.loadFile(file.c_str()))
        {
            LogError("%s failed to load json file: %s, for dataobj: %s", func, file.c_str(), name.c_str());
            return PGlObj();
        }

        PGlObj polys(new GeoObj());
        json.getGeometry(polys);
        polys->_name = name;
        return polys;
    }
    else if (type == "tiff")
    {
        std::string file;
        if (!getFile(node, &file))
        {
            LogError("%s - unable to load tiff for dataobj %s", func, name.c_str());
            return PGlObj();
        }

        std::string color =  getString(node, "colorramp");
        PColorRamp ramp = getColorRamp(color);
        PGradientPicker picker;
        if (!ramp)
        {
            LogError("%s - unable to find color ramp %s", func, color.c_str());


            picker.reset(new GradientPicker(QColor("#55aa00"), QColor("#ffff00"), QColor("#ff0000")));
        }
        else
        {
            picker = ramp->picker;
        }


        PGlObj tiff = GisSys::loadTiff(file.c_str(),  picker.get());
        if (!tiff)
        {
            LogError("%s Failed to load tiff: %s", func, name.c_str());
        }
        else
        {
            tiff->_name = name;
        }

        return tiff;
    }
    else if (type == "aerial")
    {
        GeoAerial *aerial = new GeoAerial();
        PGlObj obj(aerial);

        aerial->_name = name;

        if (node["extents"])
        {
            const YAML::Node& e = node["extents"];

            aerial->_extExtra = getDbl(e, "extra", 0.0);
        }

        return obj;
    }
    else if (type == "wordpos")
    {
        GeoWords *words = new GeoWords();
        PGlObj obj(words);

        words->_name = name;

        // get bounding box mode, and data need to find bounding box of map in pdf
        std:string computebox = UtlString::toLower(getString(node, "computebox", "auto"));
        if (computebox == "lrt")
        {
            words->setComputeBoxMode(GeoWords::BoxCompute_LRT);
            words->setWordLeft(getString(node, "wordl"), getString(node, "wordl_orient", "h"));
            words->setWordRight(getString(node, "wordr"), getString(node, "wordr_orient", "h"));
            words->setWordTop(getString(node, "wordt"), getString(node, "wordt_orient", "h"));
        }
        else if (computebox == "lrb")
        {
            words->setComputeBoxMode(GeoWords::BoxCompute_LRB);
            words->setWordLeft(getString(node, "wordl"), getString(node, "wordl_orient", "h"));
            words->setWordRight(getString(node, "wordr"), getString(node, "wordr_orient", "h"));
            words->setWordBtm(getString(node, "wordb"), getString(node, "wordb_orient", "h"));
        }
        else if (computebox == "tlbr")
        {
            words->setComputeBoxMode(GeoWords::BoxCompute_TLBR);
            words->setWordTopLeft(getString(node, "wordtl"), getString(node, "wordtl_orient", "h"));
            words->setWordBtmRight(getString(node, "wordbr"), getString(node, "wordbr_orient", "h"));
        }
        else
        {
            words->setComputeBoxMode(GeoWords::BoxCompute_Auto);
        }

        std::string file;
        if (!getFile(node, &file))
        {
            LogError("%s - unable to find words file for %s", func, name.c_str());
            return PGlObj();
        }

        words->load(file.c_str());
        return obj;
    }

    return PGlObj();
}

//============================================================================
//============================================================================
void MapYaml::loadMapObjs(const YAML::Node& node)
{
    for (std::size_t i=0;i<node.size();i++)
    {
        PGlObj obj = loadMapObj(node[i]);
        if (obj) _mapObjs.push_back(obj);
    }
}

//============================================================================
//============================================================================
PGlObj MapYaml::loadMapObj(const YAML::Node& node)
{
    const char *func = "MapYaml::loadMapObj( - )";

    std::string type = getString(node, "type");

    if (type == "layer")
    {
        return loadLayer(node);
    }

    return PGlObj();
}

//============================================================================
//============================================================================
PGlObj MapYaml::loadLayer(const YAML::Node& node)
{
    const char *func = "MapYaml::loadLayer() - ";

    GeoLayer *plyr = new GeoLayer();
    PGlObj lyr(plyr);

    plyr->_name = getString(node, "name");

    std::string style = getString(node, "style");
    if (style.size())
    {
        PDrawAttr s = getStyle(style);
        if (!s)
        {
            LogError("%s Failed to find style: %s", func, style.c_str());
        }
        else
        {
            plyr->setDrawAttr(s);
        }
    }

    if (node["children"])
    {
        loadLayerChildren(node["children"], lyr.get());
    }

    return lyr;
}

//============================================================================
//============================================================================
void MapYaml::loadLayerChildren(const YAML::Node& node, GlObj *lyr)
{
    for (std::size_t i=0;i<node.size();i++)
    {
        PGlObj obj = loadLayerChild(node[i]);
        if (obj) lyr->insertBack(obj);
    }
}

//============================================================================
//============================================================================
PGlObj MapYaml::loadLayerChild(const YAML::Node& node)
{
    const char *func = "MapYaml::loadLayerChild() - ";

    // load data objects
    std::string str = getString(node, "dataobj");
    if (str.size() > 0)
    {
        PGlObj dataobj = getDataObj(str);

        if (!dataobj)
        {
            LogError("%s Failed to find dataobj: %s", func, str.c_str());
        }

        return dataobj;
    }

    // load compute objects
    str = getString(node, "computeobj");
    if (str.size() > 0)
    {
        PGlObj dataobj = getComputeObj(str);

        if (!dataobj)
        {
            LogError("%s Failed to find computeobj: %s", func, str.c_str());
        }

        return dataobj;
    }

    return PGlObj();
}

//============================================================================
//============================================================================
void MapYaml::loadComputeObjs(const YAML::Node& node)
{
    for (std::size_t i=0;i<node.size();i++)
    {
        PGeoCompute obj = loadComputeObj(node[i]);
        if (obj) _computeObjMap[obj->getName()] = obj;
    }
}

//============================================================================
//============================================================================
PGeoCompute MapYaml::loadComputeObj(const YAML::Node& node)
{
    PGeoCompute compute(new GeoCompute());

    std::string type = getString(node, "type");
    std::string name = getString(node, "name");
    std::string output = getString(node, "output");
    std::string feature = getString(node, "feature");
    output = validateOutfile(output);

    compute->setType(type);
    compute->setName(name);
    compute->setOutput(output);
    compute->setFeature(feature);



    if (node["children"])
    {
        loadLayerChildren(node["children"], compute.get());
    }

    return compute;
}

//============================================================================
//============================================================================
std::string MapYaml::getString(const YAML::Node& node, const char *name, std::string def)
{
    if (!node[name]) return def;

    return node[name].as<std::string>();
}

//============================================================================
//============================================================================
double MapYaml::getDbl(const YAML::Node& node, const char *name, double def)
{
    if (!node[name]) return def;

    return node[name].as<double>();
}

//============================================================================
//============================================================================
float MapYaml::getDbl(const YAML::Node& node, const char *name, float def)
{
    if (!node[name]) return def;

    return node[name].as<float>();
}

//============================================================================
//============================================================================
int MapYaml::getInt(const YAML::Node& node, const char *name, int def)
{
    if (!node[name]) return def;

    return node[name].as<int>();
}
//============================================================================
//============================================================================
bool MapYaml::getBool(const YAML::Node& node, const char *name, bool def)
{
    if (!node[name]) return def;


    bool b = false;
    std::string str = node[name].as<std::string>();
    if (str == "true") b = true;
    return b;
}

//============================================================================
//============================================================================
QColor MapYaml::getColor(const YAML::Node& node, const char *name)
{
    std::string str = "#000000";
    if (node[name])
    {
       str = node[name].as<std::string>();
       if (str.size()) str = "#" + str;
       else str = "#000000";
    }

    QColor c(str.c_str());
    return c;
}

//============================================================================
//============================================================================
Rgbf MapYaml::getColorRgbf(const YAML::Node& node, const char *name)
{
    QColor c = getColor(node, name);

    Rgbf rgb(c.redF()/255.0f, c.greenF()/255.0f, c.blueF()/255.0f, 1.0f);
    return rgb;
}

//============================================================================
//============================================================================
PGlObj MapYaml::getDataObj(const std::string &name)
{
    std::map<std::string, PGlObj>::iterator it = _dataObjMap.find(name);
    if (it == _dataObjMap.end())
    {
        return PGlObj();
    }

    return it->second;
}

//============================================================================
//============================================================================
PGlObj MapYaml::getComputeObj(const std::string &name)
{
    std::map<std::string, PGeoCompute>::iterator it = _computeObjMap.find(name);
    if (it == _computeObjMap.end())
    {
        return PGlObj();
    }

    PGlObj obj = std::tr1::dynamic_pointer_cast<GlObj>(it->second);
    return obj;
}

//============================================================================
//============================================================================
MapYaml::PColorRamp MapYaml::getColorRamp(const std::string &name)
{
    std::map<std::string, PColorRamp>::iterator it = _colorRampMap.find(name);
    if (it == _colorRampMap.end())
    {
        return PColorRamp();
    }

    return it->second;
}

//============================================================================
//============================================================================
PDrawAttr MapYaml::getStyle(const std::string &name)
{
    std::map<std::string, PDrawAttr>::iterator it = _styleMap.find(name);
    if (it == _styleMap.end())
    {
        return PDrawAttr();
    }

    return it->second;
}

//============================================================================
//============================================================================
std::string MapYaml::validateOutfile(const std::string &file)
{
    std::string path = UtlString::GetPath(file.c_str(), false);
    if (path.size()) return file;

    path = UtlString::GetPath(_yamlfile.c_str(), true);
    path = path + file;
    return path;
}

//============================================================================
//============================================================================
bool MapYaml::getFile(const YAML::Node& node, std::string *file)
{
    std::string f = getString(node, "file");
    std::string path;
    return fileExists(f.c_str(), file);
}

//============================================================================
//============================================================================
bool MapYaml::fileExists(const char *path, std::string *pathresult)
{
    const char *func = "MapYaml::fileExists() - ";

    // TODO: could search config folders

    if (!UtlQt::fileExists(path))
    {
        for (unsigned int i=0; i<_searchPath.size(); i++)
        {
            std::string pathcheck = _searchPath[i] + path;
            if (UtlQt::fileExists(pathcheck.c_str()))
            {
                *pathresult = pathcheck;
                return true;
            }
        }

        LogError("%s file doesn't exist: %s", func, path);
        return false;
    }

    *pathresult = path;
    return true;
}



//============================================================================
//============================================================================
void MapYaml::clear()
{
    _searchPath.clear();
    _cfg.reset(new Config());
    _styleMap.clear();
    _colorRampMap.clear();
    _dataObjMap.clear();
    _mapObjs.clear();
}
