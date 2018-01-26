/*
 * WeaponData.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef WEAPONDATA_H_
#define WEAPONDATA_H_

#include <map>

#include "core/GRCJsonData.h"
#include "core/GRCString.h"
#include "Defines.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

class WeaponData: public GRCJsonData
{
public:
	struct DATA
	{
		GRCString name;
		int id;
		WAROIDWEAPONFIRE::ETYPE firetype;
		GRCString soundfilename;

		DATA()
				: 	id(0),
					firetype(WAROIDWEAPONFIRE::UNKNOWN)
		{
		}

		bool isValid() const
		{
			return name.isEmpty() == false && id >= 0;
		}

		bool isRepeat() const
		{
			return firetype == WAROIDWEAPONFIRE::GATLING;
		}
	};
	using MapData = std::map<int, DATA*>;

public:
	WeaponData();
	virtual ~WeaponData();

public:
	bool load();

	const DATA* find(int id) const;
	const DATA* find(GRCCSTR name) const;

protected:
	virtual bool onLoad(const RAPIDJSON_NAMESPACE::Value& data) override;

private:
	MapData m_datas;
};

#endif /* WEAPONDATA_H_ */
