#ifndef __MACHEW_HPP__
#define __MACHEW_HPP__

extern uint8_t analog_reference;

template <uint8_t timer> static inline void turnOffPWM()
{
    switch (timer)
    {
        #if defined(TCCR1A) && defined(COM1A1)
        case TIMER1A:   cbi(TCCR1A, COM1A1);    break;
        #endif
        #if defined(TCCR1A) && defined(COM1B1)
        case TIMER1B:   cbi(TCCR1A, COM1B1);    break;
        #endif

        #if defined(TCCR2) && defined(COM21)
        case  TIMER2:   cbi(TCCR2, COM21);      break;
        #endif

        #if defined(TCCR0A) && defined(COM0A1)
        case  TIMER0A:  cbi(TCCR0A, COM0A1);    break;
        #endif

        #if defined(TIMER0B) && defined(COM0B1)
        case  TIMER0B:  cbi(TCCR0A, COM0B1);    break;
        #endif
        #if defined(TCCR2A) && defined(COM2A1)
        case  TIMER2A:  cbi(TCCR2A, COM2A1);    break;
        #endif
        #if defined(TCCR2A) && defined(COM2B1)
        case  TIMER2B:  cbi(TCCR2A, COM2B1);    break;
        #endif

        #if defined(TCCR3A) && defined(COM3A1)
        case  TIMER3A:  cbi(TCCR3A, COM3A1);    break;
        #endif
        #if defined(TCCR3A) && defined(COM3B1)
        case  TIMER3B:  cbi(TCCR3A, COM3B1);    break;
        #endif
        #if defined(TCCR3A) && defined(COM3C1)
        case  TIMER3C:  cbi(TCCR3A, COM3C1);    break;
        #endif

        #if defined(TCCR4A) && defined(COM4A1)
        case  TIMER4A:  cbi(TCCR4A, COM4A1);    break;
        #endif
        #if defined(TCCR4A) && defined(COM4B1)
        case  TIMER4B:  cbi(TCCR4A, COM4B1);    break;
        #endif
        #if defined(TCCR4A) && defined(COM4C1)
        case  TIMER4C:  cbi(TCCR4A, COM4C1);    break;
        #endif
        #if defined(TCCR4C) && defined(COM4D1)
        case TIMER4D:   cbi(TCCR4C, COM4D1);    break;
        #endif

        #if defined(TCCR5A)
        case  TIMER5A:  cbi(TCCR5A, COM5A1);    break;
        case  TIMER5B:  cbi(TCCR5A, COM5B1);    break;
        case  TIMER5C:  cbi(TCCR5A, COM5C1);    break;
        #endif
    }
}

template<uint8_t pin> inline int digitalRead()
{
    constexpr uint8_t timer = _digitalPinToTimer(pin);
    constexpr uint8_t bit = _digitalPinToBitMask(pin);
    constexpr uint8_t port = _digitalPinToPort(pin);

    if (port == NOT_A_PIN) return LOW;

    // If the pin that support PWM output, we need to turn it off
    // before getting a digital reading.
    if (timer != NOT_ON_TIMER) turnOffPWM<timer>();

    if (*_portInputRegister(port) & bit) return HIGH;
    return LOW;
}

template<uint8_t pin> inline void digitalWrite(boolean val) {
    constexpr uint8_t timer = _digitalPinToTimer(pin);
    constexpr uint8_t bit = _digitalPinToBitMask(pin);
    constexpr uint8_t port = _digitalPinToPort(pin);
    volatile uint8_t *out = _portOutputRegister(port);

    if (port == NOT_A_PIN) return;

    // If the pin that support PWM output, we need to turn it off
    // before doing a digital write.
    if (timer != NOT_ON_TIMER) turnOffPWM<timer>();

    uint8_t oldSREG = SREG;
    cli();

    if (val == LOW) {
        *out &= ~bit;
    } else {
        *out |= bit;
    }

    SREG = oldSREG;
}

template <uint8_t pin, uint8_t mode> void pinMode()
{
    constexpr uint8_t bit = _digitalPinToBitMask(pin);
    constexpr uint8_t port = _digitalPinToPort(pin);
    volatile uint8_t *reg = _portModeRegister(port);
    volatile uint8_t *out = _portOutputRegister(port);

    if (port == NOT_A_PIN) return;

    uint8_t oldSREG = SREG;
    cli();

    switch(mode) {
    case INPUT:
        *reg &= ~bit;
        *out &= ~bit;
        break;
    case INPUT_PULLUP:
        *reg &= ~bit;
        *out |= bit;
        break;
    case OUTPUT:
        *reg |= bit;
        break;
    }

    SREG = oldSREG;
}

template<uint8_t pin> int analogRead()
{
    uint8_t low, high;
    constexpr uint8_t chan = _analogPinToChannel(pin);

#if defined(__AVR_ATmega32U4__)
    ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((chan >> 3) & 0x01) << MUX5);
#elif defined(ADCSRB) && defined(MUX5)
    // the MUX5 bit of ADCSRB selects whether we're reading from channels
    // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
    ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((chan >> 3) & 0x01) << MUX5);
#endif

    // set the analog reference (high two bits of ADMUX) and select the
    // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
    // to 0 (the default).
#if defined(ADMUX)
    ADMUX = (analog_reference << 6) | (chan & 0x07);
#endif

#if defined(ADCSRA) && defined(ADCL)
    // start the conversion
    sbi(ADCSRA, ADSC);

    // ADSC is cleared when the conversion finishes
    while (bit_is_set(ADCSRA, ADSC));
    // we have to read ADCL first; doing so locks both ADCL
    // and ADCH until ADCH is read.  reading ADCL second would
    // cause the results of each conversion to be discarded,
    // as ADCL and ADCH would be locked when it completed.
    low  = ADCL;
    high = ADCH;
#else
    // we dont have an ADC, return 0
    low  = 0;
    high = 0;
#endif
    // combine the two bytes
    return (high << 8) | low;
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
template <uint8_t pin> void analogWrite(int val)
{
	// We need to make sure the PWM output is enabled for those pins
	// that support it, as we turn it off when digitally reading or
	// writing with them.  Also, make sure the pin is in output mode
	// for consistenty with Wiring, which doesn't require a pinMode
	// call for the analog output pins.
	pinMode<pin,OUTPUT>();
	if (val == 0)
	{
		digitalWrite<pin>(LOW);
	}
	else if (val == 255)
	{
		digitalWrite<pin>(HIGH);
	}
	else
	{
		switch(_digitalPinToTimer(pin))
		{
			// XXX fix needed for atmega8
			#if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
			case TIMER0A:
				// connect pwm to pin on timer 0
				sbi(TCCR0, COM00);
				OCR0 = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR0A) && defined(COM0A1)
			case TIMER0A:
				// connect pwm to pin on timer 0, channel A
				sbi(TCCR0A, COM0A1);
				OCR0A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR0A) && defined(COM0B1)
			case TIMER0B:
				// connect pwm to pin on timer 0, channel B
				sbi(TCCR0A, COM0B1);
				OCR0B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR1A) && defined(COM1A1)
			case TIMER1A:
				// connect pwm to pin on timer 1, channel A
				sbi(TCCR1A, COM1A1);
				OCR1A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR1A) && defined(COM1B1)
			case TIMER1B:
				// connect pwm to pin on timer 1, channel B
				sbi(TCCR1A, COM1B1);
				OCR1B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR2) && defined(COM21)
			case TIMER2:
				// connect pwm to pin on timer 2
				sbi(TCCR2, COM21);
				OCR2 = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR2A) && defined(COM2A1)
			case TIMER2A:
				// connect pwm to pin on timer 2, channel A
				sbi(TCCR2A, COM2A1);
				OCR2A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR2A) && defined(COM2B1)
			case TIMER2B:
				// connect pwm to pin on timer 2, channel B
				sbi(TCCR2A, COM2B1);
				OCR2B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR3A) && defined(COM3A1)
			case TIMER3A:
				// connect pwm to pin on timer 3, channel A
				sbi(TCCR3A, COM3A1);
				OCR3A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR3A) && defined(COM3B1)
			case TIMER3B:
				// connect pwm to pin on timer 3, channel B
				sbi(TCCR3A, COM3B1);
				OCR3B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR3A) && defined(COM3C1)
			case TIMER3C:
				// connect pwm to pin on timer 3, channel C
				sbi(TCCR3A, COM3C1);
				OCR3C = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR4A)
			case TIMER4A:
				//connect pwm to pin on timer 4, channel A
				sbi(TCCR4A, COM4A1);
				#if defined(COM4A0)		// only used on 32U4
				cbi(TCCR4A, COM4A0);
				#endif
				OCR4A = val;	// set pwm duty
				break;
			#endif

			#if defined(TCCR4A) && defined(COM4B1)
			case TIMER4B:
				// connect pwm to pin on timer 4, channel B
				sbi(TCCR4A, COM4B1);
				OCR4B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR4A) && defined(COM4C1)
			case TIMER4C:
				// connect pwm to pin on timer 4, channel C
				sbi(TCCR4A, COM4C1);
				OCR4C = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR4C) && defined(COM4D1)
			case TIMER4D:
				// connect pwm to pin on timer 4, channel D
				sbi(TCCR4C, COM4D1);
				#if defined(COM4D0)		// only used on 32U4
				cbi(TCCR4C, COM4D0);
				#endif
				OCR4D = val;	// set pwm duty
				break;
			#endif

			#if defined(TCCR5A) && defined(COM5A1)
			case TIMER5A:
				// connect pwm to pin on timer 5, channel A
				sbi(TCCR5A, COM5A1);
				OCR5A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR5A) && defined(COM5B1)
			case TIMER5B:
				// connect pwm to pin on timer 5, channel B
				sbi(TCCR5A, COM5B1);
				OCR5B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR5A) && defined(COM5C1)
			case TIMER5C:
				// connect pwm to pin on timer 5, channel C
				sbi(TCCR5A, COM5C1);
				OCR5C = val; // set pwm duty
				break;
			#endif

			case NOT_ON_TIMER:
			default:
				if (val < 128) {
					digitalWrite<pin>(LOW);
				} else {
					digitalWrite<pin>(HIGH);
				}
		}
	}
}

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560)
constexpr uint8_t _analogPinToChannel(uint8_t pin) {
    return (pin>54?pin-54:pin);
}

constexpr inline volatile uint8_t* _portOutputRegister(uint8_t port) {
    return (port== 1?&PORTA: port== 2?&PORTB:
            port== 3?&PORTC: port== 4?&PORTD:
            port== 5?&PORTE: port== 6?&PORTF:
            port== 7?&PORTG: port== 8?&PORTH:
            port==10?&PORTJ: port==11?&PORTK:
            port==12?&PORTL:NOT_A_PORT);
}

constexpr inline uint8_t _digitalPinToTimer(uint8_t pin) {
    return (pin== 2?TIMER3B: pin== 3?TIMER3C: pin== 4?TIMER0B:
            pin== 5?TIMER3A: pin== 6?TIMER4A: pin== 7?TIMER4B:
            pin== 8?TIMER4C: pin== 9?TIMER2B: pin==10?TIMER2A:
            pin==11?TIMER1A: pin==12?TIMER1B: pin==13?TIMER0A:
            pin==44?TIMER5C: pin==45?TIMER5B: pin==46?TIMER5A:
            NOT_ON_TIMER);
}

constexpr inline uint8_t _digitalPinToBitMask(uint8_t pin) {
    return (pin== 0||pin==15||pin==17||pin==21||
            pin==22||pin==37||pin==41||pin==49||
            pin==53||pin==54||pin==62?_BV(0):
            pin== 1||pin==14||pin==16||pin==20||
            pin==23||pin==36||pin==40||pin==48||
            pin==52||pin==55||pin==63?_BV(1):
            pin==19||pin==24||pin==35||pin==39||
            pin==47||pin==51||pin==56||pin==64?_BV(2):
            pin== 5||pin== 6||pin==18||pin==25||
            pin==34||pin==46||pin==50||pin==57||
            pin==65?_BV(3):
            pin== 2||pin== 7||pin==10||pin==26||
            pin==33||pin==45||pin==58||pin==66?_BV(4):
            pin== 3||pin== 4||pin== 8||pin==11||
            pin==27||pin==32||pin==44||pin==59||
            pin==67?_BV(5):
            pin== 9||pin==12||pin==28||pin==31||
            pin==43||pin==60||pin==68?_BV(6):_BV(7));
}

constexpr inline uint8_t _digitalPinToPort(uint8_t pin) {
    return (pin== 0?PE: pin== 1?PE: pin== 2?PE:
            pin== 3?PE: pin== 4?PG: pin== 5?PE:
            pin== 6?PH: pin== 7?PH: pin== 8?PH:
            pin== 9?PH: pin==10?PB: pin==11?PB:
            pin==12?PB: pin==13?PB: pin==14?PJ:
            pin==15?PJ: pin==16?PH: pin==17?PH:
            pin==18?PD: pin==19?PD: pin==20?PD:
            pin==21?PD: pin==22?PA: pin==23?PA:
            pin==24?PA: pin==25?PA: pin==26?PA:
            pin==27?PA: pin==28?PA: pin==29?PA:
            pin==30?PC: pin==31?PC: pin==32?PC:
            pin==33?PC: pin==34?PC: pin==35?PC:
            pin==36?PC: pin==37?PC: pin==38?PD:
            pin==39?PG: pin==40?PG: pin==41?PG:
            pin==42?PL: pin==43?PL: pin==44?PL:
            pin==45?PL: pin==46?PL: pin==47?PL:
            pin==48?PL: pin==49?PL: pin==50?PB:
            pin==51?PB: pin==52?PB: pin==53?PB:
            pin==54?PF: pin==55?PF: pin==56?PF:
            pin==57?PF: pin==58?PF: pin==59?PF:
            pin==60?PF: pin==61?PF: pin==62?PK:
            pin==63?PK: pin==64?PK: pin==65?PK:
            pin==66?PK: pin==67?PK: pin==68?PK:PK);
}

constexpr inline volatile uint8_t* _portInputRegister(uint8_t port) {
    return (port== 1?&PINA: port== 2?&PINB: port== 3?&PINC:
            port== 4?&PIND: port== 5?&PINE: port== 6?&PINF:
            port== 7?&PING: port== 8?&PINH: port==10?&PINJ:
            port==11?&PINK: port==12?&PINL: (uint8_t*)NOT_A_PIN);
}

constexpr inline volatile uint8_t* _portModeRegister(uint8_t pin) {
    return (pin== 1?&DDRA: pin== 2?&DDRB: pin== 3?&DDRC:
            pin== 4?&DDRD: pin== 5?&DDRE: pin== 6?&DDRF:
            pin== 7?&DDRG: pin== 8?&DDRH: pin==10?&DDRJ:
            pin==11?&DDRK: pin==12?&DDRL: (uint8_t*)NOT_A_PORT);
}

#endif
#endif //define(__MACHEW_HPP__)
