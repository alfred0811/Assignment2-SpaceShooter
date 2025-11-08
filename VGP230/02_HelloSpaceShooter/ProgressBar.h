#pragma once

#include "Entity.h"

class ProgressBar : public Entity
{
public:
	ProgressBar();
	~ProgressBar() override;

	// Entity Functions
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;
	// ProgressBar Functions
	void SetBarValue(int current, int max);
private:
	X::TextureId mBackgroundId;
	X::TextureId mBarId;
	X::Math::Rect mBarRect;
	X::Math::Vector2 mPosition;
	float mBarFullWidth;
};
