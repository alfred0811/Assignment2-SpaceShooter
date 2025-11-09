#pragma once

#include "Entity.h"

class Boss;
class BulletPool;
class Ship;
class Game;

class BossSpawner : public Entity
{
public:
    BossSpawner();
    ~BossSpawner() override;

    void Load() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Unload() override;

    void SetBulletPool(BulletPool* bulletPool);
    void SetShip(Ship* ship);
    void SetGame(Game* game);

    Boss* GetBoss() const;

private:
    void SpawnBoss();

    Boss* mBoss;
    BulletPool* mBulletPool;
    Ship* mShip;
    Game* mGame;

    float mSpawnTimer;
    const float mInitialSpawnDelay;
    const float mRespawnDelay;
    bool mIsWaitingToSpawn;
};