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
	// directXクラスのシングルトンインスタンス取得
	dxCommon_ = DirectXCommon::GetInstance();

	// 入力系クラスのシングルトンインスタンス取得
	input_ = Input::GetInstance();

	// 音系クラスのシングルトンインスタンス取得
	audio_ = Audio::GetInstance();
}

/// <summary>
/// 更新処理
/// </summary>
void BaseScene::Update() {}

/// <summary>
/// 描画処理
/// </summary>
void BaseScene::Draw() {}
