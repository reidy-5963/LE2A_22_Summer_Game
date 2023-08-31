#pragma once

#include "BaseBullet.h"
#include "ViewProjection.h"

class PlayerBullet : public BaseBullet {
public: // メンバ関数
	/// <summary>
	/// 
	/// </summary>
	/// <param name="models"></param>
	/// <param name="position"></param>
	/// <param name="velosity"></param>
	void Initialize(
	    const std::vector<Model*>& models,
		const Vector3& position, const Vector3& velosity);

	/// <summary>
	/// 
	/// </summary>
	void Update() override;

	/// <summary>
	/// 
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// グローバル変数の取得
	/// </summary>
	void ApplyGlobalVariavles() override;

	/// <summary>
	///
	/// </summary>
	void InitializeGlobalVariavles() override;

private: // メンバ変数
};