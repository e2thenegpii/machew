#pragma once
#include <stdint.h>
#include "architecture.h"

namespace machew {
    template<typename T>
    class register_write_back {
    public:
        using template_type = T;
        using value_type = typename T::value_type;
        using pointer_type = typename T::pointer_type;

        register_write_back() : value(*reinterpret_cast<pointer_type>(T::address)) {}
        register_write_back(const value_type& data) : value(data) {}

        register_write_back& operator |= (const value_type& data) {
            value |= data;
            return *this;
        }

        register_write_back& operator &= (const value_type& data) {
            value &= data;
            return *this;
        }

        ~register_write_back() {
            *reinterpret_cast<pointer_type>(T::address) = value;
        }
    private:
        value_type value;
    };

    template <device<>::port... ports>
    class pcicr_write_back : public register_write_back<decltype(device<>::pcicr)> {
    public:
        pcicr_write_back& operator= (const interrupt::state& state) {
    
            for( const auto & p : {ports...}) {
                if (p == port::B) {
                    if (state == interrupt::state::enabled) {
                        *this |= bit_value<value_type, template_type::_PCIE0>();
                    } else if (state == interrupt::state::disabled) {
                        *this &= ibit_value<value_type, template_type::_PCIE0>();
                    }
                } else if (p == port::C) {
                    if (state == interrupt::state::enabled) {
                        *this |= bit_value<value_type, template_type::_PCIE1>();
                    } else if (state == interrupt::state::disabled) {
                        *this &= ibit_value<value_type, template_type::_PCIE1>();
                    }
                } else if (p == port::D) {
                    if (state == interrupt::state::enabled) {
                        *this |= bit_value<value_type, template_type::_PCIE2>();
                    } else if (state == interrupt::state::disabled) {
                        *this &= ibit_value<value_type, template_type::_PCIE2>();;
                    }
                }
            }
    
            return *this;
        }
    };

    template <device<>::port... ports>
    class pcifr_write_back : public register_write_back<decltype(device<>::pcifr)> {
    public:
        void clear_interrupt() {

            for( const auto & p : {ports...}) {
                if (p == port::B) {
                    *this |= bit_value<value_type, template_type::_PCIF0>();
                } else if (p == port::C) {
                    *this |= bit_value<value_type, template_type::_PCIF1>();
                } else if (p == port::D) {
                    *this |= bit_value<value_type, template_type::_PCIF2>();
                }
            }
        }
        
    };

    template <device<>::external_interrupt... interrupts>
    class eifr_write_back : public register_write_back<decltype(device<>::eifr)> {
        void clear_interrupt() {
            for( const auto & p : {interrupts...}) {
                if (p == external_interrupt::zero) {
                    *this |= bit_value<value_type, template_type::_INTF0>();
                else if (p == external_interrupt::one) {
                    *this |= bit_value<value_type, template_type::_INTF1>();
                }
            }
        }
    };

    template <device<>::external_interrupt... interrupts>
    class eimsk_write_back : public register_write_back<decltype(device<>::eimsk)> {
        eimsk_write_back& operator= (const interrupt::state& state) {
            for( const auto & p : {interrupts...} ) {
                if (p == external_interrupt::zero) {
                    if (state == interrupt::state::enabled) {
                        *this |= bit_value<value_type, template_type::_INT0>();
                    } else if (state == interrupt::state::disabled) {
                        *this &= ibit_value<value_type, template_type::_INT0>();
                    }
                } else if (p == external_interrupt::one) {
                    if (state == interrupt::state::enabled) {
                        *this |= bit_value<value_type, template_type::_INT1>();
                    } else if (state == interrupt::state::disabled) {
                        *this &= ibit_value<value_type, template_type::_INT1>();
                    }
                }
            }

            return *this;
        }
    };
}
