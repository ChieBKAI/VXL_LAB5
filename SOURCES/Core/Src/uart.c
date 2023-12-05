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

	//timer for adc print out
int time = TIME_OUT;
int time_flag = 0;

	//receive in variable
uint8_t temp = 0;
uint8_t buffer[ MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

	//stage for fsm
uint8_t mode = 0;
uint8_t mode1_code[] = "!RST#";
uint8_t mode2_code[] = "!OK#";

	//adc print out variable
char str[20];
uint32_t ADC_value = 0;

void clear_buffer() {
	for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
		buffer[i] = 0;
	}
	index_buffer = 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == huart2.Instance) {
		if (temp == '!') {
			clear_buffer();
		}
		buffer[index_buffer++] = temp ;
		if( index_buffer == 30) {
			index_buffer = 0;
		}
		buffer_flag = 1;
		time = TIME_OUT; //do not print out value while receiving data
		HAL_UART_Receive_IT(&huart2, &temp, 1);
	}
}


void command_parser_fsm () {
	for (int i = 0; i < sizeof(mode1_code); i++) {
		if (buffer[i] != mode1_code[i]) { //escape if not in this case
			break;
		}
		if (mode1_code[i] == '#') { //change stage if command is true
			mode = 1;
			HAL_GPIO_WritePin(BLINKY_LED_GPIO_Port, BLINKY_LED_Pin, 0); //turn on;
			return;
		}
	}
	for (int i = 0; i < sizeof(mode2_code); i++) {
		if (buffer[i] != mode2_code[i]) {//escape if not in this case
			break;
		}
		if (mode2_code[i] == '#') {	//change stage if command is true
			mode = 2;
			HAL_GPIO_WritePin(BLINKY_LED_GPIO_Port, BLINKY_LED_Pin, 1); //turn off
			return;
		}
	}
}

void uart_communication_fsm () {
	if (mode != 1) { //escape if not in this case
		return;
	}
	if (time_flag == 1) {
		ADC_value = HAL_ADC_GetValue(&hadc1);
		int len = sprintf(str, "!ADC=%ld#\r\n", ADC_value);
		HAL_UART_Transmit(&huart2, (uint8_t *)str, len, 50);
		time_flag = 0;
		time = TIME_OUT; //reset timer for print output
	}

}


