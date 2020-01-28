/*
 * Timer.cpp
 *
 *  Created on: Jan 23, 2020
 *      Author: chris
 */

#include <Timer.hpp>

Timer::Timer(uint32_t period, function<void (void)> callback) : period(period), callback(callback) {
}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}

void Timer::tick() {
	tickCount += 1;
	if (tickCount >= period) {
//		callback();
		ready = true;
//		tickCount = 0;
	}
}

void Timer::offerRun() {
	if (ready) {
		callback();
		reset();
	}
}

bool Timer::isReady() {
	return ready;
}

void Timer::reset() {
	ready = false;
	tickCount = 0;
}
