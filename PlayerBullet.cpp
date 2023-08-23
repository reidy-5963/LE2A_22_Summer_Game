#include "PlayerBullet.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include <cassert>

void PlayerBullet::Initialize(
    const std::vector<Model*>& models, const Vector3& position, const Vector3& velosity) {

    models_ = models;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {0.1f, 0.1f, 0.1f};

	velosity_ = velosity;
	worldTransform_.translation_.y = 5.0f;
}

void PlayerBullet::Update() {
	if (deathTimer_ <= 0) {
		isDead_ = true;
	}
	worldTransform_.translation_ = worldTransform_.translation_ + velosity_;

	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}


void PlayerBullet::OnCollision() { isDead_ = true; }

Vector3 PlayerBullet::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
