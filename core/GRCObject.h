/*
 * GRCObject.h
 *
 *  Created on: Nov 2, 2017
 *      Author: mirime
 */

#ifndef SRC_CORE_GRCOBJECT_H_
#define SRC_CORE_GRCOBJECT_H_

class GRCObject
{
public:
	GRCObject();
	virtual ~GRCObject();

public:
	const char* getObjName() const
	{
		return m_objName;
	}

	void updateObjName(const char* format, ...);

private:
	char m_objName[100];
};

#endif /* SRC_CORE_GRCOBJECT_H_ */
