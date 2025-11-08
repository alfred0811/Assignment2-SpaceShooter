#pragma once

#include "Enum.h"

class GameState
{
public:
	GameState(State state)
		:mState(state)
	{

	}
	virtual ~GameState() {}

	State GetState() const { return mState; }

	virtual void Load() = 0;
	virtual State Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Unload() = 0;

	private:
		State mState;
};