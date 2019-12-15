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

	TIM2->SMCR |= 0x0300 | 1; //SMS[2:0]
	TIM2->CR1 |= TIM_CR1_CEN;
}

void RotaryEncoder::updateSpeed() {

	int32_t currentTicks = xTaskGetTickCount();
	int32_t diffTicks = currentTicks - previousSysTick;
	if (diffTicks < 0) {
		diffTicks += 16777216;
	}

	uint16_t currentCount = TIM2->CNT;
	int32_t diffEncoder = previousEncoderCount - currentCount;
	if (diffEncoder < 0) {
		diffEncoder += 65535;
	}

	if (diffTicks > 1E2) {
		previousSysTick = currentTicks;
		previousEncoderCount = currentCount;
	}

	RotationState state(60 * diffEncoder * 1000 / diffTicks * portTICK_PERIOD_MS / 200);
	encoderStateHolder->set(&state);
}

void RotaryEncoder::setEncoderStateHolder(StateHolder<RotationState>* encoderStateHolder) {
	this->encoderStateHolder = encoderStateHolder;
}

Direction RotaryEncoder::getDirection() {

}

void RotaryEncoder::run() {

	while (1) {

		updateSpeed();

		vTaskDelay(pdMS_TO_TICKS(5));
	}
}
