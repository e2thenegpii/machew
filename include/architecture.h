#pragma once
#include <stdint.h>
#include <initializer_list>

namespace machew {
    enum class supported_device {
        atmega328
    };

    /**
     * From a collection of pins generate the relevant mask
     */
    template<typename T, uint8_t... shifts>
    constexpr T bit_value() {
        T tmp{0};
        for( const auto & x : {T{1}<<shifts...}) {
            tmp |= x;
        }
        return tmp;
    }

    template<typename T, uint8_t... shifts> constexpr T ibit_value() { return (T)(~bit_value<T, shifts...>()); }

    template<supported_device d=supported_device::__AVR_DEVICE_NAME__> struct device;

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
