#ifndef MAPYAML_H
#define MAPYAML_H

#include "sysdefs.h"
#include "yaml-cpp/yaml.h"
#include "drawattr.h"
#include "geojson.h"
#include "globj.h"
#include "geocompute.h"
#include "gissys.h"
#include "gradientpicker.h"
#include "rgbf.h"

#include <QColor>

class MapYaml
{
public:
    struct ColorRamp
    {
       std::string name;
       PGradientPicker picker;
    };
    typedef shared_ptr<ColorRamp> PColorRamp;


public:
    MapYaml();

    bool loadFile(const char *file, GisSys *pgis);
    bool load(const std::string &text, GisSys *pgis);

protected:

    bool load(YAML::Node &doc, GisSys *pgis);

    void initAerial(GisSys *pgis);
    void initCompute(GisSys *pgis);

    void clear();

    void loadOutput(const YAML::Node& node);

    void loadStyles(const YAML::Node& styles);
    PDrawAttr loadStyle(const YAML::Node& styles);

    void loadColorRamps(const YAML::Node& styles);
    PColorRamp loadColorRamp(const YAML::Node& styles);

    void loadDataObjs(const YAML::Node& styles);
    PGlObj loadDataObj(const YAML::Node& styles);

    void loadMapObjs(const YAML::Node& styles);
    PGlObj loadMapObj(const YAML::Node& styles);

    PGlObj loadLayer(const YAML::Node& node);
    void loadLayerChildren(const YAML::Node& node, GlObj *lyr);
    PGlObj loadLayerChild(const YAML::Node& node);

    void loadComputeObjs(const YAML::Node& styles);
    PGeoCompute loadComputeObj(const YAML::Node& styles);

    std::string getString(const YAML::Node& node, const char *name, std::string def="");
    double getDbl(const YAML::Node& node, const char *name, double def=0);
    float getDbl(const YAML::Node& node, const char *name, float def=0);
    int getInt(const YAML::Node& node, const char *name, int def=0);
    bool getBool(const YAML::Node& node, const char *name, bool def=false);
    QColor getColor(const YAML::Node& node, const char *name);
    Rgbf getColorRgbf(const YAML::Node& node, const char *name);

    PGlObj getDataObj(const std::string &name);
    PGlObj getComputeObj(const std::string &name);
    PColorRamp getColorRamp(const std::string &name);
    PDrawAttr getStyle(const std::string &name);

    std::string validateOutfile(const std::string &file);
    bool getFile(const YAML::Node& node, std::string *file);
    bool fileExists(const char *path, std::string *pathresult);
    void initSearchPath(const std::string &yamlfile);

protected:
    std::string _yamlfile;

    PConfig _cfg;
    std::map<std::string, PDrawAttr> _styleMap;
    std::map<std::string, PColorRamp> _colorRampMap;
    std::map<std::string, PGlObj> _dataObjMap;
    std::map<std::string, PGeoCompute> _computeObjMap;
    GlObjList _mapObjs;

    std::vector<std::string> _searchPath;

};

#endif


/*
---
output:
  mode: windowless
  width: 2000
  height: 2000
  file: map.png

styles:

  - name: polymask

    polymask:
      - draw: true
        masks: dem,chan #note: back to front order
    polyoutline:
      - draw: true
        linewidth: 2
        color: 000000
    labels:
      - draw: true
        size: 72
        color: 000000


colorramps:

  - name: colordem
    min: 00ff00
    mid: ffff00
    max: ff0000

  - name: colorchan
    min: 000000
    max: 0000ff


dataobjs:

  - name: aerialimg
    type: aerial
    extents:
      usemap: true
      extra: 250

  - name: dem
    type: tiff
    file: dem.tiff
    colorramp: colordem

  - name: chan
    type: tiff
    file: chan.tiff
    colorramp: colorchan

  - name: fielddata
    type: geojson
    file: input.json

mapobjs: #note back to front order

   - name: aerial
     type: layer
     children:
       - dataobj: aerialimg

   - name: field
     type: layer
     style: polymask
     children:
       - dataobj: fielddata
*/
