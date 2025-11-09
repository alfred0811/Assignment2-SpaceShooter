#include "EndState.h"
#include <XEngine.h>

EndState::EndState()
	: GameState(State::End)
	, mFinalScore(0)
{

}
EndState::~EndState()
{

}

void EndState::Load()
{

}
State EndState::Update(float deltaTime)
{
	if (X::IsKeyPressed(X::Keys::ENTER))
	{
		return State::Start;
	}
	return State::End;
}
void EndState::Render()
{
    const float titleSize = 100.0f;
    const char* titleText = "GAME OVER";

    float screenWidth = static_cast<float>(X::GetScreenWidth());
    float screenHeight = static_cast<float>(X::GetScreenHeight());

    float titleWidth = X::GetTextWidth(titleText, titleSize);
    float titleX = (screenWidth - titleWidth) * 0.5f;
    float titleY = screenHeight * 0.4f;

    X::DrawScreenText(titleText, titleX, titleY, titleSize, X::Colors::Red);

    const float scoreSize = 60.0f;
    char scoreText[64];
    sprintf_s(scoreText, "Final Score: %d", mFinalScore);
    float scoreWidth = X::GetTextWidth(scoreText, scoreSize);
    float scoreX = (screenWidth - scoreWidth) * 0.5f;
    float scoreY = titleY + titleSize + 20.0f;

    X::DrawScreenText(scoreText, scoreX, scoreY, scoreSize, X::Colors::Red);
}
void EndState::Unload()
{
	mFinalScore = 0;
}

void EndState::SetFinalScore(int score)
{
	mFinalScore = score;
}