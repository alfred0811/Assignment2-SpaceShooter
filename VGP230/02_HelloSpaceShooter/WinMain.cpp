#include <XEngine.h>
#include "StartState.h"
#include "RunGameState.h"
#include "EndState.h"

State gActiveState = State::Start;
GameState* gCurrentGameState = nullptr;
std::map<State, GameState*> gGameStates;
// Game setup
void GameInit()
{
	gGameStates[State::Start] = new StartState();
	gGameStates[State::RunGame] = new RunGameState();
	gGameStates[State::End] = new EndState();
	gActiveState = State::Start;

	gCurrentGameState = gGameStates[gActiveState];
	gCurrentGameState->Load();
}

void GameRender()
{

}

// Per-frame logic
bool GameLoop(float deltaTime)
{
	State newState = gGameStates[gActiveState]->Update(deltaTime);
	gCurrentGameState->Render();
	if (newState != gActiveState)
	{
		
		gCurrentGameState->Unload();
		gCurrentGameState = gGameStates[newState];
		gCurrentGameState->Load();
		gActiveState = newState;
	}
	// Exit if Escape is pressed
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

void GameCleanup()
{
	gCurrentGameState->Unload();
	for (auto& state : gGameStates)
	{
		delete state.second;
	}
	gGameStates.clear();
}

// Main function
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();

	return 0;
}