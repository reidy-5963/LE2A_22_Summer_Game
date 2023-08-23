#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "WinApp.h"

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public: // メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ターゲットの設定
	/// </summary>
	/// <param name="target">ターゲットになるワールド変換データ</param>
	void SetTarget(const WorldTransform* target);

	/// <summary>
	/// ViewProjectionの取得
	/// </summary>
	/// <returns>ViewProjection</returns>
	ViewProjection& GetViewProjection();

private: // メンバ変数
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// ターゲットになるワールド変換データ
	const WorldTransform* target_ = nullptr;

	// マウスの位置(preは前フレーム)
	POINT mousePos_;
	POINT preMousePos_;

	// 
	float xCameraRotate_ = 0.0f;
	float yCameraRotate_ = 0.0f;
};