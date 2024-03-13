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
#include "i2c.h"
#include "usart.h"

void SystemClock_Config(void);

// Declare the MEMS I2C Address
const uint8_t mems_i2c_address = 0x69;

/**
 * @brief Macro for building an int16_t from two bytes
 */
#define ASSEMBLE_INT16(LOW, HIGH) ((int16_t)LOW | (((int16_t)HIGH) << 8))

/**
 * @brief Macro for thresholds for triggering the LEDs
 */
#define LED_THRESHOLD (1000)

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

    i2c_init();
    i2c_gpio_init();

    // Perform write to who am i register
    // static uint8_t who_am_i_register = 0x0F;
    // i2c_write(&who_am_i_register, 1, mems_i2c_address);

    // // Perform read from who am i register
    // static uint8_t who_am_i_value = 0;
    // i2c_read(&who_am_i_value, 1, mems_i2c_address);

    // // Send stop command
    // i2c_stop();

    // Verify that we received correct value
    // if (who_am_i_value == 0xd3) {
    //     GPIOC->ODR |= GPIO_ODR_9; // Green for success!
    // } else {
    //     GPIOC->ODR |= GPIO_ODR_6; // Red for failure :(
    // }

    // Initialize sensor
    const uint8_t control_reg1_address = 0x20;
    static uint8_t init_sensor_data[] = {I2C_ENABLE_MULTIPLE_BYTES(control_reg1_address), 0b01001111};
    i2c_write(init_sensor_data, 2, mems_i2c_address);
    i2c_stop();

    while (1) {
        // Delay updates
        HAL_Delay(100);

        // Setup write
        const uint8_t base_angle_address = 0x28;
        static uint8_t base_angle_data[] = {I2C_ENABLE_MULTIPLE_BYTES(base_angle_address)};
        i2c_write(base_angle_data, 1, mems_i2c_address);

        // Setup multi-byte read
        static uint8_t angle_data[6];
        i2c_read(angle_data, 6, mems_i2c_address);
        i2c_stop();

        // Parse data
        int16_t x_angle = ASSEMBLE_INT16(angle_data[0], angle_data[1]);
        int16_t y_angle = ASSEMBLE_INT16(angle_data[2], angle_data[3]);
        // int16_t z_angle = ASSEMBLE_INT16(angle_data[4], angle_data[5]);

        static int32_t x_total = 0;
        static int32_t y_total = 0;

        x_total = x_angle;
        y_total = y_angle;
        // static int16_t za = 0;

        // Trigger LEDs Based on Orientation
        if (x_total > LED_THRESHOLD) {
            GPIOC->ODR |= GPIO_ODR_9;  // Green
            GPIOC->ODR &= ~GPIO_ODR_8;

        } else if (x_total < -LED_THRESHOLD) {
            GPIOC->ODR |= GPIO_ODR_8;  // Orange
            GPIOC->ODR &= ~GPIO_ODR_9;
        } else {
            GPIOC->ODR &= ~(GPIO_ODR_8 | GPIO_ODR_9);  // Not green or orange
        }
        if (y_total > LED_THRESHOLD) {
            GPIOC->ODR |= GPIO_ODR_6;  // Red
            GPIOC->ODR &= ~GPIO_ODR_7;
        } else if (y_total < -LED_THRESHOLD) {
            GPIOC->ODR |= GPIO_ODR_7;  // Blue
            GPIOC->ODR &= ~GPIO_ODR_6;
        } else {
            GPIOC->ODR &= ~(GPIO_ODR_6 | GPIO_ODR_7);  // Not red or blue
        }
    }
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
