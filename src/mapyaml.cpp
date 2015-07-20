#include "mapyaml.h"
#include <fstream>
#include <iostream>
#include "utlstring.h"
#include "geolayer.h"
#include "geoaerial.h"
#include "geoimgraster.h"
#include "geotext.h"
#include "geowords.h"
#include "stats.h"
#include "utlqt.h"
#include "platform.h"

//============================================================================
//============================================================================
MapYaml::MapYaml()
{
    Platform::getFontSearchPaths(&_searchPathFonts);
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
        //std::cout << doc << "\n";
        // debug
        /*
        for (YAML::iterator it = doc.begin(); it != doc.end(); ++it) 
        {
            std::string scalar = it->as<std::string>();
            LogTrace("Found scalar: %s", scalar.c_str());
        }
        */
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
        else
        {
            _cfg.reset(new Config());
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

        if (doc["legends"])
        {
            loadLegends(doc["legends"]);
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
    initLegends(pgis);
    initCompute(pgis);
    initAerial(pgis);
    pgis->setConfig(_cfg);
    pgis->initObjs();

    if (_cfg->mapExentsValid())
    {
        LogTrace("%s Map extents set to %s", func, _cfg->mapExtents().asStr().c_str());
        pgis->zoomExtents(_cfg->mapExtents());
    }
    else
    {
        LogTrace("%s Map extents not set or not valid, so zooming to data extents.", func);
        pgis->zoomExtents();
    }


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
void MapYaml::initLegends(GisSys *pgis)
{
    const char *func = "MapYaml::initLegends() -";

    // find an load any aerial
    std::map<std::string, PLegend>::iterator it = _legendObjMap.begin();
    while (it != _legendObjMap.end())
    {
        PLegend leg = it->second;
        it++;

        if (!leg->render())
        {
            LogError("%s Failed to render legend %s", func, leg->_name.c_str());
        }
    }
}

//============================================================================
//============================================================================
void MapYaml::loadOutput(const YAML::Node& node)
{
    _cfg.reset(new Config());

    _cfg->mode(getString(node, "mode", _cfg->mode().c_str()));
    _cfg->width(getInt(node, "width", _cfg->width()));
    _cfg->height(getInt(node, "height", _cfg->height()));
    _cfg->imgFile(getString(node, "file", _cfg->imgFile()));
    _cfg->outType(getString(node, "outtype", _cfg->outType()));
    _cfg->dataFile(getString(node, "datafile", _cfg->dataFile()));
    _cfg->lyrOutMode(getBool(node, "layeroutmode", _cfg->lyrOutMode()));
    _cfg->colrClear(getColorRgbf(node, "bgcolor", _cfg->colrClear()));
    _cfg->mapExtents(getExtents(node, "extents"));

    _cfg->imgFile(validateOutfile(_cfg->imgFile()));
}

//============================================================================
//============================================================================
void MapYaml::loadStyles(const YAML::Node& node)
{
    for (std::size_t i = 0; i < node.size(); i++)
    {
        PDrawAttr style = loadStyle(node[i]);
        if (style) _styleMap[style->_name] = style;
    }
}

//============================================================================
//============================================================================
PDrawAttr MapYaml::loadStyle(const YAML::Node& node)
{
    const char *func = "MapYaml::loadStyle() -";

    PDrawAttr attr(new DrawAttr());

    attr->_name = getString(node, "name", "");

    if (node["polymask"])
    {
        const YAML::Node& an = node["polymask"];

        attr->_drawPolyMasked = getBool(an, "draw", true);
        std::string maskstr = getString(an, "masks", "");

        std::vector<std::string> vmasks;
        UtlString::explode(maskstr, std::string(","), &vmasks);

        for (unsigned int i = 0; i < vmasks.size(); i++)
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

        attr->_drawPolyFill = getBool(an, "draw", true);
        attr->_colorPolyFill = getColorRgbf(an, "color");
        attr->_colorRandPolyFill = loadColorRand(an);
        attr->_colorByFeaturePolyFill = getBool(an, "colorbyfeature", false);
        std::string feature = getString(an, "feature");
        if (feature.size() > 0) attr->_feature = feature;
    }

    if (node["polyoutline"])
    {
        const YAML::Node& an = node["polyoutline"];

        attr->_drawPolyOutline = getBool(an, "draw", true);
        attr->_colorPolyOutline = getColorRgbf(an, "color");
        attr->_colorRandPolyOutline = loadColorRand(an);
        attr->_lineWidth = getDbl(an, "linewidth", 1.0f);
        std::string feature = getString(an, "feature");
        if (feature.size() > 0) attr->_feature = feature;

        // if (an["linewidth"])
    }

    if (node["labels"])
    {
        const YAML::Node& an = node["labels"];

        attr->_drawLabels = getBool(an, "draw", true);
        attr->_colorLabels = getColorRgbf(an, "color");
        attr->_colorRandLabels = loadColorRand(an);
        std::string feature = getString(an, "feature");
        if (feature.size() > 0) attr->_feature = feature;

        int fontsize = 36;
        std::string fontfile;
        bool haveFont = true;
        if (!loadFont(an, &fontfile, &fontsize))
        {
            if (!findFont("Arial", &fontfile))
            {
                haveFont = false;
            }
        }
        
        if (haveFont)
        {
            attr->_font.initFontTexture(fontfile.c_str(), fontsize);
        }
    }

    return attr;
}

//============================================================================
//============================================================================
bool MapYaml::loadFont(const YAML::Node& node, std::string *fontFile, int *fontSize)
{
    const char *func = "MapYaml::loadFont() -";

    *fontSize = getInt(node, "fontsize", 36);
    std::string fontname = getString(node, "font", "ARIAL.TTF");
    return findFont(fontname.c_str(), fontFile);
}

//============================================================================
//============================================================================
bool MapYaml::findFont(const char *fontName, std::string *fontFile)
{
    const char *func = "MapYaml::findFont() -";

    if (UtlQt::findFile(fontName, _searchPathFonts, fontFile))
    {
        return true;
    }

    LogError("%s Error: font file not found! File: %s", func, fontName);

    std::string err = "  Looked in: ";
    for (int i = 0; i < _searchPathFonts.size(); i++)
    {
        err += "    ";
        err += _searchPathFonts[i];
        err += "\n";
    }
    LogError("%s", err.c_str());

    return false;
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
PColorRand MapYaml::loadColorRand(const YAML::Node& node, const char *name)
{
    if (!node[name]) return PColorRand();

    double h = getDbl(node[name], "h", -1.0);
    double s = getDbl(node[name], "s", .99);
    double v = getDbl(node[name], "v", .99);

    PColorRand c;
    if (h == -1)
    {
        c.reset(new ColorRand(s, v));
    }
    else
    {
        c.reset(new ColorRand(h, s, v));
    }

    return c;
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

        GeoJson::EpsgTrans trans;
        trans.epsgFr = getInt(node, "epsgfr", 0);
        trans.epsgTo = getInt(node, "epsgTo", 0);
        if (trans.epsgFr != 0 && trans.epsgTo == 0)
        {
            if (trans.epsgFr == 3857)
            {
                trans.epsgFr = 0; // 3857 is the default we want, so set it to 0 because no transform is needed
            }
            else
            {
                // the input is not 3857 but no to specified so set it to our default of 3857
                trans.epsgTo = 3857;
            }
        }

        json.setTransform(trans);

        PGlObj polys(new GeoObj());

        // need a layer name, or layer num, default to the first layer which is 0 if neither setting is found.
        std::string lyrname = getString(node, "lyrname");
        if (lyrname.size() > 0)
        {
            json.getGeometry(lyrname.c_str(), polys);
        }
        else
        {
            int lyrnum = getInt(node, "lyrnum", 0);
            json.getGeometry(lyrnum, polys);
        }

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

        // load the colorramp
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

        // load stat settings (default will use the full range that is found in the tiff file)
        Stats stats;
        stats._forceSdevs = getDbl(node, "sdevs", Stats::SDEV_FULLRANGE);
        stats._forceMin = getDbl(node, "minrv", Stats::FORCE_MINMAX_OFF);
        stats._forceMax = getDbl(node, "maxrv", Stats::FORCE_MINMAX_OFF);



        PGlObj tiff = GisSys::loadTiff(file.c_str(),  picker, &stats);
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
        std::string computebox = UtlString::toLower(getString(node, "computebox", "auto"));
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
void MapYaml::loadLegends(const YAML::Node& node)
{
    for (std::size_t i = 0; i<node.size(); i++)
    {
        PLegend leg = loadLegend(node[i]);
        if (leg) _legendObjMap[leg->_name] = leg;
    }
}

//============================================================================
// toteros, winderos, watereros, sci, sciom, profit, soilloss, dem, yield
//============================================================================
PLegend MapYaml::loadLegend(const YAML::Node& node)
{
    const char *func = "MapYaml::loadLegend() -";

    std::string name = getString(node, "name");
    std::string type = getString(node, "type");

    if (type != "legend")
    {
        return PLegend();
    }

    // get the values we need
    std::string fileout = getString(node, "file");
    std::string legtype = getString(node, "legtype");
    std::string units = getString(node, "units", "m");
    std::string dataobjName = getString(node, "dataobj", "");
    //bool flipcolors = getBool(node, "flipcolors", false);

    bool dynamic = false;

    // TODO: only use dataobj if needed!
    // note: dem, yield, soilloss, sedload are dyamic legends and can only be created with a dem input ti

    GeoImgRaster *rimg = NULL;

    // find the data obj
    if (dataobjName.size() > 0)
    {
        PGlObj dataobj = getDataObj(dataobjName);
        if (!dataobj)
        {
            LogError("%s Failed to find dataobj: %s", func, dataobjName.c_str());
            return PLegend();
        }

        // must be a raster image to create legend from
        rimg = dynamic_cast<GeoImgRaster *>(dataobj.get());
        if (!rimg)
        {
            LogError("%s dataobj: %s not a Raster Image, unable to create legend", func, dataobjName.c_str());
            return PLegend();
        }
    }

    // validate the format
    std::string format = UtlString::GetExtension(fileout.c_str());
    format = UtlString::toLower(format);
    if (format != "png" && format != "svg")
    {
        LogTrace("%s UnRecognized legend file format extension %s, defaulting to svg", func, format.c_str());
        format = "svg";
    }

    // validate the out file
    fileout = validateOutfile(fileout);

    double min = 0;
    double mid = 0;
    double max = 0;
    
    // get dynamic values
    if (rimg)
    {
        min = rimg->stats().getMin();
        mid = rimg->stats().getMid();
        max = rimg->stats().getMax();
    }

    PLegend leg(new Legend());
    if (!leg->init(fileout, legtype, format, min, mid, max, units))
    {
        LogError("%s UnExpected Error: failed to init legend type %s", func, legtype.c_str());
        return PLegend();
    }

    // set dynamic colors to match the raster
    if (rimg)
    { 
        if (rimg->picker() == NULL)
        {
            LogTrace("%s Unable to get colors from raster image dataobj, so using legend defaults", func); 
        }
        else
        {
            leg->setColorMin(rimg->picker()->getMin());
            leg->setColorMid(rimg->picker()->getMid());
            leg->setColorMax(rimg->picker()->getMax());
        }
    }

    return leg;
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

    // load text objects
    str = getString(node, "textobj");
    if (str.size() > 0)
    {
        PGlObj dataobj = loadTextObj(node);
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
PGlObj MapYaml::loadTextObj(const YAML::Node& node)
{
    GeoText *textObj = new GeoText();

    textObj->text(getString(node, "text"));
    textObj->extents(getExtents(node, "extents"));

    Define::AlignH ah = Define::align_h_ctr;
    Define::AlignV av = Define::align_v_ctr;
    getAlign(node, "align", &ah, &av);
    textObj->alignH(ah);
    textObj->alignV(av);


    return PGlObj(textObj);
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

    float r = c.redF();
    float g = c.greenF();
    float b = c.blueF();
    float a = c.alphaF();

    Rgbf rgb(r, g, b, a);
    return rgb;
}

//============================================================================
//============================================================================
bool MapYaml::getColor(const YAML::Node& node, const char *name, QColor *c)
{
    std::string str = "#000000";
    bool ret = false;
    if (node[name])
    {
        str = node[name].as<std::string>();
        if (str.size()) str = "#" + str;
        else str = "#000000";
        ret = true;
    }

    QColor color(str.c_str());
    *c = color;
    return ret;
}

//============================================================================
//============================================================================
Rgbf MapYaml::getColorRgbf(const YAML::Node& node, const char *name, const Rgbf &def)
{
    QColor c;
    if (!getColor(node, name, &c)) return def;

    float r = c.redF();
    float g = c.greenF();
    float b = c.blueF();
    float a = c.alphaF();

    Rgbf rgb(r, g, b, a);
    return rgb;
}

//============================================================================
//============================================================================
int MapYaml::getExtents(const YAML::Node& node, const char *name, Extents *ext)
{
    const char *func = "MapYaml::getExtents() -";

    if (!node[name])
    {
        return 0;
    }

    std::vector<std::string> vexts;
    std::string strexts = node[name].as<std::string>();
    UtlString::explode(strexts, ",", &vexts);
    if (vexts.size() != 4)
    {
        LogError("%s Failed to parse extents, not enought items, should be L,R,T,B but found %s", func, strexts.c_str());
        return -1;
    }

    for (int i = 0; i < 4; i++)
    {
        double d = std::atof(vexts[i].c_str());

        switch(i)
        {
        case 0:
            ext->l = d;
            break;
        case 1:
            ext->r = d;
            break;
        case 2:
            ext->t = d;
            break;
        case 3:
            ext->b = d;
            break;
        }
    }


    return 1;
}

//============================================================================
//============================================================================
Extents MapYaml::getExtents(const YAML::Node& node, const char *name, Extents def, int *res)
{
    int ret = getExtents(node, name, &def);
    if (res) *res = ret;

    return def;
}

//============================================================================
//============================================================================
bool MapYaml::getAlign(const YAML::Node& node, const char *name, Define::AlignH *ah, Define::AlignV *av)
{
    const char *func = "MapYaml::getAlign() -";

    if (!node[name])
    {
        return false;
    }

    std::vector<std::string> alist;
    UtlString::explode(node[name].as<std::string>(), ",", &alist);
    for (unsigned int i = 0; i < alist.size(); i++)
    {
        std::string s = UtlString::toLower(alist[i]);
        s = UtlString::trim(s);

        if (s == "center")
        {
            *ah = Define::align_h_ctr;
            *av = Define::align_v_ctr;
        }
        else if (s == "centerh")
        {
            *ah = Define::align_h_ctr;
        }
        else if (s == "centerv")
        {
            *av = Define::align_v_ctr;
        }
        else if (s == "left")
        {
            *ah = Define::align_h_lft;
        }
        else if (s == "right")
        {
            *ah = Define::align_h_rht;
        }
        else if (s == "top")
        {
            *av = Define::align_v_top;
        }
        else if (s == "bottom")
        {
            *av = Define::align_v_btm;
        }
        else
        {
            LogTrace("%s UnRecognized alignment type found: %s", func, s.c_str());
        }
    }

    return true;
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

    PGlObj obj = std::dynamic_pointer_cast<GlObj>(it->second);
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

    if (!UtlQt::findFile(path, _searchPath, pathresult))
    {
        LogError("%s file doesn't exist: %s", func, path);
        return false;
    }

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
    _legendObjMap.clear();
    _mapObjs.clear();
}
