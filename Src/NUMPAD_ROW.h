#ifndef __NUMPAD_ROW_H__
#define __NUMPAD_ROW_H__

#include "stm32f1xx_hal.h"
#include "lcd.h"
#include <string.h>

char scan_col(void);
char get_key(void);
void sid_manual(char password[]);

#define KEY_ROW0_OUT_LOW  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET) 
#define KEY_ROW1_OUT_LOW  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET)
#define KEY_ROW2_OUT_LOW  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET)
#define KEY_ROW3_OUT_LOW  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET)

#define KEY_ROW0_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET) 
#define KEY_ROW1_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET)
#define KEY_ROW2_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET)
#define KEY_ROW3_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET)


#endif

