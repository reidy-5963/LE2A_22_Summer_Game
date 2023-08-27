#include "GameScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

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

void GameScene::Initialize() {
	// ベース部分の初期化処理
	BaseScene::Initialize();

	// このシーン番号の設定
	scenedNo_ = GAME_S;

	// ビュープロジェクションの初期化処理
	viewProjection_.Initialize();

	// ポーズモードフラグの初期化処理
	isPoseMode_ = false;

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
	heartTex_ = TextureManager::Load("heart.png");
	reticleTex_ = TextureManager::Load("reticle.png");

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
	player_->SetHeart(heartTex_);

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
	//enemy_ = std::make_unique<Enemy>();
	// プレイヤーのモデルの生成
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
	//enemy_->Initialize(enemyModels);
	//enemy_->SetPlayer(player_.get());

	e_bullet_model_.reset((Model::CreateFromOBJ("Bullet", true)));
	e_bullet_models = {e_bullet_model_.get()};
	LoadEnemyPopData();
#pragma endregion
}

void GameScene::Update() {
	XINPUT_STATE joyState;

	// もしポーズ中じゃなかったら
	if (!isPoseMode_) {
		if (input_->GetJoystickState(0, joyState)) {
			ShowCursor(true);
		} else {
			if (controlMouse) {
				ShowCursor(false);
				controlMouse = false;
			}
		}
		ImGui::Begin("GameScene");
		ImGui::Text("press esc -> pose");
		ImGui::End();

		if (input_->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_START) {
				isPoseMode_ = true;
			}
		} else {
			if (input_->TriggerKey(DIK_ESCAPE)) {
				isPoseMode_ = true;
			}
		}

#pragma region 追従カメラ
		// 追従カメラの更新処理
		followCamera_->Update();
		// 行列に追従カメラの行列を代入
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
#pragma endregion

		// 天球の更新処理
		skydome_->Update();

		// 地面の更新処理
		ground_->Update();

		// プレイヤーの更新処理
		player_->Update();
		UpdateEnemyPopCommands();
		//
		//enemy_->Update();
		enemys_.remove_if([](Enemy* enemy) {
			if (enemy->IsDead()) {
				delete enemy;
				return true;
			}
			return false;
		});
		for (Enemy* enemy : enemys_) {
			enemy->Update();
		}

		enemyBullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Update();
		}

		// ビュープロジェクションを転送する
		viewProjection_.TransferMatrix();

		CheckAllCollisions();
		followCamera_->SetIsDamage(player_->IsDamage());
		followCamera_->SetRotation(player_->GetWorldTransform().rotation_);
	} 
	// もしポーズ中なら
	else if (isPoseMode_) {
		if (input_->GetJoystickState(0, joyState)) {
			ShowCursor(true);
		} else {
			if (!controlMouse) {
				ShowCursor(true);
				controlMouse = true;
			}
		}

		ImGui::Begin("GameScene");
		ImGui::Text("press esc -> game");
		ImGui::Text("press 1 -> Title");
		ImGui::End();

		XINPUT_STATE joystate;
		if (input_->GetJoystickState(0, joystate)) {
			if (joystate.Gamepad.wButtons == XINPUT_GAMEPAD_START) {
				isPoseMode_ = false;
			}
		} else {
			if (input_->TriggerKey(DIK_ESCAPE)) {
				isPoseMode_ = false;
			}
		}

		if (input_->TriggerKey(DIK_1)) {
			scenedNo_ = TITLE_S;
		}
	}
}

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

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

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
			//player_->OnCollision(enemy->GetDamage());
		}
	}
#pragma endregion
#pragma region 敵弾とプレイヤー
	for (EnemyBullet* enemyBullet: enemyBullets_) {
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::AddEnemy(Enemy* enemy) {
	//
	enemys_.push_back(enemy);
}

void GameScene::EnemyIni(const std::vector<Model*>& models, const Vector3 position) {
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(models, position);
	newEnemy->SetPlayer(player_.get());
	newEnemy->SetGameScene(this);
	newEnemy->SetBulletModel(e_bullet_models);
	AddEnemy(newEnemy);
}

void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}
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

			EnemyIni(enemyModels_, Vector3(x, y, z));
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWait_ = true;
			waitTimer_ = waitTime;
			break;
		}
	}
}
