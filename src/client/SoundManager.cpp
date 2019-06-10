#include "SoundManager.h"

SoundManager* SoundManager::sInstance = NULL;

SoundManager* SoundManager::Instance() {
	if (sInstance == NULL)
		sInstance = new SoundManager();

	return sInstance;
}


void SoundManager::Release() {
	delete sInstance;
	sInstance = NULL;
}

SoundManager::SoundManager()
{
	SoundMgr = Sound::Instance();

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		std::cout << "ERROR INITIALIZING MIXER:" << Mix_GetError();
	}
}


SoundManager::~SoundManager()
{
	SoundMgr = NULL;
}

void SoundManager::PlayMusic(std::string filename, int loop) {

	Mix_PlayMusic(SoundMgr->GetMusic(filename), loop);
}

void SoundManager::ResumeMusic() {
	if (Mix_PausedMusic() != 0)
		Mix_ResumeMusic();
}

void SoundManager::VolumeMusic(int volume)
{
	Mix_VolumeMusic(volume);
}

void SoundManager::HaltMusic() {
	
	if (Mix_PlayingMusic() != 0)
		Mix_PauseMusic();
}

void SoundManager::PlaySFX(std::string filename, int loop,int channel) {
	Mix_PlayChannel(channel, SoundMgr->GetSFX(filename), loop);
}

void SoundManager::VolumeSFX(int channel, int volume)
{
	Mix_Volume( channel,  volume);
}

void SoundManager::HaltSFX(int channel)
{
	Mix_HaltChannel(channel);
}
