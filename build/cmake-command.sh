#!/bin/sh


cmake -D CMAKE_TOOLCHAIN_FILE=../Arduino-CMake-Toolchain/Arduino-toolchain.cmake -D ARDUINO_INSTALL_PATH=/var/lib/flatpak/app/cc.arduino.arduinoide/x86_64/stable/275ee51aa035c21aabdf6a2d0328350b39c12289f4f4d102160bde47b01449ac/files/Arduino/ ..
