#include "adc.h"

void adc_init() {
    /* Use pin PC3 for analog input (ADC_IN13) */

    // Enable clock to C GPIO bus
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // Set PB0 to analog mode
    GPIOC->MODER |= GPIO_MODER_MODER3;

    // No pullup or pulldown resistors
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR3;

    // Enable ADC peripheral in clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // Configure 8-bit resolution
    ADC1->CFGR1 &= ~ADC_CFGR1_RES_Msk;
    ADC1->CFGR1 |= ADC_CFGR1_RES_1;

    // Set ADC to continuous conversion mode
    ADC1->CFGR1 |= ADC_CFGR1_CONT;

    // Disable hardware triggers
    ADC1->CFGR1 &= ~ADC_CFGR1_EXTEN;

    // Enable input pin 
    ADC1->CHSELR |= ADC_CHSELR_CHSEL13;

    // Self calibrate the ADC
    ADC1->CR |= ADC_CR_ADCAL;
    // Wait for hardware to clear flag signaling completion
    while (ADC1->CR & ADC_CR_ADCAL) {}

    // Enable ADC
    ADC1->CR |= ADC_CR_ADEN;

    // Start the ADC
    ADC1->CR |= ADC_CR_ADSTART;
}

uint16_t adc_read() {
    return (uint16_t)ADC1->DR;
}