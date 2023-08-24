#include "FollowCamera.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "input/Input.h"

void FollowCamera::Initialize() {
	// ビュープロジェクションの初期化処理
	viewProjection_.Initialize();

	// カーソルの位置を調整
	SetCursorPos(int(WinApp::kWindowWidth / 2), int(WinApp::kWindowHeight / 2));
}

void FollowCamera::Update() {
	// コントローラー
	XINPUT_STATE joyState;

	// もしコントローラーなら
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//
		float speed = 0.2f;

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * speed;
		viewProjection_.rotation_.x -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * speed;

		if (viewProjection_.rotation_.x > 1.0f) {
			viewProjection_.rotation_.x = 1.0f;
		}
		if (viewProjection_.rotation_.x < -1.0f) {
			viewProjection_.rotation_.x = -1.0f;
		}

	} 
	// キーマウなら
	else {
		// 現在のカーソル位置を
		GetCursorPos(&mousePos_);

		// 速度
		float speed = 0.02f;

		// 初期位置から動かした距離を求める
		xCameraRotate_ = float(mousePos_.x) - float(WinApp::kWindowWidth / 2);
		yCameraRotate_ = float(mousePos_.y) - float(WinApp::kWindowHeight / 2);

		// 
		Vector2 mouseDis;
		mouseDis.x = xCameraRotate_ + float(preMousePos_.x);
		mouseDis.y = yCameraRotate_ + float(preMousePos_.y);

		// 正規化して速度分をかける
		mouseDis = MyMath::Normalize(mouseDis);
		viewProjection_.rotation_.y += mouseDis.x * speed;
		viewProjection_.rotation_.x += mouseDis.y * speed;

		// カーソルを固定
		SetCursorPos(int(WinApp::kWindowWidth / 2), int(WinApp::kWindowHeight / 2));

		// 視点を動かすなら
		if (xCameraRotate_ > 0.0f) {
			if (preMousePos_.x < 0.0f) {
				preMousePos_.x = LONG(0.0f);
			} 

			// 移動分を加算し続ける
			preMousePos_.x += LONG(xCameraRotate_);
		} 
		else if (xCameraRotate_ < 0.0f) {
			if (preMousePos_.x > 0.0f) {
				preMousePos_.x = LONG(0.0f);
			} 

			// 移動分を加算し続ける
			preMousePos_.x += LONG(xCameraRotate_);
		} 
		// もし視点を動かさないなら
		else if (xCameraRotate_ == 0.0f) {
			// 移動分は0
			preMousePos_.x = 0;
		}

		// 視点を動かすなら
		if (yCameraRotate_ > 0.0f) {
			if (preMousePos_.x < 0.0f) {
				preMousePos_.x = LONG(0.0f);
			} 

			// 移動分を加算し続ける
			preMousePos_.y += LONG(yCameraRotate_);
		}
		else if (yCameraRotate_ < 0.0f) {
			if (preMousePos_.x > 0.0f) {
				preMousePos_.x = LONG(0.0f);
			}

			// 移動分を加算し続ける
			preMousePos_.y += LONG(yCameraRotate_);
		}
		// もし視点を動かさないなら
		else if (yCameraRotate_ == 0.0f) {
			// 移動分は0
			preMousePos_.y = 0;
		}

		// 視点を上、もしくは下に動かしたときの上限
		if (viewProjection_.rotation_.x > 1.0f) {
			viewProjection_.rotation_.x = 1.0f;
		}
		if (viewProjection_.rotation_.x < -1.0f) {
			viewProjection_.rotation_.x = -1.0f;
		}

		ImGui::Begin("follow");
		ImGui::Text("mousePos_.x %f, mousePos_.y %f", (float)mousePos_.x, (float)mousePos_.y);
		ImGui::Text(
		    "preMousePos_.x %f, preMousePos_.y %f", (float)preMousePos_.x, (float)preMousePos_.y);

		ImGui::DragFloat("xdistance", &xCameraRotate_, 0.0f, 2.0f);
		ImGui::DragFloat("ydistance", &yCameraRotate_, 0.0f, 2.0f);

		ImGui::DragFloat2("distance", &mouseDis.x, 0.0f, 2.0f);

		ImGui::DragFloat3("rotation", &viewProjection_.rotation_.x, 0.0f, 30.0f);
		ImGui::End();
	}
	
	// もしターゲットがいるなら
	if (target_) {
		// ターゲットとカメラの距離
		Vector3 offset = {0.0f, 5.0f, 0.0f};

		// 回転行列の生成
		Matrix4x4 rotateMat = MyMath::Multiply(
		    MyMath::Multiply(
		        MyMath::MakeRotateXMatrix(viewProjection_.rotation_.x),
		        MyMath::MakeRotateYMatrix(viewProjection_.rotation_.y)),
		    MyMath::MakeRotateZMatrix(viewProjection_.rotation_.z));

		// 回転行列でどの方向を見ても同じ距離になるよう調整
		offset = MyMath::TransformNormal(offset, rotateMat);

		// ターゲットから調整した距離分を加算
		viewProjection_.translation_ = MyMath::Add(target_->translation_, offset);
	}

	// ビュープロジェクションの更新と転送
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}

void FollowCamera::SetTarget(const WorldTransform* target) {
	// 引数のターゲットに設定
	target_ = target;
}

ViewProjection& FollowCamera::GetViewProjection() { return viewProjection_; }
