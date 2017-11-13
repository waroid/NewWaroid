/*
 * GRCSoundWorker.cpp
 *
 *  Created on: Nov 8, 2017
 *      Author: mirime
 */

#include "GRCSoundWorker.h"

#include <alsa/error.h>
#include <alsa/pcm.h>
#include <pthread.h>
#include <cstring>
#include <map>
#include <queue>
#include <utility>

#include "../core/GRCCore.h"
#include "../core/GRCMutex.h"
#include "GRCWave.h"

typedef std::map<GRCString, GRCWave*> MapWave;
typedef std::queue<GRCWave*> QueueWave;

snd_pcm_t* GRCSoundWorker::s_pcm;

GRCMutex mutex;
pthread_t thread = GRC_INVALID_THREAD;
GRCString dir;
MapWave waveDatas;
GRCWave* currentWave;

bool GRCSoundWorker::start(GRCCSTR _dir)
{
	dir = _dir;

	int ret = snd_pcm_open(&s_pcm, "default", SND_PCM_STREAM_PLAYBACK, 0);
	GRC_CHECKV_RETFALSE(ret >= 0, "failed snd_pcm_open(). error=%s(%d)", snd_strerror(ret), ret);

	pthread_create(&thread, NULL, worker, NULL);

	return true;
}

void GRCSoundWorker::stop()
{
	if (thread != GRC_INVALID_THREAD)
	{
		pthread_cancel(thread);
		pthread_join(thread, NULL);
	}

	for (auto it : waveDatas)
	{
		delete it.second;
	}
	waveDatas.clear();

	if (s_pcm)
	{
		snd_pcm_hw_free (s_pcm);
		snd_pcm_drain(s_pcm);
		snd_pcm_close(s_pcm);
	}
}

bool GRCSoundWorker::add(GRCCSTR filename, bool repeat, int priority)
{
	GRC_CHECK_RETFALSE(filename);

	GRCMutexAutoLock autoLock(&mutex);

	auto iter = waveDatas.find(filename);
	if (iter != waveDatas.end()) return true;

	GRCWave* wave = new GRCWave(dir, filename, repeat, priority);
	GRC_CHECK_RETFALSE(wave);
	GRC_CHECK_FUNC_RETFALSE(wave->isValid(), delete wave);
	GRC_CHECK_FUNC_RETFALSE(waveDatas.insert(std::make_pair(filename, wave)).second, delete wave);

	return true;
}

void GRCSoundWorker::startPlay(GRCCSTR filename)
{
	GRC_CHECK_RETURN(filename);

	auto iter = waveDatas.find(filename);
	GRC_CHECK_RETURN(iter != waveDatas.end());

	{
		GRCMutexAutoLock autoLock(&mutex);
		if (currentWave)
		{
			if (currentWave->getPriority() >= iter->second->getPriority())
				return;

			currentWave->stop();
		}

		currentWave = iter->second;
	}

	mutex.signal();
}

void GRCSoundWorker::endPlay(GRCCSTR filename)
{
	{
		GRCMutexAutoLock autoLock(&mutex);
		if (currentWave && strcmp(currentWave->getFilename(), filename) == 0)
		{
			currentWave->stop();
			currentWave = NULL;
		}
	}
}

void* GRCSoundWorker::worker(void* param)
{
	GRC_INFO("[%s]start sound thread(0x%x)", "GRCSoundWorker", pthread_self());

	for (;;)
	{
		mutex.wait();

		mutex.lock();
		GRCWave* wave = currentWave;
		mutex.unlock();

		if (wave)
		{
			wave->play();

			mutex.lock();
			currentWave = NULL;
			mutex.unlock();
		}
	}

	GRC_INFO("[%s]stop sound thread(0x%x)", "GRCSoundWorker", pthread_self());

	return NULL;
}