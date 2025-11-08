#pragma once

#include "Entity.h"
#include "Collidable.h"

class BulletPool;
class Ship;
class AnimSpriteSheet;
class Game;

class Enemy : public Entity, public Collidable
{
	public:
		Enemy();
		~Enemy() override;

		// Entity Overrides
		void Load() override;
		void Update(float deltaTime) override;
		void Render() override;
		void Unload() override;

		// Collidable Overrides
		int GetType() const override;
		const X::Math::Vector2& GetPosition() const override;
		void OnCollision(Collidable* collidable) override;

		// Enemy Functions
		void SetBulletPool(BulletPool* bulletPool);
		void SetShip(Ship* ship);
		void SetGame(Game* game);
		void SetPosition(const X::Math::Vector2& position);
		void SetRotation(float rotation);

		bool IsAlive() const;

		void Reset();

private:
	BulletPool* mBulletPool;
	Ship* mShip;
	Game* mGame;
	AnimSpriteSheet* mExplosion;

	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	float mRotation;

	int mHealth;

	X::Math::Vector2 mCenterPoint;
	X::Math::Vector2 mTargetPosition;
	float mTargetPositionUpdate;
	float mFireRate;
};