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
#include <stddef.h>
#include <stdlib.h>
#include <cstring>

#include "../core/GRCCore.h"
#include "../core/GRCCoreUtil.h"
#include "GRCSoundWorker.h"

namespace GRC_WAVE
{
	const snd_pcm_format_t DEFAULT_FORMAT = SND_PCM_FORMAT_S16_LE;
}
using namespace GRC_WAVE;

GRCWave::GRCWave(GRCCSTR dir, GRCCSTR filename, bool repeat, int priority)
		: 	m_filename(filename),
			m_repeat(repeat),
			m_priority(priority),
			m_channels(0),
			m_sampleRate(0),
			m_playing(false)
{
	// TODO Auto-generated constructor stub

	snd_pcm_t* pcm = GRCSoundWorker::getPcm();
	GRC_CHECK_RETURN(pcm);

	GRCString path;
	path.format("%s%s", dir, filename);

	SF_INFO info;
	bzero(&info, sizeof(info));
	SNDFILE* file = sf_open(path, SFM_READ, &info);
	GRC_CHECKV_RETURN(file, "error=%s", sf_strerror(file));
	GRC_CHECK_RETURN((info.format & SF_FORMAT_WAV) == SF_FORMAT_WAV);

	m_channels = info.channels;
	m_sampleRate = info.samplerate;

	snd_pcm_uframes_t frames = 0;
	GRC_CHECK_RETURN(updateParams(pcm, &frames));

	size_t dataSize = frames * info.channels * sizeof(short);
	GRC_INFO("%s: channels=%d rate=%d frames=%d data=%d", filename, m_channels, m_sampleRate, frames, dataSize);

	sf_count_t count = frames;
	short* data = reinterpret_cast<short*>(malloc(dataSize));
	sf_count_t read = sf_readf_short(file, data, count);
	while (read > 0)
	{
		m_datas.push_back(new DATA(data, read));

		data = reinterpret_cast<short*>(malloc(dataSize));
		read = sf_readf_short(file, data, count);
	}
	free(data);
	sf_close(file);
}

GRCWave::~GRCWave()
{
	// TODO Auto-generated destructor stub
	for (auto* d : m_datas)
	{
		delete d;
	}
	m_datas.clear();
}

void GRCWave::play()
{
	snd_pcm_t* pcm = GRCSoundWorker::getPcm();
	GRC_CHECK_RETURN(pcm);
	GRC_CHECK_RETURN(updateParams(pcm, NULL));

	m_playing = true;

	snd_pcm_sframes_t written = 0;

	do
	{
		for (auto* d : m_datas)
		{
			if (m_playing == false)
			{
				snd_pcm_drop(pcm);
				return;
			}

			written = snd_pcm_writei(pcm, d->buffer, d->frames);
			if (written == -EPIPE)
			{
				GRC_ERR("Underrun");
				snd_pcm_prepare(pcm);
			}
			else if (written < 0)
			{
				GRC_ERR("error writing to PCM device. (%s)", snd_strerror(written));
				return;
			}
			else if (written != static_cast<snd_pcm_sframes_t>(d->frames))
			{
				GRC_ERR("write differs from read.");
			}

			GRCCoreUtil::sleep(0.001);
		}
	}
	while (m_repeat);

	snd_pcm_drain(pcm);
}

void GRCWave::stop()
{
	m_playing = false;
}

bool GRCWave::updateParams(snd_pcm_t* pcm, snd_pcm_uframes_t* frames)
{
	snd_pcm_hw_params_t* params;
	snd_pcm_hw_params_alloca(&params);
	GRC_CHECK_RETFALSE(params);
	int ret = snd_pcm_hw_params_any(pcm, params);
	GRC_CHECKV_RETFALSE(ret >= 0, "failed snd_pcm_hw_params_set_format(). error=%s(%d)", snd_strerror(ret), ret);

	ret = snd_pcm_hw_params_set_format(pcm, params, SND_PCM_FORMAT_S16_LE);
	GRC_CHECKV_RETFALSE(ret >= 0, "failed snd_pcm_hw_params_set_format(). error=%s(%d)", snd_strerror(ret), ret);

	ret = snd_pcm_hw_params_set_access(pcm, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	GRC_CHECKV_RETFALSE(ret >= 0, "failed snd_pcm_hw_params_set_access(). error=%s(%d)", snd_strerror(ret), ret);

	ret = snd_pcm_hw_params_set_channels(pcm, params, m_channels);
	GRC_CHECKV_RETFALSE(ret >= 0, "failed snd_pcm_hw_params_set_channels(). error=%s(%d)", snd_strerror(ret), ret);

	unsigned int rate = m_sampleRate;
	ret = snd_pcm_hw_params_set_rate_near(pcm, params, &rate, NULL);
	GRC_CHECKV_RETFALSE(ret >= 0, "failed snd_pcm_hw_params_set_rate_near(). error=%s(%d)", snd_strerror(ret), ret);

	ret = snd_pcm_hw_params(pcm, params);
	GRC_CHECKV_RETFALSE(ret >= 0, "failed snd_pcm_hw_params(). error=%s(%d)", snd_strerror(ret), ret);

	if (frames)
	{
		ret = snd_pcm_hw_params_get_period_size(params, frames, NULL);
		GRC_CHECKV_RETFALSE(ret >= 0, "failed snd_pcm_hw_params_get_period_size(). error=%s(%d)", snd_strerror(ret), ret);
	}

	return true;
}
