#include "BossSpawner.h"
#include "Boss.h"
#include "BulletPool.h"
#include "Ship.h"
#include "Game.h"

BossSpawner::BossSpawner()
	: Entity()
    , mBoss(nullptr)
    , mBulletPool(nullptr)
    , mShip(nullptr)
    , mGame(nullptr)
    , mSpawnTimer(0.0f)
    , mInitialSpawnDelay(25.0f)
    , mRespawnDelay(35.0f)
    , mIsWaitingToSpawn(true)
{
    mSpawnTimer = mInitialSpawnDelay;
}

BossSpawner::~BossSpawner()
{
}

void BossSpawner::Load()
{
    mBoss = new Boss();
    mBoss->Load();
    mBoss->SetBulletPool(mBulletPool);
    mBoss->SetShip(mShip);
    mBoss->SetGame(mGame);
    mBoss->Deactivate();
}

void BossSpawner::Update(float deltaTime)
{
    if (mBoss == nullptr)
        return;

    bool wasAlive = mBoss->IsAlive();

    mBoss->Update(deltaTime);

    bool isAlive = mBoss->IsAlive();

    if (wasAlive && !isAlive)
    {
        mSpawnTimer = mRespawnDelay;
        mIsWaitingToSpawn = true;
    }

    if (mIsWaitingToSpawn)
    {
        mSpawnTimer -= deltaTime;
        if (mSpawnTimer <= 0.0f)
        {
            SpawnBoss();
        }
    }
}

void BossSpawner::Render()
{
    if (mBoss)
    {
        mBoss->Render();
    }
}

void BossSpawner::Unload()
{
    if (mBoss)
    {
        mBoss->Unload();
        delete mBoss;
        mBoss = nullptr;
    }
}

void BossSpawner::SetBulletPool(BulletPool* bulletPool)
{
    mBulletPool = bulletPool;
    if (mBoss)
    {
        mBoss->SetBulletPool(bulletPool);
    }
}

void BossSpawner::SetShip(Ship* ship)
{
    mShip = ship;
    if (mBoss)
    {
        mBoss->SetShip(mShip);
    }
}

void BossSpawner::SetGame(Game* game)
{
    mGame = game;
    if (mBoss)
    {
        mBoss->SetGame(mGame);
    }
}

Boss* BossSpawner::GetBoss() const
{
    return mBoss;
}

void BossSpawner::SpawnBoss()
{
    if (mBoss == nullptr)
        return;

    mBoss->SetBulletPool(mBulletPool);
    mBoss->SetShip(mShip);
    mBoss->SetGame(mGame);
    mBoss->Activate();

    mIsWaitingToSpawn = false;
}