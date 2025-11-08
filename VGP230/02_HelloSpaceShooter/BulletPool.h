#pragma once

#include "Entity.h"

// Forward declaration
class Bullet;

class BulletPool : public Entity
{
public:
	BulletPool();
	~BulletPool() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	Bullet* GetBullet();
	std::vector<Bullet*>& GetBullets();
private:
	int mNextAvaibleIndex;
	std::vector<Bullet*> mBullets;
};