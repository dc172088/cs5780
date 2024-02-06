#include "stm32f072xb.h"

void gpio_init();

typedef void (*callback) (void);

void gpio_configure_pins();

void gpio_delay();

int gpio_check_button();