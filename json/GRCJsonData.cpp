/*
 * GRCJsonData.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#include "GRCJsonData.h"

#include <pthread.h>
#include <stddef.h>
#include <cerrno>
#include <cstring>
#include <map>
#include <string>
#include <utility>

#include "../core/GRCCore.h"
#include "../core/GRCLogger.h"
#include "json.h"

GRCJsonData::GRCJsonData()
{
	// TODO Auto-generated constructor stub

}

GRCJsonData::~GRCJsonData()
{
	// TODO Auto-generated destructor stub
	for (auto& it : m_datas)
	{
		delete it.second;
	}
	m_datas.clear();
}

bool GRCJsonData::loadString(const char* str, size_t len)
{
	GRC_CHECK_RETFALSE(str);
	GRC_CHECK_RETFALSE(len > 0);

	Json::Value root;
	Json::Reader reader;
	GRC_CHECK_RETFALSE(reader.parse(str, str + len, root, false));
	GRC_CHECK_RETFALSE(onLoad(root["Data"]));

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

	char* buffer = new char[len + 1];
	int read = fread(f, sizeof(char), len, f);
	GRC_CHECK_FUNC_RETFALSE(len > 0, efunc(f, buffer));
	buffer[read] = 0;

	GRC_CHECK_FUNC_RETFALSE(loadString(buffer, len), efunc(f, buffer));

	fclose(f);
	delete buffer;

	return true;
}

bool GRCJsonData::addData(BASEDATA* data)
{
	return m_datas.insert(std::make_pair(data->id, data)).second;
}

const GRCJsonData::BASEDATA* GRCJsonData::findData(int id) const
{
	auto iter = m_datas.find(id);
	GRC_CHECK_RETNULL(iter != m_datas.end());

	return iter->second;
}

const GRCJsonData::BASEDATA* GRCJsonData::findData(const char* name) const
{
	for (auto& it : m_datas)
	{
		if (strcmp(it.second->name, name) == 0) return it.second;
	}

	return NULL;
}
