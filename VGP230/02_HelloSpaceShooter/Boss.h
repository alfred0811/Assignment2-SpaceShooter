#pragma once

#include "Entity.h"
#include "Collidable.h"

class BulletPool;
class Ship;
class AnimSpriteSheet;
class Game;

class Boss : public Entity, public Collidable
{
public:
    Boss();
    ~Boss() override;

    void Load() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Unload() override;

    int GetType() const override;
    const X::Math::Vector2& GetPosition() const override;
    void OnCollision(Collidable* collidable) override;

    void SetBulletPool(BulletPool* bulletPool);
    void SetShip(Ship* ship);
    void SetGame(Game* game);

    void Activate();
    void Deactivate();
    bool IsAlive() const;

    void FireDoubleShot();

private:
    BulletPool* mBulletPool;
    Ship* mShip;
    Game* mGame;
    AnimSpriteSheet* mExplosion;

    X::TextureId mImageId;
    X::Math::Vector2 mPosition;
    float mRotation;

    int mHealth;
    int mMaxHealth;

    X::Math::Vector2 mCenterPoint;
    X::Math::Vector2 mTargetPosition;
    float mTargetPositionUpdate;
    float mFireRate;
};