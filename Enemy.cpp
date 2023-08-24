#include "Enemy.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include <cassert>
#include "Player.h"

void Enemy::Initialize(const std::vector<Model*>& models) {
	
	// ベース部分の初期化処理
	BaseCharacter::Initialize(models);

	// 各部位のワールド変換データの初期化処理
	for (int i = 0; i < 3; i++) {
		worldTransform_parts_[i].Initialize();
	}

	// 各部位の位置調整
	worldTransform_parts_[Body].parent_ = &worldTransform_;
	worldTransform_parts_[L_Wepon].parent_ = &worldTransform_;
	worldTransform_parts_[L_Wepon].translation_.y += 1.5f;
	worldTransform_parts_[L_Wepon].translation_.x -= 1.5f;
	worldTransform_parts_[R_Wepon].parent_ = &worldTransform_;
	worldTransform_parts_[R_Wepon].translation_.y += 1.5f;
	worldTransform_parts_[R_Wepon].translation_.x += 1.5f;


}

void Enemy::Update() {
	// 敵->プレイヤーのベクトル
	Vector3 distance = {0.0f, 0.0f, 0.0f};

	const float speed = 0.04f;

	// 敵->プレイヤーのベクトルを計算
	distance.x = player_->GetWorldPosition().x - worldTransform_.matWorld_.m[3][0];
	distance.y = player_->GetWorldPosition().y - worldTransform_.matWorld_.m[3][1];
	distance.z = player_->GetWorldPosition().z - worldTransform_.matWorld_.m[3][2];

	distance = MyMath::Normalize(distance);


	distance.x *= speed;
	distance.y *= speed;
	distance.z *= speed;


	// 敵の向きをプレイヤーの方向に変更
	worldTransform_.rotation_.y = std::atan2(distance.x, distance.z);

	Matrix4x4 moveMat = MyMath::MakeTranslateMatrix(worldTransform_.translation_);
	worldTransform_.translation_ = MyMath::TransformCoord(distance, moveMat);



	// ベース部分の更新処理
	BaseCharacter::Update();
	// 各部位の更新処理
	worldTransform_parts_[Body].UpdateMatrix();
	worldTransform_parts_[L_Wepon].UpdateMatrix();
	worldTransform_parts_[R_Wepon].UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	// モデルの描画
	models_[Body]->Draw(worldTransform_parts_[Body], viewProjection);
	//	models_[Head]->Draw(worldTransform_head_, viewProjection);
	models_[L_Wepon]->Draw(worldTransform_parts_[L_Wepon], viewProjection);
	models_[R_Wepon]->Draw(worldTransform_parts_[R_Wepon], viewProjection);
}

void Enemy::SetPosition(Vector3 velosity) {
	worldTransform_.translation_ = worldTransform_.translation_ + velosity;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];


	return worldPos;
}
