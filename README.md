# TSDesktop
Touch Screen Desktop for Raspberry pi pico

- https://www.youtube.com/watch?v=2_8hvvxUR1c
- https://www.youtu.be/W1wCEIwkDYY

2.4" TFT SPI 240X320 v1.3 is supported (see Setup.h)

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

3. `download` or `git clone` this repository

4. Copy Arduino/libraries to Arduino's libraries, pay attention on TSDesktop folder and dependenciens:

  * https://github.com/adafruit/Adafruit_ILI9341
  * https://github.com/PaulStoffregen/XPT2046_Touchscreen

5. Copy Arduino/examples to Arduino's Sketchbook directory
