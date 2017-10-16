/*
 * WeaponData.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef WEAPONDATA_H_
#define WEAPONDATA_H_

#include <cstring>

#include "json/GRCJsonData.h"
#include "json/json.h"

class WeaponData: public GRCJsonData
{
public:
	struct DATA: public BASEDATA
	{
		int secondid;
		bool repeat;
		char soundfilename[100];

		DATA()
				: secondid(0), repeat(false)
		{
			bzero(name, sizeof(name));
			bzero(soundfilename, sizeof(soundfilename));
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
	const DATA* find(const char* name) const
	{
		return (const DATA*) findData(name);
	}

protected:
	virtual bool onLoad(const Json::Value& data) override;
};

#endif /* WEAPONDATA_H_ */
