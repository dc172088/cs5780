#ifndef _DAC_H
#define _DAC_H
#include "stm32f072xb.h"

/**
 * @brief Initializes the DAC
*/
void dac_init();

/**
 * @brief Write to the DAC
*/
void dac_write();

#endif