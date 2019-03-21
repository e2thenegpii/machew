#pragma once
#include "architecture.h"

namespace machew {
    template<uintptr_t reg>
    struct timer_interrupt_flag_t : public register_t<uint8_t, reg> {
//        bool is_compare_b_set() { return value & (1<<OCF0B); }
//        bool is_compare_a_set() { return value & (1<<OCF0A); }
//        bool is_overflow_set() { return value & (1<<TOV0); }

        // TODO make this function only available for 16 bit timers
//        bool is_input_capture_set() { return value & (1<<TCF1); }
    };

    //TODO make the registers dependent on the specific device
    template<uintptr_t reg> struct pin_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct ddr_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct port_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct tifr_t : public timer_interrupt_flag_t<reg> {};
    template<uintptr_t reg> struct pcifr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _PCIF2{0x02};
        static constexpr uint8_t _PCIF1{0x01};
        static constexpr uint8_t _PCIF0{0x00};
    };
    template<uintptr_t reg> struct eifr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _INTF1{0x01};
        static constexpr uint8_t _INTF0{0x00};
    };
    template<uintptr_t reg> struct eimsk_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _INT1{0x01};
        static constexpr uint8_t _INT0{0x00};
    };
    template<uintptr_t reg> struct gpior_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct eecr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _EEPM1{0x05};
        static constexpr uint8_t _EEPM0{0x04};
        static constexpr uint8_t _EERIE{0x03};
        static constexpr uint8_t _EEMPE{0x02};
        static constexpr uint8_t _EEPE{0x01};
        static constexpr uint8_t _EERE{0x00};
    };
    template<uintptr_t reg> struct eedr_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct eear_t : public register_t<uint16_t, reg> {};
    template<uintptr_t reg> struct gtccr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _TSM{0x07};
        static constexpr uint8_t _PSRASY{0x01};
        static constexpr uint8_t _PSRSYNC{0x00};
    };
    template<uintptr_t reg> struct tccra_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct tccrb_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct tccrc_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct spcr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _SPIE{0x07};
        static constexpr uint8_t _SPE{0x06};
        static constexpr uint8_t _DORD{0x05};
        static constexpr uint8_t _MSTR{0x04};
        static constexpr uint8_t _CPOL{0x03};
        static constexpr uint8_t _CPHA{0x02};
        static constexpr uint8_t _SPR1{0x01};
        static constexpr uint8_t _SPR0{0x00};
    };
    template<uintptr_t reg> struct spsr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _SPIF{0x07};
        static constexpr uint8_t _WCOL{0x06};
        static constexpr uint8_t _SPI2X{0x00};
    };
    template<uintptr_t reg> struct spdr_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct acsr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _ACD{0x07};
        static constexpr uint8_t _ACBG{0x06};
        static constexpr uint8_t _ACO{0x05};
        static constexpr uint8_t _ACI{0x04};
        static constexpr uint8_t _ACIE{0x03};
        static constexpr uint8_t _ACIC{0x02};
        static constexpr uint8_t _ACIS1{0x01};
        static constexpr uint8_t _ACIS0{0x00};
    };
    template<uintptr_t reg> struct dwdr_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct smcr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _SM2{0x03};
        static constexpr uint8_t _SM1{0x02};
        static constexpr uint8_t _SM0{0x01};
        static constexpr uint8_t _SE{0x00};
    };
    template<uintptr_t reg> struct mcusr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _WDRF{0x03};
        static constexpr uint8_t _BORF{0x02};
        static constexpr uint8_t _EXTRF{0x01};
        static constexpr uint8_t _PORF{0x00};
    };
    template<uintptr_t reg> struct mcucr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _BODS{0x06};
        static constexpr uint8_t _BODSE{0x05};
        static constexpr uint8_t _PUD{0x04};
        static constexpr uint8_t _IVSEL{0x01};
        static constexpr uint8_t _IVCE{0x00};
    };
    template<uintptr_t reg> struct spmcsr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _SPMIE{0x07};
        static constexpr uint8_t _RWWSB{0x06};
        static constexpr uint8_t _SIGRD{0x05};
        static constexpr uint8_t _RWWSRE{0x04};
        static constexpr uint8_t _BLBSET{0x03};
        static constexpr uint8_t _PGWRT{0x02};
        static constexpr uint8_t _PGERS{0x01};
        static constexpr uint8_t _SPMEN{0x00};
    };
    template<uintptr_t reg> struct sp_t : public register_t<uint16_t, reg> {};
    template<uintptr_t reg> struct sreg_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _I{0x07};
        static constexpr uint8_t _T{0x06};
        static constexpr uint8_t _H{0x05};
        static constexpr uint8_t _S{0x04};
        static constexpr uint8_t _V{0x03};
        static constexpr uint8_t _N{0x02};
        static constexpr uint8_t _Z{0x01};
        static constexpr uint8_t _C{0x00};
    };
    template<uintptr_t reg> struct wdtcsr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _WDIF{0x07};
        static constexpr uint8_t _WDIE{0x06};
        static constexpr uint8_t _WDP3{0x05};
        static constexpr uint8_t _WDCE{0x04};
        static constexpr uint8_t _WDE{0x03};
        static constexpr uint8_t _WDP2{0x02};
        static constexpr uint8_t _WDP1{0x01};
        static constexpr uint8_t _WDP0{0x00};
    };
    template<uintptr_t reg> struct clkpr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _CLKPCE{0x07};
        static constexpr uint8_t _CLKPS3{0x03};
        static constexpr uint8_t _CLKPS2{0x02};
        static constexpr uint8_t _CLKPS1{0x01};
        static constexpr uint8_t _CLKPS0{0x00};
    };
    template<uintptr_t reg> struct prr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _PRTWI0{0x07};
        static constexpr uint8_t _PRTIM2{0x06};
        static constexpr uint8_t _PRTIM0{0x05};
        static constexpr uint8_t _PTRIM1{0x03};
        static constexpr uint8_t _PRSPI0{0x02};
        static constexpr uint8_t _PRUSART0{0x01};
        static constexpr uint8_t _PRADC{0x00};
    };
    template<uintptr_t reg> struct osccal_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct pcicr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _PCIE2{0x02};
        static constexpr uint8_t _PCIE1{0x01};
        static constexpr uint8_t _PCIE0{0x00};
    };
    template<uintptr_t reg> struct eicra_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _ISC11{0x03};
        static constexpr uint8_t _ISC10{0x02};
        static constexpr uint8_t _ISC01{0x01};
        static constexpr uint8_t _ISC00{0x00};
    };
    template<uintptr_t reg> struct pcmsk_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct timsk_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct adc_t : public register_t<uint16_t, reg> {};
    template<uintptr_t reg> struct adcsra_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _ADEN{0x07};
        static constexpr uint8_t _ADSC{0x06};
        static constexpr uint8_t _ADATE{0x05};
        static constexpr uint8_t _ADIF{0x04};
        static constexpr uint8_t _ADIE{0x03};
        static constexpr uint8_t _ADPS2{0x02};
        static constexpr uint8_t _ADPS1{0x01};
        static constexpr uint8_t _ADPS0{0x00};
    };
    template<uintptr_t reg> struct adcsrb_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _ACME{0x06};
        static constexpr uint8_t _ADTS2{0x02};
        static constexpr uint8_t _ADTS1{0x01};
        static constexpr uint8_t _ADTS0{0x00};
    };
    template<uintptr_t reg> struct admux_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _REFS1{0x07};
        static constexpr uint8_t _REFS0{0x06};
        static constexpr uint8_t _ADLAR{0x05};
        static constexpr uint8_t _MUX3{0x03};
        static constexpr uint8_t _MUX2{0x02};
        static constexpr uint8_t _MUX1{0x01};
        static constexpr uint8_t _MUX0{0x00};
    };
    template<uintptr_t reg> struct didr0_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _ADC5D{0x05};
        static constexpr uint8_t _ADC4D{0x04};
        static constexpr uint8_t _ADC3D{0x03};
        static constexpr uint8_t _ADC2D{0x02};
        static constexpr uint8_t _ADC1D{0x01};
        static constexpr uint8_t _ADC0D{0x00};
    };
    template<uintptr_t reg> struct didr1_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _AIN1D{0x01};
        static constexpr uint8_t _AIN0D{0x00};
    };
    template<uintptr_t reg> struct icr_t : public register_t<uint16_t, reg> {};
    template<uintptr_t reg> struct assr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _EXCLK{0x06};
        static constexpr uint8_t _AS2{0x05};
        static constexpr uint8_t _TCN2UB{0x04};
        static constexpr uint8_t _OCR2AUB{0x03};
        static constexpr uint8_t _OCR2BUB{0x02};
        static constexpr uint8_t _TCR2AUB{0x01};
        static constexpr uint8_t _TCR2BUB{0x00};
    };
    template<uintptr_t reg> struct twbr_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct twsr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _TWS7{0x07};
        static constexpr uint8_t _TWS6{0x06};
        static constexpr uint8_t _TWS5{0x05};
        static constexpr uint8_t _TWS4{0x04};
        static constexpr uint8_t _TWS3{0x03};
        static constexpr uint8_t _TWPS1{0x01};
        static constexpr uint8_t _TWPS0{0x00};
    };
    template<uintptr_t reg> struct twar_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _TWA6{0x07};
        static constexpr uint8_t _TWA5{0x06};
        static constexpr uint8_t _TWA4{0x05};
        static constexpr uint8_t _TWA3{0x04};
        static constexpr uint8_t _TWA2{0x03};
        static constexpr uint8_t _TWA1{0x02};
        static constexpr uint8_t _TWA0{0x01};
        static constexpr uint8_t _TWGCE{0x00};
    };
    template<uintptr_t reg> struct twdr_t : public register_t<uint8_t, reg> {};
    template<uintptr_t reg> struct twcr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _TWINT{0x07};
        static constexpr uint8_t _TWEA{0x06};
        static constexpr uint8_t _TWSTA{0x05};
        static constexpr uint8_t _TWSTO{0x04};
        static constexpr uint8_t _TWWC{0x03};
        static constexpr uint8_t _TWEN{0x02};
        static constexpr uint8_t _TWIE{0x00};
    };
    template<uintptr_t reg> struct twamr_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _TWAM6{0x07};
        static constexpr uint8_t _TWAM5{0x06};
        static constexpr uint8_t _TWAM4{0x05};
        static constexpr uint8_t _TWAM3{0x04};
        static constexpr uint8_t _TWAM2{0x03};
        static constexpr uint8_t _TWAM1{0x02};
        static constexpr uint8_t _TWAM0{0x01};
    };
    template<uintptr_t reg> struct ucsra_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _RXC{0x07};
        static constexpr uint8_t _TXC{0x06};
        static constexpr uint8_t _UDRE{0x05};
        static constexpr uint8_t _FE{0x04};
        static constexpr uint8_t _DOR{0x03};
        static constexpr uint8_t _UPE{0x02};
        static constexpr uint8_t _U2X{0x01};
        static constexpr uint8_t _MPCM{0x00};
    };
    template<uintptr_t reg> struct ucsrb_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _RXCIE{0x07};
        static constexpr uint8_t _TXCIE{0x06};
        static constexpr uint8_t _UDRIE{0x05};
        static constexpr uint8_t _RXEN{0x04};
        static constexpr uint8_t _TXEN{0x03};
        static constexpr uint8_t _UCSZ2{0x02};
        static constexpr uint8_t _RXB8{0x01};
        static constexpr uint8_t _TXB8{0x00};
    };
    template<uintptr_t reg> struct ucsrc_t : public register_t<uint8_t, reg> {
        static constexpr uint8_t _UMSEL1{0x07};
        static constexpr uint8_t _UMSEL0{0x06};
        static constexpr uint8_t _UPM1{0x05};
        static constexpr uint8_t _UPM0{0x04};
        static constexpr uint8_t _USBS{0x03};
        static constexpr uint8_t _UCSZ1{0x02};
        static constexpr uint8_t _UCSZ0{0x01};
        static constexpr uint8_t _UCPOL{0x00};
    };
    template<uintptr_t reg> struct ubrr_t : public register_t<uint16_t, reg> {};
    template<uintptr_t reg> struct udr_t : public register_t<uint8_t, reg> {};
}

