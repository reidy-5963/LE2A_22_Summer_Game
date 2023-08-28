#include "TitleScene.h"
#include "GlobalVariables.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

#include "MyMath.h"

/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene() {}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene() {}

/// <summary>
/// 初期化処理
/// </summary>
void TitleScene::Initialize() {
	// ベース部分の初期化処理
	BaseScene::Initialize();

	// このシーン番号の設定
	scenedNo_ = TITLE_S;

	// 各フラグの初期化
	isStart = false;
	isEnd_ = false;
	isSelectButton_ = false;

	selectButton = START;
	timer_ = timerSet_;
#pragma region タイトルロゴ
	// タイトルロゴテクスチャの読み込み
	titleTex_ = TextureManager::Load("TitleTexture.png");

	// タイトルロゴの初期化処理
	title_.reset(Sprite::Create(
	    titleTex_, {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
#pragma endregion

#pragma region[始める]
	// [始める]テクスチャの読み込み
	startButtonTex_ = TextureManager::Load("StartButton.png");

	// [始める]選択テクスチャの読み込み
	startButton_select_Tex_ = TextureManager::Load("SelectStartButton.png");

	// [始める]の初期化処理
	startButton_.reset(Sprite::Create(
	    startButtonTex_, {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	startButton_in_S_pos_ = {0.0f, 460.0f};
	startButton_in_E_pos_ = {640.0f, 460.0f};
	startButton_t_ = 0.0f;
#pragma endregion

#pragma region[終わる]
	// [終わる]テクスチャの読み込み
	endButtonTex_ = TextureManager::Load("EndButton.png");

	// [終わる]選択テクスチャの読み込み
	endButton_select_Tex_ = TextureManager::Load("SelectEndButton.png");

	// [終わる]の初期化処理
	endButton_.reset(Sprite::Create(
	    endButtonTex_, {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
#pragma endregion

	// グローバル変数の初期化処理
	InitializeGlobalVariavles();
}

/// <summary>
/// 更新処理
/// </summary>
void TitleScene::Update() {
	// グローバル変数の取得
	ApplyGlobalVariavles();

	// コントローラー
	XINPUT_STATE joystate;
	TitleGimmick();
	TitleLogoGimmick();
	StartButtonGimmick();
	EndButtonGimmick();
	//
	ImGui::Begin("Title");
	ImGui::Text("press Space");
	ImGui::Text("t : %f", titleGimmickTimer_);
	ImGui::Checkbox("isStart", &isStartButtonGimmick);
	ImGui::Text("%f, %f", startButtonPos_.x, startButtonPos_.y);
	ImGui::End();


	// もしコントローラーなら
	if (input_->GetJoystickState(0, joystate)) {
		JoyStateUpdate();
	} 
	// もしキーマウなら
	else {
		KeyMouseUpdate();
	}

	// もしスタートフラグが有効なら
	if (isStart) {
		if (isEnd_startButtonGimmick && isEnd_endButtonGimmick) {
			// ゲームシーンへ
			scenedNo_ = GAME_S;
		}
	}

	//if (input_->TriggerKey(DIK_1)) {
	//	scenedNo_ = GAME_S;
	//}
	//if (input_->TriggerKey(DIK_2)) {
	//	scenedNo_ = CLEAR_S;
	//}
	//if (input_->TriggerKey(DIK_3)) {
	//	scenedNo_ = OVER_S;
	//}

	title_->SetPosition(titleLogoPos_);
	startButton_->SetPosition(startButtonPos_);
	endButton_->SetPosition(endButtonPos_);
}

/// <summary>
/// 描画処理
/// </summary>
void TitleScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// タイトルロゴ描画処理
	title_->Draw();

	if (isStartButtonGimmick) {
		// [始める] 描画処理
		startButton_->Draw();
	}

	if (isEndButtonGimmick) {
		// [終わる] 描画処理
		endButton_->Draw();

	}

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

/// <summary>
/// グローバル変数の初期化処理
/// </summary>
void TitleScene::InitializeGlobalVariavles() {
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	const char* groupName = "Title";
	//
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "Title Logo Pos", titleLogoPos_);
	gloVars->AddItem(groupName, "b_in_t_offset", b_in_t_offset);
	gloVars->AddItem(groupName, "b_root_t_offset", b_root_t_offset);
	gloVars->AddItem(groupName, "b_out_t_offset", b_out_t_offset);
	gloVars->AddItem(groupName, "timer_", timerSet_);

	gloVars->AddItem(groupName, "Start Button Pos In S", startButton_in_S_pos_);
	gloVars->AddItem(groupName, "Start Button Pos In E", startButton_in_E_pos_);

	gloVars->AddItem(groupName, "Start Button Pos Root E", startButton_root_E_pos_);

	gloVars->AddItem(groupName, "Start Button Pos Out E", startButton_out_E_pos_);

	gloVars->AddItem(groupName, "Title_Logo_In_S", titleLogo_in_S_pos_);
	gloVars->AddItem(groupName, "Title_Logo_In_E", titleLogo_in_E_pos_);

	gloVars->AddItem(groupName, "Title_Logo_Root_E", titleLogo_root_E_pos_);

	gloVars->AddItem(groupName, "Title_Logo_Out_E", titleLogo_out_E_pos_);

	gloVars->AddItem(groupName, "End Button Pos In S", endButton_in_S_pos_);
	gloVars->AddItem(groupName, "End Button Pos In E", endButton_in_E_pos_);

	gloVars->AddItem(groupName, "End Button Pos Root E", endButton_root_E_pos_);

	gloVars->AddItem(groupName, "End Button Pos Out E", endButton_out_E_pos_);

}

/// <summary>
/// グローバル変数の取得
/// </summary>
void TitleScene::ApplyGlobalVariavles() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Title";
	// 作ったグループにあるアイテムから値を取得
	titleLogoPos_ = gloVars->GetVector2Value(groupName, "Title Logo Pos");
	b_in_t_offset = gloVars->GetFloatValue(groupName, "b_in_t_offset");
	b_root_t_offset = gloVars->GetFloatValue(groupName, "b_root_t_offset");
	b_out_t_offset = gloVars->GetFloatValue(groupName, "b_out_t_offset");

	timerSet_ = gloVars->GetIntValue(groupName, "timer_");

	titleLogo_in_S_pos_ = gloVars->GetVector2Value(groupName, "Title_Logo_In_S");
	titleLogo_in_E_pos_ = gloVars->GetVector2Value(groupName, "Title_Logo_In_E");

	titleLogo_root_E_pos_ = gloVars->GetVector2Value(groupName, "Title_Logo_Root_E");

	titleLogo_out_E_pos_ = gloVars->GetVector2Value(groupName, "Title_Logo_Out_E");


	startButton_in_S_pos_ = gloVars->GetVector2Value(groupName, "Start Button Pos In S");
	startButton_in_E_pos_ = gloVars->GetVector2Value(groupName, "Start Button Pos In E");

	startButton_root_E_pos_ = gloVars->GetVector2Value(groupName, "Start Button Pos Root E");

	startButton_out_E_pos_ = gloVars->GetVector2Value(groupName, "Start Button Pos Out E");


	endButton_in_S_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos In S");
	endButton_in_E_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos In E");
	
	endButton_root_E_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos Root E");
	
	endButton_out_E_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos Out E");
}

/// <summary>
/// キーマウでの更新処理
/// </summary>
void TitleScene::KeyMouseUpdate() {
	// 現在のカーソル位置を取得
	GetCursorPos(&mousePos_);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePos_);

	// [始める]のサイズ取得
	Vector2 startButtonSize = startButton_->GetSize();
	// [終わる]のサイズ取得
	Vector2 endButtonSize = endButton_->GetSize();

	// もしボタン内にカーソルが入ったら
	if (ButtonToCurSor(startButtonPos_, startButtonSize, mousePos_)) {
		// テクスチャを差し替えてボタンが光る
		startButton_->SetTextureHandle(startButton_select_Tex_);
		// もし左クリックしたらゲームスタート
		if (input_->IsTriggerMouse(0)) {
			isStart = true;
		}
	}
	// カーソルがボタン外なら
	else {
		// テクスチャは通常状態
		startButton_->SetTextureHandle(startButtonTex_);
	}

	// もしボタン内にカーソルが入ったら
	if (ButtonToCurSor(endButtonPos_, endButtonSize, mousePos_)) {
		// テクスチャを差し替えてボタンが光る
		endButton_->SetTextureHandle(endButton_select_Tex_);
		// もし左クリックしたらゲームスタート
		if (input_->IsTriggerMouse(0)) {
			isEnd_ = true;
		}
	}
	// カーソルがボタン外なら
	else {
		// テクスチャは通常状態
		endButton_->SetTextureHandle(endButtonTex_);
	}
}

/// <summary>
/// コントローラー時の更新処理
/// </summary>
void TitleScene::JoyStateUpdate() {
	//ImGui::Begin("debug");
	//ImGui::Text("%d", timer_);
	//ImGui::Checkbox("select", &isSelectButton_);
	//ImGui::End();

	// コントローラー
	XINPUT_STATE joystate;
	if (input_->GetJoystickState(0, joystate)) {
		// もしフラグが有効なら
		if (isSelectButton_) {
			// タイマー
			timer_--;
			// もしタイマーが０なら
			if (timer_ <= 0) {
				// フラグを無効に
				isSelectButton_ = false;
			}
		}
		// もしフラグが無効なら
		else if (!isSelectButton_) {
			// タイマーを規定値に設定
			timer_ = timerSet_;
			// もし上に動かしたら
			if (joystate.Gamepad.sThumbLY > 0 ||
			    joystate.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP) {
				// フラグを有効に
				isSelectButton_ = true; 
				//もし選択がスタートなら
				if (selectButton == START) {
					// エンドに
					selectButton = END;
				}
				// もし選択がエンドなら
				else if (selectButton == END) {
					// スタートに
					selectButton = START;
				}

			} 
			// もし下に動かしたら
			if (joystate.Gamepad.sThumbLY < 0 ||
			    joystate.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN) {
				// フラグを有効に
				isSelectButton_ = true;
				// もし選択がスタートなら
				if (selectButton == START) {
					// エンドに
					selectButton = END;
				}
				// もし選択がエンドなら
				else if (selectButton == END) {
					// スタートに
					selectButton = START;
				}
			}
		}

		// もし選択がスタートなら
		if (selectButton == START) {
			// テクスチャを差し替えてボタンが光る
			startButton_->SetTextureHandle(startButton_select_Tex_);
			endButton_->SetTextureHandle(endButtonTex_);
			// もしAを押したら
			if (joystate.Gamepad.wButtons == XINPUT_GAMEPAD_A &&
				preJoyButton_ != XINPUT_GAMEPAD_A) {
				// スタート
				isStart = true;
			}
		}
		// もし選択がエンドなら
		else if (selectButton == END) { 
			// テクスチャを差し替えてボタンが光る
			startButton_->SetTextureHandle(startButtonTex_);
			endButton_->SetTextureHandle(endButton_select_Tex_);
			// もしAを押したら
			if (joystate.Gamepad.wButtons == XINPUT_GAMEPAD_A &&
			    preJoyButton_ != XINPUT_GAMEPAD_A) {
				// 終了
				isEnd_ = true;
			}

		}
	
		// 前フレームの状態を保存
		preJoyButton_ = joystate.Gamepad.wButtons;
	}
}

/// <summary>
/// もしボタン内にカーソルが入ったら
/// </summary>
/// <param name="buttonPos">ボタン位置</param>
/// <param name="buttonSize">ボタンサイズ(直径)</param>
/// <param name="mousePos">カーソル位置</param>
/// <returns></returns>
bool TitleScene::ButtonToCurSor(Vector2 buttonPos, Vector2 buttonSize, POINT mousePos) {
	if ((float)mousePos.x > buttonPos.x - (buttonSize.x / 2) &&
	    (float)mousePos.x < buttonPos.x + (buttonSize.x / 2) &&
	    (float)mousePos.y > buttonPos.y - (buttonSize.y / 2) &&
	    (float)mousePos.y < buttonPos.y + (buttonSize.y / 2)) {
		return true;
	}
	return false;
}

void TitleScene::StartButtonGimmick() {
	if (!isStartButtonGimmick) {
		isEnd_startButtonGimmick = false;
		startButton_t_ = 0.0f;
		startButtonState_ = B_In;
	} 
	//
	else if (isStartButtonGimmick) {
		if (startButtonState_ == B_In) {
			startButtonPos_.x = MyMath::EaseOutCubicF(
			    startButton_t_, startButton_in_S_pos_.x, startButton_in_E_pos_.x);
			startButtonPos_.y = MyMath::EaseOutCubicF(
			    startButton_t_, startButton_in_S_pos_.y, startButton_in_E_pos_.y);

			startButton_->SetColor({1.0f, 1.0f, 1.0f, startButton_t_});

			//
			if (startButton_t_ >= 1.0f) {
				startButton_t_ = 0.0f;
				startButtonState_ = B_Root;
				startButton_root_S_pos_ = startButton_in_E_pos_;
			}
			//
			else if (startButton_t_ < 1.0f) {
				startButton_t_ += b_in_t_offset;
			}
		}
		//
		else if (startButtonState_ == B_Root) {
			startButtonPos_.x = MyMath::EaseInOutCubicF(
			    startButton_t_, startButton_root_S_pos_.x, startButton_root_E_pos_.x);
			startButtonPos_.y = MyMath::EaseInOutCubicF(
			    startButton_t_, startButton_root_S_pos_.y, startButton_root_E_pos_.y);

			if (!isRootStartButton) {
				if (startButton_t_ >= 1.0f) {
					startButton_t_ = 1.0f;
					isRootStartButton = true;
				}
				//
				else if (startButton_t_ < 1.0f) {
					startButton_t_ += b_root_t_offset;
				}
			}
			//
			else if (isRootStartButton) {
				if (startButton_t_ <= 0.0f) {
					startButton_t_ = 0.0f;
					isRootStartButton = false;
				}
				//
				else if (startButton_t_ > 0.0f) {
					startButton_t_ -= b_root_t_offset;
				}
			}

			if (isStart) {
				startButtonState_ = B_Out;
				startButton_out_S_pos_ = startButtonPos_;
				startButton_t_ = 0.0f;
			}
		}
		//
		else if (startButtonState_ == B_Out) {
			startButtonPos_.x = MyMath::EaseInCubicF(
			    startButton_t_, startButton_out_S_pos_.x, startButton_out_E_pos_.x);
			startButtonPos_.y = MyMath::EaseInCubicF(
			    startButton_t_, startButton_out_S_pos_.y, startButton_out_E_pos_.y);

			startButton_->SetColor({1.0f, 1.0f, 1.0f, (1.0f - startButton_t_)});

			if (startButton_t_ >= 1.0f) {
				startButton_t_ = 1.0f;
				isEnd_startButtonGimmick = true;
			}
			//
			else if (startButton_t_ < 1.0f) {
				startButton_t_ += b_out_t_offset;
			}
		}


	}
}

void TitleScene::EndButtonGimmick() {
	if (!isEndButtonGimmick) {
		isEnd_endButtonGimmick = false;
		endButton_t_ = 0.0f;
		endButtonState_ = B_In;
	}
	//
	else if (isEndButtonGimmick) {
		if (endButtonState_ == B_In) {
			endButtonPos_.x = MyMath::EaseOutCubicF(endButton_t_, endButton_in_S_pos_.x, endButton_in_E_pos_.x);
			endButtonPos_.y = MyMath::EaseOutCubicF(endButton_t_, endButton_in_S_pos_.y, endButton_in_E_pos_.y);

			endButton_->SetColor({1.0f, 1.0f, 1.0f, endButton_t_});

			//
			if (endButton_t_ >= 1.0f) {
				endButton_t_ = 0.0f;
				endButtonState_ = B_Root;
				endButton_root_S_pos_ = endButton_in_E_pos_;
			}
			//
			else if (endButton_t_ < 1.0f) {
				endButton_t_ += b_in_t_offset;
			}
		}
		//
		else if (endButtonState_ == B_Root) {
			endButtonPos_.x = MyMath::EaseInOutCubicF(
			    endButton_t_, endButton_root_S_pos_.x, endButton_root_E_pos_.x);
			endButtonPos_.y = MyMath::EaseInOutCubicF(
			    endButton_t_, endButton_root_S_pos_.y, endButton_root_E_pos_.y);

			if (!isRootEndButton) {
				if (endButton_t_ >= 1.0f) {
					endButton_t_ = 1.0f;
					isRootEndButton = true;
				}
				//
				else if (endButton_t_ < 1.0f) {
					endButton_t_ += b_root_t_offset;
				}
			}
			//
			else if (isRootEndButton) {
				if (endButton_t_ <= 0.0f) {
					endButton_t_ = 0.0f;
					isRootEndButton = false;
				}
				//
				else if (endButton_t_ > 0.0f) {
					endButton_t_ -= b_root_t_offset;
				}
			}

			if (isStart) {
				endButtonState_ = B_Out;
				endButton_out_S_pos_ = endButtonPos_;
				endButton_t_ = 0.0f;
			}
		}
		//
		else if (endButtonState_ == B_Out) {
			endButtonPos_.x = MyMath::EaseInCubicF(endButton_t_, endButton_out_S_pos_.x, endButton_out_E_pos_.x);
			endButtonPos_.y = MyMath::EaseInCubicF(endButton_t_, endButton_out_S_pos_.y, endButton_out_E_pos_.y);

			endButton_->SetColor({1.0f, 1.0f, 1.0f, (1.0f - endButton_t_)});

			if (endButton_t_ >= 1.0f) {
				endButton_t_ = 1.0f;
				isEnd_endButtonGimmick = true;
			}
			//
			else if (endButton_t_ < 1.0f) {
				endButton_t_ += b_out_t_offset;
			}
		}
	}
}

void TitleScene::TitleLogoGimmick() {
	if (!isTitleLogoGimmick) {
		isEnd_titleLogoGimmick = false;
		titleLogo_t_ = 0.0f;
		titleLogoState_ = B_In;
	}
	//
	else if (isTitleLogoGimmick) {
		if (titleLogoState_ == B_In) {
			titleLogoPos_.x =
			    MyMath::EaseOutCubicF(titleLogo_t_, titleLogo_in_S_pos_.x, titleLogo_in_E_pos_.x);
			titleLogoPos_.y =
			    MyMath::EaseOutCubicF(titleLogo_t_, titleLogo_in_S_pos_.y, titleLogo_in_E_pos_.y);

			title_->SetColor({1.0f, 1.0f, 1.0f, titleLogo_t_});

			//
			if (titleLogo_t_ >= 1.0f) {
				titleLogo_t_ = 0.0f;
				titleLogoState_ = B_Root;
				titleLogo_root_S_pos_ = titleLogo_in_E_pos_;
			}
			//
			else if (titleLogo_t_ < 1.0f) {
				titleLogo_t_ += b_in_t_offset;
			}
		}
		//
		else if (titleLogoState_ == B_Root) {
			titleLogoPos_.x = MyMath::EaseInOutCubicF(
			    titleLogo_t_, titleLogo_root_S_pos_.x, titleLogo_root_E_pos_.x);
			titleLogoPos_.y = MyMath::EaseInOutCubicF(
			    titleLogo_t_, titleLogo_root_S_pos_.y, titleLogo_root_E_pos_.y);

			if (!isRoottitleLogo) {
				if (titleLogo_t_ >= 1.0f) {
					titleLogo_t_ = 1.0f;
					isRoottitleLogo = true;
				}
				//
				else if (titleLogo_t_ < 1.0f) {
					titleLogo_t_ += b_root_t_offset;
				}
			}
			//
			else if (isRoottitleLogo) {
				if (titleLogo_t_ <= 0.0f) {
					titleLogo_t_ = 0.0f;
					isRoottitleLogo = false;
				}
				//
				else if (titleLogo_t_ > 0.0f) {
					titleLogo_t_ -= b_root_t_offset;
				}
			}

			if (isStart) {
				titleLogoState_ = B_Out;
				titleLogo_out_S_pos_ = endButtonPos_;
				titleLogo_t_ = 0.0f;
			}
		}
		//
		else if (titleLogoState_ == B_Out) {
			titleLogoPos_.x =
			    MyMath::EaseInCubicF(titleLogo_t_, titleLogo_out_S_pos_.x, titleLogo_out_E_pos_.x);
			titleLogoPos_.y =
			    MyMath::EaseInCubicF(titleLogo_t_, titleLogo_out_S_pos_.y, titleLogo_out_E_pos_.y);

			title_->SetColor({1.0f, 1.0f, 1.0f, (1.0f - titleLogo_t_)});

			if (titleLogo_t_ >= 1.0f) {
				titleLogo_t_ = 1.0f;
				isEnd_titleLogoGimmick = true;
			}
			//
			else if (titleLogo_t_ < 1.0f) {
				titleLogo_t_ += b_out_t_offset;
			}
		}
	}
}

void TitleScene::TitleGimmick() { 
	//
	if (titleGimmickTimer_ >= 0.0f) {
		isTitleLogoGimmick = true;
	} 
	//
	if (titleGimmickTimer_ >= 0.1f) {
		isStartButtonGimmick = true;
	}
	//
	if (titleGimmickTimer_ >= 0.2f) {
		isEndButtonGimmick = true;
	}

	if (titleGimmickTimer_ >= 1.0f) {
		titleGimmickTimer_ = 1.0f;
	}
	//
	else if (titleGimmickTimer_ < 1.0f) {
		titleGimmickTimer_ += 0.01f;
	}
}

