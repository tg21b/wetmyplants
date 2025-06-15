
#include <msp430.h>
#include ".\libraries\msp430_driverlib_2_91_13_01\driverlib\MSP430FR5xx_6xx\driverlib.h"
#include <stdio.h>
#include "C:\Users\Windows 10 PRO\workspace_ccstheia\plant_watering\libraries\HAL_UART_4746.h"

void msp430init();      //disabling WDT and 
void initADC();

int main(void)
{


    msp430init();
    UART_initGPIO();
    UART_init();
    initADC();

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);
    char buffer[100];
    uint16_t moisture;
    uint16_t prev_moisture;

     sprintf(buffer, "Moisture: ");

    UART_transmitString(buffer);

    //---------------------------------------------GET INITIAL MOISTURE READING---------------------------------------------------------------------------------------------------
    ADC12_B_startConversion(ADC12_B_BASE, ADC12_B_START_AT_ADC12MEM0, ADC12_B_SINGLECHANNEL);
    // Wait for conversion to finish
    while (!ADC12_B_getInterruptStatus(ADC12_B_BASE, 0, ADC12_B_IFG0));
    // Read result
    prev_moisture = ADC12_B_getResults(ADC12_B_BASE, ADC12_B_MEMORY_0);


//------------------------------------------MAIN WHILE LOOP---------------------------------------------------------------------------------------------------------
    while(1){
        ADC12_B_startConversion(ADC12_B_BASE, ADC12_B_START_AT_ADC12MEM0, ADC12_B_SINGLECHANNEL);
        while (!ADC12_B_getInterruptStatus(ADC12_B_BASE, 0, ADC12_B_IFG0));
        moisture = ADC12_B_getResults(ADC12_B_BASE, ADC12_B_MEMORY_0);

        //only print if new moisture value is 40 more or less than the previous value
        if ((moisture >= prev_moisture + 40) || (moisture <= prev_moisture - 40)){
            sprintf(buffer, "Moisture: %d\r\n", moisture);
            UART_transmitString(buffer);
            prev_moisture = moisture;
        }

        //prev_moisture = moisture;
        __delay_cycles(50000);
    }

}

void msp430init(){
     WDTCTL = WDTPW | WDTHOLD;               // Stop WDT
     PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
}


/*
initADC

initalizes the ADC on pin P1.2, where the soil moisture sensor is connected

*/
void initADC(){
    GPIO_setAsPeripheralModuleFunctionInputPin(
    GPIO_PORT_P1, 
    GPIO_PIN2,
    GPIO_TERNARY_MODULE_FUNCTION);

    ADC12_B_initParam initParam = {0};
    initParam.sampleHoldSignalSourceSelect = ADC12_B_SAMPLEHOLDSOURCE_SC;
    initParam.clockSourceSelect = ADC12_B_CLOCKSOURCE_ADC12OSC;
    initParam.clockSourceDivider = ADC12_B_CLOCKDIVIDER_1;
    initParam.clockSourcePredivider = ADC12_B_CLOCKPREDIVIDER__1;
    initParam.internalChannelMap = ADC12_B_NOINTCH;

    ADC12_B_init(ADC12_B_BASE, &initParam);

    ADC12_B_enable(ADC12_B_BASE);

    ADC12_B_setupSamplingTimer(
        ADC12_B_BASE, 
        ADC12_B_CYCLEHOLD_16_CYCLES, 
        ADC12_B_CYCLEHOLD_4_CYCLES, 
        ADC12_B_MULTIPLESAMPLESDISABLE);

    ADC12_B_configureMemoryParam configParam = {0};
    configParam.memoryBufferControlIndex = ADC12_B_MEMORY_0;
    configParam.inputSourceSelect = ADC12_B_INPUT_A2;  // P1.2 = A2
    configParam.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    configParam.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;

    ADC12_B_configureMemory(ADC12_B_BASE, &configParam);

    //ADC12_B_startConversion(ADC12_B_BASE, ADC12_B_START_AT_ADC12MEM0, ADC12_B_SINGLECHANNEL);
}
