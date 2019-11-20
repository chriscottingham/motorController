/*
 * Point.h
 *
 *  Created on: Nov 19, 2019
 *      Author: chris
 */

#ifndef POINT_H_
#define POINT_H_

template<class ValueType>
class Point {
private:
	ValueType x;
	ValueType y;

public:
	Point(int x, int y) : x(x), y(y){
	}
	int getX() {return x;}
	int getY() {return y;}
	virtual ~Point() {}
};

#endif /* POINT_H_ */
