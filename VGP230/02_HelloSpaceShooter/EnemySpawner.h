#pragma once

#include "Entity.h"

#include <vector>

class Enemy;
class BulletPool;
class Ship;
class Game;

class EnemySpawner : public Entity
{
public:
	EnemySpawner();
	~EnemySpawner() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void SetBulletPool(BulletPool* bulletPool);
	void SetShip(Ship* ship);
	void SetGame(Game* game);
	void SetMaxEnemies(int maxEnemies);

	const std::vector<Enemy*>& GetEnemies() const { return mEnemies; };

private:
	void SpawnEnemy();
	X::Math::Vector2 GetRandomSpawnPosition();

	std::vector<Enemy*> mEnemies;
	BulletPool* mBulletPool;
	Ship* mShip;
	Game* mGame;

	float mSpawnTimer;
	float mSpawnInterval;
	int mMaxEnemies;
	int mActiveEnemyCount;
};