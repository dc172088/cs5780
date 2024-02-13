#include "exti.h"

#include "core_cm0.h"

void exti_init() {
    // Unmask the EXTI input line for interrupt generation
    EXTI->IMR |= EXTI_IMR_IM0;
    // Configure rising edge trigger
    EXTI->RTSR |= EXTI_RTSR_RT0;
}

void exti_configure() {
    // Button interrupt
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn, 1);
    NVIC_SetPriority(SysTick_IRQn, 0);
}

void EXTI0_1_IRQHandler() {
    // Toggle the orange and green LEDs
    GPIOC->ODR ^= GPIO_ODR_8;
    for (volatile uint32_t i = 0; i < 1500000; i++) {
    }
    GPIOC->ODR ^= GPIO_ODR_9;

    // Clear (acknowledge) interrupt for exti0
    EXTI->PR |= EXTI_PR_PIF0;
}