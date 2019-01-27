# machew
Previously the intent of this library was to implement Arduino functions
efficiently in C++ templates.  That was three years ago and since then I've
decided I don't want to quite do that.  There may be a Arduino compatibility
layer at some point in the future, (don't hold your breath).  The intent now
is to create a high level AVR library using C++ templates that compile down
to assembly that is as fast as one could write using straight C and a lot of
bit twiddling.

## Supported Devices (or to support soon):
* [ATTiny25]()
* [ATTiny45]()
* [ATTiny85]()
* [ATMega168]()
* [ATMega8]()
* [ATMega328P](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf)
* [ATMega640](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf)
* [ATMega1280](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf)
* [ATMega1281](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf)
* [ATMega2560](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf)
* [ATMega2561](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf)

I don't own development boards for each of those platforms, so support will
be on a best effort basis mostly using [simulavr](https://www.nongnu.org/simulavr/)

If I can successfully impemlent support for those 4 different platforms then chances
are pretty good that generalizing support to any other AVR microcontroller shouldn't
be to difficult.

The code will be C++17 which means running it in Arduino will only work by updating
the compiler to a more recent version.  Details on how to do that can be found
at [Zakk Emble's Website](http://blog.zakkemble.net/avr-gcc-builds/).

Support for the C++ stdlib will be from my fork of [StandardCplusplus](https://github.com/e2thenegpii/StandardCplusplus)

## Todo:
1. Add tuple support to StandardCplusplus
1. Implement assignment for various interfaces that accept a tuple to preform a as few register writes as possible
1. Implement each of the register types
1. Expand support to the ATmega640+
1. Expand support to the ATTiny28
1. Expand support to the ATTiny26
1. Use CMake to avoid manually setting up tools

## To Use:
Use CMake
```
git clone --recursive git@github.com:e2thenegpii/machew.git
cmake -DAVR\_MCU=atmega328 -build-toolchain=ON .
```
