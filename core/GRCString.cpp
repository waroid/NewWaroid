/*
 * GRCString.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: mirime
 */

#include "GRCString.h"

#include <stdlib.h>
#include <string.h>
#include <cstdarg>

#include "GRCCore.h"
#include "GRCMath.h"

namespace GRC_STRING
{
	const size_t MIN_SIZE = 64;
	const size_t FORMAT_MIN_SIZE = 255;
}
;
using namespace GRC_STRING;

GRCString::GRCString()
		: 	m_str(NULL),
			m_size(0)
{
	// TODO Auto-generated constructor stub
	resize(0);
}
GRCString::GRCString(const GRCString& str)
		: 	m_str(NULL),
			m_size(0)
{
	// TODO Auto-generated constructor stub
	str.isEmpty() ? resize(0) : assign(str);

}
GRCString::GRCString(GRCCSTR str, size_t count)
		: 	m_str(NULL),
			m_size(0)
{
	// TODO Auto-generated constructor stub
	str ? assign(str, count) : resize(0);
}

GRCString::~GRCString()
{
	// TODO Auto-generated destructor stub
	if (m_str) free(m_str);
}

void GRCString::clear()
{
	m_str[0] = 0;
}

void GRCString::assign(GRCCSTR str, size_t count)
{
	GRC_CHECK_RETURN(str);

	size_t len = GRCMath::Min(strlen(str), count);
	if (len > 0)
	{
		resize(len);
		strncpy(m_str, str, len);
	}
}

void GRCString::append(GRCCSTR str, size_t count)
{
	GRC_CHECK_RETURN(str);
	size_t len = GRCMath::Min(strlen(str), count);
	if (len > 0)
	{
		resize(length() + len);
		strncat(m_str, str, len);
	}
}

void GRCString::append(char ch)
{
	size_t len = length();
	resize(len + 1);
	m_str[len] = ch;
}

void GRCString::vformat(GRCCSTR fmt, va_list& vl)
{
	resize(FORMAT_MIN_SIZE);
	int len = vsnprintf(m_str, m_size - 1, fmt, vl);
	if (len < 0) len = 0;
	m_str[len] = 0;
}

void GRCString::format(GRCCSTR fmt, ...)
{
	if (fmt == NULL) return;

	va_list vl;
	va_start(vl, fmt);
	vformat(fmt, vl);
	va_end(vl);
}

int GRCString::compare(GRCCSTR str) const
{
	return strcmp(m_str, str);
}

int GRCString::compareNoCase(GRCCSTR str) const
{
	return strcasecmp(m_str, str);
}

void GRCString::resize(size_t size)
{
	size = GRCMath::Max<size_t>(size + 1, MIN_SIZE);
	if (m_size < size)
	{
		char* temp = reinterpret_cast<char*>(malloc(size));
		memset(temp, 0, size);
		if (m_str)
		{
			strncpy(temp, m_str, size - 1);
			free(m_str);
		}
		m_str = temp;
		m_size = size;
	}
}

