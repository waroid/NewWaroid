/*
 * WeaponData.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "WeaponData.h"

#include <cstdio>

#include "core/GRCCore.h"

WeaponData::WeaponData()
{
	// TODO Auto-generated constructor stub

}

WeaponData::~WeaponData()
{
	// TODO Auto-generated destructor stub
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
		loadBaseData(iter, data);

		const RAPIDJSON_NAMESPACE::Value& v = iter->value;

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

		GRC_CHECK_RETFALSE(addData(data));
	}

	return true;
}

