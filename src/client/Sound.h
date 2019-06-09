#pragma once
#include<SDL_mixer.h>
#include <SDL.h>
#include<iostream>
#include<map>

//Singleton
class Sound
{
private:

	static Sound* sInstance;


	
	std::map<std::string, Mix_Music*> Music;
	std::map<std::string, Mix_Chunk*> SFX;

	Sound();
	~Sound();


public:

	static Sound* Instance();
	static void Release();
	
	Mix_Music* GetMusic(std::string filename);
	Mix_Chunk* GetSFX(std::string fielname);



	
};

