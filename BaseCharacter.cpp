#include "BaseCharacter.h"

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="models">使うモデルの動的配列</param>
void BaseCharacter::Initialize(const std::vector<Model*>& models) {
	// モデルデータ配列とワールド変換データの初期化
	models_ = models;
	worldTransform_.Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void BaseCharacter::Update() {
	// もしhpが0以下になったらフラグを有効にする
	if (hp_ <= 0.0f) {
		isDead_ = true;
	}

	// ワールド変換データの更新処理
	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void BaseCharacter::Draw(const ViewProjection& viewProjection) {
	// 配列内の全モデルの描画処理
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}

///// <summary>
///// もし当たっていたらの処理
///// </summary>
//void BaseCharacter::OnCollision(float damage) { damage; }

/// <summary>
/// グローバル変数の初期化処理
/// </summary>
void BaseCharacter::InitializeGlobalVariavles() {}

/// <summary>
///  グローバル変数の取得
/// </summary>
void BaseCharacter::ApplyGlobalVariavles() {}

/// <summary>
/// 浮遊ギミック更新
/// </summary>
void BaseCharacter::UpdateFloatingGimmick() {}

/// <summary>
/// 腕ふりギミック更新
/// </summary>
void BaseCharacter::UpdateArmGimmick() {}

/// <summary>
/// 攻撃を食らったときの動き
/// </summary>
void BaseCharacter::DamageGimmick() {}

/// <summary>
/// ワールド座標の取得
/// </summary>
/// <returns>ワールド座標</returns>
Vector3 BaseCharacter::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
