#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "OverScene.h"
#include "ImGuiManager.h"

	/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
/// <returns>シングルトンインスタンス</returns>
SceneManager* SceneManager::GetInstance() {
	// シングルトンインスタンス
	static SceneManager instance;
	return &instance;
}

/// <summary>
/// 初期化処理
/// </summary>
void SceneManager::Initialize() {
	// 入力系クラスのシングルトンインスタンス取得
	input_ = Input::GetInstance();

	// それぞれのシーンの初期化
	scene_[TITLE_S] = std::make_unique<TitleScene>();
	scene_[GAME_S] = std::make_unique<GameScene>();
	scene_[CLEAR_S] = std::make_unique<ClearScene>();
	scene_[OVER_S] = std::make_unique<OverScene>();

	// 最初のシーンはタイトル
	sceneNo_ = TITLE_S;

	// 選んだシーンの初期化処理
	scene_[sceneNo_]->Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void SceneManager::Update() { 
	// 前フレームのシーン
	preSceneNo_ = sceneNo_;
	
	// 今のシーンを取得
	sceneNo_ = scene_[sceneNo_]->GetSceneNo();

	// もし前フレームから切り替わっていたら
	if (preSceneNo_ != sceneNo_) {

		if (preSceneNo_ == GAME_S) {
			scene_[preSceneNo_].reset();
			scene_[GAME_S] = std::make_unique<GameScene>();
		}

		scene_[sceneNo_]->SetPreButton(preJoyButton_);
		// そのシーンの初期化処理
		scene_[sceneNo_]->Initialize();
	}

	// シーンの更新処理
	scene_[sceneNo_]->Update();
	preJoyButton_ = scene_[sceneNo_]->GetPreButton();



	// ゲーム終了を受け取る
	isEnd_ = scene_[sceneNo_]->IsEnd();		
}

/// <summary>
/// 描画処理
/// </summary>
void SceneManager::Draw() { 
	// シーンの描画処理
	scene_[sceneNo_]->Draw();
}
