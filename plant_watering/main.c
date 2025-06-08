
#include <msp430.h>
#include ".\msp430_driverlib_2_91_13_01\driverlib\MSP430FR5xx_6xx\driverlib.h"


void msp430init();


int main(void)
{

    msp430init();

    ADC12_B_init(ADC12_B_BASE, ADC12_B_SAMPLEHOLDSOURCE_SC);


    // Configure GPIO
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction


    while(1)
    {
        P1OUT ^= BIT0;                      // Toggle LED
        __delay_cycles(1000000);
    }
}

void msp430init(){
     WDTCTL = WDTPW | WDTHOLD;               // Stop WDT
     PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
}