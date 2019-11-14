/*
 * Queue.h
 *
 *  Created on: Nov 12, 2019
 *      Author: chris
 */

#ifndef STATEHOLDER_H_
#define STATEHOLDER_H_

template<typename ValueType> class StateHolder {

public:

	virtual void set(ValueType message) = 0;
	virtual ValueType get() = 0;
	virtual ValueType get(int millis) = 0;

	virtual StateHolder() {};
	virtual ~StateHolder() {};
};



#endif /* STATEHOLDER_H_ */
