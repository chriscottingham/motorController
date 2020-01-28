/*
 * PwmControl.h
 *
 *  Created on: Nov 16, 2019
 *      Author: chris
 */

#ifndef PWMCONTROL_H_
#define PWMCONTROL_H_

#include "IoDriver.h"
#include "SpeedInput.h"
#include "RotaryEncoder.h"
#include "Timer.hpp"

using namespace::std;

extern long MAX_MOTOR_SPEED;

class PwmControl {
private:
	SpeedInput* speedInput;
	RotaryEncoder* rotaryEncoder;
	AdcController* adcController;
	Timer timer;

	bool faultMotorStop = false;
	int motorStoppedTime;
	GPIO_TypeDef* gpio;
	uint8_t outputPin;
	int adcCurrentIndex;
	int adcVoltageIndex;

	long maxMotorRpm;

public:
	PwmControl(GPIO_TypeDef* gpio, uint8_t outputPin);
	virtual ~PwmControl();

	void setMaxMotorRpm(long maxRpm);
	void setAdcCurrentIndex(int channel);
	void setAdcVoltageIndex(int channel);
	void setAdcController(AdcController* adcController);
	void setSpeedInput(SpeedInput* speedInput);
	void setRotaryEncoder(RotaryEncoder* rotaryEncoder);

	void runOnce();
};

#endif /* PWMCONTROL_H_ */
