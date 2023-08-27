#include "FollowCamera.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "input/Input.h"
#include "GlobalVariables.h"

/// <summary>
/// 初期化処理
/// </summary>
void FollowCamera::Initialize() {
	// ビュープロジェクションの初期化処理
	viewProjection_.Initialize();

	// カーソルの位置を調整
	SetCursorPos(int(WinApp::kWindowWidth / 2), int(WinApp::kWindowHeight / 2));

	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "FollowCamera";
	// 作ったグループにそれぞれアイテムを追加
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "movemauseSpeed", move_mouseSpeed);
	gloVars->AddItem(groupName, "movepadSpeed", move_padSpeed);
	gloVars->AddItem(groupName, "offset", offset_);
}

/// <summary>
/// 更新処理
/// </summary>
void FollowCamera::Update() {
	ApplyGlobalVariavles();

	// コントローラー
	XINPUT_STATE joyState;
	if (!isDamage_) {
		// もしコントローラーなら
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			// 左右の視点移動
			if ((float)joyState.Gamepad.sThumbRX / SHRT_MAX != 0.0f) {
				viewProjection_.rotation_.y +=
				    (float)joyState.Gamepad.sThumbRX / SHRT_MAX * move_padSpeed;
			}
			// 上下の視点移動
			if ((float)joyState.Gamepad.sThumbRY / SHRT_MAX != 0.0f) {
				viewProjection_.rotation_.x -=
				    (float)joyState.Gamepad.sThumbRY / SHRT_MAX * move_padSpeed;
			}
		}
		// キーマウなら
		else {
			// 現在のカーソル位置を
			GetCursorPos(&mousePos_);

			// 初期位置から動かした距離を求める
			xMouseDistance = float(mousePos_.x) - float(WinApp::kWindowWidth / 2);
			yMouseDistance = float(mousePos_.y) - float(WinApp::kWindowHeight / 2);

			// 左右に視点を動かすなら
			if (xMouseDistance != 0.0f) {
				viewProjection_.rotation_.y += xMouseDistance * move_mouseSpeed;
			}
			// 上下に視点を動かすなら
			if (yMouseDistance != 0.0f) {
				viewProjection_.rotation_.x += yMouseDistance * move_mouseSpeed;
			}

			// カーソルを固定
			SetCursorPos(int(WinApp::kWindowWidth / 2), int(WinApp::kWindowHeight / 2));
		}

	}
	if (isDamage_) {
	}
	
	// 視点を上、もしくは下に動かしたときの上限
	if (viewProjection_.rotation_.x > 1.0f) {
		viewProjection_.rotation_.x = 1.0f;
	}
	if (viewProjection_.rotation_.x < -1.0f) {
		viewProjection_.rotation_.x = -1.0f;
	}

	// もしターゲットがいるなら
	if (target_) {
		// ターゲットとカメラの距離

		// 回転行列の生成
		Matrix4x4 rotateMat = MyMath::Multiply(
		    MyMath::Multiply(
		        MyMath::MakeRotateXMatrix(viewProjection_.rotation_.x),
		        MyMath::MakeRotateYMatrix(viewProjection_.rotation_.y)),
		    MyMath::MakeRotateZMatrix(viewProjection_.rotation_.z));

		// 回転行列でどの方向を見ても同じ距離になるよう調整
		offset_ = MyMath::TransformNormal(offset_, rotateMat);

		// ターゲットから調整した距離分を加算
		viewProjection_.translation_ = MyMath::Add(target_->translation_, offset_);
	}

	// ビュープロジェクションの更新と転送
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

}

/// <summary>
/// ターゲットの設定
/// </summary>
/// <param name="target">ターゲットになるワールド変換データ</param>
void FollowCamera::SetTarget(const WorldTransform* target) {
	// 引数のターゲットに設定
	target_ = target;
}


/// <summary>
/// ViewProjectionの取得
/// </summary>
/// <returns>ViewProjection</returns>
ViewProjection& FollowCamera::GetViewProjection() { return viewProjection_; }

/// <summary>
/// グローバル変数の取得
/// </summary>
void FollowCamera::ApplyGlobalVariavles() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "FollowCamera";

	// 作ったグループにあるアイテムから値を取得
	move_mouseSpeed = gloVars->GetFloatValue(groupName, "movemauseSpeed");
	move_padSpeed = gloVars->GetFloatValue(groupName, "movepadSpeed");
	offset_ = gloVars->GetVector3Value(groupName, "offset");
}
