TSD GFX Pixel font customizer
---

This is a handy utility that goes well with the utf8fontc tool for converting TTF/TTC fonts to TSD_GFX `.h` format.

Just paste a section fragment of a `.h` font file, import the glyphs, and you can edit them one by one by flipping the individual pixels for each glyphs, changing the advance, offset, or modifying the baseline.

You can then export modified font.

This is useful since the utf8fontc, based on Adafruit's fontconvert, utility does not always create all the characters correctly (it's caused by improper use of freetype library perhaps, it's seen in freemono, freesans, less in freeserif, but the CJK fonts looks perfect).

## License

BSD 2-Clause License

Copyright (c) 2022, rspber (https://github.com/rspber)
All rights reserved.

This utility based on https://github.com/tchapi/Adafruit-GFX-Font-Customiser under the MIT License, placed in ORIGINAL_LICENSE file.
