#pragma once
#include <avr/io.h>
#include "architecture.h"
#include "interrupts.h"
#include "power.h"
#include "register_write_back.h"

namespace machew {

    template<device<>::uart u, uint8_t bits=8>
    class usart {
    private:
        static_assert(bits <= 9 && bits >= 5, "usart only supports [5, 9] bits"); 
        enum class component {
            rx,
            tx
        };

        template<component c> class channel;

        usart& operator= (const power::state & state) {
            register_write_back<uint8_t, device<>::get_controlb_reg<u>()> reg;
            if (state == power::state::enabled) {
                reg |= (1<<TXEN0)|(1<<RXEN0);
            } else if (state == power::state::disabled) {
                reg &= ~((1<<TXEN0)|(1<<RXEN0));
            }

            return *this;
        }

        usart& operator= (const interrupt::state& state) {
            register_write_back<uint8_t, device<>::get_controlb_reg<u>()> reg;
            if (state == interrupt::state::enabled) {
                reg |= (1<<TXCIE0)|(1<<RXCIE0);
            } else if (state == interrupt::state::disabled) {
                reg &= ~((1<<TXCIE0)|(1<<RXCIE0));
            }

            return *this;
        }

        static void config_bitwidth_c() {
            register_write_back<uint8_t, device<>::get_controlc_reg<u>()> reg;
            switch(bits) {
            case 5:
                reg &= ~((1<<UCSZ00)|(1<<UCSZ01));
            break;
            case 6:
                reg |= (1<<UCSZ00);
                reg &= ~(1<<UCSZ01);
            break;
            case 7:
                reg &= ~(1<<UCSZ00);
                reg |= (1<<UCSZ01);
            break;
            case 8:
            case 9:
                reg |= (1<<UCSZ00)|(1<<UCSZ01);
            break;
            }
        }

        static void config_bitwidth_b() {
            register_write_back<uint8_t, device<>::get_controlb_reg<u>()> reg;
            if constexpr (bits == 9) {
                reg |= (1<<UCSZ02);
            } else {
                reg &= ~(1<<UCSZ02);
            }
        }

        static void config_bitwidth() {
            config_bitwidth_c();
            config_bitwidth_b();
        }

    public:

        usart() { config_bitwidth(); }
        channel<component::rx> recieve;
        channel<component::tx> transmit;

        enum class mode : uint8_t {
            asynchronus,
            synchronus,
            mpsim
        };

        enum class parity_mode : uint8_t {
            disabled,
            even,
            odd
        };

        enum class stop_bits : uint8_t {
            bits1,
            bits2
        };

    private:

        template<component c>
        class channel {
            channel& operator= (const power::state & state) {
                register_write_back<uint8_t, device<>::get_controlb_reg<u>()> reg;
                if constexpr(c == component::tx) {
                    if (state == power::state::enabled) {
                        reg |= (1<<TXEN0);
                    } else if (state == power::state::disabled) {
                        reg &= ~(1<<TXEN0);
                    }
                }
                else if constexpr(c == component::rx) {
                    if (state == power::state::enabled) {
                        reg |= (1<<RXEN0);
                    } else if (state == power::state::disabled) {
                        reg &= ~(1<<RXEN0);
                    }
                }

                return *this;
            }

            channel& operator= (const interrupt::state& state) {
                register_write_back<uint8_t, device<>::get_controlb_reg<u>()> reg;
                if constexpr(c == component::tx) {
                    if (state == interrupt::state::enabled) {
                        reg |= (1<<TXCIE0);
                    } else if (state == interrupt::state::disabled) {
                        reg &= ~(1<<TXCIE0);
                    }
                }
                else if constexpr(c == component::rx) {
                    if (state == interrupt::state::enabled) {
                        reg |= (1<<RXCIE0);
                    } else if (state == interrupt::state::disabled) {
                        reg &= ~(1<<RXCIE0);
                    }
                }

                return *this;
            }
        };
    };
}
