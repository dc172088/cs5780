/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "dtimer.h"
#include "gpio.h"
#include "usart.h"

// Declare globals in usart.c
extern uint8_t usart_command[2];
extern uint8_t usart_status;

void SystemClock_Config(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    gpio_init();
    gpio_configure_pins();

    usart_init();

    usart_write_string("Begin by entering a command:\n\r");
    while (1) {
        // Handle when a command is received
        if (usart_status >= 2) {
            uint8_t color = usart_command[0];
            uint8_t command = usart_command[1];
            switch (color) {
                // Red case
                case 'r': {
                    if (gpio_process_pin(GPIOC, GPIO_ODR_6, command)) {
                        usart_write_string("red\n\r\n\r");
                    }
                    break;
                }
                // Blue case
                case 'b': {
                    if (gpio_process_pin(GPIOC, GPIO_ODR_7, command)) {
                        usart_write_string("blue\n\r\n\r");
                    }
                    break;
                }
                // Orange case
                case 'o': {
                    if (gpio_process_pin(GPIOC, GPIO_ODR_8, command)) {
                        usart_write_string("orange\n\r\n\r");
                    }
                    break;
                }
                // Green case
                case 'g': {
                    if (gpio_process_pin(GPIOC, GPIO_ODR_9, command)) {
                        usart_write_string("green\n\r\n\r");
                    }
                    break;
                }

                default:
                    usart_write_string("invalid letter\n\r\n\r");
                    break;
            }
            usart_status = 0;
            usart_write_string("Enter a command:\n\r");
        }
    }
}

uint8_t gpio_process_pin(GPIO_TypeDef* gpio, uint32_t pin, uint8_t state) {
    switch (state) {
        // Clear state, turn LED off
        case '0': {
            gpio->ODR &= ~pin;
            usart_write_string("turned off: ");
            break;
        }
        // Set state, turn LED on
        case '1': {
            gpio->ODR |= pin;
            usart_write_string("turned on: ");
            break;
        }
        // Toggle state, toggle LED
        case '2': {
            usart_write_string("toggled: ");
            gpio->ODR ^= pin;
            break;
        }

        default:
            usart_write_string("invalid pin state character\n\r\n\r");
            return 0;
    }
    return 1;
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
