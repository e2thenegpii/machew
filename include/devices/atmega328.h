#pragma once

#include "../interrupts.h"
#include "../register_write_back.h"

namespace machew {
    template<> struct device<supported_device::atmega328> {

        enum class precision_mode : uint8_t {
            low,
            high
        };

        enum class port {
            B,
            C,
            D
        };

        enum class uart {
            zero
        };

        static register_t<uint8_t, 0x23> pinb;
        static register_t<uint8_t, 0x24> ddrb;
        static register_t<uint8_t, 0x25> portb;
        static register_t<uint8_t, 0x26> pinc;
        static register_t<uint8_t, 0x27> ddrc;
        static register_t<uint8_t, 0x28> portc;
        static register_t<uint8_t, 0x29> pind;
        static register_t<uint8_t, 0x2A> ddrd;
        static register_t<uint8_t, 0x2B> portd;
        static register_t<uint8_t, 0x35> tifr0;
        static register_t<uint8_t, 0x36> tifr1;
        static register_t<uint8_t, 0x37> tifr2;
        static register_t<uint8_t, 0x3B> pcifr;
        static register_t<uint8_t, 0x3C> eifr;
        static register_t<uint8_t, 0x3D> eimsk;
        static register_t<uint8_t, 0x3E> gpior0;
        static register_t<uint8_t, 0x3F> eecr;
        static register_t<uint8_t, 0x40> eedr;
        static register_t<uint16_t, 0x41> eear;
        static register_t<uint8_t, 0x43> gtccr;
        static register_t<uint8_t, 0x44> tccr0a;
        static register_t<uint8_t, 0x45> tccr0b;
        static register_t<uint8_t, 0x46> tcnt0;
        static register_t<uint8_t, 0x47> ocr0a;
        static register_t<uint8_t, 0x48> ocr0b;
        static register_t<uint8_t, 0x4A> gpior1;
        static register_t<uint8_t, 0x4B> gpior2;
        static register_t<uint8_t, 0x4C> spcr;
        static register_t<uint8_t, 0x4D> spsr;
        static register_t<uint8_t, 0x4E> spdr;
        static register_t<uint8_t, 0x50> acsr;
        static register_t<uint8_t, 0x51> dwdr;
        static register_t<uint8_t, 0x53> smcr;
        static register_t<uint8_t, 0x54> mcusr;
        static register_t<uint8_t, 0x55> mcucr;
        static register_t<uint8_t, 0x57> spmcsr;
        static register_t<uint16_t, 0x5D> sp;
        static register_t<uint8_t, 0x5F> sreg;
        static register_t<uint8_t, 0x60> wdtcsr;
        static register_t<uint8_t, 0x61> clkpr;
        static register_t<uint8_t, 0x64> prr;
        static register_t<uint8_t, 0x66> osccal;

        static register_t<uint8_t, 0x68> pcicr;
        static register_t<uint8_t, 0x69> eicra;
        static register_t<uint8_t, 0x6B> pcmsk0;
        static register_t<uint8_t, 0x6C> pcmsk1;
        static register_t<uint8_t, 0x6D> pcmsk2;
        static register_t<uint8_t, 0x6E> timsk0;
        static register_t<uint8_t, 0x6F> timsk1;
        static register_t<uint8_t, 0x70> timsk2;
        static register_t<uint16_t, 0x78> adc;
        static register_t<uint8_t, 0x78> adcl;
        static register_t<uint8_t, 0x79> adch;
        static register_t<uint8_t, 0x7A> adcsra;
        static register_t<uint8_t, 0x7B> adcsrb;
        static register_t<uint8_t, 0x7C> admux;
        static register_t<uint8_t, 0x7E> didr0;
        static register_t<uint8_t, 0x7F> didr1;
        static register_t<uint8_t, 0x80> tccr1a;
        static register_t<uint8_t, 0x81> tccr1b;
        static register_t<uint8_t, 0x82> tccr1c;
        static register_t<uint16_t, 0x84> tcnt1;
        static register_t<uint8_t, 0x84> tcnt1l;
        static register_t<uint16_t, 0x86> icr1;
        static register_t<uint8_t, 0x86> icr1l;
        static register_t<uint16_t, 0x88> ocr1a;
        static register_t<uint8_t, 0x88> ocr1al;
        static register_t<uint16_t, 0x8A> ocr1b;
        static register_t<uint8_t, 0x8A> ocr1bl;
        static register_t<uint8_t, 0xB0> tccr2a;
        static register_t<uint8_t, 0xB1> tccr2b;
        static register_t<uint8_t, 0xB2> tcnt2;
        static register_t<uint8_t, 0xB3> ocr2a;
        static register_t<uint8_t, 0xB4> ocr2b;
        static register_t<uint8_t, 0xB6> assr;
        static register_t<uint8_t, 0xB8> twbr;
        static register_t<uint8_t, 0xB9> twsr;
        static register_t<uint8_t, 0xBA> twar;
        static register_t<uint8_t, 0xBB> twdr;
        static register_t<uint8_t, 0xBC> twcr;
        static register_t<uint8_t, 0xBD> twamr;
        static register_t<uint8_t, 0xC0> ucsr0a;
        static register_t<uint8_t, 0xC1> ucsr0b;
        static register_t<uint8_t, 0xC2> ucsr0c;
        static register_t<uint16_t, 0xC4> ubrr0;
        static register_t<uint8_t, 0xC4> ubrr0l;
        static register_t<uint8_t, 0xC6> udr0;

        template<port p> static constexpr uintptr_t get_pin_reg();
        template<port p> static constexpr uintptr_t get_ddr_reg();
        template<port p> static constexpr uintptr_t get_port_reg();
        template<port p> static constexpr uintptr_t get_pcmsk_reg();

        template<uart u> static constexpr uintptr_t get_controla_reg();
        template<uart u> static constexpr uintptr_t get_controlb_reg();
        template<uart u> static constexpr uintptr_t get_controlc_reg();
        template<uart u> static constexpr uintptr_t get_baud_rate_reg();
        template<uart u> static constexpr uintptr_t get_data_reg();
    
        template <port... ports> struct pcicr_t;
        template <port... ports> struct pcifr_t;
        struct spcr_t;
        struct spsr_t;
        struct admux_t;
        struct adcsra_t;
        struct adcsrb_t;
        struct ucsra_t;
        struct ucsrb_t;
        struct ucsrc_t;
        struct ubrr_t;
        struct udr_t;
        struct eecr_t;
    };

    using port = device<>::port;    
    using uart = device<>::uart;

    template<device<>::port p>
    constexpr uintptr_t device<>::get_pin_reg() {
        if constexpr (p == port::B) {
            return pinb;
        } else if constexpr (p == port::C) {
            return pinc;
        } else if constexpr (p == port::D) {
            return pind;
        }
    }

    template<device<>::port p>
    constexpr uintptr_t device<>::get_ddr_reg() {
        if constexpr (p == port::B) {
            return ddrb;
        } else if constexpr (p == port::C) {
            return ddrc;
        } else if constexpr (p == port::D) {
            return ddrd;
        }
    }

    template<device<>::port p>
    constexpr uintptr_t device<>::get_port_reg() {
        if constexpr (p == port::B) {
            return portb;
        } else if constexpr (p == port::C) {
            return portc;
        } else if constexpr (p == port::D) {
            return portd;
        }
    }

    template<device<>::port p>
    constexpr uintptr_t device<>::get_pcmsk_reg() {
        if constexpr (p == port::B) {
            return pcmsk0;
        } else if constexpr (p == port::C) {
            return pcmsk1;
        } else if constexpr (p == port::D) {
            return pcmsk2;
        }
    }

    template<device<>::uart u>
    constexpr uintptr_t device<>::get_controla_reg() {
        if constexpr (u == uart::zero) {
            return ucsr0a;
        }
    }

    template<device<>::uart u>
    constexpr uintptr_t device<>::get_controlb_reg() {
        if constexpr (u == uart::zero) {
            return ucsr0b;
        }
    }

    template<device<>::uart u>
    constexpr uintptr_t device<>::get_controlc_reg() {
        if constexpr (u == uart::zero) {
            return ucsr0c;
        }
    }

    template<device<>::uart u>
    constexpr uintptr_t device<>::get_baud_rate_reg() {
        if constexpr (u == uart::zero) {
            return ubrr0;
        }
    }

    template<device<>::uart u>
    constexpr uintptr_t device<>::get_data_reg() {
        if constexpr (u == uart::zero) {
            return udr0;
        }
    }
    
    template <device<>::port... ports>
    struct device<>::pcicr_t {
        pcicr_t& operator= (const interrupt::state& state) {
            register_write_back<decltype(pcicr)::value_type, decltype(pcicr)::address> reg;

            for( const auto & p : {ports...}) {
                if (p == port::B) {
                    if (state == interrupt::state::enabled) {
                        reg |= (1<<PCIE0);
                    } else if (state == interrupt::state::disabled) {
                        reg &= ~(1<<PCIE0);
                    }
                } else if (p == port::C) {
                    if (state == interrupt::state::enabled) {
                        reg |= (1<<PCIE1);
                    } else if (state == interrupt::state::disabled) {
                        reg &= ~(1<<PCIE1);
                    }
                } else if (p == port::D) {
                    if (state == interrupt::state::enabled) {
                        reg |= (1<<PCIE2);
                    } else if (state == interrupt::state::disabled) {
                        reg &= ~(1<<PCIE2);
                    }
                }
            }

            return *this;
        }
    };

    template <device<>::port... ports>
    struct device<>::pcifr_t {
        void clear_interrupt() {
            register_write_back<decltype(pcifr)::value_type, decltype(pcifr)::address> reg;

            for( const auto & p : {ports...}) {
                if (p == port::B) {
                    reg |= (1<<PCIF0);
                } else if (p == port::C) {
                    reg |= (1<<PCIF1);
                } else if (p == port::D) {
                    reg |= (1<<PCIF2);
                }
            }
        }
    };
}
