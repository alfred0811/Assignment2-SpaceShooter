#include "Score.h"
#include <XEngine.h>

Score::Score()
	: Entity()
	, mScore(0)
	, mPosition(0.0f, 0.0f)
{
}

Score::~Score()
{
}

void Score::Load()
{
	mScore = 0;
	// Position in top-right corner with some padding
	mPosition.x = static_cast<float>(X::GetScreenWidth()) - 150.0f;
	mPosition.y = 20.0f;
}

void Score::Update(float deltaTime)
{
}

void Score::Render()
{
	// Create score text
	char scoreText[32];
	sprintf_s(scoreText, "Score: %d", mScore);

	// Draw the score text
	X::DrawScreenText(scoreText, mPosition.x, mPosition.y, 20.0f, X::Colors::White);
}

void Score::Unload()
{
	mScore = 0;
}

void Score::AddPoints(int points)
{
	mScore += points;
}

void Score::Reset()
{
	mScore = 0;
}

int Score::GetScore() const
{
	return mScore;
}