#include "stm32f072xb.h"

#ifndef _I2C_H
#define _I2C_H

/**
 * @brief Initializes the I2C peripheral
*/
void i2c_init();

/**
 * @brief Initializes GPIO used for I2C lab
*/
void i2c_gpio_init();

/**
 * @brief Configures the GPIO pins to their desired mode
*/
void gpio_configure_pins();

#endif