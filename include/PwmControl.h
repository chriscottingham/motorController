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

using namespace::std;

extern long MAX_MOTOR_SPEED;

class PwmControl {
private:
	SpeedInput* speedInput;
	RotaryEncoder* rotaryEncoder;
	AdcController* adcController;

	bool faultMotorStop = false;
	int motorStoppedTime;
	GPIO_TypeDef* gpio;
	uint8_t outputPin;
	int currentAdcChannel;
	int voltageAdcChannel;

	long maxMotorRpm;

public:
	PwmControl(GPIO_TypeDef* gpio, uint8_t outputPin);
	virtual ~PwmControl();

	void setMaxMotorRpm(long maxRpm);
	void setCurrentAdcChannel(int channel);
	void setVoltageAdcChannel(int channel);
	void setAdcController(AdcController* adcController);
	void setSpeedInput(SpeedInput* speedInput);
	void setRotaryEncoder(RotaryEncoder* rotaryEncoder);

	void tick();
};

#endif /* PWMCONTROL_H_ */
