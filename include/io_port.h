#pragma once

#include <stdint.h>
#include <initializer_list>
#include "architecture.h"
#include "interrupts.h"
#include "power.h"
#include "register_write_back.h"

namespace machew {
    /**
     * Sets the mode of a given collection of pins
     */
    enum class pin_mode : uint8_t {
        input,
        input_pullup,
        output
    };

    /**
     * Sets the collection of pins to be high or low or to toggle
     */
    enum class pin_state : uint8_t {
        low,
        high,
        toggle
    };

    /**
     * Represents an io port on an AVR microcontroller
     * 
     * Must be declared with machew::io_port<machew::port::N> portn;
     * Then it can be used to generate super efficient register accesses
     */
    template<device<>::port _port> class io_port {
        template<uint8_t... _pins> struct pin_group;
    public:

        /**
         * Semantically an alias to the pins function but just so you don't have to type
         * portb.pins<7>()
         */
        template<uint8_t _pin> static pin_group<_pin> pin() { return pin_group<_pin>(); }

        /**
         * Factory method to create a pin_group of multiple pins
         */
        template<uint8_t... _pins> static pin_group<_pins...> pins() { return pin_group<_pins...>(); }

        /**
         * Sets the mode for all pins in the port
         */
        io_port& operator= (const pin_mode& mode) {

            this->pins<0,1,2,3,4,5,6,7>() = mode;
            return *this;
        }

        /**
         * Sets the state for all pins in the port
         */
        io_port& operator= (const pin_state& state) {

            this->pins<0,1,2,3,4,5,6,7>() = state;
            return *this;
        }

        io_port& operator= (const interrupt::state& state) {
            device<>::pcicr_t<_port> pcicr;
            pcicr = state;
            return *this;
        }

        void clear_interrupt() const {
            device<>::pcifr_t<_port> pcifr;
            pcifr.clear_interrupt();
        }

        /**
         * Sets the state for all pins in the port
         */
        io_port& operator= (const uint8_t& data) {

            this->pins<0,1,2,3,4,5,6,7>() = data;
            return *this;
        }

        /**
         * Reads the state for all pins in the port
         */
        constexpr operator uint8_t() const {
            return *((volatile uint8_t*)device<>::get_port_reg<_port>());
        }

    private:
        /**
         * From a collection of pins generate the relevant mask
         */
        template<uint8_t... _pins>
        static constexpr uint8_t get_pin_mask() {
                
            uint8_t tmp = 0;
            for( const auto & x : {1<<_pins...}) {
                tmp |= x;
            }
            return tmp;
        }

        /**
         * Preforms a common operation on a group of pins
         */
        template <uint8_t... pins>
        struct pin_group {

            const pin_group& operator= (const interrupt::state& state) const {
                constexpr uint8_t x = get_pin_mask<pins...>();

                register_write_back<uint8_t, device<>::get_pcmsk_reg<_port>()> reg;
                if (state == interrupt::state::enabled) {
                    reg |= x;
                } else if (state == interrupt::state::disabled) {
                    reg &= ~x;
                }

                return *this;
            }

            /**
             * Set the mode for a given group of pins
             */
            [[gnu::always_inline]] inline const pin_group& operator= (const pin_mode& mode) const {
                constexpr uint8_t x = get_pin_mask<pins...>();

                // the if constexpr exists to avoid an unnecessary read
                // if we are writing to all the bits because all the registers
                // are volatile 
                if (mode == pin_mode::input) {
                    if constexpr(x != 0xff) {
                        register_write_back<uint8_t, device<>::get_ddr_reg<_port>()> reg;
                        reg &= ~x;
                    } else {
                        register_write_back<uint8_t, device<>::get_ddr_reg<_port>()> reg(0x00);
                    }
                } else if(mode == pin_mode::input_pullup) {
                    *this = pin_mode::input;
                    if constexpr(x != 0xff) {
                        register_write_back<uint8_t, device<>::get_port_reg<_port>()> reg;
                        reg |= x;
                    } else {
                        register_write_back<uint8_t, device<>::get_port_reg<_port>()> reg(0xff);
                    }
                } else if(mode == pin_mode::output) {
                    if constexpr(x != 0xff) {
                        register_write_back<uint8_t, device<>::get_ddr_reg<_port>()> reg;
                        reg |= x;
                    } else {
                        register_write_back<uint8_t, device<>::get_ddr_reg<_port>()> reg(0xff);
                    }
                }

                return *this;
            }

            /**
             * Set the pin state for a given group of pins
             */
            [[gnu::always_inline]] inline const pin_group& operator= (const pin_state& state) const {
                constexpr uint8_t x = get_pin_mask<pins...>();

                if(state == pin_state::low) {
                    if constexpr(x != 0xff) {
                        register_write_back<uint8_t, device<>::get_port_reg<_port>()> reg;
                        reg &= ~x;
                    } else {
                        register_write_back<uint8_t, device<>::get_port_reg<_port>()> reg(0x00);
                    }
                } else if(state == pin_state::high) {
                    if constexpr(x != 0xff) {
                        register_write_back<uint8_t, device<>::get_port_reg<_port>()> reg;
                        reg |= x;
                    } else {
                        register_write_back<uint8_t, device<>::get_port_reg<_port>()> reg(0xff);
                    }
                } else if(state == pin_state::toggle) {
                    if constexpr(x != 0xff) {
                        register_write_back<uint8_t, device<>::get_pin_reg<_port>()> reg;
                        reg |= x;
                    } else {
                        register_write_back<uint8_t, device<>::get_pin_reg<_port>()> reg(0xff);
                    }
                }

                return *this;
            }

            const pin_group& operator= (const uint8_t& data) const {
                register_write_back<uint8_t, device<>::get_port_reg<_port>()> reg(data);
                return *this;
            }

            operator uint8_t() const { 
                constexpr uint8_t x = get_pin_mask<pins...>();
                return *((volatile uint8_t*)device<>::get_port_reg<_port>()) & x;
            }
        };
    };
};
