#include "TitleScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "GlobalVariables.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize() { 
	//
	BaseScene::Initialize();
	scenedNo_ = TITLE_S;

	// 各Sprite用のテクスチャの読み込み
	titleTex_ = TextureManager::Load("TitleTexture.png");

	title_.reset(Sprite::Create(
	    titleTex_, {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	// 各Sprite用のテクスチャの読み込み
	startButtonTex_ = TextureManager::Load("StartButton.png");

	startButton_.reset(Sprite::Create(
	    startButtonTex_, {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	// 各Sprite用のテクスチャの読み込み
	endButtonTex_ = TextureManager::Load("EndButton.png");

	endButton_.reset(Sprite::Create(
	    endButtonTex_, {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	
	InitializeGlobalVariavles();
}

void TitleScene::Update() {
	ApplyGlobalVariavles();
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


	title_->SetPosition(titleLogoPos_);
	startButton_->SetPosition(startButtonPos_);
	endButton_->SetPosition(endButtonPos_);
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
	title_->Draw();
	startButton_->Draw();
	endButton_->Draw();

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

void TitleScene::ApplyGlobalVariavles() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Title";
	// 作ったグループにあるアイテムから値を取得
	titleLogoPos_ = gloVars->GetVector2Value(groupName, "Title Logo Pos");
	startButtonPos_ = gloVars->GetVector2Value(groupName, "Start Button Pos");
	endButtonPos_ = gloVars->GetVector2Value(groupName, "End Button Pos");
}

void TitleScene::InitializeGlobalVariavles() {
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	const char* groupName = "Title";
	//
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "Title Logo Pos", titleLogoPos_);
	gloVars->AddItem(groupName, "Start Button Pos", startButtonPos_);
	gloVars->AddItem(groupName, "End Button Pos", endButtonPos_);
}
