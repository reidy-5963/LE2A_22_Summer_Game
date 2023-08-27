#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include <memory>

/// <summary>
/// シーン
/// </summary>
enum SCENE {
	TITLE_S,
	GAME_S,
};

/// <summary>
/// シーンの基底クラス
/// </summary>
class BaseScene {
public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BaseScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BaseScene();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// シーン番号の設定
	/// </summary>
	/// <param name="sceneNo"></param>
	void SetSceneNo(int sceneNo) { scenedNo_ = sceneNo; }
	
	/// <summary>
	/// シーン番号の取得
	/// </summary>
	/// <returns></returns>
	int GetSceneNo() { return scenedNo_; }

protected: // メンバ変数
	// DirectXクラス
	DirectXCommon* dxCommon_ = nullptr;
	// 入力系クラス
	Input* input_ = nullptr;
	// 音系クラス
	Audio* audio_ = nullptr;

	// シーン番号
	int scenedNo_ = 0;
};