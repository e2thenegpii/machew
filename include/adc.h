#pragma once
#include <avr/io.h>
#include <EASTL/type_traits.h>
#include "architecture.h"
#include "interrupts.h"
#include "power.h"
#include "register_write_back.h"

namespace machew {
    template<device<>::precision_mode _precision_mode = device<>::precision_mode::high>
    class adc {
    public:

        enum class voltage_reference : uint8_t {
            AREF,
            AVCC_with_cap,
            Internal_1_1V_with_cap
        };

        enum class input_channel : uint8_t {
            ADC0,
            ADC1,
            ADC2,
            ADC3,
            ADC4,
            ADC5,
            ADC6,
            ADC7,
            Temperature,
            Internal_1_1V,
            Ground
        };

        enum class auto_trigger_source : uint8_t {
            disabled,
            free_running,
            analog_comparator,
            external_interrupt0,
            timer0_compare_match_a,
            timer0_overflow,
            timer1_compare_match_b,
            timer1_overflow,
            timer1_capture_event
        };

        enum class prescaler_mode : uint8_t {
            div2,
            div4,
            div8,
            div16,
            div32,
            div64,
            div128
        };

        enum class alignment : uint8_t {
            right,
            left
        };

        adc() {
            if constexpr ( _precision_mode == device<>::precision_mode::low ) {
                *this = alignment::left;
            }
        }

        adc& operator= (const alignment& align) {
            register_write_back<uint8_t, device<>::admux> reg;
            if (align == alignment::left) {
                reg |= bit_value<uint8_t, ADLAR>();
            } else if (align == alignment::right) {
                reg &= ibit_value<uint8_t, ADLAR>();
            }
            return *this;
        }

        template<
            device<>::precision_mode mode = _precision_mode,
            typename = typename std::enable_if<mode == device<>::precision_mode::high>::type>
        operator uint16_t() const {
            return *((volatile uint16_t*)device<>::adc);
        }

        template<
            device<>::precision_mode mode = _precision_mode,
            typename = typename std::enable_if<mode == device<>::precision_mode::low>::type>
        operator uint8_t() const {
            return *((volatile uint8_t*)device<>::adch);
        }

        template<
            device<>::precision_mode mode = _precision_mode,
            typename = typename std::enable_if<mode == device<>::precision_mode::high>::type>
        uint16_t read_async() const {
            return *this;
        }

        template<
            device<>::precision_mode mode = _precision_mode,
            typename = typename std::enable_if<mode == device<>::precision_mode::low>::type>
        uint8_t read_async() const {
            return *this;
        }

        template<
            device<>::precision_mode mode = _precision_mode,
            typename = typename std::enable_if<mode == device<>::precision_mode::high>::type>
        uint16_t read() const {
            while(!read_ready());
            return read_async();
        }

        template<
            device<>::precision_mode mode = _precision_mode,
            typename = typename std::enable_if<mode == device<>::precision_mode::low>::type>
        uint8_t read() const {
            while(!read_ready());
            return read_async();
        }

        void start() const {
            register_write_back<uint8_t, device<>::adcsra> reg;
            reg |= (1<<ADSC);
        }

        bool read_ready() const {
            return *((volatile uint8_t*)device<>::adcsra) & (1<<ADIF);
        }

        adc& operator= (auto_trigger_source source) {
            if (source != auto_trigger_source::disabled) {
                register_write_back<uint8_t, device<>::adcsrb> reg;

                switch(source) {
                case auto_trigger_source::free_running:
                    reg &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));
                break;
                case auto_trigger_source::analog_comparator:
                    reg &= ~((1<<ADTS2)|(1<<ADTS1));
                    reg |= (1<<ADTS0);
                break;
                case auto_trigger_source::external_interrupt0:
                    reg &= ~((1<<ADTS2)|(1<<ADTS0));
                    reg |= (1<<ADTS1);
                break;
                case auto_trigger_source::timer0_compare_match_a:
                    reg &= ~(1<<ADTS2);
                    reg |= (1<<ADTS1)|(1<<ADTS0);
                break;
                case auto_trigger_source::timer0_overflow:
                    reg &= ~((1<<ADTS1)|(1<<ADTS0));
                    reg |= (1<<ADTS2);
                break;
                case auto_trigger_source::timer1_compare_match_b:
                    reg &= ~(1<<ADTS1);
                    reg |= (1<<ADTS2)|(1<<ADTS0);
                break;
                case auto_trigger_source::timer1_overflow:
                    reg &= ~(1<<ADTS0);
                    reg |= (1<<ADTS2)|(1<<ADTS1);
                break;
                case auto_trigger_source::timer1_capture_event:
                    reg |= (1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0);
                break;
                }
            }

            if (source != auto_trigger_source::disabled) { // repeat conditional to save charge to stack space
                register_write_back<uint8_t, device<>::adcsra> reg;
                reg &= ~(1<<ADIF); // Don't erase the interrupt flag by writing one
                reg |= (1<<ADATE);
            } else {
                register_write_back<uint8_t, device<>::adcsra> reg;
                reg &= ~((1<<ADATE)|(1<<ADIF));
            }

            return *this;
        }

        adc& operator= (power::state state) {
            register_write_back<uint8_t, device<>::adcsra> reg;
            reg &= ~(1<<ADIF);  // Need to clear the interrupt flag bit so we don't
                                // accidentally clear the interrupt
            if (state == power::state::enabled) {
                reg |= (1<<ADEN);
            } else if (state == power::state::disabled) {
                reg &= ~(1<<ADEN);
            }
            return *this;
        }

        adc& operator= (interrupt::state state) {
            register_write_back<uint8_t, device<>::adcsra> reg;
            reg &= ~(1<<ADIF);  // Need to clear the interrupt flag bit so we don't
                                // accidentally clear the interrupt
            if (state == interrupt::state::enabled) {
                reg |= (1<<ADIE);
            } else if (state == interrupt::state::disabled) {
                reg &= ~(1<<ADIE);
            }
            return *this;
        }

        void clear_interrupt() const {
            *((volatile uint8_t*)device<>::adcsra) |= (1<<ADIF); 
        }

        adc& operator= (const input_channel& channel) const {
            //TODO implement me
        }

        adc& operator= (const prescaler_mode & mode) const {
            //TODO implement me
        }

        adc& operator= (const voltage_reference & ref) {
            register_write_back<uint8_t, device<>::admux> reg;
            if(ref == voltage_reference::AREF) {
                reg &= ~((1<<REFS1)|(1<<REFS0));
            } else if(ref == voltage_reference::AVCC_with_cap) {
                reg |= (1<<REFS0);
                reg &= ~(1<<REFS1);
            } else if(ref == voltage_reference::Internal_1_1V_with_cap) {
                reg |= (1<<REFS1)|(1<<REFS0);
            }
            return *this;
        }
    };
};
