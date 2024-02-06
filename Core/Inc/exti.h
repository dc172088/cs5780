#include "stm32f072xb.h"

#ifndef _EXTI_H
#define _EXTI_H

/**
 * @brief Initializes the EXTI peripheral
*/
void exti_init();

/**
 * @brief Configure the EXTI with NVIC
*/
void exti_configure();

#endif