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

/// <summary>
/// もし当たっていたらの処理
/// </summary>
void BaseCharacter::OnCollision(float damage) { damage; }

/// <summary>
///  グローバル変数の取得
/// </summary>
void BaseCharacter::ApplyGlobalVariavles() {}

void BaseCharacter::InitializeGlobalVariavles() {}

void BaseCharacter::UpdateFloatingGimmick() {}

void BaseCharacter::UpdateArmGimmick() {}

void BaseCharacter::DamageGimmick() {}

