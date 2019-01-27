#pragma once
#include <stdint.h>

namespace machew {
    enum class supported_device {
        atmega328
    };

    template<bool> struct wrap;

    template<typename T, uint8_t shift> constexpr T bit_value() { return T{T{1}<<shift}; }
    template<typename T, uint8_t shift> constexpr T ibit_value() { return T{~bit_value<shift, T>()}; }

    template<supported_device d=supported_device::__AVR_DEVICE_NAME__> struct device;
    template<typename T, typename return_t> constexpr return_t get_registers(const T&);

    template<typename T, uintptr_t _addr>
    struct register_t {
        static constexpr uintptr_t  address = _addr;
        typedef T                   value_type;
        typedef volatile T*         pointer_type;

        operator pointer_type() {
            return reinterpret_cast<pointer_type>(address);
        }

        constexpr operator uintptr_t() {
            return address;
        }
    };
}

#if defined(__AVR_ATmega328__)
#include "devices/atmega328.h"
#endif
