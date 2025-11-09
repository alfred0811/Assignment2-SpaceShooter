#pragma once

#include "GameState.h"

class Game;

class RunGameState : public GameState
{
public:
	RunGameState();
	~RunGameState() override;

	void Load() override;
	State Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	int GetFinalScore() const;

private:
	 Game* mGame;
};