#### The original Raspberry-pi-pico BOOTSEL tool.

For pico-sdk users only.

### Reset Pi Pico via. USB

https://github.com/illusiaDecafish/bootselBoot

Required packages:

  * Fedora:
  ```
    sudo dnf install libusb-devel
  ```
  * Ubuntu:
  ```
    sudo apt-get install libusb-1.0-0-dev
  ```
Compile:

  * Fedora:
  ```
    cc bootselBoot.c -o bootselBoot -I/usr/include/libusb-1.0 -L/usr/lib64 -lusb-1.0
  ```
  * Ubuntu:
  ```
    cc bootselBoot.c -o bootselBoot -I/usr/include/libusb-1.0 -L/usr/lib/x86_64-linux-gnu -lusb-1.0
  ```
Using:

  ```
    sudo ./bootselBoot
  ```
  - or
  ```
    sudo ./bootselBoot b      - same as above: replug pico with BOOTSEL pressed
  ```
  - or
  ```
    sudo ./bootselBoot f      - reset to regular boot mode
  ```
Notes:

  1. The program on board must have the pico_stdio_usb module:
  ```
    CMakeLists.txt
      pico_enable_stdio_usb(program name 1)
  ```
  2. Unfortunately, it's not compatible with Arduino platform.

  3. More details on Author's page.
