#include "BaseBullet.h"
#include "GlobalVariables.h"
#include "ImGuiManager.h"

void BaseBullet::Initialize(
    const std::vector<Model*>& models, const Vector3& position) {
	assert(models_.size() != 1);

	models_ = models;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {0.1f, 0.1f, 0.1f};
}

void BaseBullet::Update() {
	//ImGui::Begin("bullet");
	//ImGui::Text("%d", deathTimer_);
	//ImGui::End();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	worldTransform_.translation_ = worldTransform_.translation_ + velosity_;

	worldTransform_.UpdateMatrix();
}

void BaseBullet::Draw(const ViewProjection& viewProjection) {
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
 	}
}

void BaseBullet::OnCollision() { isDead_ = true; }

void BaseBullet::ApplyGlobalVariavles() {}

void BaseBullet::InitializeGlobalVariavles() {}

Vector3 BaseBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

