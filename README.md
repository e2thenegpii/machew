# machew
Basic Arduino functions implemented efficiently

Most of the time (at least in my sketches) the pins are known at compile time.
Pin values rarely change during the progress of the program as they are 
physically connected to the device and are either input, output or input_pullup 
by design.  As currently implemented many of the Arduino functions require
large lookup tables to determine pin data at runtime.  These lookup tables burn
a lot of unnecessary program space.  The machew library implements basic
Arduino language functions so that when pins are known at compile time the
compiler can reduce as much instruction bloat as possible resulting in smaller
programs.  The primary method of implementing this is utilizing C++ templates
specifically features from the C++11 standard.

Supported Devices:
* ATMega1280

I would like to support as many different devices as possible, but for the
forseable future I not be purchasing any other Arduinos other then the 
ATMega328P and the ATMega1280 I currently own.

Implemented Functions:
* digitalWrite
* digitalRead
* pinMode
* analogRead
* analogWrite

I would like to support advanced I/O functions as well as libraries that depend
on these functions.

