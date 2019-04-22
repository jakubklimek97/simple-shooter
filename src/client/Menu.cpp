#include "Menu.h"

Menu::Menu()
{
	TimeData = Timer::Instance();
	InputData = InputManager::Instance();
	//Top Bar
	mTopBar = new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH*0.5f, 80.0f));
	mPlayerOne = new Texture("GHOST", "andromeda.ttf", 32, { 200,0,1 });
	mHiScore = new Texture("SHOOTER", "andromeda.ttf", 32, { 200,0,1});
	TopScore = new Score();
	PlayerOneScore = new Score();

	

	TopScore->Parent(mTopBar);
	PlayerOneScore->Parent(mTopBar);
	mPlayerOne->Parent(mTopBar);
	mHiScore->Parent(mTopBar);


	mPlayerOne->Pos(Vector2(-Graphics::Instance()->SCREEN_WIDTH*0.35f, 0.0f));
	mHiScore->Pos(Vector2(-30.0f, 0.0f));
	mTopBar->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH*0.7f, Graphics::Instance()->SCREEN_HEIGHT*0.1f));


	PlayerOneScore->Pos(Vector2(-Graphics::Instance()->SCREEN_WIDTH*0.35f, 38.0f));
	TopScore->Pos(Vector2(-30.0f, 38.0f));

	TopScore->Value(122);

	mTopBar->Parent(this);
	//Logo


	Title = new Texture("ghost_logo_static.png", 0, 0, 200, 220);
	AnimatedTitle = new AnimatedTexture("Ghost.png", 0, 0,	324, 223, 4, 0.6f, AnimatedTexture::vertical);
	  

	Title->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH*0.5f, Graphics::Instance()->SCREEN_HEIGHT*0.3f));
	AnimatedTitle->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH*0.5f, Graphics::Instance()->SCREEN_HEIGHT*0.3f));
	Title->Parent(this);


	

	//Play Modes

	GameMode = new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH*0.5f, Graphics::Instance()->SCREEN_HEIGHT*0.55f));
	CreativeMode = new Texture("Creative mode", "Android Insomnia.ttf", 32, { 230,230,230 });
	MultiplayerMode = new Texture("Multiplayer mode", "Android Insomnia.ttf", 32, { 230,230,230 });
	Cursor = new Texture("cursor.png", 0, 0, 50, 25);

	CreativeMode->Parent(GameMode);
	MultiplayerMode->Parent(GameMode);
	Cursor->Parent(GameMode);

	CreativeMode->Pos(Vector2(0.0f, -35.0f));
	MultiplayerMode->Pos(Vector2(10.0f, 35.0f));
	Cursor->Pos(Vector2(-165.0f, -35.0f));

	GameMode->Parent(this);

	CursorStartPosition = Cursor->Pos(local);
	CursorOffset = Vector2(0.0f, 70.0f);
	CurrentMode = 0;
	//Bottom Bar

	BottomBar = new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH*0.5f, Graphics::Instance()->SCREEN_HEIGHT*0.7f));
	Copyright = new Texture("Kuba&Michal", "trench100.ttf", 36, { 200,0,0 });
	Date = new Texture("2019- 2020 Kuba&Michal LTD.", "trench100.ttf", 32, { 230,230,230 });


	Copyright->Parent(BottomBar);
	Date->Parent(BottomBar);
	


	BottomBar->Pos(VEC2_ZERO);
    Copyright->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH*0.51f, Graphics::Instance()->SCREEN_HEIGHT*0.8f));
	Date->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH*0.51f, Graphics::Instance()->SCREEN_HEIGHT*0.75f));

	BottomBar->Parent(this);



	// Screen Animation variables
	AnimationStartPosition = Vector2(0.0f, Graphics::Instance()->SCREEN_HEIGHT);
	AnimationEndPosition = VEC2_ZERO;
	AnimationTotalTime = 5.0f;
	AnimationTime = 0.0f;
	AnimationEnd = false;

	Pos(AnimationStartPosition);


}
Menu::~Menu()
{
	//Dealocationg Top Bar
	delete mTopBar;
	mTopBar = NULL;
	delete mPlayerOne;
	mPlayerOne = NULL;
    delete mHiScore;
	mHiScore = NULL;
	delete PlayerOneScore;
	PlayerOneScore = NULL;
	delete TopScore;
	TopScore = NULL;
	//Freeing logo

	delete Title;
	Title = NULL;
	delete AnimatedTitle;
	AnimatedTitle = NULL;

	//Dealocating Play Mode

	delete GameMode;
	GameMode = NULL;
	delete CreativeMode;
	CreativeMode = NULL;
	delete MultiplayerMode;
	MultiplayerMode = NULL;
	delete Cursor;
	Cursor = NULL;


	//Dealocating Bottom Bar

	delete BottomBar;
	BottomBar = NULL;
	delete  Date;
	Date = NULL;
	delete Copyright;
	Copyright = NULL;


}

void Menu::ChangeCurrentMode(int change)
{
	CurrentMode += change;

	if (CurrentMode < 0)
		CurrentMode = 1;
	else if (CurrentMode > 1)
		CurrentMode = 0;

	Cursor->Pos(CursorStartPosition + CursorOffset * CurrentMode);
}

void Menu::Update()
{
	if (!AnimationEnd) {

		AnimationTime += TimeData->DeltaTime();
		Pos(Lerp(AnimationStartPosition, AnimationEndPosition, AnimationTime / AnimationTotalTime));

		if (AnimationTime >= AnimationTotalTime)
			AnimationEnd = true;

		if (InputData->KeyPressed(SDL_SCANCODE_DOWN) || InputData->KeyPressed(SDL_SCANCODE_UP))
			AnimationTime = AnimationTotalTime;
	}
	else {
		AnimatedTitle->Update();

		if (InputData->KeyPressed(SDL_SCANCODE_DOWN))
			ChangeCurrentMode(1);
		else if(InputData->KeyPressed(SDL_SCANCODE_UP))
			ChangeCurrentMode(-1);
	}
}

void Menu::Render()
{
	mPlayerOne->Render();
    mHiScore->Render();
	PlayerOneScore->Render();
	TopScore->Render();


	if (!AnimationEnd)
		Title->Render();

	else
		AnimatedTitle->Render();

	CreativeMode->Render();
	MultiplayerMode->Render();
	Cursor->Render();

	Date->Render();
	Copyright->Render();


}
