#include "adc.h"
#include "eeprom.h"
#include "io_port.h"
#include "spi.h"
#include "twi.h"
#include "uart.h"
#include "power.h"

namespace {
    machew::adc<> adc;
    //machew::adc<machew::precision_mode::low> adclow;
}

int main() {
    adc = machew::power::state::enabled;
    adc = machew::interrupt::state::disabled;
    adc.start();
    while(!adc.read_ready());
    
    return 0;
}
