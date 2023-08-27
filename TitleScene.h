#pragma once

#include "BaseScene.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>


/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public BaseScene {
public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

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
	/// グローバル変数の取得
	/// </summary>
	void ApplyGlobalVariavles();

	/// <summary>
	///
	/// </summary>
	void InitializeGlobalVariavles();

private: // メンバ変数
	// タイトルロゴ
	std::unique_ptr<Sprite> title_ = nullptr;
	// タイトルロゴのテクスチャ
	uint32_t titleTex_;
	// タイトルロゴの位置
	Vector2 titleLogoPos_{};

	// [始める]
	std::unique_ptr<Sprite> startButton_ = nullptr;
	// [始める]のテクスチャ
	uint32_t startButtonTex_;
	// [始める]の位置
	Vector2 startButtonPos_{};

	// [終わる]
	std::unique_ptr<Sprite> endButton_ = nullptr;
	// [終わる]のテクスチャ
	uint32_t endButtonTex_;
	// [終わる]の位置
	Vector2 endButtonPos_{};

	bool isStart = false;
	bool isEnd = false;

	POINT mousePos_;

};