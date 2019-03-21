#pragma once
#include <avr/io.h>
#include <type_traits>
#include "architecture.h"
#include "interrupts.h"
#include "power.h"
#include "register_write_back.h"
/*
namespace machew {
    enum class adc_prescaler_mode : uint8_t {
        div2,
        div4,
        div8,
        div16,
        div32,
        div64,
        div128
    };

    enum class adc_auto_trigger_source : uint8_t {
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

    enum class adc_input_channel : uint8_t {
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

    enum class adc_voltage_reference : uint8_t {
        AREF,
        AVCC_with_cap,
        Internal_1_1V_with_cap
    };

    enum class adc_alignment : uint8_t {
        right,
        left
    };

    template<device<>::precision_mode _precision_mode = device<>::precision_mode::high>
    class adc {
    public:

        adc() {
            if constexpr ( _precision_mode == device<>::precision_mode::low ) {
                *this = adc_alignment::left;
            }
        }

        adc& operator= (const adc_alignment& align) {
            device<>::admux_t mux;
            mux = align;
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

        void start() const;

        bool read_ready() const;

        adc& operator= (adc_auto_trigger_source source) {
            if (source != adc_auto_trigger_source::disabled) {
                register_write_back<decltype(device<>::adcsrb)> reg;

                switch(source) {
                case adc_auto_trigger_source::free_running:
                    reg &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));
                break;
                case adc_auto_trigger_source::analog_comparator:
                    reg &= ~((1<<ADTS2)|(1<<ADTS1));
                    reg |= (1<<ADTS0);
                break;
                case adc_auto_trigger_source::external_interrupt0:
                    reg &= ~((1<<ADTS2)|(1<<ADTS0));
                    reg |= (1<<ADTS1);
                break;
                case adc_auto_trigger_source::timer0_compare_match_a:
                    reg &= ~(1<<ADTS2);
                    reg |= (1<<ADTS1)|(1<<ADTS0);
                break;
                case adc_auto_trigger_source::timer0_overflow:
                    reg &= ~((1<<ADTS1)|(1<<ADTS0));
                    reg |= (1<<ADTS2);
                break;
                case adc_auto_trigger_source::timer1_compare_match_b:
                    reg &= ~(1<<ADTS1);
                    reg |= (1<<ADTS2)|(1<<ADTS0);
                break;
                case adc_auto_trigger_source::timer1_overflow:
                    reg &= ~(1<<ADTS0);
                    reg |= (1<<ADTS2)|(1<<ADTS1);
                break;
                case adc_auto_trigger_source::timer1_capture_event:
                    reg |= (1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0);
                break;
                }
            }

            if (source != adc_auto_trigger_source::disabled) { // repeat conditional to save charge to stack space
                register_write_back<decltype(device<>::adcsra)> reg;
                reg &= ~(1<<ADIF); // Don't erase the interrupt flag by writing one
                reg |= (1<<ADATE);
            } else {
                register_write_back<decltype(device<>::adcsra)> reg;
                reg &= ~((1<<ADATE)|(1<<ADIF));
            }

            return *this;
        }

        adc& operator= (power::state state) {
            device<>::adcsra_t adcsra;
            adcsra = state;
            return *this;
        }

        adc& operator= (interrupt::state state) {
            register_write_back<decltype(device<>::adcsra)> reg;
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

        adc& operator= (const adc_input_channel& channel) const {
            device<>::adcsrb_t adcsrb;
            adcsrb = channel;
            return *this;
        }

        adc& operator= (const adc_prescaler_mode & mode) const {
            device<>::adcsra_t reg;
            reg = mode;
            return *this;
        }

        adc& operator= (const adc_voltage_reference & ref) {
            device<>::admux_t reg;
            reg = ref;
            return *this;
        }
    };

    struct device<>::adcsra_t {
        void start() const {
            register_write_back<decltype(device<>::adcsra)> reg;
            reg &= ~(1<<ADIF);
            reg |= (1<<ADSC);
        }

        bool read_ready() const {
            return *((volatile uint8_t*)device<>::adcsra) & (1<<ADIF);
        }

        adcsra_t& operator= (power::state state) {
            register_write_back<decltype(device<>::adcsra)> reg;

            reg &= ~(1<<ADIF);  // Need to clear the interrupt flag bit so we don't
                                // accidentally clear the interrupt
            if (state == power::state::enabled) {
                reg |= (1<<ADEN);
            } else if (state == power::state::disabled) {
                reg &= static_cast<uint8_t>(~(1<<ADEN));
            }
            return *this;
        }

        adcsra_t& operator= (const adc_prescaler_mode& mode) {
            register_write_back<decltype(device<>::adcsra)> reg;

            reg &= ~(1<<ADIF);

            if (mode == adc_prescaler_mode::div2) {
                reg &= ~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
            } else if (mode == adc_prescaler_mode::div4) {
                reg &= ~((1<<ADPS2)|(1<<ADPS1));
                reg |= (1<<ADPS0);
            } else if (mode == adc_prescaler_mode::div8) {
                reg &= ~((1<<ADPS2)|(1<<ADPS0));
                reg |= (1<<ADPS1);
            } else if (mode == adc_prescaler_mode::div16) {
                reg &= ~(1<<ADPS2);
                reg |= (1<<ADPS1)|(1<<ADPS0);
            } else if (mode == adc_prescaler_mode::div32) {
                reg &= ~(1<<ADPS1);
                reg |= (1<<ADPS2)|(1<<ADPS0);
            } else if (mode == adc_prescaler_mode::div64) {
                reg &= ~(1<<ADPS0);
                reg |= (1<<ADPS2)|(1<<ADPS1);
            } else if (mode == adc_prescaler_mode::div128) {
                reg |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
            }
            return *this;
        }
    };

    struct device<>::adcsrb_t {
        adcsrb_t& operator= (const adc_auto_trigger_source& source) {
            register_write_back<decltype(device<>::adcsrb)> reg;

            if (source == adc_auto_trigger_source::disabled) {
            } else if (source == adc_auto_trigger_source::free_running) {
                reg &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));
            } else if (source == adc_auto_trigger_source::analog_comparator) {
                reg &= ~((1<<ADTS2)|(1<<ADTS1));
                reg |= (1<<ADTS0);
            } else if (source == adc_auto_trigger_source::external_interrupt0) {
                reg &= ~((1<<ADTS2)|(1<<ADTS0));
                reg |= (1<<ADTS1);
            } else if (source == adc_auto_trigger_source::timer0_compare_match_a) {
                reg &= ~(1<<ADTS2);
                reg |= (1<<ADTS1)|(1<<ADTS0);
            } else if (source == adc_auto_trigger_source::timer0_overflow) {
                reg &= ~((1<<ADTS1)|(1<<ADTS0));
                reg |= (1<<ADTS2);
            } else if (source == adc_auto_trigger_source::timer1_compare_match_b) {
                reg &= ~(1<<ADTS1);
                reg |= (1<<ADTS2)|(1<<ADTS0);
            } else if (source == adc_auto_trigger_source::timer1_overflow) {
                reg &= ~(1<<ADTS0);
                reg |= (1<<ADTS2)|(1<<ADTS1);
            } else if (source == adc_auto_trigger_source::timer1_capture_event) {
                reg |= (1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0);
            }

            return *this;
        } 
    };

    struct device<>::admux_t {
        admux_t& operator= (const adc_voltage_reference& ref) {
            register_write_back<decltype(device<>::admux)> reg;

            if (ref == adc_voltage_reference::AREF) {
                reg &= ibit_value<uint8_t, REFS1, REFS0>();
            } else if (ref == adc_voltage_reference::AVCC_with_cap) {
                reg &= ibit_value<uint8_t, REFS1>();
                reg |= (1<<REFS0);
            } else if (ref == adc_voltage_reference::Internal_1_1V_with_cap) {
                reg |= (1<<REFS1)|(1<<REFS0);
            }

            return *this;
        }

        admux_t& operator= (const adc_input_channel& channel) {
            register_write_back<decltype(device<>::admux)> reg;

            if (channel == adc_input_channel::ADC0) {
                reg &= ~((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));
            } else if (channel == adc_input_channel::ADC1) {
                reg &= ~((1<<MUX3)|(1<<MUX2)|(1<<MUX1));
                reg |= (1<<MUX0);
            } else if (channel == adc_input_channel::ADC2) {
                reg &= ~((1<<MUX3)|(1<<MUX2)|(1<<MUX0));
                reg |= (1<<MUX1);
            } else if (channel == adc_input_channel::ADC3) {
                reg &= ~((1<<MUX3)|(1<<MUX2));
                reg |= (1<<MUX1)|(1<<MUX0);
            } else if (channel == adc_input_channel::ADC4) {
                reg &= ~((1<<MUX3)|(1<<MUX1)|(1<<MUX0));
                reg |= (1<<MUX2);
            } else if (channel == adc_input_channel::ADC5) {
                reg &= ~((1<<MUX3)|(1<<MUX1));
                reg |= (1<<MUX2)|(1<<MUX0);
            } else if (channel == adc_input_channel::ADC6) {
                reg &= ~((1<<MUX3)|(1<<MUX0));
                reg |= (1<<MUX2)|(1<<MUX1);
            } else if (channel == adc_input_channel::ADC7) {
                reg &= ~(1<<MUX3);
                reg |= (1<<MUX2)|(1<<MUX1)|(1<<MUX0);
            } else if (channel == adc_input_channel::Temperature) {
                reg &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
                reg |= (1<<MUX3);
            } else if (channel == adc_input_channel::Internal_1_1V) {
                reg &= ~(1<<MUX0);
                reg |= (1<<MUX3)|(1<<MUX2)|(1<<MUX1);
            } else if (channel == adc_input_channel::Ground) {
                reg |= (1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0);
            }

            return *this;
        }

        admux_t& operator= (const adc_alignment& align) {
            register_write_back<decltype(device<>::admux)> reg;
            if (align == adc_alignment::left) {
                reg |= bit_value<uint8_t, ADLAR>();
            } else if (align == adc_alignment::right) {
                reg &= ibit_value<uint8_t, ADLAR>();
            }
            return *this;
        }

    };

    template<device<>::precision_mode _precision_mode>
    inline void adc<_precision_mode>::start() const {
        device<>::adcsra_t adcsra;
        adcsra.start();
    }

    template<device<>::precision_mode _precision_mode>
    inline bool adc<_precision_mode>::read_ready() const {
        device<>::adcsra_t adcsra;
        return adcsra.read_ready();
    }
}
*/
