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

	// BGMハンドル
	BGMHandle_ = audio_->LoadWave("PLAY.wav");
	// 効果音ハンドル
	selectSEHandle_ = audio_->LoadWave("select01.wav");
	
	// タイトルのギミックタイマー
	isTitleGimmick_ = false;
	titleGimmickTimer_ = 0.0f;
	
	//** コントローラー関係のフラグ **//
	// ボタン選択の間を作るためのフラグ
	isSelectButton_ = false;
	// 現在選択しているボタン
	selectButton = START;
	// ボタン選択の間
	timer_ = timerSet_;
#pragma region タイトルロゴ
	// タイトルロゴテクスチャの読み込み
	titleTex_ = TextureManager::Load("TitleTexture.png");

	// タイトルロゴの初期化処理
	title_.reset(Sprite::Create(
	    titleTex_, {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	titleLogoState_ = B_In;
	titleLogo_t_ = 0.0f;
	isTitleLogoGimmick = false;
#pragma endregion

#pragma region[始める]
	// [始める]テクスチャの読み込み
	startButtonTex_[0] = TextureManager::Load("StartButton.png");

	// [始める]選択テクスチャの読み込み
	startButtonTex_[1] = TextureManager::Load("SelectStartButton.png");

	// [始める]の初期化処理
	startButton_.reset(Sprite::Create(
	    startButtonTex_[0], {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	startButton_t_ = 0.0f;
	isStart = false;
	startButtonState_ = B_In;
	isStartButtonGimmick = false;
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
	endButton_t_ = 0.0f;
	endButtonState_ = B_In;
	isEnd_ = false;
	isEndButtonGimmick = false;
#pragma endregion

#pragma region 黒画面
	// [始める]テクスチャの読み込み
	blackoutTex_ = TextureManager::Load("BlackOut.png");
	blackoutAlpha_ = 1.0f;

	// [始める]の初期化処理
	blackout_.reset(Sprite::Create(
	    blackoutTex_, {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

#pragma endregion

	// グローバル変数の初期化処理
	InitializeGlobalVariavles();
}

/// <summary>
/// 更新処理
/// </summary>
void TitleScene::Update() { 
	// コントローラー
	XINPUT_STATE joystate;

	// グローバル変数の取得
	ApplyGlobalVariavles();

	// BGM再生
	if (audio_->IsPlaying(BGMHandle_) == 0 || BGMHandle_ == -1) {
		BGMHandle_ = audio_->PlayWave(BGMHandle_, true, bolume);
	}


	// もしカーソルが表示されていないなら表示
	if (i <= 0) {
		i = ShowCursor(true);
	}

	// タイトルのギミック管理
	TitleGimmick();


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

		// もしギミックが最後までいったら
		if (isEnd_startButtonGimmick && isEnd_endButtonGimmick && isEnd_titleLogoGimmick) {
			audio_->StopWave(BGMHandle_);
			// ゲームシーンへ
			scenedNo_ = GAME_S;
		}
	}

	// それぞれ位置、α値の設定
	blackout_->SetColor({1.0f, 1.0f, 1.0f, blackoutAlpha_});
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

	blackout_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
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

	gloVars->AddItem(groupName, "Start Button Pos Root S", startButton_root_S_pos_);
	gloVars->AddItem(groupName, "Start Button Pos Root E", startButton_root_E_pos_);

	gloVars->AddItem(groupName, "Start Button Pos Out S", startButton_out_S_pos_);
	gloVars->AddItem(groupName, "Start Button Pos Out E", startButton_out_E_pos_);

	gloVars->AddItem(groupName, "Title_Logo_In_S", titleLogo_in_S_pos_);
	gloVars->AddItem(groupName, "Title_Logo_In_E", titleLogo_in_E_pos_);

	gloVars->AddItem(groupName, "Title_Logo_Root_S", titleLogo_root_S_pos_);
	gloVars->AddItem(groupName, "Title_Logo_Root_E", titleLogo_root_E_pos_);

	gloVars->AddItem(groupName, "Title_Logo_Out_S", titleLogo_out_S_pos_);
	gloVars->AddItem(groupName, "Title_Logo_Out_E", titleLogo_out_E_pos_);

	gloVars->AddItem(groupName, "End Button Pos In S", endButton_in_S_pos_);
	gloVars->AddItem(groupName, "End Button Pos In E", endButton_in_E_pos_);

	gloVars->AddItem(groupName, "End Button Pos Root S", endButton_root_S_pos_);
	gloVars->AddItem(groupName, "End Button Pos Root E", endButton_root_E_pos_);

	gloVars->AddItem(groupName, "End Button Pos Out S", endButton_out_S_pos_);
	gloVars->AddItem(groupName, "End Button Pos Out E", endButton_out_E_pos_);
	
	gloVars->AddItem(groupName, "bolume", bolume);
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

	titleLogo_root_S_pos_ = gloVars->GetVector2Value(groupName, "Title_Logo_Root_S");
	titleLogo_root_E_pos_ = gloVars->GetVector2Value(groupName, "Title_Logo_Root_E");

	titleLogo_out_S_pos_ = gloVars->GetVector2Value(groupName, "Title_Logo_Out_S");
	titleLogo_out_E_pos_ = gloVars->GetVector2Value(groupName, "Title_Logo_Out_E");


	startButton_in_S_pos_ = gloVars->GetVector2Value(groupName, "Start Button Pos In S");
	startButton_in_E_pos_ = gloVars->GetVector2Value(groupName, "Start Button Pos In E");

	startButton_root_S_pos_ = gloVars->GetVector2Value(groupName, "Start Button Pos Root S");
	startButton_root_E_pos_ = gloVars->GetVector2Value(groupName, "Start Button Pos Root E");

	startButton_out_S_pos_ = gloVars->GetVector2Value(groupName, "Start Button Pos Out S");
	startButton_out_E_pos_ = gloVars->GetVector2Value(groupName, "Start Button Pos Out E");


	endButton_in_S_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos In S");
	endButton_in_E_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos In E");
	
	endButton_root_S_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos Root S");
	endButton_root_E_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos Root E");
	
	endButton_out_S_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos Out S");
	endButton_out_E_pos_ = gloVars->GetVector2Value(groupName, "End Button Pos Out E");

	bolume = gloVars->GetFloatValue(groupName, "bolume");
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

	// [始める]ボタンの処理
	KeyMouse_ButtonUpdate(
	    startButtonPos_, startButtonSize, isStartSelect, isStart, startButton_.get(), startButtonTex_);
	
	// [終わる]ボタンの処理
	KeyMouse_ButtonUpdate(
		endButtonPos_, endButtonSize, isEndSelect, isEnd_, endButton_.get(), endButtonTex_);

}

/// <summary>
/// コントローラー時の更新処理
/// </summary>
void TitleScene::JoyStateUpdate() {
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
				audio_->PlayWave(selectSEHandle_, false, bolume);
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

		// [始める]ボタンの処理
		JoyState_ButtonUpdate(
		    START, startButton_.get(), endButton_.get(), startButtonTex_[1], endButtonTex_[0],
		    isStart);

		// [終わる]ボタンの処理
		JoyState_ButtonUpdate(
		    END, startButton_.get(), endButton_.get(), startButtonTex_[0], endButtonTex_[1],
		    isEnd_);

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
bool TitleScene::ButtonToCurSor(Vector2 buttonPos, Vector2 buttonSize) {
	POINT cursorPos;
	// 現在のカーソル位置を取得
	GetCursorPos(&cursorPos);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &cursorPos);

	// もしカーソルが範囲内だったら
	if ((float)cursorPos.x > buttonPos.x - (buttonSize.x / 2) &&
	    (float)cursorPos.x < buttonPos.x + (buttonSize.x / 2) &&
	    (float)cursorPos.y > buttonPos.y - (buttonSize.y / 2) &&
	    (float)cursorPos.y < buttonPos.y + (buttonSize.y / 2)) {
		return true;
	}
	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="buttonPos"></param>
/// <param name="buttonSize"></param>
/// <param name="isSelect"></param>
/// <param name="whatDO"></param>
/// <param name="sprite"></param>
/// <param name="texture"></param>
void TitleScene::KeyMouse_ButtonUpdate(
    Vector2 buttonPos, Vector2 buttonSize, bool& isSelect, bool& whatDO, Sprite* sprite, uint32_t texture[2]) {
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

/// <summary>
/// 
/// </summary>
/// <param name="selectState"></param>
/// <param name="start"></param>
/// <param name="end"></param>
/// <param name="startTex"></param>
/// <param name="endTex"></param>
/// <param name="whatDO"></param>
void TitleScene::JoyState_ButtonUpdate(
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

/// <summary>
/// 
/// </summary>
void TitleScene::StartButtonGimmick() {
	if (!isStartButtonGimmick) {
		isEnd_startButtonGimmick = false;
		startButton_t_ = 0.0f;
		startButtonState_ = B_In;
	} 
	//
	else if (isStartButtonGimmick) {
		if (startButtonState_ == B_In) {
			startButtonPos_.x = MyMath::EaseOutQuadF(
			    startButton_t_, startButton_in_S_pos_.x, startButton_in_E_pos_.x);
			startButtonPos_.y = MyMath::EaseOutQuadF(
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
			startButtonPos_.x = MyMath::EaseOutQuadF(
			    startButton_t_, startButton_root_S_pos_.x, startButton_root_E_pos_.x);
			startButtonPos_.y = MyMath::EaseOutQuadF(
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
			startButtonPos_.x = MyMath::EaseInQuadF(
			    startButton_t_, startButton_out_S_pos_.x, startButton_out_E_pos_.x);
			startButtonPos_.y = MyMath::EaseInQuadF(
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

/// <summary>
/// 
/// </summary>
void TitleScene::EndButtonGimmick() {
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

			if (isStart) {
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

/// <summary>
/// 
/// </summary>
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
			titleLogoPos_.x = MyMath::EaseOutCubicF(
			    titleLogo_t_, titleLogo_root_S_pos_.x, titleLogo_root_E_pos_.x);
			titleLogoPos_.y = MyMath::EaseOutCubicF(
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
				titleLogo_out_S_pos_ = titleLogoPos_;
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

			blackoutAlpha_ = titleLogo_t_;
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

/// <summary>
/// 
/// </summary>
void TitleScene::TitleGimmick() { 
	//
	if (!isTitleGimmick_) {
		if (blackoutAlpha_ > 0.0f) {
			blackoutAlpha_ -= blackoutAlpha_offset_;
		}
		//
		else if (blackoutAlpha_ <= 0.0f) {
			blackoutAlpha_ = 0.0f;
		}

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
			isTitleGimmick_ = true;
		}
		//
		else if (titleGimmickTimer_ < 1.0f) {
			titleGimmickTimer_ += 0.01f;
		}
	}
	//
	else if (isTitleGimmick_) {
		
	}

	//
	TitleLogoGimmick();
	StartButtonGimmick();
	EndButtonGimmick();
}

