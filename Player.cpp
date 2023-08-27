#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include <cassert>
#include "WinApp.h"
#include "GlobalVariables.h"
#include <time.h>



/// <summary>
/// 
/// </summary>
Player::Player() {}

/// <summary>
/// 
/// </summary>
Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

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
	worldTransform_3dReticle_.Initialize();

	// 向きの初期化
	worldTransform_.rotation_.y =
	    std::atan2(worldTransform_.rotation_.x, worldTransform_.rotation_.z);

	// レティクルの初期化
	sprite_2dReticle_.reset(Sprite::Create(
	    texReticle_, 
		{float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f},
		{0.5f, 0.5f}));

	// 体力表示の初期化
	p_heart_.reset(Sprite::Create(heartTex_, {0.0f, 0.0f}));

	InitializeGlobalVariavles();
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() { 
	ApplyGlobalVariavles();
	
	ImGui::Begin("Player Hp");
	ImGui::Text("hp : %f", hp_);
	ImGui::End();
	
	if (hp_ < 0.0f) {
		isDead_ = true;
	}

	// レティクル表示の更新処理
	ReticleUpdate();

	// 通常状態の更新処理
	RootUpdate();

	// ベース部分の更新処理
	BaseCharacter::Update(); 

	// 各部位のワールド変換データ更新処理
	for (int i = 0; i < kModelNo; i++) {
		worldTransform_parts_[i].UpdateMatrix();
	}

	// 弾が有効かどうか
	BulletIsDead();

	// 攻撃処理
	Attack();

	// 弾の更新処理
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="viewProjection">ViewProjection</param>
void Player::Draw(const ViewProjection& viewProjection) {
	// ベース部分の描画処理
	// BaseCharacter::Draw(viewProjection);
	// モデルの描画
	models_[Body]->Draw(worldTransform_parts_[Body], viewProjection);
	//models_[Head]->Draw(worldTransform_parts_[Head], viewProjection);
	models_[L_Arm]->Draw(worldTransform_parts_[L_Arm], viewProjection);
	models_[R_Arm]->Draw(worldTransform_parts_[R_Arm], viewProjection);

	// 弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

/// <summary>
///
/// </summary>
void Player::OnCollision(float damage) {
	if (!isDamage_) {
		hp_ -= damage;
	}
	isDamage_ = true;
}

/// <summary>
///
/// </summary>
void Player::ApplyGlobalVariavles() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Player";
	// 作ったグループにあるアイテムから値を取得
	setHp_ = gloVars->GetFloatValue(groupName, "HP");

	worldTransform_parts_[Body].translation_ =
	    gloVars->GetVector3Value(groupName, "Body Translation");
	worldTransform_parts_[Head].translation_ =
	    gloVars->GetVector3Value(groupName, "Head Translation");
	worldTransform_parts_[L_Arm].translation_ =
	    gloVars->GetVector3Value(groupName, "ArmL Translation");
	worldTransform_parts_[R_Arm].translation_ =
	    gloVars->GetVector3Value(groupName, "ArmR Translation");

	worldTransform_parts_[L_Arm].rotation_ = gloVars->GetVector3Value(groupName, "ArmL Rotation");
	worldTransform_parts_[R_Arm].rotation_ = gloVars->GetVector3Value(groupName, "ArmR Rotation");

	floatingPeriod_ = gloVars->GetFloatValue(groupName, "floatingPeriod_");
	floatingAmplitude_ = gloVars->GetFloatValue(groupName, "floatingAmplitude_");

	armPeriod_ = gloVars->GetFloatValue(groupName, "armPeriod_");
	armAmplitude_ = gloVars->GetFloatValue(groupName, "armAmplitude_");

	bulletTime = gloVars->GetIntValue(groupName, "bulletTime");

	moveSpeed = gloVars->GetFloatValue(groupName, "moveSpeed");
	bulletSpeed = gloVars->GetFloatValue(groupName, "bulletSpeed");
	radius_ = gloVars->GetFloatValue(groupName, "radius");
}

/// <summary>
///
/// </summary>
void Player::InitializeGlobalVariavles() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Player";

	// 作ったグループにそれぞれアイテムを追加
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "HP", setHp_);

	gloVars->AddItem(groupName, "Body Translation", worldTransform_parts_[Body].translation_);
	gloVars->AddItem(groupName, "Head Translation", worldTransform_parts_[Head].translation_);
	gloVars->AddItem(groupName, "ArmL Translation", worldTransform_parts_[L_Arm].translation_);
	gloVars->AddItem(groupName, "ArmR Translation", worldTransform_parts_[R_Arm].translation_);

	gloVars->AddItem(groupName, "ArmL Rotation", worldTransform_parts_[L_Arm].rotation_);
	gloVars->AddItem(groupName, "ArmR Rotation", worldTransform_parts_[R_Arm].rotation_);

	gloVars->AddItem(groupName, "floatingPeriod_", floatingPeriod_);
	gloVars->AddItem(groupName, "floatingAmplitude_", floatingAmplitude_);

	gloVars->AddItem(groupName, "armPeriod_", armPeriod_);
	gloVars->AddItem(groupName, "armAmplitude_", armAmplitude_);

	gloVars->AddItem(groupName, "moveSpeed", moveSpeed);
	gloVars->AddItem(groupName, "bulletSpeed", bulletSpeed);
	gloVars->AddItem(groupName, "bulletTime", bulletTime);
	gloVars->AddItem(groupName, "radius", radius_);
}

/// <summary>
///
/// </summary>
void Player::UpdateFloatingGimmick() {
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
void Player::UpdateArmGimmick() {
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * 3.14f / armPeriod_;

	// パラメータを1ステップ分加算
	armParameter_ += step;
	// 2πを越えたら0に戻す
	armParameter_ = std::fmod(armParameter_, 2.0f * 3.14f);

	// 腕の動きをワールド変換データに反映
	worldTransform_parts_[L_Arm].rotation_.x = std::sin(armParameter_) * armAmplitude_;
	worldTransform_parts_[R_Arm].rotation_.x = std::sin(armParameter_) * armAmplitude_;
}

/// <summary>
/// 
/// </summary>
void Player::DamageGimmick() {
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
void Player::ModelInitialize() {
	// worldTransform_body_.Initialize();
	// worldTransform_head_.Initialize();
	// worldTransform_l_arm_.Initialize();
	// worldTransform_r_arm_.Initialize();
	// worldTransform_wepon_.Initialize();
	for (int i = 0; i < kModelNo; i++) {
		worldTransform_parts_[i].Initialize();
	}

	worldTransform_parts_[Body].parent_ = &worldTransform_;

	worldTransform_parts_[Head].parent_ = &worldTransform_parts_[Body];

	worldTransform_parts_[L_Arm].parent_ = &worldTransform_parts_[Body];

	worldTransform_parts_[R_Arm].parent_ = &worldTransform_parts_[Body];

	worldTransform_parts_[Wepon].parent_ = &worldTransform_;

	// worldTransform_body_.parent_ = &worldTransform_;
	// worldTransform_head_.parent_ = &worldTransform_;
	// worldTransform_head_.translation_.y += 4.3f;
	// worldTransform_l_arm_.parent_ = &worldTransform_body_;
	// worldTransform_l_arm_.translation_.y += 4.0f;
	// worldTransform_l_arm_.translation_.x -= 1.0f;
	// worldTransform_r_arm_.parent_ = &worldTransform_body_;
	// worldTransform_r_arm_.translation_.y += 4.0f;
	// worldTransform_r_arm_.translation_.x += 1.0f;
	// worldTransform_wepon_.parent_ = &worldTransform_;
}

/// <summary>
/// 通常状態の更新処理
/// </summary>
void Player::RootUpdate() {
	UpdateFloatingGimmick();
	DamageGimmick();
	//UpdateArmGimmick();

	// 平行移動成分の行列
	Matrix4x4 matTranslate = MyMath::MakeTranslateMatrix(worldTransform_.translation_);
	// 移動量
	Vector3 move = {0.0f, 0.0f, 0.0f};
	Vector3 direction = {0.0f, 0.0f, 1.0f};
	// コントローラー
	XINPUT_STATE joyState;
	// 速度

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
	worldTransform_parts_[Body].rotation_.y = std::atan2(direction.x, direction.z);




	// コントローラーの場合
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 左スティックから移動量を設定
		move = {(float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY};

	/*	if (!isJump_) {
			worldTransform_.translation_.y--;

			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_B) {
				isJump_ = true;
			}
		} else if (isJump_) {
			if (worldTransform_.translation_.y < 10.0f) {
				worldTransform_.translation_.y += 1.0f;
			} else if (worldTransform_.translation_.y >= 10.0f) {
				isJump_ = false;
			}
		}*/
		if (worldTransform_.translation_.y < 0) {
			worldTransform_.translation_.y = 0;
		}

	}
	// キーマウ
	else {


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

		/*if (!isJump_) {
			worldTransform_.translation_.y--;

			if (input_->TriggerKey(DIK_SPACE)) {
				isJump_ = true;
			}
		}
		else if (isJump_) {
			if (worldTransform_.translation_.y < 10.0f) {
				worldTransform_.translation_.y += 1.1f;
			} else if (worldTransform_.translation_.y >= 10.0f) {
				isJump_ = false;
			}
		}*/
		if (worldTransform_.translation_.y < 0) {
			worldTransform_.translation_.y = 0;
		}


	}

	ImGui::Begin("jump");
	ImGui::Checkbox("isJump", &isJump_);
	ImGui::DragFloat("trans", &worldTransform_.translation_.y, 0.0f, 120.0f);

	ImGui::End();


	// 移動量の計算
	move = MyMath::Normalize(move);
	move.x *= moveSpeed;
	move.y *= moveSpeed;
	move.z *= moveSpeed;


	// 移動量から平行移動行列を生成
	Matrix4x4 moveMat = MyMath::MakeTranslateMatrix(move);


	// 二つの行列を合成
	moveMat = MyMath::Multiply(moveMat, rotateMat);
	// 行列の平行移動成分のみ取り出す
	move.x = moveMat.m[3][0];
	//move.y = 0.0f;
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
/// 攻撃処理
/// </summary>
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
				timer_ = bulletTime;
			} else if (timer_ >= 0) {
				isAttack_ = false;
				timer_--;
			}
		} else {
			isAttack_ = false;
			timer_ = 0;
		}

	} else {
		if (input_->IsPressMouse(0)) {
			if (timer_ < 0) {
				isAttack_ = true;
				timer_ = bulletTime;
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
		Vector3 headTrans;
		Vector3 reticle3DPos;
		headTrans.x = worldTransform_parts_[Head].matWorld_.m[3][0];
		headTrans.y = worldTransform_parts_[Head].matWorld_.m[3][1];
		headTrans.z = worldTransform_parts_[Head].matWorld_.m[3][2];
		reticle3DPos.x = worldTransform_3dReticle_.matWorld_.m[3][0];
		reticle3DPos.y = worldTransform_3dReticle_.matWorld_.m[3][1];
		reticle3DPos.z = worldTransform_3dReticle_.matWorld_.m[3][2];

		Vector3 velosity = {};

		velosity = reticle3DPos - headTrans;
		velosity = MyMath::Multiply(bulletSpeed, MyMath::Normalize(velosity));

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletModels_, GetWorldPosition(), velosity);

		bullets_.push_back(newBullet);
	}
}

/// <summary>
/// レティクルの更新処理
/// </summary>
void Player::ReticleUpdate() {
	/*
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {

	}
	else {
	    POINT mousePos_;


	    GetCursorPos(&mousePos_);

	    HWND hwnd = WinApp::GetInstance()->GetHwnd();
	    ScreenToClient(hwnd, &mousePos_);
	}*/

	Matrix4x4 viewPortMat = MyMath::MakeViewPortMatrix(
	    0, 0, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);

	Matrix4x4 matVPV = MyMath::Multiply(
	    viewProjection_->matView, MyMath::Multiply(viewProjection_->matProjection, viewPortMat));

	Matrix4x4 matInverseVPV = MyMath::Inverse(matVPV);

	Vector3 posNear =
	    Vector3(sprite_2dReticle_->GetPosition().x, sprite_2dReticle_->GetPosition().y, 0);
	Vector3 posFar =
	    Vector3(sprite_2dReticle_->GetPosition().x, sprite_2dReticle_->GetPosition().y, 1);

	posNear = MyMath::TransformCoord(posNear, matInverseVPV);
	posFar = MyMath::TransformCoord(posFar, matInverseVPV);

	Vector3 direction = posFar - posNear;
	direction = MyMath::Normalize(direction);

	const float kDistanceObject = 100.0f;
	worldTransform_3dReticle_.translation_ = MyMath::Multiply(kDistanceObject, direction) + posNear;

	worldTransform_3dReticle_.UpdateMatrix();
}

/// <summary>
///
/// </summary>
void Player::BulletIsDead() {
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}

/// <summary>
/// 
/// </summary>
void Player::DrawUI() { 
	//
	sprite_2dReticle_->Draw();
	p_heart_->Draw();
}

/// <summary>
/// ViewProjectionを設定
/// </summary>
/// <param name="viewProjection">ViewProjection</param>
void Player::SetViewProjection(const ViewProjection* viewProjection) {
	// 引数のviewProjectionを設定
	viewProjection_ = viewProjection;
}












