#include "Enemy.h"
#include "BulletPool.h"
#include "Bullet.h"
#include "Ship.h"
#include "AnimSpriteSheet.h"
#include "Game.h"
#include "Score.h"

Enemy::Enemy()
	: Entity()
	, Collidable(30.0f)
	, mBulletPool(nullptr)
	, mShip(nullptr)
	, mGame(nullptr)
	, mExplosion(nullptr)
	, mImageId(0)
	, mPosition(0.0f, 0.0f)
	, mRotation(0.0f)
	, mHealth(0)
	, mCenterPoint(0.0f, 0.0f)
	, mTargetPosition (0.0f)
	, mTargetPositionUpdate(0.0f)
	, mFireRate(0.0f)
{
	
}

Enemy::~Enemy()
{

}

void Enemy::Load()
{
	mImageId = X::LoadTexture("carrier_01.png");
	XASSERT(mImageId > 0, "Enemy: image not found");

	mPosition = X::Math::Vector2::Zero();
	mRotation = 0.0f;

	SetCollisionFilter(ET_SHIP | ET_BULLET_PLAYER);

	mExplosion = new AnimSpriteSheet();
	mExplosion->Load();

	mTargetPositionUpdate = 0.0f;
	mFireRate = 5.0f;
}

void Enemy::Update(float deltaTime)
{
	if (IsAlive())
	{
		const float speed = 70.0f;
		const float rotationSpeed = X::Math::kPiByTwo;
		const float offSetDistance = 200.0f;

		mTargetPositionUpdate -= deltaTime;
		if (mTargetPositionUpdate <= 0.0f || X::Math::MagnitudeSqr(mTargetPosition - mPosition) <= 100.0f)
		{
			// Keep enemy within screen bounds
			const float screenWidth = static_cast<float>(X::GetScreenWidth());
			const float screenHeight = static_cast<float>(X::GetScreenHeight());
			const float margin = GetRadius();

			X::Math::Vector2 newTarget = mCenterPoint + (X::RandomUnitCircle() * offSetDistance);

			newTarget.x = X::Math::Clamp(newTarget.x, margin, screenWidth - margin);
			newTarget.y = X::Math::Clamp(newTarget.y, margin, screenHeight - margin);

			mTargetPosition = newTarget;
			mTargetPositionUpdate = X::RandomFloat(3.0f, 5.0f);
		}

		X::Math::Vector2 moveDirection = X::Math::Normalize(mTargetPosition - mPosition);
		mPosition += moveDirection * speed * deltaTime;

		// Keep enemy within screen bounds
		const float screenWidth = static_cast<float>(X::GetScreenWidth());
		const float screenHeight = static_cast<float>(X::GetScreenHeight());
		const float margin = GetRadius();

		mPosition.x = X::Math::Clamp(mPosition.x, margin, screenWidth - margin);
		mPosition.y = X::Math::Clamp(mPosition.y, margin, screenHeight - margin);

		if (mShip != nullptr && mShip->IsAlive())
		{
			moveDirection = X::Math::Normalize(mShip->GetPosition() - mPosition);
			float targetRotation = atan2(moveDirection.x, -moveDirection.y);
			mRotation = targetRotation;

			mFireRate -= deltaTime;
			if (mFireRate <= 0.0f)
			{
				Bullet* bullet = mBulletPool->GetBullet();
				if (bullet != nullptr)
				{
					X::Math::Vector2 bulletPos = mPosition + X::Math::Vector2::Forward(mRotation) * 50.0f;
					bullet->SetEntityType(ET_BULLET_ENEMY);
					bullet->SetActive(bulletPos, mRotation, 3.0f);
					mFireRate = X::RandomFloat(1.0f, 3.0f);
				}
			}
		}
	}
	mExplosion->Update(deltaTime);
}

void Enemy::Render()
{
	if (IsAlive())
	{
		X::DrawSprite(mImageId, mPosition, mRotation);
		X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::AliceBlue);
	}
	mExplosion->Render();
}

void Enemy::Unload()
{
}

int Enemy::GetType() const
{
	return ET_ENEMY;
}

const X::Math::Vector2& Enemy::GetPosition() const
{
	return mPosition;
}

void Enemy::OnCollision(Collidable* collidable)
{
	if (IsAlive())
	{
		int damage = 0;
		if (collidable->GetType() == ET_SHIP)
		{
			damage = mHealth;
		}
		else
		{
			damage = 10;
		}
		mHealth -= damage;
		if (!IsAlive())
		{
			SetCollisionFilter(0);
			mExplosion->SetActive(mPosition);

			if (mGame != nullptr && mGame->GetScore() != nullptr)
			{
				mGame->GetScore()->AddPoints(10);
			}
		}
	}
}

void Enemy::SetBulletPool(BulletPool* bulletPool)
{
	mBulletPool = bulletPool;
}

void Enemy::SetShip(Ship* ship)
{
	mShip = ship;
}

void Enemy::SetGame(Game* game)
{
	mGame = game;
}

void Enemy::SetPosition(const X::Math::Vector2& position)
{
	mPosition = position;
	mCenterPoint = position;
	mTargetPosition = position;
	mTargetPositionUpdate = 0.0f;
}

void Enemy::SetRotation(float rotation)
{
	mRotation = rotation;
}

bool Enemy::IsAlive() const
{
	return mHealth > 0;
}

void Enemy::Reset()
{
	mHealth = 100;
	mFireRate = 0.0f;
	mTargetPositionUpdate = X::RandomFloat(1.0f, 3.0f);
	SetCollisionFilter(ET_SHIP | ET_BULLET_PLAYER);
}