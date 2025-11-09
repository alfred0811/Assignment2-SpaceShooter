#pragma once

#include "GameState.h"

class EndState : public GameState
{
public:
	EndState();
	~EndState() override;

	void Load() override;
	State Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void SetFinalScore(int score);

private:
	int mFinalScore;
};