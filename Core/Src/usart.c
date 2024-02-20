#include "usart.h"

#include "stm32f0xx_hal.h"

uint8_t usart_command[2];
uint8_t usart_status = 0;

void usart_init() {
    /* Enable USART pins in RCC */
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  // Enable GPIOB
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

    // Configure alternate function
    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL10;              // AF2
    GPIOB->AFR[1] |= 0x4UL << GPIO_AFRH_AFSEL10_Pos;  // AF2

    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL11;              // AF0
    GPIOB->AFR[1] |= 0x4UL << GPIO_AFRH_AFSEL11_Pos;  // AF0

    /* Configure USART peripheral */
    // Compute baud rate register from baud = f_CK / USARTDIV
    uint32_t freq = HAL_RCC_GetHCLKFreq();
    uint16_t baud = freq / 115200UL;
    USART3->BRR = baud;

    // Enable receive interrupt
    USART3->CR1 |= USART_CR1_RXNEIE;
    NVIC_EnableIRQ(USART3_4_IRQn);

    USART3->CR1 |= USART_CR1_TE;  // Enable transmitter
    USART3->CR1 |= USART_CR1_RE;  // Enable receiver
    USART3->CR1 |= USART_CR1_UE;  // Enable USART
}

/**
 * @brief Handler for read interrupt
*/
void USART3_4_IRQHandler() {
    // Return if last message has not been processed
    if (usart_status >= 2) {
        // Read register to clear interrupt
        (void)USART3->RDR;
    }
    // Write data into command buffer
    usart_command[usart_status++] = USART3->RDR;
}

void usart_write_byte(uint8_t byte) {
    // Wait for register to become available to write into
    while (!(USART3->ISR & USART_ISR_TXE)) {
        asm("nop");
    }

    // Write data into transmit register
    USART3->TDR = byte;
}

uint8_t usart_read_byte() {
    // Wait for register to have data to read
    while (!(USART3->ISR & USART_ISR_RXNE)) {
        asm("nop");
    }

    // Read data from register
    return USART3->RDR;
}

void usart_write_string(const char* const str) {
    for (uint8_t i = 0; i < UINT8_MAX; i++) {
        if (str[i] == 0) {
            break;
        }
        usart_write_byte(str[i]);
    }
}
