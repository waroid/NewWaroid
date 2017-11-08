/*
 * GRCSoundWorker.h
 *
 *  Created on: Nov 8, 2017
 *      Author: mirime
 */

#ifndef SOUND_GRCSOUNDWORKER_H_
#define SOUND_GRCSOUNDWORKER_H_

#include "../core/GRCString.h"

class GRCSoundWorker
{
public:
	static bool start(GRCCSTR dir);
	static void stop();

	static bool add(GRCCSTR filename, bool repeat, int priority);

	static void startPlay(GRCCSTR filename);
	static void endPlay(GRCCSTR filename);

private:
	static void* worker(void* param);
};

#endif /* SOUND_GRCSOUNDWORKER_H_ */
