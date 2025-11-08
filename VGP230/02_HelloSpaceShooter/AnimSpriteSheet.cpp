#include "AnimSpriteSheet.h"

AnimSpriteSheet::AnimSpriteSheet()
	: Entity()
	, mSpriteSheetId(0)
	, mSpriteRect()
	, mPosition(0.0f, 0.0f)
	, mFrameRate(0.0f)
	, mCurrentFrameTime(0.0f)
	, mRows(0)
	, mColumns(0)
	, mMaxFrames(0)
	, mCurrentIndex(0)
	, mIsLooping(false)
{
}

AnimSpriteSheet::~AnimSpriteSheet()
{
}

void AnimSpriteSheet::Load()
{
	mSpriteSheetId = X::LoadTexture("explosion_anim.png");
	mSpriteRect.right = 100.0f;
	mSpriteRect.bottom = 100.0f;
	mMaxFrames = 81;
	mRows = 9;
	mColumns = 9;
	mFrameRate = 1.0f / 120.0f;
	mIsLooping = false;
	mCurrentIndex = -1;
}

void AnimSpriteSheet::Update(float deltaTime)
{
	if (IsActive())
	{
		mCurrentFrameTime += deltaTime;
		while (mCurrentFrameTime > mFrameRate)
		{
			mCurrentFrameTime -= mFrameRate;
			++mCurrentIndex;
		}

		if (mCurrentIndex >= mMaxFrames)
		{
			if (mIsLooping)
			{
				mCurrentIndex -= mMaxFrames;
			}
			else
			{
				mCurrentIndex = -1;
			}
		}

		const float rectWidth = 100.0f;
		const float rectHeight = 100.0f;
		mSpriteRect.left = (mCurrentIndex % mColumns) * rectWidth;
		mSpriteRect.top = (mCurrentIndex / mColumns) * rectHeight;
		mSpriteRect.right = mSpriteRect.left + rectWidth;
		mSpriteRect.bottom = mSpriteRect.top + rectHeight;
	}
}

void AnimSpriteSheet::Render()
{
	if (IsActive())
	{
		X::DrawSprite(mSpriteSheetId, mSpriteRect, mPosition);
	}
}

void AnimSpriteSheet::Unload()
{

}

void AnimSpriteSheet::SetActive(X::Math::Vector2 position, bool loop /*= false*/)
{
	mPosition = position;
	mIsLooping = loop;
	mCurrentIndex = 0;
	mCurrentFrameTime = 0.0f;
}

bool AnimSpriteSheet::IsActive() const
{
	return mIsLooping || mCurrentIndex >= 0;
}

