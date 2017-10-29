/*
 * GRCJsonData.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef GRCJSONDATA_H_
#define GRCJSONDATA_H_

#include <cstring>
#include <map>

#include "json.h"

class GRCJsonData
{
public:
	struct BASEDATA
	{
		char name[100];
		int id;

		BASEDATA()
				: id(0)
		{
			bzero(name, sizeof(name));
		}
	};
	typedef std::map<int, BASEDATA*> MapData;

public:
	GRCJsonData();
	virtual ~GRCJsonData();

public:
	bool loadString(const char* str, size_t len);
	bool loadFile(const char* path);

protected:
	virtual bool onLoad(const Json::Value& data) = 0;

	bool addData(BASEDATA* data);
	const BASEDATA* findData(int id) const;
	const BASEDATA* findData(const char* name) const;

private:
	MapData m_datas;
};

#endif /* GRCJSONDATA_H_ */
