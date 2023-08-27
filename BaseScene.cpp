#include "BaseScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
BaseScene::BaseScene() {}

/// <summary>
/// デストラクタ
/// </summary>
BaseScene::~BaseScene() {}

/// <summary>
/// 初期化処理
/// </summary>
void BaseScene::Initialize() {
#pragma region DirectXと入力系と音系の初期化
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
#pragma endregion
}

/// <summary>
/// 更新処理
/// </summary>
void BaseScene::Update() {}

/// <summary>
/// 描画処理
/// </summary>
void BaseScene::Draw() {}
