#include "Boss.h"
#include "BulletPool.h"
#include "Bullet.h"
#include "Ship.h"
#include "AnimSpriteSheet.h"
#include "Game.h"
#include "Score.h"

#include <XEngine.h>

Boss::Boss()
	: Entity()
	, Collidable(60.0f)  // Boss radius (bigger than regular enemy)
	, mBulletPool(nullptr)
	, mShip(nullptr)
	, mGame(nullptr)
	, mExplosion(nullptr)
	, mImageId(0)
	, mPosition(X::Math::Vector2::Zero())
	, mRotation(0.0f)
	, mHealth(0)
	, mMaxHealth(300)  // Boss health
	, mCenterPoint(X::Math::Vector2::Zero())
	, mTargetPosition(X::Math::Vector2::Zero())
	, mTargetPositionUpdate(0.0f)
	, mFireRate(0.0f)
{
}

Boss::~Boss()
{
}

void Boss::Load()
{
	mImageId = X::LoadTexture("ImpressCat.png");
	XASSERT(mImageId > 0, "Boss: image not found");

	mPosition = X::Math::Vector2::Zero();
	mRotation = 0.0f;

	SetCollisionFilter(ET_SHIP | ET_BULLET_PLAYER);

	mExplosion = new AnimSpriteSheet();
	mExplosion->Load();

	mTargetPositionUpdate = 0.0f;
	mFireRate = 0.0f;
	mHealth = 0;
}

void Boss::Update(float deltaTime)
{
	if (IsAlive())
	{
		const float moveSpeed = 60.0f;
		const float offsetDistance = 250.0f;

		mTargetPositionUpdate -= deltaTime;
		if (mTargetPositionUpdate <= 0.0f || X::Math::MagnitudeSqr(mTargetPosition - mPosition) <= 100.0f)
		{
			const float screenWidth = static_cast<float>(X::GetScreenWidth());
			const float screenHeight = static_cast<float>(X::GetScreenHeight());
			const float margin = GetRadius();

			X::Math::Vector2 newTarget = mCenterPoint + (X::RandomUnitCircle() * offsetDistance);
			newTarget.x = X::Math::Clamp(newTarget.x, margin, screenWidth - margin);
			newTarget.y = X::Math::Clamp(newTarget.y, margin, screenHeight - margin);

			mTargetPosition = newTarget;
			mTargetPositionUpdate = X::RandomFloat(3.0f, 5.0f);
		}

		X::Math::Vector2 moveDir = X::Math::Normalize(mTargetPosition - mPosition);
		mPosition += moveDir * moveSpeed * deltaTime;

		// Clamp to screen
		const float screenWidth = static_cast<float>(X::GetScreenWidth());
		const float screenHeight = static_cast<float>(X::GetScreenHeight());
		const float margin = GetRadius();
		mPosition.x = X::Math::Clamp(mPosition.x, margin, screenWidth - margin);
		mPosition.y = X::Math::Clamp(mPosition.y, margin, screenHeight - margin);

		// Rotation towards ship
		if (mShip != nullptr && mShip->IsAlive())
		{
			X::Math::Vector2 toShip = X::Math::Normalize(mShip->GetPosition() - mPosition);
			float targetRot = atan2(toShip.x, -toShip.y);
			mRotation = targetRot;
		}

		// Firing
		mFireRate -= deltaTime;
		if (mFireRate <= 0.0f)
		{
			FireDoubleShot();
			mFireRate = X::RandomFloat(1.0f, 2.5f); // Boss fire interval
		}
	}

	if (mExplosion)
	{
		mExplosion->Update(deltaTime);
	}
}

void Boss::Render()
{
	if (IsAlive())
	{
		X::DrawSprite(mImageId, mPosition, mRotation);
		X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::OrangeRed);

		// Health bar
		const float w = 140.0f;
		const float h = 10.0f;
		const float offsetY = 80.0f;
		float pct = X::Math::Clamp(static_cast<float>(mHealth) / static_cast<float>(mMaxHealth), 0.0f, 1.0f);
		X::Math::Vector2 barPos = { mPosition.x - w * 0.5f, mPosition.y - offsetY };
		X::DrawScreenRect({ barPos.x, barPos.y, barPos.x + w, barPos.y + h }, X::Colors::DarkGray);
		X::DrawScreenRect({ barPos.x, barPos.y, barPos.x + w * pct, barPos.y + h }, X::Colors::Red);
	}
	if (mExplosion)
	{
		mExplosion->Render();
	}
}

void Boss::Unload()
{
	if (mExplosion)
	{
		mExplosion->Unload();
		delete mExplosion;
		mExplosion = nullptr;
	}
}

int Boss::GetType() const
{
	return ET_BOSS;
}

const X::Math::Vector2& Boss::GetPosition() const
{
	return mPosition;
}

void Boss::OnCollision(Collidable* collidable)
{
	if (!IsAlive())
		return;

	int damage = 0;
	if (collidable->GetType() == ET_SHIP)
	{
		damage = 50;
	}
	else if (collidable->GetType() == ET_BULLET_PLAYER)
	{
		damage = 10;
	}

	if (damage > 0)
	{
		mHealth -= damage;
		if (!IsAlive())
		{
			SetCollisionFilter(0);
			if (mExplosion)
				mExplosion->SetActive(mPosition);

			if (mGame && mGame->GetScore())
			{
				mGame->GetScore()->AddPoints(100); // Boss reward
			}
		}
	}
}

void Boss::SetBulletPool(BulletPool* bulletPool)
{
	mBulletPool = bulletPool;
}

void Boss::SetShip(Ship* ship)
{
	mShip = ship;
}

void Boss::SetGame(Game* game)
{
	mGame = game;
}

void Boss::Activate()
{
	// Spawn near top center
	const float screenWidth = static_cast<float>(X::GetScreenWidth());
	const float screenHeight = static_cast<float>(X::GetScreenHeight());
	mPosition = { screenWidth * 0.5f, screenHeight * 0.25f };

	mCenterPoint = mPosition;
	mTargetPosition = mPosition;
	mTargetPositionUpdate = 0.0f;

	mRotation = 0.0f;
	mHealth = mMaxHealth;
	mFireRate = X::RandomFloat(1.0f, 2.5f);
	SetCollisionFilter(ET_SHIP | ET_BULLET_PLAYER);
}

void Boss::Deactivate()
{
	mHealth = 0;
	SetCollisionFilter(0);
}

bool Boss::IsAlive() const
{
	return mHealth > 0;
}

void Boss::FireDoubleShot()
{
	if (!mBulletPool || !IsAlive())
		return;

	// Forward and side for dual cannons
	X::Math::Vector2 forward = X::Math::Vector2::Forward(mRotation);
	X::Math::Vector2 right(forward.y, -forward.x);

	X::Math::Vector2 leftMuzzle = mPosition + forward * 60.0f - right * 30.0f;
	X::Math::Vector2 rightMuzzle = mPosition + forward * 60.0f + right * 30.0f;

	const float spread = X::Math::kPi / 24.0f;

	// Left bullet
	if (Bullet* bullet1 = mBulletPool->GetBullet())
	{
		bullet1->SetEntityType(ET_BULLET_ENEMY);
		bullet1->SetActive(leftMuzzle, mRotation - spread, 3.5f);
	}
	// Right bullet
	if (Bullet* bullet2 = mBulletPool->GetBullet())
	{
		bullet2->SetEntityType(ET_BULLET_ENEMY);
		bullet2->SetActive(rightMuzzle, mRotation + spread, 3.5f);
	}
}