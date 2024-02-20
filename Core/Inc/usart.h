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
 * @brief Helper method for writing a string to USART
 * @note BLOCKING
 * @param str the string to write
*/
void usart_write_string(const char* const str);

#endif