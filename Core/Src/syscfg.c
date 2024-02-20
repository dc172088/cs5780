#include "syscfg.h"

void syscfg_init() {
    // Enable the SYSCFG peripheral clock
    RCC->APB1ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

    // Route the correct PA0 to EXTI0
    SYSCFG->EXTICR[0] &= SYSCFG_EXTICR1_EXTI0_PA;
}