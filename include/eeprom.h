#pragma once

#include <avr/io.h>
#include "architecture.h"
#include "interrupts.h"
#include "power.h"
#include "register_write_back.h"

namespace machew {

    class eeprom {
    private:
        static void start_read() {
            register_write_back<uint8_t, device<>::eecr> reg;
            reg |= (1<<EERE);
        }

    public:
        enum class mode {
            erase_write,
            erase,
            write
        };

        using address = uintptr_t;

        eeprom& operator= (const mode& mode) {

            register_write_back<uint8_t, device<>::eecr> reg;
            if (mode == mode::erase_write) {
                reg &= ~((1<<EEPM1)|(1<<EEPM0));
            } else if (mode == mode::erase) {
                reg &= ~(1<<EEPM1);
                reg |= (1<<EEPM0);
            } else if (mode == mode::write) {
                reg |= (1<<EEPM1);
                reg &= ~(1<<EEPM0);
            }
            return *this;
        }        

        eeprom& operator= (uint8_t data) {
            this->write(data);
            return *this;
        }

        eeprom& operator=(address addr) {
            register_write_back<address, device<>::eear> reg(addr);
            return *this;
        }

        eeprom& operator= (const interrupt::state& state) {
            register_write_back<uint8_t, device<>::eecr> reg;
            if (state == interrupt::state::enabled) {
                reg |= (1<<EERIE);
            } else if (state == interrupt::state::disabled) {
                reg &= ~(1<<EERIE);
            }
            return *this;
        }

        operator uint8_t() const {
            return *((volatile uint8_t*)device<>::eedr);
        }

        bool write_ready() const {
            return *((volatile uint8_t*)device<>::eecr) & (1<<EEPE);
        }

        void write_async(uint8_t data) {
            {   // Scope the write_back to ensure it is written before
                // clearing interrupts
                register_write_back<uint8_t, device<>::eedr> reg(data);
            }
            uint8_t tmp = SREG;
            __builtin_avr_cli();
            
            *((volatile uint8_t*)device<>::eecr) |= (1<<EEMPE);
            *((volatile uint8_t*)device<>::eecr) |= (1<<EEPE);
            SREG = tmp;
        }

        void write(uint8_t data) {
            while(!write_ready());
            write_async(data);
        }

        uint8_t read_async() {
            start_read();
            return *((volatile uint8_t*)device<>::eedr);
        }

        uint8_t read() {
            while(!write_ready());
            return read_async();
        }
    };
}
