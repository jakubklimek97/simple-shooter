#include "Score.h"



void Score::ClearBoard()
{
	for (int i = 0; i < mScore.size(); i++) {
		delete mScore[i];
		mScore[i] = NULL;
	}
	mScore.clear();
}

 Score::Score()
{
	Value(0);
}


Score::~Score()
{
	ClearBoard();

}

void Score::Value(int score)
{
	ClearBoard();
	if (score == 0) {

		for (int i = 0; i < 2; i++) {
			mScore.push_back(new Texture("0", "andromeda.ttf", 32, { 230,230,230 }));
			mScore[i]->Parent(this);
			mScore[i]->Pos(Vector2(-32.0f*i, 0.0f));
		}
		
	}
	else {
		std::string str = std::to_string(score);
		int lastindex = str.length() - 1;

		for (int i = 0; i <= lastindex; i++) {
			mScore.push_back(new Texture(str.substr(i, 1), "andromeda.ttf", 32, { 230,230,230 }));
			mScore[i]->Parent(this);
			mScore[i]->Pos(Vector2(-32.0f*(lastindex - i), 0.0f));
		}
	}
}

void Score::Render()
{
	for (int i = 0; i < mScore.size() ;i++)
		mScore[i]->Render();
}
