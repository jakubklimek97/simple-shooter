#ifndef  _SCORE_H
#define _SCORE_H
#include"Texture.h"
#include <vector>

using namespace QuickSDL;

class Score: public GameEntity
{
private:

	std::vector<Texture*> mScore;
	void ClearBoard();


public:
	Score();
	~Score();

	void Value(int score);

	void Render();
};
#endif
