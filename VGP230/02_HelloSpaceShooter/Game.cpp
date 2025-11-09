#include "Game.h"
#include "Ship.h"
#include "BulletPool.h"
#include "Bullet.h"
#include "Enemy.h"
#include "ProgressBar.h"
#include "EnemySpawner.h"
#include "BossSpawner.h"
#include "Score.h"

Game::Game()
	: Entity()
	, mPlayer(nullptr)
	, mBulletPool(nullptr)
	, mHealthBar(nullptr)
	, mEnemySpawner(nullptr)
	, mBossSpawner(nullptr)
	, mBackgroundId(0)
	, mPowerUp(nullptr)
	, mPowerUpSpawnTimer(15.0f)
	, mPowerUpSpawnInterval(15.0f)
	, mScore(nullptr)
{

}
Game::~Game()
{
	XASSERT(mPlayer == nullptr, "Game: Unload must be called");
}

void Game::Load()
{
	mBackgroundId = X::LoadTexture("space.jpg");

	mPlayer = new Ship();
	mBulletPool = new BulletPool();
	mHealthBar = new ProgressBar();
	mHealthBar->Load();

	mPlayer->Load();
	mPlayer->SetBullePool(mBulletPool);
	AddCollidable(mPlayer);

	mEnemySpawner = new EnemySpawner();
	mEnemySpawner->SetBulletPool(mBulletPool);
	mEnemySpawner->SetShip(mPlayer);
	mEnemySpawner->SetGame(this);
	mEnemySpawner->SetMaxEnemies(5);  // Set max enemies to spawn
	mEnemySpawner->Load();

	// Add spawned enemies to collidables
	const std::vector<Enemy*>& enemies = mEnemySpawner->GetEnemies();
	for (Enemy* enemy : enemies)
	{
		AddCollidable(enemy);
	}

	mBossSpawner = new BossSpawner();
	mBossSpawner->SetBulletPool(mBulletPool);
	mBossSpawner->SetShip(mPlayer);
	mBossSpawner->SetGame(this);
	mBossSpawner->Load();
	if (Boss* boss = mBossSpawner->GetBoss())
	{
		AddCollidable(boss);
	}

	// Setup bullet pool
	mBulletPool->Load();
	std::vector<Bullet*>& bullets = mBulletPool->GetBullets();
	for (Bullet* bullet : bullets)
	{
		AddCollidable(bullet);
	}

	// Setup PowerUp
	mPowerUp = new PowerUp();
	mPowerUp->Load();
	AddCollidable(mPowerUp);

	// Setup Score
	mScore = new Score();
	mScore->Load();
}
void Game::Update(float deltaTime)
{
	mPlayer->Update(deltaTime);

	mEnemySpawner->Update(deltaTime);

	if (mBossSpawner)
	{
		mBossSpawner->Update(deltaTime);
	}

	mBulletPool->Update(deltaTime);

	// PowerUp spawn logic
	mPowerUpSpawnTimer -= deltaTime;
	if (mPowerUpSpawnTimer <= 0.0f && !mPowerUp->IsActive())
	{
		// Pick a random number (0, 1, or 2) to decide the PowerUp type
		int randomType = rand() % 3; // rand() gives a random number
		PowerUpType type = static_cast<PowerUpType>(randomType);

		// Pick a random position inside the screen
		float randomX = 50.0f + static_cast<float>(rand() % (X::GetScreenWidth() - 100));
		float randomY = 50.0f + static_cast<float>(rand() % (X::GetScreenHeight() - 100));

		// Spawn it on screen
		mPowerUp->Spawn(type, { randomX, randomY });
		mPowerUp->SetCollisionFilter(ET_SHIP);

		// Reset timer for next spawn
		mPowerUpSpawnTimer = mPowerUpSpawnInterval;
	}

	mPowerUp->Update(deltaTime);

	// Collision detection
	int numCollidables = mCollidables.size();
	for (int i = 0; i < numCollidables - 1; ++i)
	{
		for (int n = i + 1; n < numCollidables; ++n)
		{
			if (mCollidables[i]->DidCollide(mCollidables[n]))
			{
				mCollidables[i]->OnCollision(mCollidables[n]);
				mCollidables[n]->OnCollision(mCollidables[i]);
			}
		}
	}

	mHealthBar->SetBarValue(mPlayer->GetHealth(), mPlayer->GetMaxHealth());
}
void Game::Render()
{
	// Render background scaled to fit the screen
	float screenWidth = static_cast<float>(X::GetScreenWidth());
	float screenHeight = static_cast<float>(X::GetScreenHeight());

	// Get the background texture dimensions
	float bgWidth = static_cast<float>(X::GetSpriteWidth(mBackgroundId));
	float bgHeight = static_cast<float>(X::GetSpriteHeight(mBackgroundId));

	// Calculate scale to fit screen
	float scaleX = screenWidth / bgWidth;
	float scaleY = screenHeight / bgHeight;

	X::Math::Rect sourceRect = { 0.0f, 0.0f, bgWidth, bgHeight };
	X::DrawSprite(mBackgroundId, sourceRect, { screenWidth / 2.0f, screenHeight / 2.0f });

	mPlayer->Render();
	mEnemySpawner->Render();
	if (mBossSpawner)
	{
		mBossSpawner->Render();
	}
	mBulletPool->Render();
	mPowerUp->Render();
	mHealthBar->Render();
	mScore->Render();
}
void Game::Unload()
{
	mBulletPool->Unload();
	delete mBulletPool;
	mBulletPool = nullptr;

	mHealthBar->Unload();
	delete mHealthBar;
	mHealthBar = nullptr;

	mPlayer->Unload();
	delete mPlayer;
	mPlayer = nullptr;

	mEnemySpawner->Unload();
	delete mEnemySpawner;
	mEnemySpawner = nullptr;

	if (mBossSpawner)
	{
		mBossSpawner->Unload();
		delete mBossSpawner;
		mBossSpawner = nullptr;
	}

	mPowerUp->Unload();
	delete mPowerUp;
	mPowerUp = nullptr;

	mScore->Unload();
	delete mScore;
	mScore = nullptr;
}

void Game::AddCollidable(Collidable* collidable)
{
	mCollidables.push_back(collidable);
}

bool Game::IsGameOver()
{ 
	return !mPlayer->IsAlive();
}

Score* Game::GetScore() const
{
	return mScore;
}