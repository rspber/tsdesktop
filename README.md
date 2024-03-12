
All the examples work and the basis is solid connections, do spare time to make them (it takes many hours, but without it nothing will work).

The connections should be made straight from A to B with the wires in the protective sleeves, the factory ones are the best, and then it will work.

<!--
<p dir="auto">
  <a href="https://github.com/vshymanskyy/StandWithUkraine/blob/main/docs/README.md">
    <img src="https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/banner-direct.svg" alt="SWUbanner" style="max-width: 100%;">
  </a>
</p>
-->

# TSDesktop
  Touch Screen Desktop for Raspberry Pi Pico.

- https://www.youtube.com/watch?v=2_8hvvxUR1c

### News

* ESP32 8bit parallel

* idyll example https://youtu.be/NPU_-LH7fMA

* spider example https://youtube.com/shorts/JbAyZdpuKMw

* fill gradient modernization - not finshed yet

* BufferedDisplay, graphics modernization, quadraball example https://youtube.com/shorts/xA41QBkwiz4

* esp32 support

* pic32 support

* Resistive touch was misinterpreted

* Touch over PIO SPI

* More sensitive touch for Arduino SPI (taken from pico-sdk)

* Touch rotation has been finally mastered.
  One of the touch_??? procedure from Touch_Rotation.hh is responsible for touch rotation.
  If none of the available procedures work properly, we have two options:
  1. if one of the procedures could be suitable but has the same deviation in all 4 cases, we can correct it using the TOUCH_REV parameter.
  2. Define another routine for touch rotation.

* Resistive touch for RP2040 GPIO 8BITP, ATTENTION: pinout changed

* Chunks in PIO

* A new protocol: RP2040 GPIO 8BITP write/read (enable LCD_RD pin)

* RP2040 PIO SPI RAED finally works with RP2040 PIO SPI WRITE protocol

* RP2040 PIO 8BITP Write protocol, developed for buffered writing and speed now

* Writing by PIO and reading by SPI work now together in Arduino only, the touch needs to be commented out

* User setups in setup folder

* Buffered canvas - see canvas3 example

* New repository https://github.com/rspber/TSD_Fonts

* New repository https://github.com/rspber/TFT_eSPI_light

* TFT write/read and touch protocols were separated from each other and from gfx

* A new RP2040 PIO SPI WRITE protocol

* Available TFT drivers:

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

* Overlaid objects

  GFXButton's objects can 'flow' over others thanks to setOverlaid property.
  The method is to remember the background when drawing the object 'draw' and recreate it when hiding the object 'hide'.
  See examples canvas1 and canvas2. It is an uresolved problem when several overlaid objects overlap.

### Topics on this site

* BufferedDisplay
  - BufferedDisplay is like a window on full sizes, abstract, not existing in this moment, screen
  - Drawing in window refers to absolute positions of full sizes screen
  - What remains on the window after this drawing depends on its position during this drawing

* TSDesktop
  - easy way to place objects on the screen: all examples
  - integrated touch screen: almost all examples
  - scroller: see examples: scroll1, menu1, menu2, menu3
* TFT Stack (TSD_GFX -> TSD_SCREEN -> TFT_SCREEN(TFT_API) -> Display) to easy implement protocols and drivers for tft
* Modified gamma for ILI9341 to reflect better color vibrancy.
* 565/666 color support
* Gradient with percentage
  - see examples: canvas1, menu3, checkboxes
* Flexible font format
  - visit https://github.com/rspber/TSD_Fonts repository
* UTF-8/Unicode Support

    Although utf-8/unicode fonts are not included, see https://github.com/rspber/TSD_Fonts for more details.

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
   sudo apt install cmake g++ gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
   ```
3. Install pico-sdk:

  * `download` or `git clone` https://github.com/raspberrypi/pico-sdk.git

  * Set `PICO_SDK_PATH` to the SDK location in your environment

4. `download` or `git clone` this repository

5. Resolve links

6. Install VS Code

7. In VS Code:

  * Install extensions: C/C++ Extension Pack, CMake Tools

  * File / Open Folder ... select location to /tsdesktop/pico-sdk

  * In the bottom panel:

    - CMake : [Scan for kits], select: GCC `X.X.X` arm-none-eabi, Release

    - Build

8. Use bootselBoot tool to upload *.uf2 examples

## Arduino

1. No other libraries are required

2. In Arduino:

  * File / Preferences / Settings / Additional boards manager URLs (can be comma separated):

    - rp2040

      https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

    - pic32

      https://raw.githubusercontent.com/chipKIT32/chipKIT-core/master/package_chipkit_index.json

    - esp32

      https://dl.espressif.com/dl/package_esp32_index.json

    - Hit OK to close the dialog.

  * Tools / Board / Boards Manager ...

    - rp2040

      Type "pico" in the search box, you should see: Raspberry Pi Pico/RP2040, and select "Install"

      If after upgrading system to a new version of eabi e.g. to 10.x.x, Invalid -march:armv6-m error occures, it's time to upgrade this library too.

    - pic32

      Type "chipKIT" ...

    - esp32

      Look for "esp32" by Espressif

  * Tools / Board / ... Boards / `select your board`

3. `download` or `git clone` this repository

4. Copy Arduino/libraries to Arduino's directory

5. Copy selected (1) Arduino/libraries_??? to Arduino/libraries

6. Resolve links to tsdesktop directories in Arduino/libraries

7. Copy Arduino/examples to Arduino's Sketchbook directory

8. For linux users:

  * In Arduino:

    - Tools / Port / `/dev/ttyACM0`  or, if it appears instead `/dev/ttyUSB0`

  * In console:

    - for one-time access:
    ```
      sudo chmod a+rw /dev/ttyACM0
    ```
    - or
    ```
      sudo chmod a+rw /dev/ttyUSB0
    ```
    - for permanent access:
    ```
      sudo usermod -aG dialout $USER
      sudo reboot
    ```
9. rp2040: If you want to use SerialUSB in/for tft pio debugging you must comment #include "Arduino.h" in packages/rp2040/hardware/rp2040/2.7.1/cores/rp2040/SerialUSB.h, similarly with SerialUART.h and SerialUART.cpp where you need to add #include "api/Common.h", "hardware/gpio.h", "pins_arduino.h" and copy/paste __bitset from Arduino.h wherever it appears instead.

## Hints

1. If incidental touch pressed or not calibrated touch illusion occures, try reduce touch speed first

2. If not all colors are displayed, try reduce tft speed first
