/*
 * uart.c
 *
 *  Created on: Dec 4, 2023
 *      Author: K21
 */

#include "uart.h"
#include <stdio.h>

ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

int time = 300;
int time_flag = 0;


uint8_t mode = 0;
uint8_t mode1_code[] = "!RST#";
uint8_t mode2_code[] = "!OK#";
char str[9];
uint32_t ADC_value = 0;


void command_parser_fsm (uint8_t* buffer) {
	for (int i = 0; i < sizeof(mode1_code); i++) {
		if (buffer[i] != mode1_code[i]) {
			break;
		}
		if (mode1_code[i] == '#') {
			mode = 1;
			return;
		}
	}
	for (int i = 0; i < sizeof(mode2_code); i++) {
		if (buffer[i] != mode2_code[i]) {
			break;
		}
		if (mode1_code[i] == '#') {
			mode = 2;
			return;
		}
	}
}

void uart_communication_fsm (uint8_t* index_buffer) {
	if (mode != 1) return;
	if (time_flag == 1) {
		ADC_value = HAL_ADC_GetValue(&hadc1);
		int len = sprintf(str, "!%ld#\r\n", ADC_value);
		HAL_UART_Transmit(&huart2, (uint8_t *)str, len, 50);
		time_flag = 0;
		time = 300;
		index_buffer = 0;
	}

}


