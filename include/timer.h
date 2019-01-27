#pragma once

#include <avr/pgmspace.h>
#include "architecture.h"

namespace machew {

    enum class prescaler {
        stopped,
        div1,
        div8,
        div64,
        div256,
        div1024,
        falling,
        rising
    };

    template<timer_t t>
    struct timer {
        enum class channel_t {
            A,
            B
        };

        enum class compare_output_mode {
            normal,
            toggle,
            clear,
            set
        };

        template<channel_t c> class channel {}

        template<> class channel<channel_t::A> {
            channel<channel_t::A> operator= (const compare_output_mode& com) {
                uint8_t sreg = SREG;
                cli();
                uint8_t controla = *registers.controla;
                controla &= ~((COM0
                switch(com) {
                    case normal:
                    break;
                    case toggle:
                    break;
                    case clear:
                    break;
                    case set:
                    break;
                }
                SREG = sreg;
            }
            return *this;
        };

        template<> class channel<channel_t::B> {
        };

        enum class flags_t {
            A,
            B,
            overflow
        };


        timer<t>& operator=(prescaler p) {
            *registers.controlb &= 0xF8;
            switch(p) {
                case stopped:
                break;
                case div1:
                *registers.controlb |= 0x01;
                break;
                case div8:
                *registers.controlb |= 0x02;
                break;
                case div64:
                *registers.controlb |= 0x03;
                break;
                case div256:
                *registers.controlb |= 0x04;
                break;
                case div1024:
                *registers.controlb |= 0x05;
                break;
                case falling:
                *registers.controlb |= 0x06;
                break;
                case rising:
                *registers.controlb |= 0x07
            }
        }

        enum class waveform_generation_mode {
            normal,
            phase_correct_pwm, // 0xFF as top
            clear_on_compare, // CTC mode
            fast_pwm,
            phase_correct_pwm_oc, // OCR0
            fast_pwm_oc
        };

        timer<t>& operator=(waveform_generation_mode w) {
            uint8_t status = SREG;
            cli();
            uint8_t controla = *registers.controla;
            uint8_t controlb = *registers.controlb;

            controla &= ~((1<<WGM01)|(1<<WGM00));
            controlb &= ~(1<<WGM02);
            switch(w) {
                case normal:
                break;
                case pwm_phase_correct:
                controla |= (1<<WGM00);
                break;
                case clear_on_compare:
                controla |= (1<<WGM01);
                break;
                case fast_pwm:
                controla |= (1<<WGM00)|(1<<WGM01);
                break;
                case phase_correct_pwm_oc:
                controla |= (1<<WGM00);
                controlb |= (1<<WGM02);
                break;
                case fast_pwm_oc:
                controla |= (1<<WGM00)|(1<<WGM01);
                controlb |= (1<<WGM02);
            }

            *registers.controla = controla;
            *registers.controlb = controlb;
            SREG = status;
        }



    private:
        static constexpr timer_registers<timer_type::bit8> registers = get_timer_registers<t>();
    }

    template<>
    struct timer<timer_t::one> {
        enum class flags_t {
            A,
            B,
            overflow,
            capture
        };

        timer& operator=(prescaler p) {
            *registers.controlb &= 0xF8;
            switch(p) {
                case stopped:
                break;
                case div1:
                *registers.controlb |= 0x01;
                break;
                case div8:
                *registers.controlb |= 0x02;
                break;
                case div64:
                *registers.controlb |= 0x03;
                break;
                case div256:
                *registers.controlb |= 0x04;
                break;
                case div1024:
                *registers.controlb |= 0x05;
                break;
                case falling:
                *registers.controlb |= 0x06;
                break;
                case rising:
                *registers.controlb |= 0x07
            }
        }

    private:
        static constexpr timer_registers<timer_type::bit16> registers = get_timer_registers<timer::one>();
    }
};
