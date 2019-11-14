/*
 * RotaryEncoder.cpp
 *
 *  Created on: Nov 3, 2019
 *      Author: chris
 */

#include <cstring>
#include "RotaryEncoder.h"

#include "task.h"
#include "portmacro.h"

RotaryEncoder::RotaryEncoder(GPIO_TypeDef* timerPort, vector<uint8_t>* const encoderPins) :
		timerPort(timerPort) {

	this->encoderPins = *encoderPins;

	IoDriver::initPin(timerPort, this->encoderPins, GpioMode::floatingInput);

	RCC->APB1ENR |= 1;

	TIM2->SMCR |= 1; //SMS[2:0]
	TIM2->CR1 |= TIM_CR1_CEN;
}

EncoderState* RotaryEncoder::getState() {

	return &encoderState;
}

void RotaryEncoder::updateSpeed() {

	int32_t diffTicks = previousSysTick - TIM2->CNT;
	if (diffTicks < 0) {
		diffTicks += 16777216;
	}

	int32_t diffEncoder = previousEncoderCount - TIM2->CNT;
	if (diffEncoder < 0) {
		diffEncoder += 65535;
	}

	encoderState.rpm = diffEncoder / 200 / diffTicks * portTICK_PERIOD_MS;
}

Direction RotaryEncoder::getDirection() {

}

void RotaryEncoder::run() {

	while (1) {

		updateSpeed();

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void RotaryEncoderTask(void* param) {
	((RotaryEncoder*) param)->run();
}

