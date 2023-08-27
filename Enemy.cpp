#include "Enemy.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include <cassert>
#include "Player.h"
#include "GlobalVariables.h"
#include "GameScene.h"

/// <summary>
/// 
/// </summary>
Enemy::Enemy() {}

/// <summary>
/// 
/// </summary>
Enemy::~Enemy() {}

/// <summary>
/// 
/// </summary>
/// <param name="models"></param>
void Enemy::Initialize(const std::vector<Model*>& models, const Vector3 position) {
	
	// ベース部分の初期化処理
	BaseCharacter::Initialize(models);

	worldTransform_.translation_ = position;


	// 各部位のワールド変換データの初期化処理
	for (int i = 0; i < kModelNo; i++) {
		worldTransform_parts_[i].Initialize();
	}

	// 各部位の位置調整
	worldTransform_parts_[Body].parent_ = &worldTransform_;

	worldTransform_parts_[Head].parent_ = &worldTransform_parts_[Body];

	worldTransform_parts_[L_Arm].parent_ = &worldTransform_parts_[Body];

	worldTransform_parts_[R_Arm].parent_ = &worldTransform_parts_[Body];



	InitializeGlobalVariavles();
}

/// <summary>
/// 
/// </summary>
void Enemy::Update() {
	ApplyGlobalVariavles();

	ImGui::Begin("Enemy Hp");
	ImGui::Text("hp : %f", hp_);
	ImGui::End();

	if (hp_ < 0.0f) {
		isDead_ = true;
	}


	DamageGimmick();

	if (!isDamage_) {
		UpdateFloatingGimmick();
		RootUpdate();
		Attack();
	}

	// ベース部分の更新処理
	BaseCharacter::Update();
	// 各部位のワールド変換データ更新処理
	for (int i = 0; i < kModelNo; i++) {
		worldTransform_parts_[i].UpdateMatrix();
	}
}

/// <summary>
///
/// </summary>
/// <param name="viewProjection"></param>
void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	models_[Body]->Draw(worldTransform_parts_[Body], viewProjection);
	models_[Head]->Draw(worldTransform_parts_[Head], viewProjection);
	models_[L_Arm]->Draw(worldTransform_parts_[L_Arm], viewProjection);
	models_[R_Arm]->Draw(worldTransform_parts_[R_Arm], viewProjection);
}

/// <summary>
///
/// </summary>
void Enemy::OnCollision(float damage) {
	if (!isDamage_) {
		hp_ -= damage;
	}
	isDamage_ = true;
}

/// <summary>
///
/// </summary>
void Enemy::ApplyGlobalVariavles() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Enemy";
	// 作ったグループにあるアイテムから値を取得
	setHp_ = gloVars->GetFloatValue(groupName, "HP");
	damage_ = gloVars->GetFloatValue(groupName, "Damage");

	worldTransform_parts_[Body].translation_ =
	    gloVars->GetVector3Value(groupName, "Body Translation");
	worldTransform_parts_[Head].translation_ =
	    gloVars->GetVector3Value(groupName, "Head Translation");
	worldTransform_parts_[L_Arm].translation_ =
	    gloVars->GetVector3Value(groupName, "ArmL Translation");
	worldTransform_parts_[R_Arm].translation_ =
	    gloVars->GetVector3Value(groupName, "ArmR Translation");

	floatingPeriod_ = gloVars->GetFloatValue(groupName, "floatingPeriod_");
	floatingAmplitude_ = gloVars->GetFloatValue(groupName, "floatingAmplitude_");

	damagingPeriod_ = gloVars->GetFloatValue(groupName, "damagingPeriod_");
	damagingAmplitude_ = gloVars->GetFloatValue(groupName, "damagingAmplitude_");

	bulletTime = gloVars->GetIntValue(groupName, "bulletTime");
	bulletSpeed = gloVars->GetFloatValue(groupName, "bulletSpeed");
	moveSpeed = gloVars->GetFloatValue(groupName, "moveSpeed");
	radius_ = gloVars->GetFloatValue(groupName, "radius");
}

/// <summary>
/// 
/// </summary>
void Enemy::InitializeGlobalVariavles() {
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";
	//
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "HP", setHp_);
	gloVars->AddItem(groupName, "Damage", damage_);

	gloVars->AddItem(groupName, "Body Translation", worldTransform_parts_[Body].translation_);
	gloVars->AddItem(groupName, "Head Translation", worldTransform_parts_[Head].translation_);
	gloVars->AddItem(groupName, "ArmL Translation", worldTransform_parts_[L_Arm].translation_);
	gloVars->AddItem(groupName, "ArmR Translation", worldTransform_parts_[R_Arm].translation_);

	gloVars->AddItem(groupName, "floatingPeriod_", floatingPeriod_);
	gloVars->AddItem(groupName, "floatingAmplitude_", floatingAmplitude_);

	gloVars->AddItem(groupName, "damagingPeriod_", damagingPeriod_);
	gloVars->AddItem(groupName, "damagingAmplitude_", damagingAmplitude_);

	gloVars->AddItem(groupName, "moveSpeed", moveSpeed);
	gloVars->AddItem(groupName, "bulletSpeed", bulletSpeed);
	gloVars->AddItem(groupName, "bulletTime", bulletTime);
	gloVars->AddItem(groupName, "radius", radius_);
}

/// <summary>
///
/// </summary>
void Enemy::UpdateFloatingGimmick() {
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * 3.14f / floatingPeriod_;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを越えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * 3.14f);

	// 浮遊を座標に反映
	worldTransform_parts_[Body].translation_.y = std::sin(floatingParameter_) * floatingAmplitude_;
}

/// <summary>
/// 
/// </summary>
void Enemy::UpdateArmGimmick() {}

/// <summary>
/// 
/// </summary>
void Enemy::DamageGimmick() {
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * 3.14f / damagingPeriod_;

	if (isDamage_) {

		// パラメータを1ステップ分加算
		damagingParameter_ += step;
		// 2πを越えたら0に戻す
		damagingParameter_ = std::fmod(damagingParameter_, 2.0f * 3.14f);

		// 腕の動きをワールド変換データに反映
		worldTransform_parts_[Body].rotation_.y = damagingParameter_ * damagingAmplitude_;

		if (damagingParameter_ >= 2.0f * 3.14f - (2.0f * 3.14f / damagingPeriod_)) {

			worldTransform_parts_[Body].rotation_.y = 0;
			isDamage_ = false;
		}
	}
}

/// <summary>
/// 
/// </summary>
void Enemy::RootUpdate() {
	// 敵->プレイヤーのベクトル
	Vector3 distance = {0.0f, 0.0f, 0.0f};

	// 敵->プレイヤーのベクトルを計算
	distance.x = player_->GetWorldPosition().x - worldTransform_.matWorld_.m[3][0];
	distance.y = player_->GetWorldPosition().y - worldTransform_.matWorld_.m[3][1];
	distance.z = player_->GetWorldPosition().z - worldTransform_.matWorld_.m[3][2];

	distance = MyMath::Normalize(distance);

	distance.x *= moveSpeed;
	distance.y *= moveSpeed;
	distance.z *= moveSpeed;

	// 敵の向きをプレイヤーの方向に変更
	worldTransform_.rotation_.y = std::atan2(distance.x, distance.z);

	Matrix4x4 moveMat = MyMath::MakeTranslateMatrix(worldTransform_.translation_);
	worldTransform_.translation_ = MyMath::TransformCoord(distance, moveMat);
}

/// <summary>
/// 
/// </summary>
void Enemy::Attack() {
	if (timer_ < 0) {
		isAttack_ = true;
		timer_ = bulletTime;
	} else if (timer_ >= 0) {
		isAttack_ = false;
		timer_--;
	}

	if (isAttack_) {
		// 敵->プレイヤーのベクトル
		Vector3 distance = {0.0f, 0.0f, 0.0f};

		// 敵->プレイヤーのベクトルを計算
		distance.x = player_->GetWorldPosition().x - worldTransform_.matWorld_.m[3][0];
		distance.y = player_->GetWorldPosition().y - worldTransform_.matWorld_.m[3][1];
		distance.z = player_->GetWorldPosition().z - worldTransform_.matWorld_.m[3][2];

		distance = MyMath::Multiply(bulletSpeed, MyMath::Normalize(distance));

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(bulletModels_, GetWorldPosition(), distance);

		gameScene_->AddEnemyBullet(newBullet);
		//bullets_.push_back(newBullet);
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="velosity"></param>
void Enemy::SetPosition(Vector3 velosity) {
	worldTransform_.translation_ = worldTransform_.translation_ + velosity;
}