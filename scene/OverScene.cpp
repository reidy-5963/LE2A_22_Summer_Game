#include "OverScene.h"
#include "GlobalVariables.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

#include "MyMath.h"

OverScene::OverScene() {
}

OverScene::~OverScene() {}

void OverScene::Initialize() { 
	//
	BaseScene::Initialize();

	//
	scenedNo_ = OVER_S;
	BGMHandle_ = audio_->LoadWave("game_over.wav");
	selectSEHandle_ = audio_->LoadWave("select01.wav");

	isOverGimmick_ = false;
	overGimmickTimer_ = 0.0f;

#pragma region ロゴ
	// タイトルロゴテクスチャの読み込み
	logoTex_ = TextureManager::Load("Game_Over_Tex.png");

	// タイトルロゴの初期化処理
	logo_.reset(Sprite::Create(
	    logoTex_, {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	isLogoGimmick = false;
	isEnd_LogoGimmick = false;
	isRootLogo = false;
	logo_t_ = 0.0f;
	logoState_ = B_In;
#pragma endregion

#pragma region ロゴ
	// タイトルロゴテクスチャの読み込み
	backTitleTex_[0] = TextureManager::Load("Back_Title_Tex.png");
	backTitleTex_[1] = TextureManager::Load("Select_Back_Title_Tex.png");

	// タイトルロゴの初期化処理
	backTitle_.reset(Sprite::Create(
	    backTitleTex_[0], {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	isBackTitleGimmick = false;
	isEnd_BackTitleGimmick = false;
	isRootBackTitle_ = false;
	backTitle_t_ = 0.0f;
	backTitleState_ = B_In;

	isTitle_ = false;
#pragma endregion

#pragma region[終わる]
	// [終わる]テクスチャの読み込み
	endButtonTex_[0] = TextureManager::Load("EndButton.png");

	// [終わる]選択テクスチャの読み込み
	endButtonTex_[1] = TextureManager::Load("SelectEndButton.png");

	// [終わる]の初期化処理
	endButton_.reset(Sprite::Create(
	    endButtonTex_[0], {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	isEndButtonGimmick = false;
	isEnd_endButtonGimmick = false;
	isRootEndButton = false;
	endButton_t_ = 0.0f;
	endButtonState_ = B_In;

	isEnd_ = false;
#pragma endregion

#pragma region 黒画面
	// [始める]テクスチャの読み込み
	blackoutTex_ = TextureManager::Load("BlackOut.png");

	// [始める]の初期化処理
	blackout_.reset(Sprite::Create(
	    blackoutTex_, {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	blackoutAlpha_ = 0.0f;
#pragma endregion

	//
	InitializeGlobalVariavles();

}

void OverScene::Update() {
	//
	ApplyGlobalVariavles();

	if (i <= 0) {
		i = ShowCursor(true);
	}
	// BGM再生
	if (audio_->IsPlaying(BGMHandle_) == 0 || BGMHandle_ == -1) {
		BGMHandle_ = audio_->PlayWave(BGMHandle_, true, bolume);
	}

	// コントローラー
	XINPUT_STATE joystate;
	OverGimmick();
	LogoGimmick();
	BackTitleBGimmick();
	EndButtonGimmick();

	// もしコントローラーなら
	if (input_->GetJoystickState(0, joystate)) {
		JoyStateUpdate();
	}
	// もしキーマウなら
	else {
		KeyMouseUpdate();
	}

	// もしスタートフラグが有効なら
	if (isTitle_) {
		if (isEnd_BackTitleGimmick && isEnd_endButtonGimmick && isEnd_LogoGimmick) {
			audio_->StopWave(BGMHandle_);
			// ゲームシーンへ
			scenedNo_ = TITLE_S;
		}
	}

	blackout_->SetColor({1.0f, 1.0f, 1.0f, blackoutAlpha_});
	logo_->SetPosition(logoPos_);
	endButton_->SetPosition(endButtonPos_);
	backTitle_->SetPosition(backTitlePos_);
}

void OverScene::Draw() { 
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画 
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	
		//
	logo_->Draw();

	if (isBackTitleGimmick) {
		//
		backTitle_->Draw();
	}

	if (isEndButtonGimmick) {
		// [終わる] 描画処理
		endButton_->Draw();
	}

	blackout_->Draw();

	
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
void OverScene::InitializeGlobalVariavles() {
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	const char* groupName = "OVER";
	//
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "b_in_t_offset", b_in_t_offset);
	gloVars->AddItem(groupName, "b_root_t_offset", b_root_t_offset);
	gloVars->AddItem(groupName, "b_out_t_offset", b_out_t_offset);
	gloVars->AddItem(groupName, "timer_", timerSet_);

	gloVars->AddItem(groupName, "logo_in_S_pos_", logo_in_S_pos_);
	gloVars->AddItem(groupName, "logo_in_E_pos_", logo_in_E_pos_);

	gloVars->AddItem(groupName, "logo_root_S_pos_", logo_root_S_pos_);
	gloVars->AddItem(groupName, "logo_root_E_pos_", logo_root_E_pos_);

	gloVars->AddItem(groupName, "logo_out_S_pos_", logo_out_S_pos_);
	gloVars->AddItem(groupName, "logo_out_E_pos_", logo_out_E_pos_);

	gloVars->AddItem(groupName, "backTitle_in_S_pos_", backTitle_in_S_pos_);
	gloVars->AddItem(groupName, "backTitle_in_E_pos_", backTitle_in_E_pos_);

	gloVars->AddItem(groupName, "backTitle_root_S_pos_", backTitle_root_S_pos_);
	gloVars->AddItem(groupName, "backTitle_root_E_pos_", backTitle_root_E_pos_);

	gloVars->AddItem(groupName, "backTitle_out_S_pos_", backTitle_out_S_pos_);
	gloVars->AddItem(groupName, "backTitle_out_E_pos_", backTitle_out_E_pos_);

	gloVars->AddItem(groupName, "End Button Pos In S", endButton_in_S_pos_);
	gloVars->AddItem(groupName, "End Button Pos In E", endButton_in_E_pos_);

	gloVars->AddItem(groupName, "End Button Pos Root S", endButton_root_S_pos_);
	gloVars->AddItem(groupName, "End Button Pos Root E", endButton_root_E_pos_);

	gloVars->AddItem(groupName, "End Button Pos Out S", endButton_out_S_pos_);
	gloVars->AddItem(groupName, "End Button Pos Out E", endButton_out_E_pos_);

	gloVars->AddItem(groupName, "bolume", bolume);
}

void OverScene::ApplyGlobalVariavles() {
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "OVER";
	// 作ったグループにあるアイテムから値を取得
	b_in_t_offset = gloVars->GetFloatValue(groupName, "b_in_t_offset");
	b_root_t_offset = gloVars->GetFloatValue(groupName, "b_root_t_offset");
	b_out_t_offset = gloVars->GetFloatValue(groupName, "b_out_t_offset");
	timerSet_ = gloVars->GetIntValue(groupName, "timer_");

	logo_in_S_pos_ = gloVars->GetVector2Value(groupName, "logo_in_S_pos_");
	logo_in_E_pos_ = gloVars->GetVector2Value(groupName, "logo_in_E_pos_");

	logo_root_S_pos_ = gloVars->GetVector2Value(groupName, "logo_root_S_pos_");
	logo_root_E_pos_ = gloVars->GetVector2Value(groupName, "logo_root_E_pos_");

	logo_out_S_pos_ = gloVars->GetVector2Value(groupName, "logo_out_S_pos_");
	logo_out_E_pos_ = gloVars->GetVector2Value(groupName, "logo_out_E_pos_");

	backTitle_in_S_pos_ = gloVars->GetVector2Value(groupName, "backTitle_in_S_pos_");
	backTitle_in_E_pos_ = gloVars->GetVector2Value(groupName, "backTitle_in_E_pos_");

	backTitle_root_S_pos_ = gloVars->GetVector2Value(groupName, "backTitle_root_S_pos_");
	backTitle_root_E_pos_ = gloVars->GetVector2Value(groupName, "backTitle_root_E_pos_");

	backTitle_out_S_pos_ = gloVars->GetVector2Value(groupName, "backTitle_out_S_pos_");
	backTitle_out_E_pos_ = gloVars->GetVector2Value(groupName, "backTitle_out_E_pos_");

	endButton_in_S_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos In S");
	endButton_in_E_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos In E");

	endButton_root_S_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos Root S");
	endButton_root_E_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos Root E");

	endButton_out_S_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos Out S");
	endButton_out_E_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos Out E");

	bolume = gloVars->GetFloatValue(groupName, "bolume");
}

void OverScene::KeyMouseUpdate() { 
	// 現在のカーソル位置を取得
	GetCursorPos(&mousePos_);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePos_);

	// [始める]のサイズ取得
	Vector2 backTitleSize = backTitle_->GetSize();
	// [終わる]のサイズ取得
	Vector2 endButtonSize = endButton_->GetSize();

	KeyMouse_ButtonUpdate(
	    backTitlePos_, backTitleSize, isStartSelect, isTitle_, backTitle_.get(), backTitleTex_);

	KeyMouse_ButtonUpdate(
	    endButtonPos_, endButtonSize, isEndSelect, isEnd_, endButton_.get(), endButtonTex_);
}

void OverScene::JoyStateUpdate() { 
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
				audio_->PlayWave(selectSEHandle_, false, bolume);

				// もし選択がスタートなら
				if (selectButton == BACKTITLE) {

					// エンドに
					selectButton = END;
				}
				// もし選択がエンドなら
				else if (selectButton == END) {
					// スタートに
					selectButton = BACKTITLE;
				}
			}
			// もし下に動かしたら
			if (joystate.Gamepad.sThumbLY < 0 ||
			    joystate.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN) {
				audio_->PlayWave(selectSEHandle_, false, bolume);
				// フラグを有効に
				isSelectButton_ = true;
				// もし選択がスタートなら
				if (selectButton == BACKTITLE) {
					// エンドに
					selectButton = END;
				}
				// もし選択がエンドなら
				else if (selectButton == END) {
					// スタートに
					selectButton = BACKTITLE;
				}
			}
		}

		JoyState_ButtonUpdate(
		    BACKTITLE, backTitle_.get(), endButton_.get(), backTitleTex_[1], endButtonTex_[0],
		    isTitle_);

		JoyState_ButtonUpdate(
		    END, backTitle_.get(), endButton_.get(), backTitleTex_[0], endButtonTex_[1], isEnd_);

		// 前フレームの状態を保存
		preJoyButton_ = joystate.Gamepad.wButtons;
	}
}

bool OverScene::ButtonToCurSor(Vector2 buttonPos, Vector2 buttonSize) {
	POINT cursorPos;
	// 現在のカーソル位置を取得
	GetCursorPos(&cursorPos);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &cursorPos);

	if ((float)cursorPos.x > buttonPos.x - (buttonSize.x / 2) &&
	    (float)cursorPos.x < buttonPos.x + (buttonSize.x / 2) &&
	    (float)cursorPos.y > buttonPos.y - (buttonSize.y / 2) &&
	    (float)cursorPos.y < buttonPos.y + (buttonSize.y / 2)) {
		return true;
	}
	return false;
}

void OverScene::KeyMouse_ButtonUpdate(
    Vector2 buttonPos, Vector2 buttonSize, bool& isSelect, bool& whatDO, Sprite* sprite,
    uint32_t texture[2]) { 
	// もしボタン内にカーソルが入ったら
	if (ButtonToCurSor(buttonPos, buttonSize)) {
		if (!isSelect) {
			audio_->PlayWave(selectSEHandle_, false, bolume);

			// テクスチャを差し替えてボタンが光る
			sprite->SetTextureHandle(texture[1]);
			isSelect = true;
		}
		// もし左クリックしたらフラグを有効にする
		if (input_->IsTriggerMouse(0)) {
			audio_->PlayWave(selectSEHandle_, false, bolume);
			whatDO = true;
		}
	}
	// カーソルがボタン外なら
	else {
		// テクスチャは通常状態
		sprite->SetTextureHandle(texture[0]);
		isSelect = false;
	}
}

void OverScene::JoyState_ButtonUpdate(
    int selectState, Sprite* start, Sprite* end, uint32_t startTex, uint32_t endTex, bool& whatDO) {
	XINPUT_STATE joystate;

	if (input_->GetJoystickState(0, joystate)) {

		// もし選択がスタートなら
		if (selectButton == selectState) {
			// テクスチャを差し替えてボタンが光る
			start->SetTextureHandle(startTex);
			end->SetTextureHandle(endTex);
			// もしAを押したら
			if (joystate.Gamepad.wButtons == XINPUT_GAMEPAD_A &&
			    preJoyButton_ != XINPUT_GAMEPAD_A) {
				audio_->PlayWave(selectSEHandle_, false, bolume);

				// スタート
				whatDO = true;
			}
		}
	}
}

void OverScene::BackTitleBGimmick() {
	if (!isBackTitleGimmick) {
		isEnd_BackTitleGimmick = false;
		backTitle_t_ = 0.0f;
		backTitleState_ = B_In;
	}
	//
	else if (isBackTitleGimmick) {
		if (backTitleState_ == B_In) {
			backTitlePos_.x =
			    MyMath::EaseOutQuadF(backTitle_t_, backTitle_in_S_pos_.x, backTitle_in_E_pos_.x);
			backTitlePos_.y =
			    MyMath::EaseOutQuadF(backTitle_t_, backTitle_in_S_pos_.y, backTitle_in_E_pos_.y);

			backTitle_->SetColor({1.0f, 1.0f, 1.0f, backTitle_t_});

			//
			if (backTitle_t_ >= 1.0f) {
				backTitle_t_ = 0.0f;
				backTitleState_ = B_Root;
				backTitle_root_S_pos_ = backTitle_in_E_pos_;
			}
			//
			else if (backTitle_t_ < 1.0f) {
				backTitle_t_ += b_in_t_offset;
			}
		}
		//
		else if (backTitleState_ == B_Root) {
			backTitlePos_.x = MyMath::EaseOutQuadF(
			    backTitle_t_, backTitle_root_S_pos_.x, backTitle_root_E_pos_.x);
			backTitlePos_.y = MyMath::EaseOutQuadF(
			    backTitle_t_, backTitle_root_S_pos_.y, backTitle_root_E_pos_.y);

			if (!isRootBackTitle_) {
				if (backTitle_t_ >= 1.0f) {
					backTitle_t_ = 1.0f;
					isRootBackTitle_ = true;
				}
				//
				else if (backTitle_t_ < 1.0f) {
					backTitle_t_ += b_root_t_offset;
				}
			}
			//
			else if (isRootBackTitle_) {
				if (backTitle_t_ <= 0.0f) {
					backTitle_t_ = 0.0f;
					isRootBackTitle_ = false;
				}
				//
				else if (backTitle_t_ > 0.0f) {
					backTitle_t_ -= b_root_t_offset;
				}
			}

			if (isTitle_) {
				backTitleState_ = B_Out;
				backTitle_out_S_pos_ = backTitlePos_;
				backTitle_t_ = 0.0f;
			}
		}
		//
		else if (backTitleState_ == B_Out) {
			backTitlePos_.x =
			    MyMath::EaseInQuadF(backTitle_t_, backTitle_out_S_pos_.x, backTitle_out_E_pos_.x);
			backTitlePos_.y =
			    MyMath::EaseInQuadF(backTitle_t_, backTitle_out_S_pos_.y, backTitle_out_E_pos_.y);

			backTitle_->SetColor({1.0f, 1.0f, 1.0f, (1.0f - backTitle_t_)});

			if (backTitle_t_ >= 1.0f) {
				backTitle_t_ = 1.0f;
				isEnd_BackTitleGimmick = true;
			}
			//
			else if (backTitle_t_ < 1.0f) {
				backTitle_t_ += b_out_t_offset;
			}
		}
	}
}

void OverScene::LogoGimmick() {
	if (!isLogoGimmick) {
		isEnd_LogoGimmick = false;
		logo_t_ = 0.0f;
		logoState_ = B_In;
	}
	//
	else if (isLogoGimmick) {
		if (logoState_ == B_In) {
			logoPos_.x = MyMath::EaseOutCubicF(logo_t_, logo_in_S_pos_.x, logo_in_E_pos_.x);
			logoPos_.y = MyMath::EaseOutCubicF(logo_t_, logo_in_S_pos_.y, logo_in_E_pos_.y);

			logo_->SetColor({1.0f, 1.0f, 1.0f, logo_t_});

			//
			if (logo_t_ >= 1.0f) {
				logo_t_ = 0.0f;
				logoState_ = B_Root;
				logo_root_S_pos_ = logo_in_E_pos_;
			}
			//
			else if (logo_t_ < 1.0f) {
				logo_t_ += b_in_t_offset;
			}
		}
		//
		else if (logoState_ == B_Root) {
			logoPos_.x = MyMath::EaseOutCubicF(logo_t_, logo_root_S_pos_.x, logo_root_E_pos_.x);
			logoPos_.y = MyMath::EaseOutCubicF(logo_t_, logo_root_S_pos_.y, logo_root_E_pos_.y);

			if (!isRootLogo) {
				if (logo_t_ >= 1.0f) {
					logo_t_ = 1.0f;
					isRootLogo = true;
				}
				//
				else if (logo_t_ < 1.0f) {
					logo_t_ += b_root_t_offset;
				}
			}
			//
			else if (isRootLogo) {
				if (logo_t_ <= 0.0f) {
					logo_t_ = 0.0f;
					isRootLogo = false;
				}
				//
				else if (logo_t_ > 0.0f) {
					logo_t_ -= b_root_t_offset;
				}
			}

			if (isTitle_ || isStart_) {
				logoState_ = B_Out;
				backTitle_out_S_pos_ = logoPos_;
				logo_t_ = 0.0f;
			}
		}
		//
		else if (logoState_ == B_Out) {
			logoPos_.x = MyMath::EaseInCubicF(logo_t_, logo_root_S_pos_.x, logo_root_E_pos_.x);
			logoPos_.y = MyMath::EaseInCubicF(logo_t_, logo_root_S_pos_.y, logo_root_E_pos_.y);

			logo_->SetColor({1.0f, 1.0f, 1.0f, (1.0f - logo_t_)});

			blackoutAlpha_ = logo_t_;
			if (logo_t_ >= 1.0f) {
				logo_t_ = 1.0f;
				isEnd_LogoGimmick = true;
			}
			//
			else if (logo_t_ < 1.0f) {
				logo_t_ += b_out_t_offset;
			}
		}
	}
}

void OverScene::OverGimmick() {
	//
	if (!isOverGimmick_) {
		if (blackoutAlpha_ > 0.0f) {
			blackoutAlpha_ -= blackoutAlpha_offset_;
		}
		//
		else if (blackoutAlpha_ <= 0.0f) {
			blackoutAlpha_ = 0.0f;
		}

		if (overGimmickTimer_ >= 0.0f) {
			isLogoGimmick = true;
		}
		//
		if (overGimmickTimer_ >= 0.1f) {
			isBackTitleGimmick = true;
		}

		if (overGimmickTimer_ >= 0.2f) {
			isEndButtonGimmick = true;
		}

		if (overGimmickTimer_ >= 1.0f) {
			overGimmickTimer_ = 1.0f;
			isOverGimmick_ = true;
		}
		//
		else if (overGimmickTimer_ < 1.0f) {
			overGimmickTimer_ += 0.01f;
		}
	}
	//
	else if (isOverGimmick_) {
	}
}

void OverScene::EndButtonGimmick() {
	if (!isEndButtonGimmick) {
		isEnd_endButtonGimmick = false;
		endButton_t_ = 0.0f;
		endButtonState_ = B_In;
	}
	//
	else if (isEndButtonGimmick) {
		if (endButtonState_ == B_In) {
			endButtonPos_.x =
			    MyMath::EaseOutQuadF(endButton_t_, endButton_in_S_pos_.x, endButton_in_E_pos_.x);
			endButtonPos_.y =
			    MyMath::EaseOutQuadF(endButton_t_, endButton_in_S_pos_.y, endButton_in_E_pos_.y);

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
			endButtonPos_.x = MyMath::EaseOutQuadF(
			    endButton_t_, endButton_root_S_pos_.x, endButton_root_E_pos_.x);
			endButtonPos_.y = MyMath::EaseOutQuadF(
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

			if (isTitle_) {
				endButtonState_ = B_Out;
				endButton_out_S_pos_ = endButtonPos_;
				endButton_t_ = 0.0f;
			}
		}
		//
		else if (endButtonState_ == B_Out) {
			endButtonPos_.x =
			    MyMath::EaseInQuadF(endButton_t_, endButton_out_S_pos_.x, endButton_out_E_pos_.x);
			endButtonPos_.y =
			    MyMath::EaseInQuadF(endButton_t_, endButton_out_S_pos_.y, endButton_out_E_pos_.y);

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
