
* In Arduino:

  Follow the instructions please.

  - File / Preferences / Settings / Additional boards manager URLs

    https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

    Hit OK to close the dialog.

  - Tools / Board / Boards Manager ...

      Type "pico" in the search box, you should see: Raspberry Pi Pico/RP2040, and select "Install"

      If after upgrading system to a new version of eabi e.g. to 10.x.x, Invalid -march:armv6-m error occures, it's time to upgrade this library too.

  - Tools / Board / ... Boards / `select your board`
