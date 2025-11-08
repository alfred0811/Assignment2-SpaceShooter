#pragma once

#include "Entity.h"

class AnimSpriteSheet : public Entity
{
public:
	AnimSpriteSheet();
	~AnimSpriteSheet() override;

	// Entity Functions
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	// AnimSpriteSheet Functions
	void SetActive(X::Math::Vector2 position, bool loop = false);
	bool IsActive() const;
private:
	X::TextureId mSpriteSheetId;
	X::Math::Rect mSpriteRect;
	X::Math::Vector2 mPosition;
	float mFrameRate;
	float mCurrentFrameTime;
	int mRows;
	int mColumns;
	int mMaxFrames;
	int mCurrentIndex;
	bool mIsLooping;
};