/*
 * GRCWave.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef GRCWAVE_H_
#define GRCWAVE_H_

#include <alsa/asoundlib.h>
#include <vector>

#include "../core/GRCObject.h"
#include "../core/GRCString.h"

class GRCWave: public GRCObject
{
public:
	struct DATA
	{
		const void* buffer;
		snd_pcm_uframes_t frames;

		DATA(const void* _buffer, snd_pcm_uframes_t _frames)
				: 	buffer(_buffer),
					frames(_frames)
		{
		}
	};
	typedef std::vector<DATA*> VecData;

public:
	GRCWave(GRCCSTR dir, GRCCSTR filename, bool repeat, int priority);
	virtual ~GRCWave();

public:
	bool isValid() const
	{
		return m_pcm && m_datas.empty() == false;
	}

	GRCCSTR getFilename() const
	{
		return m_filename;
	}

	int getPriority() const
	{
		return m_priority;
	}

	void play();
	void stop();

private:
	GRCString m_filename;
	bool m_repeat;
	int m_priority;
	snd_pcm_t* m_pcm;
	snd_pcm_hw_params_t* m_params;
	VecData m_datas;
	bool m_playing;
};

#endif /* GRCWAVE_H_ */
