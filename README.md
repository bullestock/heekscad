HeeksCAD is an open source, CAD application based on OpenCASCADE

Best known as the CAD in HeeksCNC ( CAD/CAM software )
see http://sites.google.com/site/heekscad/

# About this fork

This fork (https://github.com/bullestock/heekscad/) and the associated HeeksCNC fork
are based on the original at https://github.com/Heeks/heekscad
and include the following changes:

* Compiles with OpenCASCADE 7.6.0, wxWidgets 3.2, gcc 12.2
* Support for selecting Entry Move when creating a pocket
* General code cleanup and modernization

Note that they have only been tested on Linux.

# Features

* Import solid models from STEP and IGES files;
* Draw construction geometry and lines and arcs;
* Create new primitive solids, or make solids by extruding a sketch or by making a lofted solid between sketches;
* Modify solids using blending, or boolean operations;
* Save IGES, STEP and STL;
* Printer plot the 2D geometry or to HPGL;
* Import and export DXF files (lines, arcs, ellipses, splines and polylines are supported).

See [here](https://github.com/Heeks/heekscad/wiki/UsingHeeksCAD) for instruction how to use HeeksCAD.

# Technologies

The solid modeling is provided by [Open CASCADE](http://www.opencascade.org).
The graphical user interface is made using [wxWidgets](http://www.wxwidgets.org).

HeeksCAD can be translated into any language.
There are currently English, German, French and Italian translations.

# Platforms

HeeksCAD has been built for _Windows_, _Ubuntu_, _Debian_, and _OpenSUSE_.

See the Wiki tab for details

There are daily built packages for Ubuntu through [this PPA](https://launchpad.net/~neomilium/+archive/ubuntu/heekscnc-devel)

Instructions to build it under Unix-like systems are available in [README-Unix](README-Unix.md).

# License

New BSD License.

This means you can take all my work and use it for your own commercial application. Do what you want with it.

Any changes to wxWidgets or Open CASCADE are subject to their licenses.

# Mailing-lists

The best thing to do is use the discussion group: http://groups.google.com/group/heekscad-users

or if you are helping to develop the software use this group:  http://groups.google.com/group/heekscad-devel

You might also want to join the #cam IRC channel on freenode where most of the HeeksCAD developers hang out.

This #cam IRC channel is logged at http://camlog.archivist.info/

# Contact #

_Dan Heeks_: <danheeks@gmail.com>
