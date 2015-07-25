mapgen
======

The codehammer mapgen rendering tools

* Building & Deps

Linux Environment:

Here are some of the libs I had to install to get things to build:

sudo apt-get install qtdeclarative5-dev
sudo apt-get install freeglut3-dev
sudo apt-get install libmesa-dev
sudo apt-get install xorg-dev
sudo apt-get install opengl-dev

The deps are:

freetype-2.5.3
ftgl-2.1.3-rc5
freeglut-3.0.0-rc2
glew-1.12.0
yaml-cpp-0.5.1
gdal-1-11-1
QT5.4

=====================================================================================================================
* Yaml format
=====================================================================================================================

Sections:
---------------------
output, styles, colorramps, dataobjs, mapobjs, legends, bucket colorramp list


=====================================================================================================================
* Output Section
=====================================================================================================================
output section is required to output a map, it is not required if you want to output a legend only.

output properties:
---------------------
mode: window | windowless   # in window mode you can see the map in a gui window and it will keep displaying until you exit, in windowless the app will automatically exit after rendering the output to disk
width:  1024                # width in pixels of the output
height: 1024                # height in pixels of the output
file: "somefile.png"        # file to output to, if no path is given it will save to the path of the yaml input file
layeroutmode: true | false  # if true, each layer will be output to its own png file with a transparent background, using the "name of the layer.png", the path will be the same path as the output file
bgcolor: #FFFFFF            # optional if you want to set the background color, default is white, for layer output it won't matter
extents: l,r,t,b            # optional if you want to force the extents of the map you can here, otherwise map will automatically zoom to extents of the data

Example:
---------------------
output:
  mode: window
  width: 1024
  height: 1024
  file: map.png
  layerout: true
  

=====================================================================================================================  
* Styles Section
=====================================================================================================================
There are 4 different optional style categories per style, polymask, polyfill, polyoutline, labels. If you don't specify a category, such as polymask, no polymask will be drawn

style properties:
---------------------
name: name of the style

polymask
---------------
draw: true | false          # true to enable the mask, false no mask. default is true.
masks: dataobj1, dataobj2   # you can specify multiple tiff dataobjs, though usually one is sufficient. if you specify more than 1 back to front ordering is used
 
polyfill
---------------
draw: true | false
color: 000000               # fill color
crand:                      # this is for random color generation and you would not need to specify color
    h: .1                   # set h to maintain same result between runs, leave h off to create a different random starting point each run
    s: .55                  # smaller the number the lighter
    v: 1                    # smaller the number the darker
colorbyfeature: true        # this will specify that each polygon with the same feature, and new random color will be generated from the crand settings
feature: name               # name of the feature to use when color by feature is on
colorramp: name             # name of the color ramp with bucket information to color the polygon based on the feature value

polyoutline
---------------
draw: true | false
color: 000000
crand:                      # see polyfill for details
linewidth: 1.0              # width of the line


labels
---------------
draw: true | false
color: 000000
crand:                      # see polyfill for details
fontsize: 32                # size of the font to use
font: ARIAL.TTF             # true type font file and will look in the following directies for the font file
                            #       windows -   %WINDIR%/fonts
                            #       linux -     /usr/share/fonts
                            #                   ~/.fonts
                            #       mac -       /System/Library/Fonts
                            #                   /Library/Fonts
                            #                   ~/Library/Fonts
                            #                   /Network/Library/Fonts
                            
Example (a total of 4 styles):
---------------------                        
 styles:

  - name: polymask

    polymask:
        draw: true
        masks: stormimg #note: back to front order
    polyoutline:
        draw: true
        linewidth: 2
        color: 000000
    labels:
        draw: true
        fontsize: 72
        color: 000000
        feature: name
        
  - name: fill

    polyfill:
        draw: true
        crand:
            h: .1 # set h to maintain same result between runs, leave h off to create a different random starting point each run
            s: .55 # smaller the number the lighter
            v: 1 #smaller the number the darker
        colorbyfeature: true
        
  - name: outline
        
    polyoutline:
        draw: true
        linewidth: 2
        color: 000000

  - name: labels

    labels:
        draw: true
        fontsize: 32
        color: FF0000
        feature: musym
  

=====================================================================================================================
* Colorramps Section
=====================================================================================================================
Tiff dataobjs depend on color ramps to color the data within them.
Bucket fill type polygon region maps also need colorramp with buckets, minv, maxv values.

colorramp properties:
---------------------
name: colordem
min: 00ff00
mid: ffff00
max: ff0000
buckets: 11
minv: 0.0
maxv: 0.0

Example:
---------------------
colorramps:

  - name: coloryield
    min: 00ff00
    mid: ffff00
    max: ff0000
    

=====================================================================================================================
* Dataobjs Section
=====================================================================================================================
3 types of dataobjs: geojson, tiff, aerial

dataobj common properties:
---------------------
name: name                      # name of the dataobj, this is how other object will find and use the dataobj
type: geojson | tiff | aerial   # must specify the type of data

geojson
---------------
file: oregon_field.json         # the datafile (if no path is specified, look in the the yaml input file's location)
lyrname: mups_geojson           # the layer within the geojson, this is optional and not always needed
epsgfr: 4326                    # coordinate system to convert from (optional, only needed if not 3857)
epsgto: 3857                    # coordinate system to convert to, always need data in 3857

tiff
---------------
file: yield.tif                 # geo tif file (if no path is specified, look in the the yaml input file's location)
colorramp: coloryield           # color ramp (optional if not specified, min: 55aa00, mid: ffff00, max: ff0000 will be used)
sdevs: 1.5                      # specify number of standard deviation from mean to use for low and high range, -1 use full range (optional default is -1)
minrv: -9999                    # set the min data value to use, -9999 means this is not used (optional default is -9999)
maxrv: -9999                    # set the max data value to use, -9999 means this is not used (optional default is -9999)

aerial
---------------
extents:                        # optional, specify extra amount in 3857 distance to add to the extents of the data bounding box to get the aerial image for
    extra: 250
    
Example:
---------------------
dataobjs:

  - name: aerialimg
    type: aerial
    extents:
      extra: 250

  - name: yieldimg
    type: tiff
    file: yield.tif
    colorramp: coloryield
    sdevs: 1.5 # - 1 use full range
    minrv: -9999 # use min from file
    maxrv: -9999 # use max from file

  - name: fielddata
    type: geojson
    file: field.geojson 
    
    
=====================================================================================================================
* Mapobjs Section
=====================================================================================================================
Currently there is only 1 type of map object which is a layer. 
Note: Map objects will be rendered in back to front order

mapobj properties:
---------------------
name: layername                     # name used for output, layername.png when in layerout mode
type: layer                         # layer is the only valid type
style: stylename                    # style to apply to the layer
children:
    - dataobj: dataobjname          # currently only dataobjects are supported, you can add multiple objects by seperating them by ,
       
Example:
---------------------

mapobjs:

   - name: aerial
     type: layer
     children:
       - dataobj: aerialimg 

   - name: field
     type: layer
     style: polymask
     children:
       - dataobj: fielddata
       
   - name: field_fill
     type: layer
     style: fill
     children:
       - dataobj: fielddata
       
   - name: field_outline
     type: layer
     style: outline
     children:
       - dataobj: fielddata
       
   - name: field_labels
     type: layer
     style: labels
     children:
       - dataobj: fielddata
       

=====================================================================================================================
* Legends Section
=====================================================================================================================
Legends can be specified with or without mapobjs, if no mapobjs are specified in the yaml then a legend will be written to disk and the application will close with no map output
If ever needed legends could be rendered out on the map, but for now just saved to disk

legend properties:
---------------------
name: legendname                # name of the legend, for future use
type: legend                    # type of object for future use
file: legend.svg | legend.png   # legend file with svg or png extension (if no path is specified, will write out to yaml input file's location)
legtype: dem | yield | soillos | sedload | brkevcom | brkevexp | toteros | winderos | watereros | sci | sciom | profit | rr | r2d2 
                                # note: dem, yield, soilloss, sedload are dyamic legends and can only be created with a tiff dataobj, the rest can be generated without data
units: m | ft                   # meter or feet units
dataobj: yieldimg               # tiff dataobj to use when appropriate based on legend type
colorramp: name                 # name of color ramp that specifies colors bucket count, minv, maxv

Example:
---------------------
legends:
    
   - name: maplegend
     type: legend
     file: legend.png
     legtype: yield
     units: m
     dataobj: yieldimg
     
     
=====================================================================================================================
* Bucket Color Ramp Section
=====================================================================================================================

toteros, winderos, watereros
----------------------------------------------
colorramps:
  - name: toteros_color
    min: 55aa00
    mid: ffff00
    max: ff0000
    buckets: 11
    minv: 0.0
    maxv: 5.0
    
sci, sciom
----------------------------------------------
colorramps:
  - name: sci_color
    min: ff0000
    mid: ffff00
    max: 55aa00
    buckets: 12
    minv: -1.0
    maxv: 1.0
    
rr
----------------------------------------------
colorramps:
  - name: rr_color
    min: ff0000
    mid: ffff00
    max: 55aa00
    buckets: 11
    minv: -100.0
    maxv: 100.0
    
r2d2
----------------------------------------------
colorramps:
  - name: rr_color
    min: ff0000
    mid: ffff00
    max: 55aa00
    buckets: 11
    minv: -500.0
    maxv: 500.0
