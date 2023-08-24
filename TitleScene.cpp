#include "TitleScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize() { 
	//
	BaseScene::Initialize();
	scenedNo_ = TITLE_S;
}

void TitleScene::Update() {
	ImGui::Begin("Title");
	ImGui::Text("press Space");
	ImGui::End();

	XINPUT_STATE joystate;
	if (input_->GetJoystickState(0, joystate)) {
		if (joystate.Gamepad.wButtons == XINPUT_GAMEPAD_B) {
			scenedNo_ = GAME_S;
		}
	}
	else {
		if (input_->TriggerKey(DIK_SPACE)) {
			scenedNo_ = GAME_S;
		}
	}

}

void TitleScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
