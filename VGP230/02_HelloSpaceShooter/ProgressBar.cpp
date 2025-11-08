#include "ProgressBar.h"

ProgressBar::ProgressBar()
	: Entity()
	, mBackgroundId(0)
	, mBarId(0)
	, mBarRect()
	, mPosition(0.0f, 0.0f)
	, mBarFullWidth(0.0f)
{

}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::Load()
{
	mBackgroundId = X::LoadTexture("bar_empty.png");
	mBarId = X::LoadTexture("bar_red.png");
	mBarRect.right = X::GetSpriteWidth(mBarId);
	mBarRect.bottom = X::GetSpriteHeight(mBarId);
	mBarFullWidth = mBarRect.right;
	mPosition.x = (X::GetScreenWidth() - mBarFullWidth) * 0.5f;
	mPosition.y = 20.0f;
}

void ProgressBar::Update(float deltaTime)
{

}

void ProgressBar::Render()
{
	X::DrawSprite(mBackgroundId, mPosition, X::Pivot::Left);
	X::DrawSprite(mBarId, mBarRect, mPosition, X::Pivot::Left);
}

void ProgressBar::Unload()
{

}

void ProgressBar::SetBarValue(int current, int max)
{
	mBarRect.right = X::Math::Clamp((float)current / (float)max, 0.0f, 1.0f) * mBarFullWidth;
}


