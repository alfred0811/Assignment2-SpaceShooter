#include <XEngine.h>

#include "Collidable.h"

Collidable::Collidable(float radius) 
	: mRadius(radius)
	, mCollidableFilter(-1)

{

}
Collidable::~Collidable()
{

}
void Collidable::SetCollisionFilter(int filter)
{
	mCollidableFilter = filter;
}
int Collidable::GetCollisionFilter() const
{
	return mCollidableFilter;
}
float Collidable::GetRadius() const
{
	return mRadius;
}
bool Collidable::DidCollide(const Collidable* collidable) const
{
	if (collidable == nullptr)
	{
		return false;
	}

	int collidableFilter = collidable->GetCollisionFilter();
	int collisionFilter = GetCollisionFilter();
	if (collidable == 0 || collisionFilter == 0)
	{
		return false;
	}

	if (collidableFilter < 0 || collisionFilter < 0 || (collidableFilter & GetType()) > 0)
	{
		float distSq = X::Math::Vector2::SqrMagnitude(collidable->GetPosition() - GetPosition());
		float radiusSq = X::Math::Sqr(collidable->GetRadius() + GetRadius());
		return distSq <= radiusSq;
	}

	return false;
}