#pragma once
#include "architecture.h"
#include "power.h"

namespace machew {

    class twi {
    public:

        enum class prescaler : uint8_t {
            div1,
            div4,
            div16,
            div64
        };

        using address = uint8_t;

        twi& operator= (const prescaler& scale) {

            register_write_back<uint8_t, device<>::twsr> reg;
            if (scale == prescaler::div1) {
                reg &= ~((1<<TWPS1)|(1<<TWPS0));
            } else if (scale == prescaler::div4) {
                reg &= ~(1<<TWPS1);
                reg |= (1<<TWPS0);
            } else if (scale == prescaler::div16) {
                reg |= (1<<TWPS1);
                reg &= ~(1<<TWPS0);
            } else if (scale == prescaler::div64) {
                reg |= (1<<TWPS1)|(1<<TWPS0);
            }

            return *this;
        }

        twi& operator= (const interrupt::state& state) {

            register_write_back<uint8_t, device<>::twcr> reg;
            if (state == interrupt::state::enabled) {
                reg |= (1<<TWIE);
            } else if (state == interrupt::state::disabled) {
                reg &= ~(1<<TWIE);
            }

            return *this;
        }

        twi& operator= (const power::state& state) {

            register_write_back<uint8_t, device<>::twcr> reg;
            if (state == power::state::enabled) {
                reg |= (1<<TWEN);
            } else if (state == power::state::disabled) {
                reg &= ~(1<<TWEN);
            }

            return *this;
        }

        operator uint8_t() {
            return *((volatile uint8_t*)device<>::twdr);
        }

        twi& operator= (uint8_t data) {
            register_write_back<uint8_t, device<>::twdr> reg(data);
            return *this;
        }

        twi& operator= (const address& addr) {
            register_write_back<uint8_t, device<>::twar> reg(addr);
            return *this;
        }

        bool read_ready() const {
            return *((volatile uint8_t*)device<>::twcr) & (1<<TWINT);
        }
    };
}
