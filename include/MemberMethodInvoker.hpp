/*
 * MemberMethodInvoker.h
 *
 *  Created on: Jan 24, 2020
 *      Author: chris
 */

#ifndef MEMBERMETHODINVOKER_H_
#define MEMBERMETHODINVOKER_H_

template <class TargetType>
class MemberMethodInvoker {
private:
	TargetType* target;
	void (TargetType::*method)();

public:
	MemberMethodInvoker(TargetType* target, void (TargetType::*method)()) : target(target), method(method) {
	}

	virtual ~MemberMethodInvoker();

	void operator()(void) {
		target->*method();
	}
};

#endif /* MEMBERMETHODINVOKER_H_ */
