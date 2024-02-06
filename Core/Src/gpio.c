#include "gpio.h"
#include "stm32f0xx_hal.h"

void gpio_init() {
    // Enable Clock for GPIO
    // RCC->APB1ENR |= 0x1U << 19U;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN;

}

void gpio_configure_pins() {
    /* USER Button */
    // Clear GPIOA
    GPIOA->MODER &= 0;

    // Set bit for input mode
    GPIOA->MODER = 0x0U << GPIO_MODER_MODER0_Pos;
    // Configure low speed
    GPIOA->OSPEEDR = 0x0U << GPIO_OSPEEDR_OSPEEDR0_Pos;
    // Configure pull down resistor
    GPIOA->PUPDR = 0x2U << GPIO_PUPDR_PUPDR0_Pos;

    /* LEDs */
    GPIOC->MODER &= 0;

    // Configure PC6 PC7 PC8 and PC9
    
    // Set bit for general purpose output mode
    GPIOC->MODER = (0x1U << GPIO_MODER_MODER6_Pos) | (0x1U << GPIO_MODER_MODER7_Pos) | (0x1U << GPIO_MODER_MODER8_Pos) | (0x1U << GPIO_MODER_MODER9_Pos);
    // Configure push-pull output type
    GPIOC->OTYPER &= 0;
    // Configure low speed
    GPIOC->OSPEEDR &= 0;
    // No pullup or pulldown resistors
    GPIOC->PUPDR &= 0;
}