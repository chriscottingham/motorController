/*
 * GpioMode.h
 *
 *  Created on: Oct 21, 2019
 *      Author: chris
 */

#ifndef GPIOMODE_H_
#define GPIOMODE_H_

#include "TypedEnum.h"

struct GpioModeEnum {

	enum type {
		analogInput = 0,
		floatingInput = 4,
		pushPullInput = 8,
		pushPullOutput = 3,
		openDrainOutput = 7,
		altPushPullOutput = 0xb,
		altOpenDrainOutput = 0xf
	};
};

class GpioMode : public TypedEnum<GpioModeEnum> {
public:
	  GpioMode(type v) : TypedEnum(v) {}
	  bool isAlternate();
};

#endif /* GPIOMODE_H_ */
