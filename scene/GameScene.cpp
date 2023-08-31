#include "GameScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>
#include "GlobalVariables.h"

/// <summary>
/// コンストクラタ
/// </summary>
GameScene::GameScene() {}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() {
	// 敵の開放
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	// 敵弾の開放
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}

/// <summary>
/// 初期化処理
/// </summary>
void GameScene::Initialize() {
	// ベース部分の初期化処理
	BaseScene::Initialize();
	
	enemys_.resize(0);

	// このシーン番号の設定
	scenedNo_ = GAME_S;

	uint32_t handle = Audio::GetInstance()->LoadWave("PLAY.wav");

	if (Audio::GetInstance()->IsPlaying(handle)) {
		Audio::GetInstance()->StopWave(handle);
	}
	selectSEHandle_ = Audio::GetInstance()->LoadWave("select01.wav");

	BGMHandle_ = Audio::GetInstance()->LoadWave("Nostalgia.wav");

	// ビュープロジェクションの初期化処理
	viewProjection_.Initialize();

	// ポーズモードフラグの初期化処理
	isNotPose_ = true;
	isEnemyEnd_ = false;

#pragma region 天球
	// 天球モデルの生成
	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	// 天球の初期化処理
	skydome_->Initialize(skydomeModel_.get(), {0.0f, 0.0f, 0.0f});
#pragma endregion

#pragma region 地面
	// 地面モデルの生成
	groundModel_.reset(Model::CreateFromOBJ("Ground", true));
	// 地面の生成
	ground_ = std::make_unique<Ground>();
	// 地面の初期化処理
	ground_->Initialize(groundModel_.get(), {0.0f, 0.0f, 0.0f});
#pragma endregion

#pragma region プレイヤー
	// 各Sprite用のテクスチャの読み込み
	hp_root_tex_ = TextureManager::Load("Hp_Root_Tex.png");
	hp_gauge_root_tex_ = TextureManager::Load("Hp_Gauge_Root_Tex.png");
	hp_out_tex_ = TextureManager::Load("Hp_Out_Tex.png");
	hp_gauge_out_tex_ = TextureManager::Load("Hp_Gauge_Out_Tex.png");
	
	reticleTex_ = TextureManager::Load("reticle.png");

	// 各Sprite用のテクスチャの読み込み
	damageEffectTex_ = TextureManager::Load("Damage_Scene.png");


	// プレイヤーの生成
	player_ = std::make_unique<Player>();
	// プレイヤーのモデルの生成
	p_body_model_.reset(Model::CreateFromOBJ("Player_Body", true));
	p_head_model_.reset(Model::CreateFromOBJ("Player_Head", true));
	p_l_arm_model_.reset(Model::CreateFromOBJ("Player_L_Arm", true));
	p_r_arm_model_.reset(Model::CreateFromOBJ("Player_R_Arm", true));
	p_wepon_model_.reset(Model::CreateFromOBJ("wepon", true));
	// プレイヤーのモデルを配列に
	std::vector<Model*> playerModels = {
	    p_body_model_.get(),  p_head_model_.get(),  p_l_arm_model_.get(),
	    p_r_arm_model_.get(), p_wepon_model_.get(),
	};
	player_->SetReticle(reticleTex_);
	player_->SetHeart(hp_root_tex_, hp_gauge_root_tex_, hp_out_tex_, hp_gauge_out_tex_);
	player_->SetDamageEffect(damageEffectTex_);
	// プレイヤーの初期化処理
	player_->Initialize(playerModels);

	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	// 追従カメラの初期化処理
	followCamera_->Initialize();
	// 追従カメラのターゲット設定
	followCamera_->SetTarget(&player_->GetWorldTransform());
	// プレイヤーにビュープロジェクションを設定
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	p_bullet_model_.reset((Model::CreateFromOBJ("Bullet", true)));
	std::vector<Model*> p_bullet_models = {p_bullet_model_.get()};

	player_->SetBulletModel(p_bullet_models);

#pragma endregion

#pragma region 敵
	// enemy_ = std::make_unique<Enemy>();
	//  プレイヤーのモデルの生成
	e_body_model_.reset(Model::CreateFromOBJ("Enemy_Body", true));
	e_head_model_.reset(Model::CreateFromOBJ("Enemy_Head", true));
	e_l_arm_model_.reset(Model::CreateFromOBJ("Enemy_L_Arm", true));
	e_r_arm_model_.reset(Model::CreateFromOBJ("Enemy_R_Arm", true));
	// プレイヤーのモデルを配列に
	enemyModels_ = {
	    e_body_model_.get(),
	    e_head_model_.get(),
	    e_l_arm_model_.get(),
	    e_r_arm_model_.get(),
	};
	// プレイヤーの初期化処理
	// enemy_->Initialize(enemyModels);
	// enemy_->SetPlayer(player_.get());

	e_bullet_model_.reset((Model::CreateFromOBJ("Bullet", true)));
	e_bullet_models = {e_bullet_model_.get()};
	LoadEnemyPopData();

#pragma endregion

#pragma region 黒画面
	// 各Sprite用のテクスチャの読み込み
	blackoutTex_ = TextureManager::Load("BlackOut.png");

	// [始める]の初期化処理
	blackout_.reset(Sprite::Create(
	    blackoutTex_, {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	blackoutAlpha_ = 1.0f;
#pragma endregion


#pragma region [タイトルに戻る]
	// [タイトルに戻る]テクスチャの読み込み
	menu_backTitle_Tex_[0] = TextureManager::Load("PoseMode_TitleBack_Tex.png");
	// [タイトルに戻る]テクスチャの読み込み
	menu_backTitle_Tex_[1] = TextureManager::Load("PoseMode_Select_TitleBack_Tex.png");

	// [タイトルに戻る]の初期化処理
	menu_backTitle_.reset(Sprite::Create(
	    menu_backTitle_Tex_[0], {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	isTitle_ = false;
#pragma endregion
#pragma region [プレイに戻る]
	// [プレイに戻る]テクスチャの読み込み
	menu_backPlay_Tex_[0] = TextureManager::Load("PoseMode_PlayBack_Tex.png");
	// [プレイに戻る]テクスチャの読み込み
	menu_backPlay_Tex_[1] = TextureManager::Load("PoseMode_Select_PlayBack_Tex.png");

	// [プレイに戻る]の初期化処理
	menu_backPlay_.reset(Sprite::Create(
	    menu_backPlay_Tex_[0], {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
#pragma endregion

#pragma region チュートリアル
	// [タイトルに戻る]テクスチャの読み込み
	tutorial_tex_[Base] = TextureManager::Load("Tutorial_Base.png");
	tutorial_tex_[Joy_Base] = TextureManager::Load("Tutorial_Base_Joy.png");
	tutorial_tex_[Mouse_Base] = TextureManager::Load("Tutorial_Base_Mause.png");
	tutorial_tex_[Joy_Left] = TextureManager::Load("Tutorial_Joy_Left.png");
	tutorial_tex_[Joy_Right] = TextureManager::Load("Tutorial_Joy_Right.png");
	tutorial_tex_[Joy_RB] = TextureManager::Load("Tutorial_Joy_RB.png");
	tutorial_tex_[Joy_Start] = TextureManager::Load("Tutorial_Joy_Start.png");
	tutorial_tex_[Mouse_W] = TextureManager::Load("Tutorial_Mause_W.png");
	tutorial_tex_[Mouse_A] = TextureManager::Load("Tutorial_Mause_A.png");
	tutorial_tex_[Mouse_S] = TextureManager::Load("Tutorial_Mause_S.png");
	tutorial_tex_[Mouse_D] = TextureManager::Load("Tutorial_Mause_D.png");
	tutorial_tex_[Mouse_ESC] = TextureManager::Load("Tutorial_Mause_ESC.png");
	tutorial_tex_[Mouse_Left] = TextureManager::Load("Tutorial_Mause_Left.png");



	for (int j = 0; j < tutorialNum; j++) {
		// [タイトルに戻る]の初期化処理
		tutorial_[j].reset(Sprite::Create(
		    tutorial_tex_[j], {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2)},
		    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	}
#pragma endregion

	InitializeGlobalVariavles();
	isStart_ = false;
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {
	ApplyGlobalVariavles();
	XINPUT_STATE joyState;

	// BGM再生
	if (audio_->IsPlaying(BGMHandle_) == 0 || BGMHandle_ == -1) {
		BGMHandle_ = audio_->PlayWave(BGMHandle_, true, bolume);
	}


	// もしポーズ中じゃなかったら
	if (isNotPose_) {
		// もしコントローラーなら
		if (input_->GetJoystickState(0, joyState)) {
			if (i < 0) {
				i = ShowCursor(true);
			}
			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_START &&
			    preJoyButton_ != XINPUT_GAMEPAD_START) {
				isNotPose_ = false;
			}
		} 
		// もしキーマウなら
		else {
			if (i >= 0) {
				i = ShowCursor(false);
			}
			if (input_->TriggerKey(DIK_ESCAPE)) {
				isNotPose_ = false;
			}
		}

		// 天球の更新処理
		skydome_->Update();

		// 地面の更新処理
		ground_->Update();

		// プレイヤーの更新処理
		player_->SetIsFps(followCamera_->IsFps());
		player_->Update();

#pragma region 追従カメラ
		// 追従カメラの更新処理
		followCamera_->Update();
		// 行列に追従カメラの行列を代入
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
#pragma endregion

		//
		if (isStart_) {
			UpdateEnemyPopCommands();
		}
		// 
		else if (!isStart_) {
			if (blackoutAlpha_ <= 0.0f) {
				isStart_ = true;
				blackoutAlpha_ = 0.0f;
			}
			//
			else if (blackoutAlpha_ > 0.0f) {
				blackoutAlpha_ -= blackoutAlpha_offset_;
			}
		}

		// 
		enemys_.remove_if([](Enemy* enemy) {
			if (enemy->IsDead()) {
				delete enemy;
				return true;
			}
			return false;
		});
		//
		for (Enemy* enemy : enemys_) {
			enemy->Update();
		}

		//
		enemyBullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});
		//
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Update();
		}

		// ビュープロジェクションを転送する
		viewProjection_.TransferMatrix();

		//
		CheckAllCollisions();
		//
		followCamera_->SetIsDamage(player_->IsDamage());
		//
		followCamera_->SetRotation(player_->GetWorldTransform().rotation_);

		//
		if (player_->IsDead()) {
			if (audio_->IsPlaying(BGMHandle_)) {
				audio_->StopWave(BGMHandle_);
			}
			if (i <= 0) {
				i = ShowCursor(true);
			}

			scenedNo_ = OVER_S;

		}
		//
		if (isEnemyEnd_ && enemys_.size() == 0) {
			if (audio_->IsPlaying(BGMHandle_)) {
				audio_->StopWave(BGMHandle_);
			}
			if (i <= 0) {
				i = ShowCursor(true);
			}

			scenedNo_ = CLEAR_S;

		}

	}
	// もしポーズ中なら
	else if (!isNotPose_) {
		blackoutAlpha_ = 0.5f;

		if (i <= 0) {
			i = ShowCursor(true);
		}

		if (input_->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_START &&
				preJoyButton_ != XINPUT_GAMEPAD_START) {
				isNotPose_ = true;
				blackoutAlpha_ = 0.0f;
			}

			JoyStateUpdate();
			if (isNotPose_) {
				blackoutAlpha_ = 0.0f;
			}

		} 
		else {
			if (input_->TriggerKey(DIK_ESCAPE)) {
				blackoutAlpha_ = 0.0f;
				isNotPose_ = true;
			}
			// 現在のカーソル位置を取得
			GetCursorPos(&mousePos_);

			// クライアントエリア座標に変換する
			HWND hwnd = WinApp::GetInstance()->GetHwnd();
			ScreenToClient(hwnd, &mousePos_);
			// [始める]のサイズ取得
			Vector2 backTitleSize = menu_backTitle_->GetSize();

			KeyMouse_ButtonUpdate(
			    menu_backTitle_pos_, backTitleSize, isSelect_backTitle_, isTitle_, menu_backTitle_.get(),
			    menu_backTitle_Tex_);

			KeyMouse_ButtonUpdate(
			    menu_backPlay_pos_, backTitleSize, isSelect_backPlay_, isNotPose_,
			    menu_backPlay_.get(), menu_backPlay_Tex_);
			if (isNotPose_) {
				blackoutAlpha_ = 0.0f;
			}
		}
		if (isTitle_) {
			if (audio_->IsPlaying(BGMHandle_)) {
				audio_->StopWave(BGMHandle_);
			}
			scenedNo_ = TITLE_S;
		}
	}
	//ImGui::End();
	menu_backPlay_->SetPosition(menu_backPlay_pos_);
	menu_backTitle_->SetPosition(menu_backTitle_pos_);
	blackout_->SetColor({1.0f, 1.0f, 1.0f, blackoutAlpha_});

	if (input_->GetJoystickState(0, joyState)) {
		preJoyButton_ = joyState.Gamepad.wButtons;
	}
	for (int j = 0; j < tutorialNum; j++) {
		tutorial_[j]->SetPosition(tutorialPos_);
	}

	if (--tutAnime_timer_ < 0) {
		if (isAnimation_)
			isAnimation_ = false;
		else if (!isAnimation_)
			isAnimation_ = true;

		tutAnime_timer_ = setTime_tutAnime_;
	}



}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw() {

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

	// 天球の描画処理
	skydome_->Draw(viewProjection_);
	// 地面の描画処理
	ground_->Draw(viewProjection_);
	// プレイヤーの描画処理
	player_->Draw(viewProjection_);
	//
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();

	blackout_->Draw();

	if (!isNotPose_) {
		menu_backTitle_->Draw();
		menu_backPlay_->Draw();
	}
	XINPUT_STATE joyState;
	tutorial_[Base]->Draw();
	if (input_->GetJoystickState(0, joyState)) {
		tutorial_[Joy_Base]->Draw();
		tutorial_[Joy_Left]->Draw();
		tutorial_[Joy_Right]->Draw();
		if (isAnimation_ || joyState.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			tutorial_[Joy_RB]->Draw();
		}
		if (isAnimation_ || joyState.Gamepad.wButtons == XINPUT_GAMEPAD_START) {
			tutorial_[Joy_Start]->Draw();
		}
	}
	//
	else {
		tutorial_[Mouse_Base]->Draw();
		if (isAnimation_ || input_->PushKey(DIK_W)) {
			tutorial_[Mouse_W]->Draw();
		}
		if (isAnimation_ || input_->PushKey(DIK_A)) {
			tutorial_[Mouse_A]->Draw();
		}
		if (isAnimation_ || input_->PushKey(DIK_S)) {
			tutorial_[Mouse_S]->Draw();
		}
		if (isAnimation_ || input_->PushKey(DIK_D)) {
			tutorial_[Mouse_D]->Draw();
		}
		if (isAnimation_ || input_->PushKey(DIK_ESCAPE)) {
			tutorial_[Mouse_ESC]->Draw();
		}
		if (isAnimation_ || input_->IsPressMouse(0)) {
			tutorial_[Mouse_Left]->Draw();
		}
	}



	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::InitializeGlobalVariavles() {
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	const char* groupName = "GameScene";
	//
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "menu_backTitle_pos_", menu_backTitle_pos_);
	gloVars->AddItem(groupName, "menu_backPlay_pos_", menu_backPlay_pos_);
	gloVars->AddItem(groupName, "tutorialPos_", tutorialPos_);
}

void GameScene::ApplyGlobalVariavles() {
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "GameScene";
	// 作ったグループにあるアイテムから値を取得
	menu_backTitle_pos_ = gloVars->GetVector2Value(groupName, "menu_backTitle_pos_");
	menu_backPlay_pos_ = gloVars->GetVector2Value(groupName, "menu_backPlay_pos_");
	tutorialPos_ = gloVars->GetVector2Value(groupName, "tutorialPos_");
}

/// <summary>
/// 全ての当たり判定処理
/// </summary>
void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

#pragma region 自弾と敵
	for (PlayerBullet* playerBullet : playerBullets) {
		for (Enemy* enemy : enemys_) {
			posB = playerBullet->GetWorldPosition();

			posA = enemy->GetWorldPosition();
			posA.y = enemy->GetWorldPosition().y + 2.5f;

			float Judge = (posB.x - posA.x) * (posB.x - posA.x) +
			              (posB.y - posA.y) * (posB.y - posA.y) +
			              (posB.z - posA.z) * (posB.z - posA.z);

			float bulletRad = playerBullet->GetRadius();
			float enemyRad = enemy->GetRadius();
			if (Judge <= (bulletRad + enemyRad) * (bulletRad + enemyRad)) {
				playerBullet->OnCollision();
				enemy->OnCollision(playerBullet->GetDamage());
			}
		}
	}

#pragma endregion
#pragma region 敵とプレイヤー
	for (Enemy* enemy : enemys_) {
		posB = player_->GetWorldPosition();
		posB.y = player_->GetWorldPosition().y + 2.5f;
		posA = enemy->GetWorldPosition();
		posA.y = enemy->GetWorldPosition().y + 2.5f;

		float Judge = (posB.x - posA.x) * (posB.x - posA.x) +
		              (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z);

		float bulletRad = player_->GetRadius();
		float enemyRad = enemy->GetRadius();
		if (Judge <= (bulletRad + enemyRad) * (bulletRad + enemyRad)) {
			// player_->OnCollision(enemy->GetDamage());
		}
	}
#pragma endregion
#pragma region 敵弾とプレイヤー
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		posB = player_->GetWorldPosition();
		posB.y = player_->GetWorldPosition().y + 2.5f;
		posA = enemyBullet->GetWorldPosition();

		float Judge = (posB.x - posA.x) * (posB.x - posA.x) +
		              (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z);

		float bulletRad = player_->GetRadius();
		float enemyRad = enemyBullet->GetRadius();
		if (Judge <= (bulletRad + enemyRad) * (bulletRad + enemyRad)) {
			player_->OnCollision(enemyBullet->GetDamage());
		}
	}

#pragma endregion
}

/// <summary>
/// 敵弾の追加
/// </summary>
/// <param name="enemyBullet">敵弾</param>
void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//
	enemyBullets_.push_back(enemyBullet);
}

/// <summary>
/// 敵の追加
/// </summary>
/// <param name="enemy">敵</param>
void GameScene::AddEnemy(Enemy* enemy) {
	//
	enemys_.push_back(enemy);
}

/// <summary>
/// 敵の初期化処理
/// </summary>
/// <param name="models">敵のモデル</param>
/// <param name="position">敵の位置</param>
void GameScene::EnemyIni(
    const std::vector<Model*>& models, const Vector3 position, bool isLastEnemy) {
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(models, position, isLastEnemy);
	newEnemy->SetPlayer(player_.get());
	newEnemy->SetGameScene(this);
	newEnemy->SetBulletModel(e_bullet_models);
	AddEnemy(newEnemy);
}

/// <summary>
/// 敵ポップデータの読み込み
/// </summary>
void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

/// <summary>
/// 敵ポップデータの更新処理
/// </summary>
void GameScene::UpdateEnemyPopCommands() {
	if (isWait_) {
		waitTimer_--;

		if (waitTimer_ <= 0) {
			isWait_ = false;
		}
		return;
	}

	std::string line;
	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		} else if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			EnemyIni(enemyModels_, Vector3(x, y, z), false);
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWait_ = true;
			waitTimer_ = waitTime;
			break;
		}

		else if (word.find("ENDPOP")) {
			isEnemyEnd_ = true;

			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			EnemyIni(enemyModels_, Vector3(x, y, z), true);
		}
	}
}

bool GameScene::ButtonToCurSor(Vector2 buttonPos, Vector2 buttonSize) {
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

void GameScene::KeyMouse_ButtonUpdate(
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

void GameScene::JoyStateUpdate() {
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
					selectButton = BACKPLAY;
				}
				// もし選択がエンドなら
				else if (selectButton == BACKPLAY) {
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
					selectButton = BACKPLAY;
				}
				// もし選択がエンドなら
				else if (selectButton == BACKPLAY) {
					// スタートに
					selectButton = BACKTITLE;
				}
			}
		}

		JoyState_ButtonUpdate(
		    BACKTITLE, menu_backTitle_.get(), menu_backPlay_.get(), menu_backTitle_Tex_[1],
		    menu_backPlay_Tex_[0],
		    isTitle_);

		JoyState_ButtonUpdate(
		    BACKPLAY, menu_backTitle_.get(), menu_backPlay_.get(), menu_backTitle_Tex_[0],
		    menu_backPlay_Tex_[1],
		    isNotPose_);

		// 前フレームの状態を保存
		preJoyButton_ = joystate.Gamepad.wButtons;
	}
}

void GameScene::JoyState_ButtonUpdate(
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
