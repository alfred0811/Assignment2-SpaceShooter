#pragma once

#include "Entity.h"

class Score : public Entity
{
public:
	Score();
	~Score() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void AddPoints(int points);
	void Reset();
	int GetScore() const;

private:
	int mScore;
	X::Math::Vector2 mPosition;
};