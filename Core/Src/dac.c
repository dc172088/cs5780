#include "dac.h"

// Sawtooth table for converting with the DAC
const uint8_t sawtooth_table[32] = {0,7,15,23,31,39,47,55,63,71,79,87,95,103,111,119,127,134,142,150,158,166,174,182,190,198,206,214,222,230,238,246};

void dac_init() {
    /* Use PA4 DAC_OUT1 */

    // Enable clock to A GPIO bus
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // Set PA4 to analog mode
    GPIOA->MODER |= GPIO_MODER_MODER4;

    // No pullup or pulldown resistors
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4;

    // Enable DAC clock
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;

    // Set DAC to software trigger mode
    DAC1->CR |= DAC_CR_TSEL1;

    // Enable DAC channel
    DAC1->CR |= DAC_CR_EN1;
}


void dac_write() {
    static uint8_t dac_index = 0;
    DAC1->DHR8R1 = sawtooth_table[dac_index];
    dac_index = (dac_index + 1) % 32;
}