#pragma once
#include"Sound.h"
class SoundManager  // Singleton
{
private:

	static SoundManager* sInstance;

	Sound* SoundMgr;

public:

	static SoundManager* Instance();
	static void Release();

	void PlayMusic(std::string filename, int loop = -1);
	void HaltMusic();
	void ResumeMusic();
	void VolumeMusic(int volume = 64);


	void PlaySFX(std::string filename, int loop = 0, int channel = 0);
	void VolumeSFX(int channel, int volume = 64);
	void HaltSFX(int channel);

private:


	SoundManager();
	~SoundManager();
};

