#pragma once

#include "Model.h"
#include "WorldTransform.h"


class BaseBullet {
public:
	/// <summary>
	///
	/// </summary>
	/// <param name="models"></param>
	/// <param name="position"></param>
	/// <param name="velosity"></param>
	void Initialize(
	    const std::vector<Model*>& models, const Vector3& position);

	/// <summary>
	///
	/// </summary>
	virtual void Update();

	/// <summary>
	///
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection);

	/// <summary>
	///
	/// </summary>
	virtual void OnCollision();

	/// <summary>
	/// グローバル変数の取得
	/// </summary>
	virtual void ApplyGlobalVariavles();

	/// <summary>
	///
	/// </summary>
	virtual void InitializeGlobalVariavles();

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <returns>半径</returns>
	float GetRadius() { return radius_; }

	/// <summary>
	/// 
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
	int lifeTime = 60 * 5;

	//
	int deathTimer_ = lifeTime;

	//
	bool isDead_ = false;

	//
	float radius_ = 1.0f;

	float damage_ = 100.0f;
};