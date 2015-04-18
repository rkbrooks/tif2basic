# TIF to Applesoft BASIC

This program can convert two-color (black/white) TIF images into BASIC programs that can be interpreted by an Apple II.

## Usage

Compile the program:

```bash
cc -o tif2basic tif2basic.c
```

And run it with your image file:

```bash
./tif2basic funnycatpicture.tif
```

## Formatting an image

Most-strictly speaking, your image MUST be 280x192px. If it isn't cropped to this size (the size of an Apple II screen in graphics mode) it will probably be angry at you.

Otherwise, it's very nice to have your image in two colors: black and white. Using Adobe Photoshop CS4, this can be done with *indexed colors*.

Finally, save your TIF with no compression, order the pixels per-channel (RRGGBB, **not** RGBRGB), and save the file with a Macintosh byte order.

## License

This software is licensed under a **modified** MIT License. The stipulation I've added is that you have to send me an email telling me how you're using this program. :)

Copyright (c) 2015 Davis Remmel.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software, and the copyright holders
shall be notified when this program is being modified or redistributed.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
