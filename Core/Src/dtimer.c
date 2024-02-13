#include "dtimer.h"

void timer_init() {
    // Enable peripherals on clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Setup update event at 4Hz
    TIM2->PSC = (uint16_t)7999;  // 1ms period
    TIM2->ARR = (uint16_t)250;   // count to 250ms

    // Setup update event at 800Hz
    TIM3->PSC = (uint16_t)999;  // 0.125ms period
    TIM3->ARR = (uint16_t)9;   // count to 1.25ms

    // Configure capture/compare
    TIM3->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIM3->CCMR1 &= ~TIM_CCMR1_CC2S;

    // Output channel 1 to PWM mode 1
    TIM3->CCMR1 |= TIM_CCMR1_OC1M;
    TIM3->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;

    // Enable output compare reload
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM3->CCMR1 |= TIM_CCMR1_OC2PE;

    // Set capture/compare registers to 20% PWM
    TIM3->CCR1 = (uint16_t)2;
    TIM3->CCR2 = (uint16_t)2;

    // Enable output channels
    TIM3->CCER |= TIM_CCER_CC1E;
    TIM3->CCER |= TIM_CCER_CC2E;

    // Enable DMA interrupt
    TIM2->DIER |= TIM_DIER_UIE;

    // Enable timer AFTER configuration
    TIM2->CR1 |= TIM_CR1_CEN;

    TIM3->CR1 |= TIM_CR1_CEN;

    // Timer 2 interrupt
    NVIC_EnableIRQ(TIM2_IRQn);

    // Set bit for alternate function mode
    GPIOC->MODER |= GPIO_MODER_MODER6_1;
    GPIOC->MODER |= GPIO_MODER_MODER7_1;

    // Set to use AF0
    GPIOC->AFR[0] &= ~GPIO_AFRL_AFSEL6;
    GPIOC->AFR[0] &= ~GPIO_AFRL_AFSEL7;
}

void TIM2_IRQHandler() {
    // Toggle the orange and green LEDs
    GPIOC->ODR ^= GPIO_ODR_8;
    GPIOC->ODR ^= GPIO_ODR_9;

    // Clear (acknowledge) interrupt for TIM2
    TIM2->SR &= ~TIM_SR_UIF;
}