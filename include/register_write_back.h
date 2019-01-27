#pragma once
#include <stdint.h>

namespace machew {
    template<typename T, uintptr_t reg>
    class register_write_back {
    public:
        typedef T           value_type;
        typedef T&          reference_type;
        typedef volatile T* pointer_type;
        register_write_back() : value(*reinterpret_cast<pointer_type>(reg)) {}
        register_write_back(const T& data) : value(data) {}

        register_write_back& operator |= (const T& data) {
            value |= data;
            return *this;
        }

        register_write_back& operator &= (const T& data) {
            value &= data;
            return *this;
        }

        ~register_write_back() {
            *reinterpret_cast<pointer_type>(reg) = value;
        }
    private:
        value_type value;
    };
};
