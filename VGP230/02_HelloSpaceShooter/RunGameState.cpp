#include "RunGameState.h"
#include <XEngine.h>
#include "Game.h"
#include "Score.h"

RunGameState::RunGameState()
	: GameState(State::RunGame)
	, mGame(nullptr)
{

}
RunGameState::~RunGameState()
{

}

void RunGameState::Load()
{
	mGame = new Game();
	mGame->Load();
}
State RunGameState::Update(float deltaTime)
{
	if (X::IsKeyPressed(X::Keys::ENTER))
	{
		return State::Start;
	}
	mGame->Update(deltaTime);
	if (mGame->IsGameOver())
	{
		return State::End;
	}
	return State::RunGame;
}
void RunGameState::Render()
{
	mGame->Render();
}
void RunGameState::Unload()
{
	mGame->Unload();
	delete mGame;
	mGame = nullptr;
}
int RunGameState::GetFinalScore() const
{
	if (mGame == nullptr)
		return 0;

	Score* score = mGame->GetScore();
	if (score != nullptr)
		return score->GetScore();

	return 0;
}