/*
 * GRCWave.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef GRCWAVE_H_
#define GRCWAVE_H_

#include <alsa/asoundlib.h>
#include <pthread.h>
#include <stddef.h>
#include <cstring>

#include "GRCMutex.h"
#include "GRCObject.h"

class GRCWave : public GRCObject
{
public:
	enum DEFAULT_DATA
	{
		LOOPING = -1,
	};

	struct DATA
	{
		short* buffer;
		snd_pcm_uframes_t frames;
		DATA* next;

		DATA(short* _buffer, size_t _bufferSize, snd_pcm_uframes_t _frames)
				: 	buffer(NULL),
					frames(_frames),
					next(NULL)
		{
			buffer = new short[_bufferSize];
			memcpy(buffer, _buffer, _bufferSize);
		}
		~DATA()
		{
			if (buffer)
			{
				delete buffer;
				buffer = NULL;
			}
			frames = 0;
			next = NULL;
		}
	};

public:
	GRCWave();
	virtual ~GRCWave();

public:
	bool load(const char* soundDir, const char* wavFilename);
	void close();

	void play(int count = 1);
	void stop();

private:
	void playing();

protected:
	GRCMutex m_mutex;
	pthread_t m_playThread;
	snd_pcm_t* m_pcm;
	DATA* m_data;
	bool m_playing;
	int m_count;

private:
	static void* playWorker(void* param);
};

#endif /* GRCWAVE_H_ */
