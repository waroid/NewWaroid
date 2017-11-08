/*
 * GRCWave.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#include "GRCWave.h"

#include <sndfile.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#include "../core/GRCCore.h"

namespace GRC_WAVE
{
	const snd_pcm_format_t DEFAULT_FORMAT = SND_PCM_FORMAT_S16_LE;
}
using namespace GRC_WAVE;

GRCWave::GRCWave(GRCCSTR dir, GRCCSTR filename, bool repeat, int priority)
		: 	m_filename(filename),
			m_repeat(repeat),
			m_priority(priority),
			m_pcm(NULL),
			m_params(NULL),
			m_playing(false)
{
	// TODO Auto-generated constructor stub

	GRCString path;
	path.format("%s%s", dir, filename);

	SF_INFO info;
	SNDFILE* file = sf_open(path, SFM_READ, &info);
	GRC_CHECK_RETURN(file);

	GRC_CHECK_RETURN((info.format & SF_FORMAT_WAV) == SF_FORMAT_WAV);

	GRC_CHECK_RETURN(snd_pcm_open(&m_pcm, "default", SND_PCM_STREAM_PLAYBACK, 0) >= 0);
	GRC_CHECK_RETURN(m_pcm);

	GRC_CHECK_RETURN(snd_pcm_hw_params_malloc(&m_params) >= 0);
	GRC_CHECK_RETURN(snd_pcm_hw_params_any(m_pcm, m_params) >= 0);

	GRC_CHECK_RETURN(snd_pcm_hw_params_set_access(m_pcm, m_params, SND_PCM_ACCESS_RW_INTERLEAVED) >= 0);
	GRC_CHECK_RETURN(snd_pcm_hw_params_set_format(m_pcm, m_params, SND_PCM_FORMAT_S16_LE) >= 0);

	unsigned int rate = info.samplerate;
	GRC_CHECK_RETURN(snd_pcm_hw_params_set_rate_near(m_pcm, m_params, &rate, NULL)>=0);

	snd_pcm_uframes_t frames = 32;
	GRC_CHECK_RETURN(snd_pcm_hw_params_set_period_size_near(m_pcm, m_params, &frames, NULL)>=0);

	GRC_CHECK_RETURN(snd_pcm_hw_params_set_channels(m_pcm, m_params, info.channels) >= 0);

	GRC_CHECK_RETURN(snd_pcm_hw_params(m_pcm, m_params) >= 0);

	GRC_CHECK_RETURN(snd_pcm_hw_params_get_period_size(m_params, &frames, NULL)>=0);

	size_t dataSize = frames * info.channels * sizeof(short);

	short* data = reinterpret_cast<short*>(malloc(dataSize));
	sf_count_t read = sf_readf_short(file, data, frames);
	while (read > 0)
	{
		m_datas.push_back(new DATA(data, read));

		data = reinterpret_cast<short*>(malloc(dataSize));
		read = sf_readf_short(file, data, frames);
	}
	free(data);
	sf_close(file);

}

GRCWave::~GRCWave()
{
	// TODO Auto-generated destructor stub
	if (m_params) snd_pcm_hw_params_free(m_params);
	if (m_pcm) snd_pcm_close(m_pcm);

	for (auto* d : m_datas)
	{
		delete d;
	}
	m_datas.clear();
}

void GRCWave::play()
{
	m_playing = true;
	GRC_CHECK_RETURN(snd_pcm_hw_params(m_pcm, m_params) >= 0);

	snd_pcm_sframes_t written = 0;

	do
	{
		for (auto* d : m_datas)
		{
			if (m_playing == false)
				return;

			written = snd_pcm_writei(m_pcm, d->buffer, d->frames);
			if (written == -EPIPE)
			{
				GRC_ERR("Underrun");
				snd_pcm_prepare (m_pcm);
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

			usleep(1000);
		}
	}
	while (m_repeat);
}

void GRCWave::stop()
{
	m_playing = false;
}
