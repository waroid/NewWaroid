/*
 * GRCWave.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef GRCWAVE_H_
#define GRCWAVE_H_

#include <stdio.h>
#include <pthread.h>
#include <alsa/asoundlib.h>

#include "GRCMutex.h"

class GRCWave
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
				: buffer(NULL), frames(_frames), next(NULL)
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

	void play(int count=1);
	void stop();

private:
	void playing();

protected:
	char m_name[100];
	pthread_t m_thread;
	snd_pcm_t* m_pcm;
	DATA* m_data;
	GRCMutex m_mutex;
	bool m_playing;
	int m_count;

private:
	static void* playWorker(void* param);
};

#endif /* GRCWAVE_H_ */
