/*
 * GRCJsonData.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef GRCJSONDATA_H_
#define GRCJSONDATA_H_

#include <stddef.h>

#include "../rapidjson/document.h"
#include "../rapidjson/rapidjson.h"

class GRCJsonData
{
public:
	GRCJsonData();
	virtual ~GRCJsonData();

public:
	bool loadString(const char* str, size_t len);
	bool loadFile(const char* path);

protected:
	virtual bool onLoad(const RAPIDJSON_NAMESPACE::Value& data) = 0;
};

#endif /* GRCJSONDATA_H_ */
