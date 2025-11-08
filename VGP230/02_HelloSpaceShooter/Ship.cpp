#include "Ship.h"
#include "Bullet.h"
#include "BulletPool.h"
#include "AnimSpriteSheet.h"

Ship::Ship()
	: Entity()
	, Collidable(30.0f)
	, mImageId(0)
	, mPosition(0.0f)
	, mRotation(0.0f)
	, mHealth(0)
	, mMaxHealth(100)
	, mExplosion(nullptr)
	, mBulletPool(nullptr)
	, mTripleShotTimer(0.0f)
	, mShieldTimer(0.0f)
	, mSpeedBoostTimer(0.0f)
{
	
}

Ship::~Ship()
{

}

void Ship::Load()
{
	mImageId = X::LoadTexture("fighter.png");
	XASSERT(mImageId != 0, "Ship: image not found");

	mPosition.x = X::GetScreenWidth() * 0.5f;
	mPosition.y = X::GetScreenHeight() * 0.5f;

	SetCollisionFilter(ET_ENEMY | ET_BULLET_ENEMY);
	
	mHealth = mMaxHealth;
	mExplosion = new AnimSpriteSheet();
	mExplosion->Load();
}

void Ship::Update(float deltaTime)
{
	// Update power-up timers
	if (mTripleShotTimer > 0.0f)
		mTripleShotTimer -= deltaTime;
	if (mShieldTimer > 0.0f)
		mShieldTimer -= deltaTime;
	if (mSpeedBoostTimer > 0.0f)
		mSpeedBoostTimer -= deltaTime;

	const float baseSpeed = 100.0f;
	const float speed = HasSpeedBoost() ? baseSpeed * 1.5f : baseSpeed;
	const float turnSpeed = X::Math::kPiByTwo;
	//Up-down movement
	if (X::IsKeyDown(X::Keys::W))
	{
		mPosition += X::Math::Vector2::Forward(mRotation) * speed * deltaTime;
	}
	else if (X::IsKeyDown(X::Keys::S))
	{
		mPosition -= X::Math::Vector2::Forward(mRotation) * speed * deltaTime;
	}

	//Left-right rotation
	if (X::IsKeyDown(X::Keys::A))
	{
		mRotation -= turnSpeed * deltaTime;
	}
	else if (X::IsKeyDown(X::Keys::D))
	{
		mRotation += turnSpeed * deltaTime;
	}

	// Keep ship within screen bounds
	const float screenWidth = static_cast<float>(X::GetScreenWidth());
	const float screenHeight = static_cast<float>(X::GetScreenHeight());
	const float margin = GetRadius();

	mPosition.x = X::Math::Clamp(mPosition.x, margin, screenWidth - margin);
	mPosition.y = X::Math::Clamp(mPosition.y, margin, screenHeight - margin);

	//Shooting
	if (X::IsKeyPressed(X::Keys::SPACE))
	{
		X::Math::Vector2 spawnPos = mPosition + X::Math::Vector2::Forward(mRotation) * 50.0f;

		if (HasTripleShot())
		{
			// Shoot three bullets
			for (int i = -1; i <= 1; ++i)
			{
				float angle = mRotation + (i * X::Math::kPi / 12.0f);
				X::Math::Vector2 offset = X::Math::Vector2::Forward(angle) * 50.0f;
				Bullet* bullet = mBulletPool->GetBullet();
				bullet->SetEntityType(ET_BULLET_PLAYER);
				bullet->SetActive(mPosition + offset, angle);
			}
		}
		else
		{
			// Shoot single bullet
			Bullet* bullet = mBulletPool->GetBullet();
			bullet->SetEntityType(ET_BULLET_PLAYER);
			bullet->SetActive(spawnPos, mRotation);
		}
	}

	mExplosion->Update(deltaTime);
}

void Ship::Render()
{
	if (IsAlive()) 
	{
		X::DrawSprite(mImageId, mPosition, mRotation);
		X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::Yellow);
	}
	mExplosion->Render();
}

void Ship::Unload()
{
	mExplosion->Unload();
	delete mExplosion;
	mExplosion = nullptr;
}

int Ship::GetType() const
{
	return ET_SHIP;
}

const X::Math::Vector2& Ship::GetPosition() const
{
	return mPosition;
}

void Ship::OnCollision(Collidable* collidable)
{
	if (IsAlive())
	{
		if (collidable->GetType() == ET_POWERUP)
		{
			PowerUp* powerUp = static_cast<PowerUp*>(collidable);
			ActivatePowerUp(powerUp->GetPowerUpType());
			return;
		}

		// Shield
		if (HasShield())
			return;

		// Damage
		int damage = 0;
		if (collidable->GetType() == ET_ENEMY)
		{
			damage = 10;
		}
		else if (collidable->GetType() == ET_BULLET_ENEMY)
		{
			damage = 2;
		}

		if (damage > 0)
		{
			mHealth -= damage;
			if (!IsAlive())
			{
				mExplosion->SetActive(mPosition);
				SetCollisionFilter(0);
			}
		}
	}
}

void Ship::SetBullePool(BulletPool* bulletPool)
{
	mBulletPool = bulletPool;
}

int Ship::GetHealth() const
{
	return mHealth;
}

int Ship::GetMaxHealth() const
{
	return mMaxHealth;
}

bool Ship::IsAlive() const
{
	return mHealth > 0;
}

bool Ship::HasTripleShot() const
{
	return mTripleShotTimer > 0.0f;
}

bool Ship::HasShield() const
{
	return mShieldTimer > 0.0f;
}

bool Ship::HasSpeedBoost() const
{
	return mSpeedBoostTimer > 0.0f;
}

void Ship::ActivatePowerUp(PowerUpType type)
{
	switch (type)
	{
	case PowerUpType::TripleShot:
		mTripleShotTimer = 10.0f;
		break;
	case PowerUpType::Shield:
		mShieldTimer = 5.0f;
		break;
	case PowerUpType::SpeedBoost:
		mSpeedBoostTimer = 5.0f;
		break;
	}
}