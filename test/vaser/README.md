
Issue details:
--------------

* First appears with commit 81a3022cbfba212a5e768cea6c8c645e7822d5cb  (update line rendering to remove artifacts, also checking in some additional test data)
* 
* Happens for some geojson inputs, but not others; however, behavior is consistent: if it happens with a given geojson input, it always happens with that input
* 
* Only happens when run headless; when run under "normal" X Server, there is no issue (production machines are headless though)
* 
* * Production and test env for us is Linux, so has only been tested there
* * On production machines we use a dummy driver for X
* * On test machines we use `Xvfb` to simulate headless. Same behavior in either place.
* * Just for the record, my xvfb-run incantation: `xvfb-run -a -s "-screen 0 1024x768x24"`
* * 

Questions:
----------

* Is there a particular GL extension the vaser code requires that is not otherwise used by mapgen?

Description of included files:
------------------------------

* `bad.yml` is a mapgen map spec that references `bad.geojson`. Running `mapgen bad.yml` will produce a file named `bad.png`
* `good.yml` is a mapgen map spec that references `good.geojson` ---> `good.png`
* `example_bad_output.png` is a renamed version of what I get when I run `mapgen bad.yml` on a headless machine