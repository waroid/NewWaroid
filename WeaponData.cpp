/*
 * WeaponData.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "WeaponData.h"

#include <pthread.h>
#include <cerrno>
#include <cstdio>
#include <map>
#include <string>
#include <utility>

#include "core/GRCCore.h"
#include "core/GRCLogger.h"
#include "Defines.h"

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
	sprintf(path, "%s%s", DATA_DIR, "WaroidWeaponData.json");

	return loadFile(path);
}

bool WeaponData::onLoad(const Json::Value& data)
{
	for (auto iter = data.begin(); iter != data.end(); ++iter)
	{
		DATA* data = new DATA();
		strcpy(data->name, iter.name().c_str());
		data->id = iter->get("id", 0).asInt();
		data->secondid = iter->get("secondid", 0).asInt();
		data->repeat = iter->get("repeat", 0).asBool();
		strcpy(data->soundfilename, iter->get("soundfilename", "").asString().c_str());
		GRC_CHECK_RETFALSE(addData(data));
	}

	return true;
}

