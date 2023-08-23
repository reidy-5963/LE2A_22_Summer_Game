#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class PlayerBullet {
public:
	void Initialize(
	    const std::vector<Model*>& models, const Vector3& position, const Vector3& velosity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void OnCollision();

	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	std::vector<Model*> models_;
	uint32_t textureHandle_ = 0u;

	Vector3 velosity_;

	static const int kLifeTime = 60 * 5;

	int deathTimer_ = kLifeTime;
	
	bool isDead_ = false;
};