#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include <cassert>

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="models">モデルデータ</param>
void Player::Initialize(const std::vector<Model*>& models) { 
	
	// 入力系の初期化
	input_ = Input::GetInstance();

	// ベース部分の初期化
	BaseCharacter::Initialize(models);
	
	// 各部位の初期化と位置の調整
	ModelInitialize();
	
	// 向きの初期化
	worldTransform_.rotation_.y =
	    std::atan2(worldTransform_.rotation_.x, worldTransform_.rotation_.z);

}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() { 
	// 通常状態の更新処理
	RootUpdate();
	// ベース部分の更新処理
	BaseCharacter::Update(); 

	

	// 各部位のワールド変換データ更新処理
	//worldTransform_body_.UpdateMatrix();
	//worldTransform_head_.UpdateMatrix();
	//worldTransform_l_arm_.UpdateMatrix();
	//worldTransform_r_arm_.UpdateMatrix();
	//worldTransform_wepon_.UpdateMatrix();

	for (int i = 0; i < 5; i++) {
		worldTransform_parts_[i].UpdateMatrix();
	}

	///
	BulletIsDead();
	///
	Attack();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
}

/// <summary>
/// 通常状態の更新処理
/// </summary>
void Player::RootUpdate() {
	// 平行移動成分の行列
	Matrix4x4 matTranslate = MyMath::MakeTranslateMatrix(worldTransform_.translation_);
	// 移動量
	Vector3 move = {0.0f, 0.0f, 0.0f};
	Vector3 direction = {0.0f, 0.0f, 1.0f};
	// コントローラー
	XINPUT_STATE joyState;
	// 速度
	const float speed = 0.3f;

	// 追従カメラから回転行列を生成
	Matrix4x4 rotateMat = MyMath::Multiply(
	    MyMath::Multiply(
	        MyMath::MakeRotateXMatrix(viewProjection_->rotation_.x),
	        MyMath::MakeRotateYMatrix(viewProjection_->rotation_.y)),
	    MyMath::MakeRotateZMatrix(viewProjection_->rotation_.z));

	Matrix4x4 directMat = MyMath::MakeTranslateMatrix(direction);
	directMat = MyMath::Multiply(directMat, rotateMat);
	direction.x = directMat.m[3][0];
	direction.y = directMat.m[3][1];
	direction.z = directMat.m[3][2];
	// キャラの向きを変更
	//worldTransform_.rotation_.y = std::atan2(direction.x, direction.z);
	worldTransform_parts_[Head].rotation_.y = std::atan2(direction.x, direction.z);





	// コントローラーの場合
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (!isJump_) {
			worldTransform_.translation_.y--;

			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) {
				isJump_ = true;
			}
		} else if (isJump_) {
			if (worldTransform_.translation_.y < 10.0f) {
				worldTransform_.translation_.y += 1.0f;
			} else if (worldTransform_.translation_.y >= 10.0f) {
				isJump_ = false;
			}
		}
		if (worldTransform_.translation_.y < 0) {
			worldTransform_.translation_.y = 0;
		}


		// 左スティックから移動量を設定
		move = {(float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY};
	}
	// キーマウ
	else {
		if (!isJump_) {
			worldTransform_.translation_.y--;

			if (input_->TriggerKey(DIK_SPACE)) {
				isJump_ = true;
			}
		} else if (isJump_) {
			if (worldTransform_.translation_.y < 10.0f) {
				worldTransform_.translation_.y += 1.0f;
			} else if (worldTransform_.translation_.y >= 10.0f) {
				isJump_ = false;
			}
		}
		if (worldTransform_.translation_.y < 0) {
			worldTransform_.translation_.y = 0;
		}


		// wasdで移動量を設定
		if (input_->PushKey(DIK_W)) {
			move.z = 1.0f;
		}
		if (input_->PushKey(DIK_S)) {
			move.z = -1.0f;
		}
		if (input_->PushKey(DIK_A)) {
			move.x = -1.0f;
		}
		if (input_->PushKey(DIK_D)) {
			move.x = 1.0f;
		}
	}

	// 移動量の計算
	move = MyMath::Normalize(move);
	move.x *= speed;
	move.y *= speed;
	move.z *= speed;


	// 移動量から平行移動行列を生成
	Matrix4x4 moveMat = MyMath::MakeTranslateMatrix(move);


	// 二つの行列を合成
	moveMat = MyMath::Multiply(moveMat, rotateMat);
	// 行列の平行移動成分のみ取り出す
	move.x = moveMat.m[3][0];
	move.y = 0.0f;
	move.z = moveMat.m[3][2];

	if (input_->PushKey(DIK_W) || input_->PushKey(DIK_A) || input_->PushKey(DIK_S) ||
	    input_->PushKey(DIK_D)) {
		// キャラの向きを変更
		worldTransform_parts_[Body].rotation_.y = std::atan2(move.x, move.z);

	}

	// 移動させる
	worldTransform_.translation_ = MyMath::TransformCoord(move, matTranslate);

}


/// <summary>
/// 描画処理
/// </summary>
/// <param name="viewProjection">ViewProjection</param>
void Player::Draw(const ViewProjection& viewProjection) { 
	// ベース部分の描画処理
	//BaseCharacter::Draw(viewProjection);
	// モデルの描画
	models_[Body]->Draw(worldTransform_parts_[Body], viewProjection);
	//	models_[Head]->Draw(worldTransform_head_, viewProjection);
	models_[L_Arm]->Draw(worldTransform_parts_[L_Arm], viewProjection);
	models_[R_Arm]->Draw(worldTransform_parts_[R_Arm], viewProjection);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

/// <summary>
/// ViewProjectionを設定
/// </summary>
/// <param name="viewProjection">ViewProjection</param>
void Player::SetViewProjection(const ViewProjection* viewProjection) {
	// 引数のviewProjectionを設定
	viewProjection_ = viewProjection;
}

void Player::ModelInitialize() {
	//worldTransform_body_.Initialize();
	//worldTransform_head_.Initialize();
	//worldTransform_l_arm_.Initialize();
	//worldTransform_r_arm_.Initialize();
	//worldTransform_wepon_.Initialize();
	for (int i = 0; i < 5; i++) {
		worldTransform_parts_[i].Initialize();
	}


	worldTransform_parts_[Body].parent_ = &worldTransform_;

	worldTransform_parts_[Head].parent_ = &worldTransform_;
	worldTransform_parts_[Head].translation_.y += 4.3f;

	worldTransform_parts_[L_Arm].parent_ = &worldTransform_parts_[Body];
	worldTransform_parts_[L_Arm].translation_.y += 4.0f;
	worldTransform_parts_[L_Arm].translation_.x -= 1.0f;

	worldTransform_parts_[R_Arm].parent_ = &worldTransform_parts_[Body];
	worldTransform_parts_[R_Arm].translation_.y += 4.0f;
	worldTransform_parts_[R_Arm].translation_.x += 1.0f;

	worldTransform_parts_[Wepon].parent_ = &worldTransform_;

	//worldTransform_body_.parent_ = &worldTransform_;
	//worldTransform_head_.parent_ = &worldTransform_;
	//worldTransform_head_.translation_.y += 4.3f;
	//worldTransform_l_arm_.parent_ = &worldTransform_body_;
	//worldTransform_l_arm_.translation_.y += 4.0f;
	//worldTransform_l_arm_.translation_.x -= 1.0f;
	//worldTransform_r_arm_.parent_ = &worldTransform_body_;
	//worldTransform_r_arm_.translation_.y += 4.0f;
	//worldTransform_r_arm_.translation_.x += 1.0f;
	//worldTransform_wepon_.parent_ = &worldTransform_;
}

void Player::Attack() {
//
	ImGui::Begin("debug");
	ImGui::SliderInt("timer", &timer_, 0, 120);
	ImGui::End();

	XINPUT_STATE joystate;
	if (input_->GetJoystickState(0, joystate)) {
		if (joystate.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			if (timer_ < 0) {
				isAttack_ = true;
				timer_ = kBulletoffset;
			} else if (timer_ >= 0) {
				isAttack_ = false;
				timer_--;
			}
		} else {
			isAttack_ = false;
			timer_ = 0;
		}

	}
	else {
		if (input_->IsPressMouse(0)) {
			if (timer_ < 0) {
				isAttack_ = true;
				timer_ = kBulletoffset;
			} else if (timer_ >= 0) {
				isAttack_ = false;
				timer_--;
			}
		} else {
			isAttack_ = false;
			timer_ = 0;
		}
	}


	if (isAttack_) {
		const float kBulletSpeed = 1.0f;
		Vector3 velosity = {0.0f, 0.0f, kBulletSpeed};

		velosity = MyMath::TransformNormal(velosity, worldTransform_parts_[Head].matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletModels_, GetWorldPosition(), velosity);

		bullets_.push_back(newBullet);
	}
}

void Player::MauseUpdate() { 
	//
	//XINPUT_STATE joyState;



}
