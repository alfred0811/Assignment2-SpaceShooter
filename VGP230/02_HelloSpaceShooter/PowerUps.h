#pragma once

#include "Entity.h"
#include "Collidable.h"

enum class PowerUpType
{
	TripleShot,
	Shield,
	SpeedBoost
};

class PowerUp : public Entity, public Collidable
{
public:
	PowerUp();
	~PowerUp() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void Spawn(PowerUpType type, const X::Math::Vector2& position);
	void Deactivate();
	bool IsActive() const;

	int GetType() const override;

	const X::Math::Vector2& GetPosition() const override;
	void OnCollision(Collidable* collidable) override;

	PowerUpType GetPowerUpType() const;

private:
	X::TextureId mTripleShotImageId;
	X::TextureId mShieldImageId;
	X::TextureId mSpeedBoostImageId;

	X::Math::Vector2 mPosition;
	PowerUpType mPowerUpType;
	bool mIsActive;
	float mLifeTime;
	float mMaxLifeTime;
};