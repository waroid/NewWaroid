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
#include "core/GRCString.h"
#include "Defines.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

class RobotData: public GRCJsonData
{
public:
	struct DATA: BASEDATA
	{
		GRCString weaponname;
		GRCString attackedsoundfilename;
		GRCString deathsoundfilename;
		GRCString revivesoundfilename;
		unsigned char movepowers[WAROIDDIRECTION::TOTAL];

		DATA()
		{
			bzero(movepowers, sizeof(movepowers));
		}

		virtual bool isValid() const override
		{
			return BASEDATA::isValid() && weaponname.isEmpty() == false;
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
	const DATA* find(GRCCSTR name) const
	{
		return (const DATA*) findData(name);
	}

protected:
	virtual bool onLoad(const RAPIDJSON_NAMESPACE::Value& data) override;

private:
	bool setMovePowers(DATA* data, const RAPIDJSON_NAMESPACE::Value& value);
};

#endif /* ROBOTDATA_H_ */
