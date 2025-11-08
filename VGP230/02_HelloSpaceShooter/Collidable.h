#pragma once

#include "Enum.h"

namespace X
{
	namespace Math
	{
		class Vector2;
	}
}

class Collidable
{
	public:
	Collidable(float radius);
	virtual ~Collidable();

	virtual int GetType() const = 0;
	virtual const X::Math::Vector2& GetPosition() const = 0;
	virtual void OnCollision(Collidable* collidable) = 0;

	virtual void SetCollisionFilter(int filter);
	virtual int GetCollisionFilter() const;
	virtual float GetRadius() const;
	virtual bool DidCollide(const Collidable* collidable) const;

private:
	float mRadius;
	int mCollidableFilter;
};