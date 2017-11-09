/*
 * RobotData.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "RobotData.h"

#include <cstdio>

#include "core/GRCCore.h"
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
	GRCString path(JSON_DATA_DIR);
	path.append("WaroidRobotData.json");

	return loadFile(path);
}

bool RobotData::onLoad(const RAPIDJSON_NAMESPACE::Value& data)
{
	for (auto iter = data.MemberBegin(); iter != data.MemberEnd(); ++iter)
	{
		DATA* data = new DATA();
		loadBaseData(iter, data);

		const RAPIDJSON_NAMESPACE::Value& v = iter->value;

		{
			auto siter = v.FindMember("weaponname");
			if (siter != v.MemberEnd())
				data->weaponname = siter->value.GetString();
		}

		{
			auto siter = v.FindMember("attackedsoundfilename");
			if (siter != v.MemberEnd())
				data->attackedsoundfilename = siter->value.GetString();
		}

		{
			auto siter = v.FindMember("deathsoundfilename");
			if (siter != v.MemberEnd())
				data->deathsoundfilename = siter->value.GetString();
		}

		{
			auto siter = v.FindMember("revivesoundfilename");
			if (siter != v.MemberEnd())
				data->revivesoundfilename = siter->value.GetString();
		}

		GRC_CHECK_RETFALSE(addData(data));
	}

	return true;
}
