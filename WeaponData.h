/*
 * WeaponData.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef WEAPONDATA_H_
#define WEAPONDATA_H_

#include "core/GRCJsonData.h"
#include "core/GRCString.h"
#include "Defines.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

class WeaponData: public GRCJsonData
{
public:
	struct DATA: public BASEDATA
	{
		WAROIDWEAPONFIRE::ETYPE firetype;
		GRCString soundfilename;

		DATA()
				: firetype(WAROIDWEAPONFIRE::UNKNOWN)
		{
		}

		bool isRepeat() const
		{
			return firetype == WAROIDWEAPONFIRE::GATLING;
		}
	};

public:
	WeaponData();
	virtual ~WeaponData();

public:
	bool load();

	const DATA* find(int id) const
	{
		return (const DATA*)findData(id);
	}
	const DATA* find(GRCCSTR name) const
	{
		return (const DATA*)findData(name);
	}

protected:
	virtual bool onLoad(const RAPIDJSON_NAMESPACE::Value& data) override;
};

#endif /* WEAPONDATA_H_ */
