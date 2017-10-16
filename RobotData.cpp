/*
 * RobotData.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "RobotData.h"

#include <pthread.h>
#include <cerrno>
#include <cstdio>
#include <fstream>
#include <map>
#include <string>
#include <utility>

#include "core/GRCCore.h"
#include "core/GRCLogger.h"
#include "Defines.h"

//#include "json/json.h"

RobotData::RobotData()
{
	// TODO Auto-generated constructor stub

}

RobotData::~RobotData()
{
	// TODO Auto-generated destructor stub
}

bool RobotData::load()
{
	char path[256];
	sprintf(path, "%s%s", DATA_DIR, "WaroidRobotData.json");

	return loadFile(path);
}

bool RobotData::onLoad(const Json::Value& data)
{
	for (auto iter = data.begin(); iter != data.end(); ++iter)
	{
		DATA* data = new DATA();
		strcpy(data->name, iter.name().c_str());
		data->id = iter->get("id", 0).asInt();
		strcpy(data->weaponname, iter->get("weaponname", "").asString().c_str());
		strcpy(data->attackedsoundfilename, iter->get("attackedsoundfilename", "").asString().c_str());
		strcpy(data->deathsoundfilename, iter->get("deathsoundfilename", "").asString().c_str());
		strcpy(data->revivesoundfilename, iter->get("revivesoundfilename", "").asString().c_str());
		GRC_CHECK_RETFALSE(addData(data));
	}

	return true;
}
