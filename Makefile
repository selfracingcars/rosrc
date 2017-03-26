# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

BOARD_TAG    = uno
MONITOR_PORT = /dev/rc0
ARDUINO_PORT = /dev/rc0
ARDUINO_LIBS = ros_lib
USER_LIB_PATH = .

include  /usr/share/arduino/Arduino.mk

