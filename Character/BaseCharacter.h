#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>

/// <summary>
/// ベースキャラクター
/// </summary>
class BaseCharacter {
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">使うモデルの動的配列</param>
	virtual void Initialize(const std::vector<Model*>& models);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	virtual void Draw(const ViewProjection& viewProjection);
	
	/// <summary>
	/// もし当たっていたらの処理
	/// </summary>
	virtual void OnCollision(float damage) = 0;

	/// <summary>
	/// グローバル変数の初期化処理
	/// </summary>
	virtual void InitializeGlobalVariavles();

	/// <summary>
	/// グローバル変数の取得
	/// </summary>
	virtual void ApplyGlobalVariavles();

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	virtual void UpdateFloatingGimmick();

	/// <summary>
	/// 腕ふりギミック更新
	/// </summary>
	virtual void UpdateArmGimmick();

	/// <summary>
	/// 攻撃を食らったときの動き
	/// </summary>
	virtual void DamageGimmick();

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns>ワールド変換データ</returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <returns>半径</returns>
	float GetRadius() { return radius_; }

	/// <summary>
	/// 攻撃を食らったかのフラグ
	/// </summary>
	/// <returns></returns>
	bool IsDamage() { return isDamage_; }

	/// <summary>
	/// 死んだかどうかのフラグ
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return isDead_; }

protected: // メンバ変数
	// モデルデータ配列
	std::vector<Model*> models_;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// 体力(仮で200)
	float hp_ = 200.0f;

	// 浮遊動作の媒介変数
	float floatingParameter_ = 0.0f;

	// 腕ふりギミックの媒介変数
	float armParameter_ = 0.0f;

	// 被ダメージギミックの媒介変数
	float damagingParameter_ = 0.0f;


	// 攻撃を食らったかどうか
	bool isDamage_ = false;

	// 死んだかどうか
	bool isDead_ = false;
protected:
	// hp設定する値
	float setHp_ = 200.0f;


	// 弾の間隔
	int bulletTime = 15;

	// 動くときの速さ
	float moveSpeed = 0.3f;

	// 弾の速さ
	float bulletSpeed = 1.0f;

	// 当たり判定用の半径(仮で1.0f)
	float radius_ = 1.0f;


	// 浮遊動作のサイクル
	float floatingPeriod_ = 60.0f;
	// 浮遊動作の振れ幅
	float floatingAmplitude_ = 60.0f;

	
	// 腕ふりのサイクル<flame>
	float armPeriod_ = 40.0f;
	// 腕ふりの振幅<m>
	float armAmplitude_ = 0.15f;


	// 被ダメージのサイクル<flame>
	float damagingPeriod_ = 40.0f;
	// 被ダメージの振幅<m>
	float damagingAmplitude_ = 0.15f;
};