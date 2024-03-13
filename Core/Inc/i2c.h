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
 * @brief Perform a blocking I2C write
 * @param buffer buffer to write data from
 * @param num_bytes number of bytes to write
 * @param address 7-bit I2C address to write to
 * @return a bool
 * 1 - operation succeeded
 * 0 - operation failed
*/
uint8_t i2c_write(uint8_t* buffer, uint8_t num_bytes, uint8_t address);

/**
 * @brief Perform a blocking I2C read
 * @param buffer buffer to read data to
 * @param num_bytes number of bytes to read
 * @param address 7-bit I2C address to read from
 * @return a bool
 * 1 - operation succeeded
 * 0 - operation failed
*/
uint8_t i2c_read(uint8_t* buffer, uint8_t num_bytes, uint8_t address);

/**
 * @brief Send the I2C stop condition
*/
void i2c_stop();

/**
 * @brief Macro for setting most significant bit to enable
 * multiple byte transactions for specific devices
*/
#define I2C_ENABLE_MULTIPLE_BYTES(ADDRESS) (ADDRESS | 128)

#endif