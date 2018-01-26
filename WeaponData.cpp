/*
 * WeaponData.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "WeaponData.h"

#include <cstdio>
#include <initializer_list>
#include <utility>

#include "core/GRCCore.h"

WeaponData::WeaponData()
{
	// TODO Auto-generated constructor stub

}

WeaponData::~WeaponData()
{
	// TODO Auto-generated destructor stub
	for (auto& it : m_datas)
	{
		delete it.second;
	}
	m_datas.clear();
}

const WeaponData::DATA* WeaponData::find(int id) const
{
	auto iter = m_datas.find(id);
	GRC_CHECK_RETNULL(iter != m_datas.end());

	return iter->second;
}

const WeaponData::DATA* WeaponData::find(GRCCSTR name) const
{
	for (auto& it : m_datas)
	{
		if (it.second->name.compareNoCase(name) == 0) return it.second;
	}

	return nullptr;
}

bool WeaponData::load()
{
	char path[256];
	sprintf(path, "%s%s", JSON_DATA_DIR, "WaroidWeaponData.json");

	return loadFile(path);
}

bool WeaponData::onLoad(const RAPIDJSON_NAMESPACE::Value& data)
{
	for (auto iter = data.MemberBegin(); iter != data.MemberEnd(); ++iter)
	{
		DATA* data = new DATA();

		data->name = iter->name.GetString();

		const RAPIDJSON_NAMESPACE::Value& v = iter->value;

		{
			auto siter = v.FindMember("id");
			if (siter != v.MemberEnd()) data->id = siter->value.GetInt();
		}

		{
			auto siter = v.FindMember("firetype");
			if (siter != v.MemberEnd())
				data->firetype = WAROIDWEAPONFIRE::getType(siter->value.GetString());
		}

		{
			auto siter = v.FindMember("soundfilename");
			if (siter != v.MemberEnd())
				data->soundfilename = siter->value.GetString();
		}

		GRC_CHECK_RETFALSE(data->isValid());
		GRC_CHECK_RETFALSE(m_datas.insert(std::make_pair(data->id, data)).second);
	}

	return true;
}

