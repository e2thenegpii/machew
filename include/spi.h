#pragma once
#include "architecture.h"
#include "interrupts.h"
#include "power.h"
#include "register_write_back.h"

namespace machew {

    class spi {
    public:

        enum class data_order : uint8_t {
            lsb,
            msb
        };
    
        enum class mode : uint8_t {
            master,
            slave
        };

        enum class clock_polarity : uint8_t {
            high,
            low
        };

        enum class sample_edge : uint8_t {
            leading,
            trailing
        };

        enum class clock_divider : uint8_t {
            div2,
            div4,
            div8,
            div16,
            div32,
            div64,
            div128
        };

        spi& operator= (const interrupt::state& state) {
            register_write_back<uint8_t, device<>::spcr> reg;
            if (state == interrupt::state::enabled) {
                reg |= (1<<SPIE);
            } else if (state == interrupt::state::disabled) {
                reg &= static_cast<uint8_t>(~(1<<SPIE));
            }
            return *this;    
        }

        spi& operator= (const power::state& state) {
            register_write_back<uint8_t, device<>::spcr> reg;
            if (state == power::state::enabled) {
                reg |= (1<<SPE);
            } else if (state == power::state::disabled) {
                reg &= ~(1<<SPE);
            }
            return *this;
        }

        spi& operator= (const spi::data_order& order) {
            register_write_back<uint8_t, device<>::spcr> reg;
            if (order == spi::data_order::lsb) {
                reg |= (1<<DORD);
            } else if (order == spi::data_order::msb) {
                reg &= ~(1<<DORD);
            }
            return *this;
        }

        spi& operator= (const spi::mode& mode) {
            register_write_back<uint8_t, device<>::spcr> reg;
            if (mode == spi::mode::master) {
                reg |= (1<<MSTR);
            } else if (mode == spi::mode::slave) {
                reg &= ~(1<<MSTR);
            }
            return *this;
        }

        spi& operator= (const spi::clock_polarity& polarity) {
            register_write_back<uint8_t, device<>::spcr> reg;
            if (polarity == spi::clock_polarity::high) {
                reg |= (1<<CPOL);
            } else if (polarity == spi::clock_polarity::low) {
                reg &= ~(1<<CPOL);
            } 
            return *this;
        }

        spi& operator= (const spi::sample_edge& edge) {
            register_write_back<uint8_t, device<>::spcr> reg;
            if (edge == spi::sample_edge::leading) {
                reg &= ~(1<<CPHA);
            } else if (edge == spi::sample_edge::trailing) {
                reg |= (1<<CPHA);
            }
            return *this;
        }

        spi& operator= (const spi::clock_divider& div) {
            register_write_back<uint8_t, device<>::spcr> reg;
            switch(div) {
               //TODO implement me 
            }
            return *this;
        }

        operator uint8_t() const {
            return *((volatile uint8_t*)device<>::spdr);
        };

        bool read_ready() const {
            return *((volatile uint8_t*)device<>::spsr) & (1<<SPIF);
        }

        uint8_t read_async() const {
            return *this;
        }

        uint8_t read() const {
            while(!read_ready());
            return read_async();
        }
    };
};
