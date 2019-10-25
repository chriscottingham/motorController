/*
 * TypedEnum.h
 *
 *  Created on: Oct 21, 2019
 *      Author: chris
 */

#ifndef TYPEDENUM_H_
#define TYPEDENUM_H_

template<typename EnumStruct, typename EnumValue = typename EnumStruct::type>
class TypedEnum : public EnumStruct
{
protected:
  typedef EnumValue type;
  EnumValue val;

public:

  TypedEnum() {}
  TypedEnum(type v) : val(v) {}
  EnumValue getValue() const { return val; }

  friend bool operator == (const TypedEnum & lhs, const TypedEnum & rhs) { return lhs.val == rhs.val; }
  friend bool operator != (const TypedEnum & lhs, const TypedEnum & rhs) { return lhs.val != rhs.val; }
  friend bool operator <  (const TypedEnum & lhs, const TypedEnum & rhs) { return lhs.val <  rhs.val; }
  friend bool operator <= (const TypedEnum & lhs, const TypedEnum & rhs) { return lhs.val <= rhs.val; }
  friend bool operator >  (const TypedEnum & lhs, const TypedEnum & rhs) { return lhs.val >  rhs.val; }
  friend bool operator >= (const TypedEnum & lhs, const TypedEnum & rhs) { return lhs.val >= rhs.val; }

};

struct color_def {
  enum type { red = 1, green = 13, blue };
};
typedef TypedEnum<color_def> color;

struct shape_def {
  enum type { circle, square, triangle };
};
typedef TypedEnum<shape_def, unsigned char> shape; // unsigned char representation

//int main(void)
//{
//  color c = color::red;
//  color d = color(color::red);
//  c=d;
//  bool flag = (c >= shape::triangle); // Compiler error.
//  return 0;
//}



#endif /* TYPEDENUM_H_ */
