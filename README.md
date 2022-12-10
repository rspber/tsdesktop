# TSDesktop
Touch Screen Desktop for Raspberry pi pico

- https://youtube.com/shorts/Ooa5Czfw7PA
- https://youtu.be/W1wCEIwkDYY
- https://www.youtube.com/watch?v=2_8hvvxUR1c

2.4" TFT SPI 240X320 v1.3 is supported (see Setup.h)

### UTF-8/Unicode Support

  Although utf-8/unicode fonts are not included, see the tools directory for more details.

  When the correct font with UTF-8 encoding is included in the code, use the following scheme
  to deliver utf-8 text:

```c++
  const char buf[] { 0xc4, 0x85, 0xc4, 0xb2, 0 };
  display.drawText(x, y, buf, FontName);
  // or
  textBtn.setText(buf);
```
  and for unicode:
```c++
  const uint16_t buf[] { 0x4E00, 0x3007, 0 };
  display.drawText(x, y, buf, FontName);
  // or
  textBtn.setText(buf);
```

  remember to end the constructions with a 0.

## pico-sdk

1. No other libraries are required,

2. Install CMake (at least version 3.13), and GCC cross compiler
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

    - CMake : GCC 6.3.1 arm-none-eabi, Release

    - Build

7. Use BOOTSEL to upload *.uf2 examples

## Arduino

1. Tested on Arduino 1.8.19, Arduino 1.8.5 should works too,

2. In Arduino:

  * File / Preferences / Settings

    Additional boards manager URLs: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

    - Hit OK to close the dialog.

  * Tools / Boards / Board Manager

    - Type "pico" in the search box, you should see: Raspberry Pi Pico/RP2040, and select "Add"

3. `download` or `git clone` this repository

4. Copy Arduino/libraries to Arduino's libraries, pay attention on TSDesktop, TSD_GFX and TSD_ILI9341 folders and dependenciens:

  * https://github.com/PaulStoffregen/XPT2046_Touchscreen

5. Copy Arduino/examples to Arduino's Sketchbook directory
