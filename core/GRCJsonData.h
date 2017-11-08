/*
 * GRCJsonData.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef GRCJSONDATA_H_
#define GRCJSONDATA_H_

#include <stddef.h>
#include <cstring>
#include <map>

#include "../rapidjson/document.h"
#include "../rapidjson/rapidjson.h"
#include "GRCString.h"

class GRCJsonData
{
public:
	struct BASEDATA
	{
		GRCString name;
		int id;

		BASEDATA()
				: id(0)
		{
		}
		virtual ~BASEDATA()
		{
		}

		virtual bool isValid() const
		{
			return name.isEmpty() == false && id >= 0;
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
	virtual bool onLoad(const RAPIDJSON_NAMESPACE::Value& data) = 0;

	const BASEDATA* findData(int id) const;
	const BASEDATA* findData(GRCCSTR name) const;

	void loadBaseData(const RAPIDJSON_NAMESPACE::Value::ConstMemberIterator& iter, BASEDATA* data);
	bool addData(BASEDATA* data);

private:
	MapData m_datas;
};

#endif /* GRCJSONDATA_H_ */
