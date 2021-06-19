#pragma once

using namespace FMOD;

#define EXTRCHANNERBUFFER 5
#define SOUNDBUFFER 20
#define TOTALSOUNDBUFFER SOUNDBUFFER * EXTRCHANNERBUFFER

class SoundManager : public SingletonBase<SoundManager>
{
private:
	typedef map<string, Sound**> arrSound;
	typedef map<string, Sound**>::iterator arrSoundIter;
	Sound** m_sound;
	Channel** m_channel;
	System* m_system;
	arrSound m_totalSound;
public:
	SoundManager();
	~SoundManager();
	void addSound(string keyName, string soundName, bool loop = false);
	void play(string keyName, float volume = 1.0f);
	void stop(string keyName);
	void pause(string keyName);
	void resume(string keyName, float volume = 1.0f);
	bool isPlaySound(string keyName);
	bool isPauseSound(string keyName);
	void Update();
};