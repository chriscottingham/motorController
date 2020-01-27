/*
 * LedDisplayDriver.h
 *
 *  Created on: Oct 15, 2019
 *      Author: chris
 */

#ifndef LEDDISPLAYDRIVER_H_
#define LEDDISPLAYDRIVER_H_

#include "stm32f10x.h"
#include "IoDriver.h"
#include "Logger.h"
#include "RotaryEncoder.h"
#include "SpeedInput.h"
#include "Point.h"
#include "Timer.hpp"

#define kI2cGpio GPIOB
#define kPowerGpio GPIOB

using namespace std;

class MotorDisplay {

public:
	enum INSTRUCTION_TYPE {
		COMMAND = 0x00,
		DATA = 0x40
	};

	struct BitValues {
		BitValues() : displayDirty(true), powerOn(false), displayInitialized(false) {};
		bool displayDirty:1;
		bool powerOn:1;
		bool displayInitialized:1;
	};

private:
	uint8_t displayBuffer[1025];
	uint8_t* font32_chars[85];

	BitValues bitValues;
	SpeedInput* speedInput;

	Timer timer;

	void initI2c();
	void initDma();

	void resetBuffer();
	Point<int> drawChars(char* source, uint8_t charCount, Point<int> topLeft);
	Point<int> drawNumber(int value, int charCount, Point<int> topLeft);
	void drawPowerBar();
	void drawBuffer();
	void sendBuffer();

public:
	const std::vector<uint8_t> kI2cPins = { 6, 7 };
	const std::vector<uint8_t> kPowerPin = { 9 };

	static uint8_t initializationSequence[];

	MotorDisplay();

	void setSpeedInput(SpeedInput* speedInput);

	void startI2c();
	void stop();

	void handleDma();
	void handleI2cInterrupt();
	void handleI2cError();

	void offerRun();
};

extern "C" void MotorDisplayTask(void*);

#endif /* LEDDISPLAYDRIVER_H_ */
