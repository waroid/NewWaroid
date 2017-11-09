/*
 * GRCObject.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: mirime
 */

#include "GRCObject.h"

#include <cstdarg>


GRCObject::GRCObject()
{
	// TODO Auto-generated constructor stub
}

GRCObject::~GRCObject()
{
	// TODO Auto-generated destructor stub
}

void GRCObject::updateObjName(GRCCSTR format, ...)
{
	va_list vl;
	va_start(vl, format);
	m_objName.vformat(format, vl);
	va_end(vl);
}

