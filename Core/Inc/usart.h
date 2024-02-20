#ifndef _USART_H
#define _USART_H
#include "stm32f072xb.h"

/**
 * @brief Initializes usart
 * PB10 - UART3 TX
 * PB11 - UART3 RX
*/
void usart_init();

/**
 * @brief Write a character to USART
 * @note BLOCKING
 * @param byte the byte to transmit
*/
void usart_write_byte(uint8_t byte);

/**
 * @brief Reads a byte from USART
 * @note BLOCKING
 * @return the byte read
*/
uint8_t usart_read_byte();

/**
 * @brief Helper method for writing a string to USART
 * @note BLOCKING
 * @note strings are limited to 256 bytes
 * @param str the string to write
*/
void usart_write_string(const char* const str);

#endif