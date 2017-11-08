/*
 * WeaponData.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef WEAPONDATA_H_
#define WEAPONDATA_H_

#include <cstring>

#include "core/GRCJsonData.h"
#include "core/GRCString.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

class WeaponData: public GRCJsonData
{
public:
	struct DATA: public BASEDATA
	{
		int secondid;
		bool repeat;
		GRCString soundfilename;

		DATA()
				: secondid(0), repeat(false)
		{
		}
	};

public:
	WeaponData();
	virtual ~WeaponData();

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
};

#endif /* WEAPONDATA_H_ */
