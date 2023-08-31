#pragma once

#include "Model.h"
#include "WorldTransform.h"


class BaseBullet {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="models">モデル動的配列</param>
	/// <param name="position">位置</param>
	/// <param name="velosity">打ち出す向き&強さ</param>
	void Initialize(
	    const std::vector<Model*>& models, const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// もし当たっていたら
	/// </summary>
	virtual void OnCollision();

	/// <summary>
	/// グローバル変数の初期化処理
	/// </summary>
	virtual void InitializeGlobalVariavles();

	/// <summary>
	/// グローバル変数の取得
	/// </summary>
	virtual void ApplyGlobalVariavles();


	/// <summary>
	/// 弾の有効フラグ
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <returns>半径</returns>
	float GetRadius() { return radius_; }

	/// <summary>
	/// ダメージ数の取得
	/// </summary>
	/// <returns></returns>
	float GetDamage() { return damage_; }

protected:
	//
	WorldTransform worldTransform_;
	
	//
	std::vector<Model*> models_;

	//
	uint32_t textureHandle_ = 0u;

	//
	Vector3 velosity_;

	//
	int lifeTime = 60 * 2;

	//
	int deathTimer_ = lifeTime;

	//
	bool isDead_ = false;

	//
	float radius_ = 1.0f;

	float damage_ = 100.0f;
};