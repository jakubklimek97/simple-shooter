#ifndef _MENU_H
#define _MENU_H
#include "AnimatedTexture.h"
#include "InputManager.h"
#include  "Score.h"

using namespace QuickSDL;

class Menu : public GameEntity {

private:
	//Top
	Timer* TimeData;
	InputManager* InputData;

	GameEntity* mTopBar;
	Texture* mHiScore;
	Texture* mPlayerOne;
	//Title
	Texture* Title;
	AnimatedTexture* AnimatedTitle;

	//Player Mode
	GameEntity*GameMode;
	Texture* CreativeMode;
	Texture* MultiplayerMode;
	Texture* Cursor;
	Vector2 CursorStartPosition;
	Vector2 CursorOffset;
	Score* PlayerOneScore;
	Score* TopScore;
	//Bottom Bar
	GameEntity *BottomBar;
	Texture* Copyright;
	Texture* Date;
	int CurrentMode;

	// Animation Variable
	float AnimationTotalTime;
	float AnimationTime;
	bool AnimationEnd;
	Vector2 AnimationStartPosition;
	Vector2 AnimationEndPosition;
	

public:


	Menu();
	~Menu();
	void ChangeCurrentMode(int CurrentMode);
	void Update();
	void Render();




};

#endif