makeSimplexTexture
=======================================
Create textures using Simplex Noise algorithms.

This program is published under MIT license, which can be found in the dedicated
LICENSE file and at the end of this README.


Usage
---------------------------------------
The usage is explained in the help text, displayed using the --help option.

The default behavior, when no options are given, is to open
a 800x600 window, generate one texture, and wait for a
keypress.
```
  Options:
  -x|y|z|w <value>         Set offset of the specified dimension.
                           All four dimensions default to 0.0 and use a
                           stepping of one per pixel, modified by zoom
                           which defaults to 9.0. (This means every
                           pixel raises x and/or y by 1.0 / 9.0) To use
                           z, you have to set 3D or 4D, to use w 4D is
                           needed. See "sequence patterns" below about
                           how to control z and w.
      --3D|4D              Enable third/fourth dimension
  -B --batch               Enable batch mode (no GUI)
  -h --bordhi     <value>  Set high border (-1.0 to 1.0)
  -l --bordlo     <value>  Set low border (-1.0 to 1.0)
                           The borders are the limit under/over which
                           value the low and high color are set. The
                           middle color is always set in the middle
                           between those borders. The default sequence
                           for low->mid->high is -1.0->0.0->+1.0
  -H --colhi      <value>  Set the high color
  -M --colmid     <value>  Set the middle color
  -L --collow     <value>  Set the low color
                           Colors are needed as 0xRRGGBB and default to
                           0xffff00 for colHi, 0x7f7f00 for colMid and
                           0x000000 for colLow
  -e --ext        <ext>    Set the file extension to the desired image
                           format. The default is "png".
     --height     <height> Set window height (minimum 480)
     --help                Show this help and exit
  -n --no-bumpmap          Omit the calculation of a bumpmap
  -q --quiet               No output to the console
  -R --reduct     <value>  Set the reduction on each wave (minimum 1.0)
  -s --seed       <value>  Set seed
  -S --smooth     <value>  Set the smoothing on each wave (minimum 1.0)
  -t --threads    <num>    Set number of threads (minimum 4, default 8)
     --version             Show the programs version and exit
     --width      <width>  Set window width (minimum 640)
  -W --waves      <value>  Set number of waves (minimum 1)
  -Z --zoom       <value>  Set the zoom factor (minimum 0.001)

The next set of arguments can be used to change the modifiers for
the coordinates.
Changing the modifier for w and/or z may lead to very strange results,
because they are used with the *IncMod* sequence patterns, too.
      --mod<zw> <value>  Change the modification value for z or w

  Sequence Patterns:
The following options may be added to command line options or set
via alt-z or alt-w at runtime. w and z are static values by default.
 (to keep the output short, <xy> means either x or y.)
  <wz>IncMod<XYZW>         w/z is increased whenever x/y/z/w is modified
  <wz>Is<XYZW>             Set w/z to be equal to x/y/z/w
  <wz>IsXY                 Set w/z to be equal to x * y
  <wz>IsXYd<ZW>            Set w/z to be equal to (x * y) / <ZW>
  <wz>IsXYm<ZW>            Set w/z to be equal to (x * y) % <ZW>
  <wz>Is<XY>d<YX>          Set w/z to be either x / y or y / x
  <wz>IsXaY                Set w/z to be equal to x + y
  <wz>Is<XY>s<YX>          Set w/z to be either x - y or y - x
  <wz>Is<XY>m<YX>          Set w/z to be either x % y or y % x

  GUI key mapping:
CURSOR: Modify x with left/right, y with up/down cursor keys
d/D   : Increase/decrease dimensions (2-4)
ESC   : Quit program
h     : Show on-screen help
R     : Render image with current settings
s     : Save current texture (and bumpmap unless -n is specified)
SPACE : Switch between texture and bumpmap. This does only work if the
          -n/no-bumpmap option isn't used.
TAB   : Show stats of the current image
w/W   : Increase/decrease offset w by mod w. (defaults to 1.0)
        Press ctrl to increase/decrease with 10 times mod w.
        Press alt to toggle w coordinate sequence setting.
z/Z   : Increase/decrease offset z by mod z. (defaults to 1.0)
        Press alt to toggle z coordinate sequence setting.
```

Build and Install
---------------------------------------
Building and installation is done using make.

> `make`
> `make install`

See the `Makefile` for the options you have.


Prerequisites
---------------------------------------
 * makeSimplexTexture depends on the latest PrydeWorX Library (pwxLib).  
   Get it at https://pwxlib.prydeworx.com
 * The graphical display is done using SFML.  
   Get it at http://www.sfml-dev.org/

   
Contributing
---------------------------------------
To contribute to makeSimplexTexture, fork the current source code from
  [gitHub](https://github.com/Yamakuzure/makeSimplexTexture/).
Send a pull request for the changes you like.

If you want to report bugs, please do so at the makeSimplexTexture
  [issue tracker](https://github.com/Yamakuzure/makeSimplexTexture/issues)
on github.


License
---------------------------------------
MIT License

Copyright (c) 2010-2018 Sven Eden

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
