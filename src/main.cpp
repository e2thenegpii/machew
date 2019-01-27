#include "adc.h"
#include "eeprom.h"
#include "io_port.h"
#include "spi.h"
#include "twi.h"
#include "uart.h"

namespace {
    machew::io_port<machew::port::B> portb;
    //machew::adc<> adc;
    //machew::adc<machew::precision_mode::low> adclow;
    //machew::twi twi;
    machew::eeprom eeprom;
};

int main() {
    //portb = machew::pin_mode::input;
    //portb = machew::interrupt::state::disabled;
    machew::device<>::pcifr_t<machew::port::B, machew::port::C> pcifr;
    pcifr.clear_interrupt();

    eeprom = machew::interrupt::state::disabled;
    eeprom = machew::eeprom::mode::erase_write;
    eeprom = uint16_t{32};
    eeprom = uint8_t{5};
    
    return 0;
}
