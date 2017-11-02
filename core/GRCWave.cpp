/*
 * GRCWave.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#include "GRCWave.h"

#include <alsa/error.h>
#include <alsa/pcm.h>
#include <asm-generic/errno-base.h>
#include <sndfile.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>

#include "GRCCore.h"

namespace GRC_WAVE
{
}
using namespace GRC_WAVE;

GRCWave::GRCWave()
		: 	m_playThread(GRC_INVALID_THREAD),
			m_pcm(NULL),
			m_data(NULL),
			m_playing(false),
			m_count(0)
{
	// TODO Auto-generated constructor stub
}

GRCWave::~GRCWave()
{
	// TODO Auto-generated destructor stub
}

bool GRCWave::load(const char* soundDir, const char* wavFilename)
{
	GRC_CHECK_RETFALSE(soundDir);
	GRC_CHECK_RETFALSE(wavFilename);

	GRC_CHECK_RETFALSE(m_pcm==NULL);

	SNDFILE* file = NULL;
	snd_pcm_t* pcm = NULL;
	auto efunc = [&file,&pcm]()
	{
		if (file) sf_close(file);
		if (pcm)
		{
			snd_pcm_drain(pcm);
			snd_pcm_close(pcm);
		}
	};

	char path[256];
	sprintf(path, "%s%s", soundDir, wavFilename);
	SF_INFO info;
	file = sf_open(path, SFM_READ, &info);
	GRC_CHECK_RETFALSE(file);
	GRC_CHECK_FUNC_RETFALSE((info.format & SF_FORMAT_WAV) == SF_FORMAT_WAV, efunc());

	GRC_CHECK_FUNC_RETFALSE(snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0) >= 0, efunc());

	snd_pcm_hw_params_t* params;
	GRC_CHECK_FUNC_RETFALSE(snd_pcm_hw_params_malloc(&params) >= 0, efunc());
	GRC_CHECK_FUNC_RETFALSE(snd_pcm_hw_params_any(m_pcm, params) >= 0, efunc());
	GRC_CHECK_FUNC_RETFALSE(snd_pcm_hw_params_set_access(m_pcm, params, SND_PCM_ACCESS_RW_INTERLEAVED) >= 0, efunc());
	GRC_CHECK_FUNC_RETFALSE(snd_pcm_hw_params_set_format(m_pcm, params, SND_PCM_FORMAT_S16_LE) >= 0, efunc());
	GRC_CHECK_FUNC_RETFALSE(snd_pcm_hw_params_set_channels(m_pcm, params, info.channels) >= 0, efunc());
	unsigned int rate = info.samplerate;
	GRC_CHECK_FUNC_RETFALSE(snd_pcm_hw_params_set_rate_near(m_pcm, params, &rate, NULL)>=0, efunc());
	GRC_CHECK_FUNC_RETFALSE(snd_pcm_hw_params(m_pcm, params) >= 0, efunc());

	snd_pcm_uframes_t frames;
	GRC_CHECK_FUNC_RETFALSE(snd_pcm_hw_params_get_period_size(params, &frames, NULL)>=0, efunc());

	snd_pcm_hw_params_free(params);

	DATA** data = &m_data;
	size_t bufferSize = frames * info.channels * sizeof(short);
	short* buffer = (short*)malloc(bufferSize);
	sf_count_t readFrames = sf_readf_short(file, buffer, frames);
	while (readFrames > 0)
	{
		*data = new DATA(buffer, bufferSize, readFrames);
		data = &((*data)->next);

		readFrames = sf_readf_short(file, buffer, frames);
	}

	free(buffer);
	sf_close(file);

	updateObjName(wavFilename);
	m_pcm = pcm;

	pthread_create(&m_playThread, NULL, playWorker, this);

	return true;
}

void GRCWave::close()
{
	if (m_playThread != GRC_INVALID_THREAD)
	{
		if (pthread_cancel(m_playThread) == 0)
		{
			pthread_join(m_playThread, NULL);
		}
		GRC_DEV("[%s]cancel thread", getObjName());
	}

	DATA* data = m_data;
	m_data = NULL;
	while (data)
	{
		DATA* next = data->next;
		delete data;
		data = next;
	}

	if (m_pcm)
	{
		snd_pcm_drain(m_pcm);
		snd_pcm_close(m_pcm);
		m_pcm = NULL;
	}

}

void GRCWave::play(int count)
{
	m_playing = true;
	m_count = count;
	m_mutex.signal();
}

void GRCWave::stop()
{
	m_playing = false;
	m_count = 0;
}

void GRCWave::playing()
{
	for (;;)
	{
		m_mutex.wait();

		if (m_pcm)
		{
			for (int i = 0; i < m_count || m_count == LOOPING; ++i)
			{
				snd_pcm_sframes_t writeFrames;
				DATA* data = m_data;
				while (m_playing && data)
				{
					writeFrames = snd_pcm_writei(m_pcm, data->buffer, data->frames);
					if (writeFrames == -EPIPE)
					{
						GRC_LOG("Underrun");
						snd_pcm_prepare(m_pcm);
					}
					else if (writeFrames < 0)
					{
						GRC_LOG("error writing to PCM device. (%s)", snd_strerror(writeFrames));
						return;
					}
					else if (writeFrames != (snd_pcm_sframes_t)data->frames)
					{
						GRC_LOG("write differs from read. (%s)");
					}

					usleep(1000);
					data = data->next;
				}
				usleep(10000);
			}
		}
	}
}

void* GRCWave::playWorker(void* param)
{
	GRCWave* wave = (GRCWave*)param;

	GRC_LOG("[%s]start play thread(0x%x)", wave->getObjName(), pthread_self());
	wave->playing();
	GRC_LOG("[%s]stop play thread(0x%x)", wave->getObjName(), pthread_self());

	return NULL;
}

