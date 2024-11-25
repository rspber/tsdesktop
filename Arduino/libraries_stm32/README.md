
Follow the instructions please

1. To compile example:

  * In Arduino:

    - File / Preferences / Settings / Additional boards manager URLs

      https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json

      Hit OK to close the dialog.

    - Tools / Board / Boards Manager ...

      stm32

    - Tools / Board / ... Boards / `select your board`


2. To upload example by USB:

  * https://github.com/rogerclarkmelbourne/Arduino_STM32

    - for linux run tools/linux64/install.sh<br/>
      -- see env/udev/rules.d/45-maple.rules  (very important for DFU)

  * https://github.com/stm32duino/Arduino_Core_STM32/wiki/Upload-methods

    - follow by DFU<br/>

      -- press BOOT0 or B0 and plug board via USB<br/>

    - in STM32CubeProgrammer<br/>

      -- connect to USB:USB1 (USB1 will be accesible if DFU is configurted correctly)<br/>
      -- use lsusb and dmesg to control DFU</br>
      -- select some example.elf to upload
