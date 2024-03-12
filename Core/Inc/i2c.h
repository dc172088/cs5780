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
 * @brief Perform a blocking I2C read
*/
void i2c_write(uint8_t* buffer, uint8_t num_bytes, uint8_t address);

#endif