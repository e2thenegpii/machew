#ifndef __MACHEW_H__
#define __MACHEW_H__

#include <Arduino.h>

//Can't think of a good way to pull these in from Arduino.h
#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12

//Don't know why these aren't found in the avr library
#define cbi(port,bit) \
    (port) &= ~(1 << (bit))
#define sbi(port,bit) \
    (port) |= (1 << (bit))

constexpr inline volatile uint8_t* _portOutputRegister(uint8_t port);
constexpr inline uint8_t _digitalPinToTimer(uint8_t pin);
constexpr inline uint8_t _digitalPinToBitMask(uint8_t pin);
constexpr inline uint8_t _digitalPinToPort(uint8_t pin);
constexpr inline volatile uint8_t* _portInputRegister(uint8_t port);
constexpr inline volatile uint8_t* _portModeRegister(uint8_t pin);
constexpr uint8_t _analogPinToChannel(uint8_t pin);

#include "machew.hpp"

template <uint8_t timer> static inline void turnOffPWM();

template<uint8_t pin> inline int digitalRead();
template<uint8_t pin> inline void digitalWrite(boolean val);
template <uint8_t pin, uint8_t mode> void pinMode();

template<uint8_t pin> int analogRead();
template <uint8_t pin> void analogWrite(int val);

#endif //define(__MACHEW_H__)
