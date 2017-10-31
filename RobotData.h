/*
 * RobotData.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef ROBOTDATA_H_
#define ROBOTDATA_H_

#include <cstring>

#include "core/GRCJsonData.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

class RobotData: public GRCJsonData
{
public:
	struct DATA: BASEDATA
	{
		char weaponname[100];
		char attackedsoundfilename[100];
		char deathsoundfilename[100];
		char revivesoundfilename[100];

		DATA()
		{
			bzero(weaponname, sizeof(weaponname));
			bzero(attackedsoundfilename, sizeof(attackedsoundfilename));
			bzero(deathsoundfilename, sizeof(deathsoundfilename));
			bzero(revivesoundfilename, sizeof(revivesoundfilename));
		}

		virtual bool isValid() const override
		{
			return BASEDATA::isValid() && weaponname[0] != 0;
		}

	};

public:
	RobotData();
	virtual ~RobotData();

public:
	bool load();

	const DATA* find(int id) const
	{
		return (const DATA*) findData(id);
	}
	const DATA* find(const char* name) const
	{
		return (const DATA*) findData(name);
	}

protected:
	virtual bool onLoad(const RAPIDJSON_NAMESPACE::Value& data) override;
};

#endif /* ROBOTDATA_H_ */
