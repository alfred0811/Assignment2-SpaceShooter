#pragma once

#include "Entity.h"
#include <vector>

class Ship;
class BulletPool;
class Collidable;
class Enemy;
class ProgressBar;
class EnemySpawner;
class BossSpawner;
class PowerUp;
class Score;

class Game : public Entity
{
public:
	Game();
	~Game() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void AddCollidable(Collidable* collidable);
	bool IsGameOver();

	Score* GetScore() const;

private:
	Ship* mPlayer;
	BulletPool* mBulletPool;
	ProgressBar* mHealthBar;
	std::vector<Collidable*> mCollidables;

	// Spawners
	EnemySpawner* mEnemySpawner;
	BossSpawner* mBossSpawner;

	// Background
	X::TextureId mBackgroundId;

	// PowerUps
	PowerUp* mPowerUp;
	float mPowerUpSpawnTimer;
	const float mPowerUpSpawnInterval;

	// Score
	Score* mScore;
};