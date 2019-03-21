#pragma once

#include <avr/io.h>
#include "architecture.h"
#include "interrupts.h"
#include "power.h"
#include "register_write_back.h"
/*
namespace machew {

    class eeprom {
    private:

    public:
        enum class mode {
            erase_write,
            erase,
            write
        };

        using address = uintptr_t;

        eeprom& operator= (const mode& mode);

        eeprom& operator= (uint8_t data) {
            this->write(data);
            return *this;
        }

        eeprom& operator=(address addr) {
            register_write_back<decltype(device<>::eear)> reg(addr);
            return *this;
        }

        
        eeprom& operator= (const interrupt::state& state);

        operator uint8_t() const {
            return *((volatile uint8_t*)device<>::eedr);
        }

        bool write_ready() const;

        void write_async(uint8_t data);

        void write(uint8_t data) {
            while(!write_ready());
            write_async(data);
        }

        uint8_t read_async();

        uint8_t read() {
            while(!write_ready());
            return read_async();
        }
    };

    struct eecr_t {
        eecr_t& operator= (const interrupt::state& state ) {
            register_write_back<decltype(device<>::eecr)> reg;
            if (state == interrupt::state::enabled) {
                reg |= (1<<EERIE);
            } else if (state == interrupt::state::disabled) {
                reg &= ~(1<<EERIE);
            }
            return *this;
        }

        eecr_t& operator= (const eeprom::mode& mode) {
            register_write_back<decltype(device<>::eecr)> reg;
            if (mode == eeprom::mode::erase_write) {
                reg &= ~((1<<EEPM1)|(1<<EEPM0));
            } else if (mode == eeprom::mode::erase) {
                reg &= ~(1<<EEPM1);
                reg |= (1<<EEPM0);
            } else if (mode == eeprom::mode::write) {
                reg |= (1<<EEPM1);
                reg &= ~(1<<EEPM0);
            }
            return *this;
        }        

        void start_read() {
            register_write_back<decltype(device<>::eecr)> reg;
            reg |= (1<<EERE);
        }

        void start_write() {
            *((volatile uint8_t*)device<>::eecr) |= (1<<EEMPE);
            *((volatile uint8_t*)device<>::eecr) |= (1<<EEPE);
        }

        bool write_ready() {
            return *((volatile uint8_t*)device<>::eecr) & (1<<EEPE);
        }
    };

    bool eeprom::write_ready() const {
        return eecr_t().write_ready();
    }

    void eeprom::write_async(uint8_t data) {
        {   // Scope the write_back to ensure it is written before
            // clearing interrupts
            register_write_back<decltype(device<>::eedr)> reg(data);
        }
        eecr_t eecr;
        uint8_t tmp = SREG;
        __builtin_avr_cli();
        
        eecr.start_write();
        SREG = tmp;
    }

    uint8_t eeprom::read_async() {
        eecr_t eecr;
        eecr.start_read();
        return *((volatile uint8_t*)device<>::eedr);
    }

    eeprom& eeprom::operator= (const mode& mode) {
        eecr_t eecr;
        eecr = mode;
        return *this;
    }        

    eeprom& eeprom::operator= (const interrupt::state& state) {
        eecr_t eecr;
        eecr = state;
        return *this;
    }
}
*/
