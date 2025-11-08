#include "StartState.h"
#include <XEngine.h>

StartState::StartState()
	: GameState(State::Start)
{

}
StartState::~StartState()
{

}

void StartState::Load()
{

}
State StartState::Update(float deltaTime)
{
	if (X::IsKeyPressed(X::Keys::ENTER))
	{
		return State::RunGame;
	}
	return State::Start;
}
void StartState::Render()
{
	const float textSize = 100.0f;
	const char* text = "Start Screen \n <Press Enter>";
	float textWidth = X::GetTextWidth(text, textSize);
	float screenX = (X::GetScreenWidth() - textWidth) * 0.5f;
	float screenY = X::GetScreenHeight() * 0.5f;
	X::DrawScreenText(text, screenX, screenY, textSize, X::Colors::Green);
}
void StartState::Unload()
{

}