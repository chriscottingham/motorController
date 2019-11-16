/*
 * SpeedState.h
 *
 *  Created on: Nov 16, 2019
 *      Author: chris
 */

#ifndef ROTATIONSTATE_H_
#define ROTATIONSTATE_H_

struct RotationState {
	RotationState(long rpm) : rpm(rpm) { }
	uint16_t rpm;
};

#endif /* ROTATIONSTATE_H_ */
