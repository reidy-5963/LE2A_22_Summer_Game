#include "FollowCamera.h"
#include "MyMath.h"
#include "input/Input.h"

void FollowCamera::Initialize() {
	//
	viewProjection_.Initialize();

	preMousePos_ = mousePos_;
}

void FollowCamera::Update() {
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//
		float speed = 0.2f;

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * speed;
		viewProjection_.rotation_.x += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * speed;

	}
	else {
		GetCursorPos(&mousePos_);

		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePos_);

		float speed = 0.003f;

		float xMouseDistance = float(mousePos_.x) - float(preMousePos_.x);
		float yMouseDistance = float(mousePos_.y) - float(preMousePos_.y);

		xCameraRotate_ = xMouseDistance;
		yCameraRotate_ = yMouseDistance;

		viewProjection_.rotation_.y += xCameraRotate_ * speed;
		viewProjection_.rotation_.x += yCameraRotate_ * speed;

		preMousePos_ = mousePos_;
		//SetCursorPos(int(WinApp::kWindowWidth / 2), int(WinApp::kWindowHeight / 2));

	}

	if (target_) {
		//
		Vector3 offset = {0.0f, 5.0f, 0.0f};

		Matrix4x4 rotateMat = MyMath::Multiply(
		    MyMath::Multiply(
		        MyMath::MakeRotateXMatrix(viewProjection_.rotation_.x),
		        MyMath::MakeRotateYMatrix(viewProjection_.rotation_.y)),
		    MyMath::MakeRotateZMatrix(viewProjection_.rotation_.z));

		offset = MyMath::TransformNormal(offset, rotateMat);

		//
		viewProjection_.translation_ = MyMath::Add(target_->translation_, offset);	
	}
	
	// 
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}

void FollowCamera::SetTarget(const WorldTransform* target) {
	//
	target_ = target;
}

ViewProjection& FollowCamera::GetViewProjection() {
	return viewProjection_;
}
