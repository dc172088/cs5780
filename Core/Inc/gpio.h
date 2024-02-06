#include "stm32f072xb.h"

#ifndef _GPIO_H
#define _GPIO_H

/**
 * @brief Initializes the GPIO peripheral
*/
void gpio_init();

/**
 * @brief Configures the GPIO pins to their desired mode
*/
void gpio_configure_pins();

#endif