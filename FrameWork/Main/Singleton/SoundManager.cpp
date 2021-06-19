#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
	:m_system(nullptr), m_channel(nullptr), m_sound(nullptr)
{
	System_Create(&m_system);
	m_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);
	m_sound = new Sound *[TOTALSOUNDBUFFER];
	m_channel = new Channel *[TOTALSOUNDBUFFER];
	memset(m_sound, 0, sizeof(Sound*)*(TOTALSOUNDBUFFER));
	memset(m_channel, 0, sizeof(Channel*)*(TOTALSOUNDBUFFER));
}

SoundManager::~SoundManager()
{
	if (m_channel != nullptr || m_sound != nullptr)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (m_channel != nullptr)
			{
				if (m_channel[i]) m_channel[i]->stop();
			}

			if (m_sound != nullptr)
			{
				if (m_sound[i]) m_sound[i]->release();
			}
		}
	}

	SAFE_DELETE(m_channel);
	SAFE_DELETE(m_sound);

	if (m_system != nullptr)
	{
		m_system->release();
		m_system->close();
	}

	m_totalSound.clear();
}

void SoundManager::addSound(string keyName, string soundName, bool loop)
{
	string add = "../../Asset/Sound/";
	soundName = add + soundName;
	if (loop)
	{
		m_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_sound[m_totalSound.size()]);
	}
	else
	{
		m_system->createStream(soundName.c_str(), FMOD_DEFAULT, nullptr, &m_sound[m_totalSound.size()]);
	}

	m_totalSound.insert(make_pair(keyName, &m_sound[m_totalSound.size()]));
}

void SoundManager::play(string keyName, float volume)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->isPlaying(&isPlay);
			if (isPlay) return;
			m_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &m_channel[count]);
			m_channel[count]->setVolume(volume);
		}
	}
}

void SoundManager::stop(string keyName)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->isPlaying(&isPlay);
			if (!isPlay) return;
			m_channel[count]->stop();
			break;
		}
	}
}

void SoundManager::pause(string keyName)
{
	bool isPause;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->getPaused(&isPause);
			if (isPause) return;
			m_channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::resume(string keyName, float volume)
{
	bool isPause;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->getPaused(&isPause);
			if (!isPause)return;
			m_channel[count]->setPaused(false);
			m_channel[count]->setVolume(volume);
			break;
		}
	}
}

bool SoundManager::isPlaySound(string keyName)
{
	bool isPlay = false;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->isPlaying(&isPlay);
			break;
		}
	}
	return isPlay;
}

bool SoundManager::isPauseSound(string keyName)
{
	bool isPasued = false;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->getPaused(&isPasued);
			break;
		}
	}
	return isPasued;
}

void SoundManager::Update()
{
	m_system->update();
}