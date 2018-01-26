/*
 * RobotData.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "RobotData.h"

#include <utility>

#include "core/GRCCore.h"

//#include "json/json.h"

RobotData::RobotData()
{
	// TODO Auto-generated constructor stub

}

RobotData::~RobotData()
{
	// TODO Auto-generated destructor stub
	for (auto& it : m_datas)
		{
			delete it.second;
		}
		m_datas.clear();
}

bool RobotData::load()
{
	GRCString path(JSON_DATA_DIR);
	path.append("WaroidRobotData.json");

	return loadFile(path);
}

const RobotData::DATA* RobotData::find(int id) const
{
	auto iter = m_datas.find(id);
	GRC_CHECK_RETNULL(iter != m_datas.end());

	return iter->second;
}

const RobotData::DATA* RobotData::find(GRCCSTR name) const
{
	for (auto& it : m_datas)
	{
		if (it.second->name.compareNoCase(name) == 0) return it.second;
	}

	return nullptr;
}

bool RobotData::onLoad(const RAPIDJSON_NAMESPACE::Value& data)
{
	for (auto iter = data.MemberBegin(); iter != data.MemberEnd(); ++iter)
	{
		DATA* data = new DATA();

		data->name = iter->name.GetString();

		const RAPIDJSON_NAMESPACE::Value& v = iter->value;

		{
			auto siter = v.FindMember("type");
			if (siter != v.MemberEnd()) data->type = siter->value.GetInt();
		}

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

		{
			auto siter = v.FindMember("movepower");
			if (siter != v.MemberEnd())
			{
				GRC_CHECK_RETFALSE(setMovePowers(data, siter->value));
			}

		}

		GRC_CHECK_RETFALSE(data->isValid());
		GRC_CHECK_RETFALSE(m_datas.insert(std::make_pair(data->type, data)).second);
	}

	return true;
}

bool RobotData::setMovePowers(DATA* data, const RAPIDJSON_NAMESPACE::Value& value)
{

	for (auto iter = value.MemberBegin(); iter != value.MemberEnd(); ++iter)
	{
		WAROIDDIRECTION::ETYPE dir = WAROIDDIRECTION::getType(iter->name.GetString());
		GRC_CHECK_RETFALSE(dir != WAROIDDIRECTION::NONE);

		data->movepowers[dir] = iter->value.GetInt();
	}

	return true;
}
