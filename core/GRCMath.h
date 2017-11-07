/*
 * GRCMath.h
 *
 *  Created on: Nov 6, 2017
 *      Author: mirime
 */

#ifndef CORE_GRCMATH_H_
#define CORE_GRCMATH_H_

class GRCMath
{
public:
	template<typename T> static T Min(T v1, T v2)
	{
		return v1 > v2 ? v2 : v1;
	}

	template<typename T> static T Max(T v1, T v2)
	{
		return v1 > v2 ? v1 : v2;
	}

	template<typename T> static T Clamp(T v, T low, T hi)
	{
		return v < low ? low : (v > hi ? hi : v);
	}
};

#endif /* CORE_GRCMATH_H_ */
