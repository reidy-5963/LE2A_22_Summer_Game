#pragma once
#include "BaseScene.h"

/// <summary>
/// シーンマネージャー
/// </summary>
class SceneManager {
public: // 静的メンバ関数
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static SceneManager* GetInstance();

public: // 動的メンバ関数

	/// <summary>
	/// コピー禁止
	/// </summary>
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	const SceneManager& operator=(const SceneManager&) = delete;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ゲーム終了かどうか
	/// </summary>
	/// <returns>isEnd_</returns>
	bool IsEnd() { return isEnd_; }

private: // メンバ変数
	// 各シーン
	std::unique_ptr<BaseScene> scene_[4];
	
	// 入力系クラス
	Input* input_ = nullptr;

	// シーン番号
	int sceneNo_;

	// シーン番号(前フレーム)
	int preSceneNo_;

	// ゲーム終了フラグ
	bool isEnd_ = false;
};