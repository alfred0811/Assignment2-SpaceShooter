#include "Bullet.h"

Bullet::Bullet()
	: Entity()
	, Collidable(5.0f)
	, mImageId(0)
	, mPosition(0.0f)
	, mRotation(0.0f)
	, mLifeTime(0.0f)
	, mBulletType(ET_BULLET_PLAYER)
{
	
}
Bullet::~Bullet()
{
	
}
void Bullet::Load()
{
	mImageId = X::LoadTexture("bullet1.png");
}
void Bullet::Update(float deltaTime)
{
	if (IsActive())
	{
		mLifeTime -= deltaTime;
		const float speed = 200.0f;
		mPosition += X::Math::Vector2::Forward(mRotation) * speed * deltaTime;
		if (!IsActive())
		{
			SetCollisionFilter(ET_NONE); 
		}
	}
}
void Bullet::Render()
{
	if (IsActive())
	{
		X::DrawSprite(mImageId, mPosition, mRotation);
		X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::HotPink);
	}
}
void Bullet::Unload()
{

}
void Bullet::SetActive(const X::Math::Vector2& pos, float rotation, float lifeTime)
{
	mPosition = pos;
	mRotation = rotation;
	mLifeTime = lifeTime;

	SetEntityType(mBulletType);
}
void Bullet::SetEntityType(EntityType entitytype)
{
	mBulletType = entitytype;
	if (entitytype == ET_BULLET_PLAYER)
	{
		SetCollisionFilter(ET_ENEMY | ET_BOSS);
	}
	else if (entitytype == ET_BULLET_ENEMY)
	{
		SetCollisionFilter(ET_SHIP);
	}
	else
	{
		SetCollisionFilter(0);
	}
}
bool Bullet::IsActive() const
{
	return mLifeTime > 0.0f;
}

int Bullet::GetType() const
{
	return mBulletType;
}

const X::Math::Vector2& Bullet::GetPosition() const
{
	return mPosition;
}

void Bullet::OnCollision(Collidable* collidable)
{
	mLifeTime = 0.0f;
	SetCollisionFilter(ET_NONE);
}