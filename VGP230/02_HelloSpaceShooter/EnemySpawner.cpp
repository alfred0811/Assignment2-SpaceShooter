#include "EnemySpawner.h"
#include "Enemy.h"
#include "BulletPool.h"
#include "Ship.h"
#include "Game.h"
#include <XEngine.h>

EnemySpawner::EnemySpawner()
	: Entity()
	, mBulletPool(nullptr)
	, mShip(nullptr)
	, mGame(nullptr)
	, mSpawnTimer(0.0f)
	, mSpawnInterval(4.0f)
	, mMaxEnemies(5)
	, mActiveEnemyCount(0)
{
}

EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::Load()
{
	// Pre-create enemies
	for (int i = 0; i < mMaxEnemies; ++i)
	{
		Enemy* enemy = new Enemy();
		enemy->Load();
		enemy->SetBulletPool(mBulletPool);
		enemy->SetShip(mShip);
		enemy->SetGame(mGame);
		mEnemies.push_back(enemy);
	}
}

void EnemySpawner::Update(float deltaTime)
{
	mSpawnTimer -= deltaTime;

	// Count active enemies
	mActiveEnemyCount = 0;
	for (Enemy* enemy : mEnemies)
	{
		if (enemy->IsAlive())
		{
			enemy->Update(deltaTime);
			mActiveEnemyCount++;
		}
	}

	// Spawn new enemy if timer is under max limit
	if (mSpawnTimer <= 0.0f && mActiveEnemyCount < mMaxEnemies)
	{
		SpawnEnemy();
		mSpawnInterval = X::RandomFloat(2.0f, 5.0f);
		mSpawnTimer = mSpawnInterval;
	}
}

void EnemySpawner::Render()
{
	for (Enemy* enemy : mEnemies)
	{
		if (enemy->IsAlive())
		{
			enemy->Render();
		}
	}
	
}

void EnemySpawner::Unload()
{
	for (auto* enemy : mEnemies)
	{
		enemy->Unload();
		delete enemy;
	}
	mEnemies.clear();
}

void EnemySpawner::SetGame(Game* game)
{
	mGame = game;
}

void EnemySpawner::SetBulletPool(BulletPool* bulletPool)
{
	mBulletPool = bulletPool;
}

void EnemySpawner::SetShip(Ship* ship)
{
	mShip = ship;
}

void EnemySpawner::SetMaxEnemies(int maxEnemies)
{
	mMaxEnemies = maxEnemies;
}

void EnemySpawner::SpawnEnemy()
{
	for (Enemy* enemy : mEnemies)
	{
		if (!enemy->IsAlive())
		{
			X::Math::Vector2 spawnPos = GetRandomSpawnPosition();
			float spawnRotation = X::RandomFloat(0.0f, X::Math::kTwoPi);

			enemy->SetPosition(spawnPos);
			enemy->SetRotation(spawnRotation);
			enemy->Reset();
			
			break;
		}
	}
}

X::Math::Vector2 EnemySpawner::GetRandomSpawnPosition()
{
	// Get screen dimensions
	const float screenWidth = static_cast<float>(X::GetScreenWidth());
	const float screenHeight = static_cast<float>(X::GetScreenHeight());

	// Spawn at random edge of screen
	const float margin = 50.0f;
	X::Math::Vector2 position;
	position.x = X::RandomFloat(margin, screenWidth - margin);
	position.y = X::RandomFloat(margin, screenHeight - margin);

	return position;
}