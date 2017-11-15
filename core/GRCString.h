/*
 * GRCString.h
 *
 *  Created on: Nov 7, 2017
 *      Author: mirime
 */

#ifndef CORE_GRCSTRING_H_
#define CORE_GRCSTRING_H_

#include <stddef.h>
#include <stdio.h>
#include <cstring>

typedef const char* GRCCSTR;

class GRCString
{
public:
	static const size_t TRUNCATE = (size_t)-1;

public:
	GRCString();
	GRCString(const GRCString& str);
	GRCString(GRCCSTR str, size_t count=TRUNCATE);
	virtual ~GRCString();

public:
	bool isEmpty() const
	{
		return m_str[0] == 0;
	}
	size_t length() const
	{
		return strlen(m_str);
	}
	size_t size() const
	{
		return m_size;
	}

	void clear();
	void assign(GRCCSTR str, size_t count = TRUNCATE);
	void append(GRCCSTR str, size_t count = TRUNCATE);
	void append(char ch);
	void vformat(GRCCSTR fmt, va_list& vl);
	void format(GRCCSTR fmt, ...);
	int compare(GRCCSTR str) const;
	int compareNoCase(GRCCSTR str) const;

	GRCString& operator=(const GRCString& str)
	{
		assign(str.m_str);
		return *this;
	}
	GRCString& operator=(GRCCSTR str)
	{
		assign(str);
		return *this;
	}

	GRCString& operator+=(const GRCString& str)
	{
		append(str.m_str);
		return *this;
	}
	GRCString& operator+=(GRCCSTR str)
	{
		append(str);
		return *this;
	}
	GRCString& operator+=(char ch)
	{
		append(ch);
		return *this;
	}

	GRCCSTR operator*() const
	{
		return m_str;
	}
	operator GRCCSTR() const
	{
		return m_str;
	}
	operator char*()
	{
		return m_str;
	}

	bool operator==(const GRCString& str) const
	{
		return compare(str.m_str) == 0;
	}
	bool operator==(GRCCSTR str) const
	{
		return compare(str) == 0;
	}
	bool operator!=(const GRCString& str) const
	{
		return compare(str.m_str) != 0;
	}
	bool operator!=(GRCCSTR str) const
	{
		return compare(str) != 0;
	}
	bool operator<(const GRCString& str) const
	{
		return compare(str.m_str) < 0;
	}
	bool operator<(GRCCSTR str) const
	{
		return compare(str) < 0;
	}

private:
	void resize(size_t size);

private:
	char* m_str;
	size_t m_size;
};

#endif /* CORE_GRCSTRING_H_ */

