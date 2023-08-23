#pragma once

#include "BaseScene.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>


/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public BaseScene {
public:
	TitleScene();
	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

private:
	


};