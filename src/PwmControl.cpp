/*
 * PwmControl.cpp
 *
 *  Created on: Nov 16, 2019
 *      Author: chris
 */

#include <cmath>
#include "PwmControl.h"
#include "SpeedInput.h"
#include "system.hpp"

#define MAX_ARR 0xffff

PwmControl::PwmControl(GPIO_TypeDef* gpio, uint8_t outputPin) :
		timer(Timer(20, bind(&PwmControl::runOnce, this))), gpio(gpio), outputPin(outputPin) {

	System::getInstance().registerTickListener(&timer);

	IoDriver::initPin(gpio, vector<uint8_t>( { outputPin }), GpioMode::altPushPullOutput);

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->CCMR1 |= 0x60 | TIM_CCMR1_OC1FE;
//	TIM3->CCR2 |= 0x70;
	TIM3->ARR = MAX_ARR;
	TIM3->CCER |= 0x01;

	TIM3->CR1 |= TIM_CR1_CEN;

}

void PwmControl::setMaxMotorRpm(long maxRpm) {
	this->maxMotorRpm = maxRpm;
}

void PwmControl::setAdcVoltageIndex(int channel) {
	this->adcVoltageIndex = channel;
}

void PwmControl::setAdcCurrentIndex(int channel) {
	this->adcCurrentIndex = channel;
}

void PwmControl::setSpeedInput(SpeedInput* speedInput) {
	this->speedInput = speedInput;
}

void PwmControl::setRotaryEncoder(RotaryEncoder* rotaryEncoder) {
	this->rotaryEncoder = rotaryEncoder;
}
void PwmControl::setAdcController(AdcController* adcController) {
	this->adcController = adcController;
}
void PwmControl::runOnce() {

	static int previousValue = 0;

	uint16_t desiredSpeedValue = speedInput->getInputSpeed();
	uint16_t currentSpeedValue = rotaryEncoder->getSpeed();
	uint16_t diff = desiredSpeedValue - currentSpeedValue;
	if (diff > maxMotorRpm) {
		diff = 0;
	}
	uint32_t onPercentage = MAX_ARR * diff / desiredSpeedValue;
	if (onPercentage > MAX_ARR) {
		onPercentage = MAX_ARR;
	}

	//voltage adjustment
	uint16_t voltageAdc = adcController->getChannelValue(adcVoltageIndex);
	int voltage = 100 * 3.3 * voltageAdc / (float) 0xff;
	onPercentage *= 0.5f * (170 - voltage);

	//current limiting
	uint16_t currentAdc = adcController->getChannelValue(adcCurrentIndex);
	float senseVoltage = 3.3f * currentAdc / (float) MAX_ARR;
	float currentLimitFactor = 1 - senseVoltage; //std::pow(senseVoltage, 2);
	if (currentLimitFactor < 0) {
		currentLimitFactor = 0;
	}
	onPercentage *= currentLimitFactor;

	int isRunSwitch = (GPIO_IDR_IDR12 & GPIOA->IDR) >> 12;

	//stall
	if (isRunSwitch && !faultMotorStop) {

		if (currentSpeedValue <= 10) {

			if (desiredSpeedValue > 20) {

				if (motorStoppedTime == -1) {
					motorStoppedTime = System::getInstance().getSysTick();
				} else {
					int stoppedElapsedTime = System::getInstance().getSysTick() - motorStoppedTime;
					if (stoppedElapsedTime < 0) {
						stoppedElapsedTime += 2147483647;
					}
					if (stoppedElapsedTime > 1000) {
						faultMotorStop = true;
					}
				}
			} else {
				motorStoppedTime = -1;
			}
		} else {
			motorStoppedTime = -1;
		}
	}

	if (faultMotorStop) {
		onPercentage = 0;
	}

	onPercentage *= isRunSwitch;

	float i = 0.02;
	onPercentage = i * onPercentage + (1 - i) * previousValue;

	TIM3->CCR1 = onPercentage;
	previousValue = onPercentage;
}

PwmControl::~PwmControl() {
	// TODO Auto-generated destructor stub
}

