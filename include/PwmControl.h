/*
 * PwmControl.h
 *
 *  Created on: Nov 16, 2019
 *      Author: chris
 */

#ifndef PWMCONTROL_H_
#define PWMCONTROL_H_

#include <task.h>

#include "IoDriver.h"
#include "SpeedInput.h"
#include "RotationState.h"

using namespace::std;

extern long MAX_MOTOR_SPEED;

class PwmControl {
private:
	GPIO_TypeDef* gpio;
	uint8_t outputPin;

	long maxMotorRpm;
	StateHolder<RotationState>* currentSpeed;
	StateHolder<RotationState>* desiredSpeed;

public:
	PwmControl(GPIO_TypeDef* gpio, uint8_t outputPin);
	virtual ~PwmControl();

	void setMaxMotorRpm(long maxRpm);
	void setCurrentSpeedHolder(StateHolder<RotationState>* currentSpeed);
	void setDesiredSpeedHolder(StateHolder<RotationState>* desiredSpeed);

	void run();
};

#endif /* PWMCONTROL_H_ */
