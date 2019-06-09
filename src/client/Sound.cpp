#include "Sound.h"

Sound* Sound::sInstance = NULL;

Sound::Sound() {

}

Sound::~Sound()
{
	
}

Sound * Sound::Instance()
{
	if (sInstance == NULL)
		sInstance = new Sound();

	return sInstance;
}

void Sound::Release()
{
	delete sInstance;
	sInstance = NULL;
}

Mix_Music * Sound::GetMusic(std::string filename)
{
	std::string Path = "res/img/" + filename;

	if (Music[Path] == nullptr) {
		
			Music[Path] = Mix_LoadMUS(Path.c_str());
		
		
			if (Music[Path] == NULL)
				std::cout << "ERROR LOADING MUSIC: FILE:" << filename.c_str() << " ERROR TYPE: " << Mix_GetError();
	 }
	return Music[Path];

}


Mix_Chunk * Sound::GetSFX(std::string filename)
{
	std::string Path = "res/img/"+ filename;
	//Path.append("res/img/" + filename);

	if (SFX[Path] == nullptr) {
		SFX[Path] = Mix_LoadWAV(Path.c_str());

		if (SFX[Path] == NULL)
			std::cout << "ERROR LOADING SFX: FILE:" << filename.c_str() << " ERROR TYPE: " << Mix_GetError();

	}
	return SFX[Path];
}
