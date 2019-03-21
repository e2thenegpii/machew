#include "adc.h"
#include "eeprom.h"
#include "io_port.h"
#include "spi.h"
#include "twi.h"
#include "uart.h"
#include "power.h"

namespace {
}

int main() {
    machew::pcicr_write_back<machew::device<>::port::B, machew::device<>::port::C> reg;
    reg = machew::interrupt::state::enabled;
    
    return 0;
}
