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

	/// <summary>
	/// グローバル変数の取得
	/// </summary>
	void ApplyGlobalVariavles();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="isDamage"></param>
	void SetIsDamage(bool isDamage) { isDamage_ = isDamage; }

	void SetRotation(Vector3 rotation) { playerRotation_ = rotation; }


	void SetDamagingValue(float param, float period, float amp) {
		damagingParameter_ = param;
		damagingPeriod_ = period;
		damagingAmplitude_ = amp;
	}

	bool IsFps() { return isFps_; }

private: // メンバ変数
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// fps視点でのカメラ位置
	Vector3 fpsView_ = {0.0f, 4.0f, 0.0f};
	// tps視点でのカメラ位置
	Vector3 tpsView_ = {0.0f, 4.0f, -30.0f};

	// ターゲットになるワールド変換データ
	const WorldTransform* target_ = nullptr;

	// マウスの位置
	POINT mousePos_;
	// 移動量を加算するためのやつ
	POINT allMouseDistance_;

	// 1フレームごとの移動量のためのやつ
	float xMouseDistance = 0.0f;
	float yMouseDistance = 0.0f;

	// キーマウ、コントローラーそれぞれの移動速度
	float move_mouseSpeed = 0.002f;
	float move_padSpeed = 5.0f; 

	Vector3 offset_ = {0.0f, 5.0f, 0.0f};

	bool isDamage_ = false;

	Vector3 playerRotation_{};

	// 浮遊動作の媒介変数
	float damagingParameter_ = 0.0f;

	// 浮遊動作のサイクル
	float damagingPeriod_ = 60.0f;

	// 浮遊動作の振れ幅
	float damagingAmplitude_ = 60.0f;

	//
	bool isFps_ = true;
};