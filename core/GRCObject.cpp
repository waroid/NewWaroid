/*
 * GRCObject.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: mirime
 */

#include "GRCObject.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>


GRCObject::GRCObject()
{
	// TODO Auto-generated constructor stub
	bzero(m_objName, sizeof(m_objName));
}

GRCObject::~GRCObject()
{
	// TODO Auto-generated destructor stub
}

void GRCObject::updateObjName(const char* format, ...)
{
	va_list vl;
	va_start(vl, format);
	int len = vsnprintf(m_objName, sizeof(m_objName) - 2, format, vl);
	va_end(vl);
	if (len < 0) len = 0;
	m_objName[len] = 0;
}

