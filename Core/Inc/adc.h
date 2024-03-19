#ifndef _ADC_H
#define _ADC_H
#include "stm32f072xb.h"

/**
 * @brief Initializes the ADC
*/
void adc_init();

/**
 * @brief Reads the value in the ADC register
 * @return The value of the ADC data register
 * @note returns is limited to the selected data output size
 * of the configured ADC
*/
uint16_t adc_read();

#endif