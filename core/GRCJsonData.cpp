/*
 * GRCJsonData.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#include "GRCJsonData.h"

#include <cstdio>

#include "GRCCore.h"

GRCJsonData::GRCJsonData()
{
	// TODO Auto-generated constructor stub

}

GRCJsonData::~GRCJsonData()
{
	// TODO Auto-generated destructor stub
}

bool GRCJsonData::loadString(const char* str, size_t len)
{
	GRC_CHECK_RETFALSE(str);
	GRC_CHECK_RETFALSE(len > 0);

	RAPIDJSON_NAMESPACE::Document doc;
	doc.Parse(str, len);

	GRC_CHECK_RETFALSE(onLoad(doc["Data"]));

	return true;
}

bool GRCJsonData::loadFile(const char* path)
{
	GRC_CHECK_RETFALSE(path);

	FILE* f = fopen(path, "r");
	GRC_CHECK_RETFALSE(f);

	auto efunc = [](FILE* f, char* buffer)
	{
		if (f) fclose(f);
		if (buffer) delete buffer;
	};

	fseek(f, 0, SEEK_END);
	int len = ftell(f);
	GRC_CHECK_FUNC_RETFALSE(len > 0, efunc(f, NULL));
	fseek(f, 0, SEEK_SET);

	char* buffer = new char[len + 1];
	int read = fread(buffer, sizeof(char), len, f);
	GRC_CHECK_FUNC_RETFALSE(len > 0, efunc(f, buffer));
	buffer[read] = 0;

	GRC_CHECK_FUNC_RETFALSE(loadString(buffer, len), efunc(f, buffer));

	fclose(f);
	delete buffer;

	return true;
}



