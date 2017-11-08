/*
 * GRCObject.h
 *
 *  Created on: Nov 2, 2017
 *      Author: mirime
 */

#ifndef SRC_CORE_GRCOBJECT_H_
#define SRC_CORE_GRCOBJECT_H_

#include "GRCString.h"

class GRCObject
{
public:
	GRCObject();
	virtual ~GRCObject();

public:
	GRCCSTR getObjName() const
	{
		return m_objName;
	}

	void updateObjName(GRCCSTR format, ...);

private:
	GRCString m_objName;
};

#endif /* SRC_CORE_GRCOBJECT_H_ */
