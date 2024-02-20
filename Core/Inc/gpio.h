#include "stm32f072xb.h"

#ifndef _GPIO_H
#define _GPIO_H

/**
 * @brief Initializes the GPIO peripheral
*/
void gpio_init();

/**
 * @brief Helper method for setting a pin's state based
 * on a given number
 * @return Whether the given state was valid
*/
uint8_t gpio_process_pin(GPIO_TypeDef * gpio, uint32_t pin, uint8_t state);

/**
 * @brief Configures the GPIO pins to their desired mode
*/
void gpio_configure_pins();

#endif