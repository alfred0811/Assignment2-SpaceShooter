#include "PowerUps.h"

PowerUp::PowerUp()
	: Entity()
	, Collidable(20.0f)
	, mTripleShotImageId(0)
	, mShieldImageId(0)
	, mSpeedBoostImageId(0)
	, mPosition(0.0f, 0.0f)
	, mPowerUpType(PowerUpType::TripleShot)
	, mIsActive(false)
	, mLifeTime(0.0f)
	, mMaxLifeTime(10.0f)
{
}

PowerUp::~PowerUp()
{
}

void PowerUp::Load()
{
	mTripleShotImageId = X::LoadTexture("crosshair.png");
	mShieldImageId = X::LoadTexture("explosion.png");
	mSpeedBoostImageId = X::LoadTexture("mushroom.png");

	SetCollisionFilter(ET_SHIP);
}

void PowerUp::Update(float deltaTime)
{
	if (!mIsActive)
		return;

	mLifeTime -= deltaTime;
	if (mLifeTime <= 0.0f)
	{
		Deactivate();
	}
}

void PowerUp::Render()
{
	if (!mIsActive)
		return;

	X::TextureId imageId = 0;
	switch (mPowerUpType)
	{
	case PowerUpType::TripleShot:
		imageId = mTripleShotImageId;
		break;
	case PowerUpType::Shield:
		imageId = mShieldImageId;
		break;
	case PowerUpType::SpeedBoost:
		imageId = mSpeedBoostImageId;
		break;
	}

	X::DrawSprite(imageId, mPosition);
	X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::Cyan);
}

void PowerUp::Unload()
{
}

void PowerUp::Spawn(PowerUpType type, const X::Math::Vector2& position)
{
	mPowerUpType = type;
	mPosition = position;
	mIsActive = true;
	mLifeTime = mMaxLifeTime;
}

void PowerUp::Deactivate()
{
	mIsActive = false;
	SetCollisionFilter(0);
}

bool PowerUp::IsActive() const
{
	return mIsActive;
}

int PowerUp::GetType() const
{
	return ET_POWERUP;
}

const X::Math::Vector2& PowerUp::GetPosition() const
{
	return mPosition;
}

void PowerUp::OnCollision(Collidable* collidable)
{
	if (mIsActive && collidable->GetType() == ET_SHIP)
	{
		Deactivate();
	}
}

PowerUpType PowerUp::GetPowerUpType() const
{
	return mPowerUpType;
}