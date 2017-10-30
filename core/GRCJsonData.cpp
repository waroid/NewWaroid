/*
 * GRCJsonData.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#include "GRCCore.h"
#include "GRCJsonData.h"

#include <cstdio>
#include <utility>


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

	char* buffer = new char[len + 1];
	int read = fread(f, sizeof(char), len, f);
	GRC_CHECK_FUNC_RETFALSE(len > 0, efunc(f, buffer));
	buffer[read] = 0;

	GRC_CHECK_FUNC_RETFALSE(loadString(buffer, len), efunc(f, buffer));

	fclose(f);
	delete buffer;

	return true;
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
		if (strcmp(it.second->name, name) == 0)
			return it.second;
	}

	return NULL;
}

bool GRCJsonData::addData(BASEDATA* data)
{
	GRC_CHECK_RETFALSE(data);
	GRC_CHECK_RETFALSE(data->isValid());

	return m_datas.insert(std::make_pair(data->id, data)).second;
}

void GRCJsonData::loadBaseData(const RAPIDJSON_NAMESPACE::Value::ConstMemberIterator& iter, BASEDATA* data)
{
	SAFE_STR_COPY(data->name, sizeof(data->name), iter->name.GetString());

	const RAPIDJSON_NAMESPACE::Value& v = iter->value;

	{
		auto siter = v.FindMember("id");
		if (siter != v.MemberEnd())
			data->id = siter->value.GetInt();
	}
}

