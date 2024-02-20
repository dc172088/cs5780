#include "usart.h"

#include "stm32f0xx_hal.h"

void usart_init() {
    /* Enable USART pins in RCC */
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;     // Enable GPIOB
    (void)RCC->AHBENR;
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;  // Enable USART3
    (void)RCC->APB1ENR;

    /* Configure GPIO pins */
    // Clear bits
    GPIOB->MODER &= ~GPIO_MODER_MODER10_Msk;
    GPIOB->MODER &= ~GPIO_MODER_MODER11_Msk;
    // Set bit for alternate function mode
    GPIOB->MODER |= GPIO_MODER_MODER10_1;  // PB10 - AF2
    GPIOB->MODER |= GPIO_MODER_MODER11_1;  // PB11 - AF0
    GPIOB->ODR |= GPIO_ODR_11;

    // Configure alternate function
    GPIOB->AFR[1] &= ~(0xDUL << GPIO_AFRH_AFSEL10_Pos);  // AF2
    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL11;                 // AF0

    // Configure GPIO modes
    // Pullup on receive line
    // GPIOB->PUPDR |= GPIO_PUPDR_PUPDR11_0;

    /* Configure USART peripheral */
    // Compute baud rate register from baud = f_CK / USARTDIV
    uint32_t freq = HAL_RCC_GetHCLKFreq();
    uint16_t baud = freq / 115200UL;
    USART3->BRR = baud;
    // USART3->BRR |= (uint16_t) (HAL_RCC_GetHCLKFreq() / 115200UL);
    USART3->CR1 |= USART_CR1_TE;  // Enable transmitter
    USART3->CR1 |= USART_CR1_RE;  // Enable receiver
    USART3->CR1 |= USART_CR1_UE;  // Enable USART
}

void usart_write_byte(uint8_t byte) {
    // Wait for register to become available to write into
    while (!(USART3->ISR & USART_ISR_TXE)) {
    }

    // Write data into transmit register
    USART3->TDR = (uint8_t)byte;
}

void usart_write_string(const char* const str) {
}
