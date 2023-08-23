#include "Enemy.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include <cassert>

void Enemy::Initialize(const std::vector<Model*>& models) {
	
	// 
	BaseCharacter::Initialize(models);

	for (int i = 0; i < 3; i++) {
		worldTransform_parts_[i].Initialize();
	}

	worldTransform_parts_[Body].parent_ = &worldTransform_;
	worldTransform_parts_[L_Wepon].parent_ = &worldTransform_;
	worldTransform_parts_[L_Wepon].translation_.y += 1.5f;
	worldTransform_parts_[L_Wepon].translation_.x -= 1.5f;
	worldTransform_parts_[R_Wepon].parent_ = &worldTransform_;
	worldTransform_parts_[R_Wepon].translation_.y += 1.5f;
	worldTransform_parts_[R_Wepon].translation_.x += 1.5f;


}

void Enemy::Update() {


	BaseCharacter::Update();
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
