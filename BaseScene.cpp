#include "BaseScene.h"

BaseScene::BaseScene() {}

BaseScene::~BaseScene() {}

void BaseScene::Initialize() {
#pragma region DirectXと入力系と音系の初期化
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
#pragma endregion
}

void BaseScene::Update() {}

void BaseScene::Draw() {}
