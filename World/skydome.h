#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

/// <summary>
/// 天球
/// </summary>
class Skydome {
public: // メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">位置</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(ViewProjection& viewProjection);

private: // メンバ変数
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;
};
