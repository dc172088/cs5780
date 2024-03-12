#include "i2c.h"

void i2c_init() {
    // Enable the I2C2 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // Configure I2C2 to be 100KHz
    I2C2->TIMINGR |= I2C_TIMINGR_PRESC;              // Set prescaler bit
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
    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL10;              // AF2
    GPIOB->AFR[1] |= 0x4UL << GPIO_AFRH_AFSEL10_Pos;  // AF2

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