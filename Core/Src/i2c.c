#include "i2c.h"

void i2c_init() {
    // Enable the I2C2 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // Configure I2C2 to be 100KHz
    I2C2->TIMINGR |= 0x1 << I2C_TIMINGR_PRESC_Pos;   // Set prescaler bit
    I2C2->TIMINGR |= 0x13 << I2C_TIMINGR_SCLL_Pos;   // Set SCLL value
    I2C2->TIMINGR |= 0xF << I2C_TIMINGR_SCLH_Pos;    // Set SCLH value
    I2C2->TIMINGR |= 0x2 << I2C_TIMINGR_SDADEL_Pos;  // Set SDADEL value
    I2C2->TIMINGR |= 0x4 << I2C_TIMINGR_SCLDEL_Pos;  // Set SCLDEL value

    // Enable I2C2 peripheral
    I2C2->CR1 |= I2C_CR1_PE;
}

void i2c_gpio_init() {
    // Enable Clock for GPIO B and C
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

    /* Configure PB11 and PB13 */
    GPIOB->MODER &= ~GPIO_MODER_MODER11_Msk;  // SDA
    GPIOB->MODER &= ~GPIO_MODER_MODER13_Msk;  // SCL

    // Set bit for alternate function mode
    GPIOB->MODER |= GPIO_MODER_MODER11_1;  // PB11 - AF1
    GPIOB->MODER |= GPIO_MODER_MODER13_1;  // PB13 - AF5

    // Configure output type to open drain
    GPIOB->OTYPER |= GPIO_OTYPER_OT_11 || GPIO_OTYPER_OT_13;

    // Configure alternate function
    GPIOB->AFR[1] |= 0x1UL << GPIO_AFRH_AFSEL11_Pos;  // AF1
    GPIOB->AFR[1] |= 0x5UL << GPIO_AFRH_AFSEL13_Pos;  // AF5

    // Configure internal pull-up resistors
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR11_0;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR13_0;

    /* Configure PB14 */
    // Set bit for output mode
    GPIOB->MODER |= GPIO_MODER_MODER14_0;

    // Clear bit for push-pull output type
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT_14;

    // Initialize value to 1
    GPIOB->ODR |= GPIO_ODR_14;

    /* Configure PC0 */
    // Set bit for output mode
    GPIOC->MODER |= GPIO_MODER_MODER0_0;

    // Clear bit for push-pull output type
    GPIOC->OTYPER &= ~GPIO_OTYPER_OT_0;

    // Initialize value to 1
    GPIOC->ODR |= GPIO_ODR_0;
}

uint8_t i2c_write(uint8_t* buffer, uint8_t num_bytes, uint8_t address) {
    // Set I2C address
    I2C2->CR2 &= ~I2C_CR2_SADD;                     // Clear I2C address
    I2C2->CR2 |= address << I2C_CR2_SADD_Pos << 1;  // IMPORTANT: bit shift by 1 for 7-bit addresses

    // Indicate number of bytes to write
    I2C2->CR2 &= ~I2C_CR2_NBYTES;                  // First clear out old data
    I2C2->CR2 |= num_bytes << I2C_CR2_NBYTES_Pos;  // Write number of bytes

    // Indicate Write Operation
    I2C2->CR2 &= ~I2C_CR2_RD_WRN;

    // Indicate to not use AUTOEND
    I2C2->CR2 &= ~I2C_CR2_AUTOEND;

    // Set start bit to begin transfer
    I2C2->CR2 |= I2C_CR2_START;

    // Wait for transmit ready bit
    while (!(I2C2->ISR & I2C_ISR_TXIS)) {
    }

    // Check for ACK
    if (I2C2->ISR & I2C_ISR_NACKF) {
        return 0;
    }

    // Loop to send data
    for (uint8_t i = 0; i < num_bytes; i++) {
        // Send byte at a time
        I2C2->TXDR = buffer[i];

        // Wait for register to be transferred
        while (!(I2C2->ISR & I2C_ISR_TXE)) {
        }
    }

    // Wait for transmission
    while (!(I2C2->ISR & I2C_ISR_TC)) {
    }
    return 1;
}

uint8_t i2c_read(uint8_t* buffer, uint8_t num_bytes, uint8_t address) {
    // Set I2C address
    I2C2->CR2 &= ~I2C_CR2_SADD;                     // Clear I2C address
    I2C2->CR2 |= address << I2C_CR2_SADD_Pos << 1;  // IMPORTANT: bit shift by 1 for 7-bit addresses

    // Indicate number of bytes to read
    I2C2->CR2 &= ~I2C_CR2_NBYTES;                  // First clear out old data
    I2C2->CR2 |= num_bytes << I2C_CR2_NBYTES_Pos;  // Read number of bytes

    // Indicate Read Operation
    I2C2->CR2 |= I2C_CR2_RD_WRN;

    // Indicate to not use AUTOEND
    I2C2->CR2 &= ~I2C_CR2_AUTOEND;

    // Set start bit to begin transfer
    I2C2->CR2 |= I2C_CR2_START;

    // Wait for receive ready bit
    while (!(I2C2->ISR & I2C_ISR_RXNE)) {
    }

    // Check for ACK
    if (I2C2->ISR & I2C_ISR_NACKF) {
        return 0;
    }

    // Loop to send data
    for (uint8_t i = 0; i < num_bytes; i++) {
        // Wait to receive data
        while (!(I2C2->ISR & I2C_ISR_RXNE)) {
        }

        // Receive one byte at a time
        buffer[i] = I2C2->RXDR;
    }

    // Wait for transmission
    while (!(I2C2->ISR & I2C_ISR_TC)) {
    }
    return 1;
}

void i2c_stop() {
    I2C2->CR2 |= I2C_CR2_STOP;
}
