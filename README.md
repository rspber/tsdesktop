# TSDesktop
  Touch Screen Desktop for Raspberry pi pico.

- https://youtube.com/shorts/Ooa5Czfw7PA
- https://youtu.be/W1wCEIwkDYY
- https://www.youtube.com/watch?v=2_8hvvxUR1c

2.4" TFT SPI 240X320 v1.3 is supported (see Setup.h)

### News

* available TFT SPI drivers:

  - GC9A01
  - HX8357
  - ILI9341
  - ILI9481
  - ILI9486
  - ILI9488
  - R61581
  - RM68140
  - SSD1963
  - ST7789
  - ST7796
  
* overlaid objects

  GFXButton's objects can 'flow' over others thanks to setOverlaid property.
  The method is to remember the background when drawing the object 'draw' and recreate it when hiding the object 'hide'.
  There are still many problems to solve to get perfect results, but the examples of canvas1 and canvas2 show the operation for simple objects like: line, text, square.
  Objects with a redundant method of drawing pixels such as triangles, circles leave streaks on the screen.
  This also happens when several overlaid objects overlap.

### Topics on this site
* TSDesktop
  - easy way to place objects on the screen: all examples
  - integrated touch screen: almost all examples
  - scroller: see examples: scroll1, menu1, menu2, menu3
* Modified gamma for ILI9341 to reflect better color vibrancy.
* 565/666 color support
  - for ILI9341
* Gradient with percentage
  - see examples: canvas1, menu3, checkboxes
* Flexible font format
  - visit tools folder
* UTF-8/Unicode Support

    Although utf-8/unicode fonts are not included, see the tools directory for more details.

    When the correct font with UTF-8 encoding is included in the code, use the following scheme
      to deliver utf-8 text:
    ```
      display.drawText(x, y, "\xc4\x85\xc4\xb2", FontName);
    ```
    or
    ```
      textBtn.setStaticText("\xc4\x85\xc4\xb2");
    ```
    and for unicode:
    ```
      const uint16_t buf[] { 0x4E00, 0x3007, 0 };
      display.drawText(x, y, buf, FontName);
    ```
    or
    ```
      textBtn.setStaticText(buf);
    ```
    remember to end the construction with a 0.
* Thickness parameter for lines, rectangles, triangles and circles

## pico-sdk

1. No other libraries are required,

2. Install CMake (at least version 3.13), and GCC cross compiler
* Fedora
   ```
   sudo dnf install cmake gcc-c++ arm-none-eabi-gcc-cs-c++ arm-none-eabi-newlib
   ```
* Ubuntu
   ```
   sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
   ```
3. Install pico-sdk:

  * `download` or `git clone` https://github.com/raspberrypi/pico-sdk.git

  * Set `PICO_SDK_PATH` to the SDK location in your environment

4. Install VS Code

5. `download` or `git clone` this repository

6. In VS Code:

  * Install extensions: C/C++ Extension Pack, CMake Tools

  * File / Open Folder ... select location to /tsdesktop/pico-sdk

  * In the bottom panel:

    - CMake : GCC `X.X.X` arm-none-eabi, Release

    - Build

7. Use bootselBoot tool to upload *.uf2 examples

## Arduino

1. Tested on Arduino 1.8.19, Arduino 1.8.5 should works too,

2. In Arduino:

  * File / Preferences / Settings

    Additional boards manager URLs: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

    - Hit OK to close the dialog.

  * Tools / Board / Boards Manager ...

    - Type "pico" in the search box, you should see: Raspberry Pi Pico/RP2040, and select "Install"

  * Tools / Board / Raspberry Pi RP2040 Boards / `select your board`

3. `download` or `git clone` this repository

4. Copy Arduino/libraries to Arduino's libraries, pay attention on
    TSDesktop, TSD_GFX, TSD_TFT, TSD_Drivers and TSD_Utils folders
  * and dependenciens:

    https://github.com/PaulStoffregen/XPT2046_Touchscreen

5. Copy Arduino/examples to Arduino's Sketchbook directory

6. For linux users:

  * In Arduino:

    - Tools / Port / `/dev/ttyACM0`

  * In console:

    - for one-time access:
    ```
      sudo chmod a+rw /dev/ttyACM0
    ```

    - for permanent access:
    ```
      sudo usermod -aG dialout $USER
      sudo reboot
    ```
## Hints

1. If incidental touch pressed or not calibrated touch illusion occures, try reduce touch speed first

2. If not all colors are displayed, try reduce tft speed first

3. If touch does not follow display (does not apply to ILI9341 and ST7789, which are already matched) find in code: defined(ILI9341) or defined(ST7789) and try to place your driver name respectively.
