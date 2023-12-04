/*
 * uart.h
 *
 *  Created on: Dec 4, 2023
 *      Author: K21
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#define MAX_BUFFER_SIZE 30
#define TIME_OUT 100

extern uint8_t temp;
extern uint8_t buffer_flag;
extern int time;
extern int time_flag;

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void command_parser_fsm ();
void uart_communication_fsm ();

#endif /* INC_UART_H_ */
