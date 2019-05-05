#include "Sound.h"



Sound::Sound()
{
	ISoundEngine *SoundEng = createIrrKlangDevice();
	SoundEng->play2D("res/audio/test.mp3", GL_TRUE);

	if (!SoundEng) {
		std::cout << "SOUND ERROR" << std::endl;
		return;
	}
	SoundEng->drop();

}


Sound::~Sound()
{
}
