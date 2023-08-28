#include "Skydome.h"
#include "MyMath.h"

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="model">モデル</param>
/// <param name="position">位置</param>
void Skydome::Initialize(Model* model, const Vector3& position) {
	// モデルがないならassertで落とす
	assert(model);

	// モデルを代入
	model_ = model;

	// ワールド変換データの初期化処理
	worldTransform_.Initialize();

	// 位置を引数の値で初期化
	worldTransform_.translation_ = position;
}

/// <summary>
/// 更新処理
/// </summary>
void Skydome::Update() {
	// ワールド変換データの更新処理
	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Skydome::Draw(ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}
