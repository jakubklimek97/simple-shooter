#include "Sound.h"



Sound::Sound()
{
	ISoundEngine *SoundEng = createIrrKlangDevice();
	SoundEng->Play2D("res/audio/test.mp3", GL_TRUE);
}


Sound::~Sound()
{
}
