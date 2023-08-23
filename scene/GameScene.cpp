#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	//
	BaseScene::Initialize();

	scenedNo_ = GAME_S;

	// ビュープロジェクションの初期化処理
	viewProjection_.Initialize();
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
	// プレイヤーの生成
	player_ = std::make_unique<Player>();
	// プレイヤーのモデルの生成
	p_body_model_.reset(Model::CreateFromOBJ("TestPlayerver_body", true));
	p_head_model_.reset(Model::CreateFromOBJ("TestPlayerver_head", true));
	p_l_arm_model_.reset(Model::CreateFromOBJ("TestPlayerver_l_arm", true));
	p_r_arm_model_.reset(Model::CreateFromOBJ("TestPlayerver_r_arm", true));
	p_wepon_model_.reset(Model::CreateFromOBJ("wepon", true));
	// プレイヤーのモデルを配列に
	std::vector<Model*> playerModels = {
	    p_body_model_.get(), 
		p_head_model_.get(),
		p_l_arm_model_.get(),
	    p_r_arm_model_.get(), 
		p_wepon_model_.get(),
	};
	// プレイヤーの初期化処理
	player_->Initialize(playerModels);
	heartTex_ = TextureManager::Load("heart.png");
	p_heart_.reset(Sprite::Create(heartTex_, {0.0f, 0.0f}));


	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	// 追従カメラの初期化処理
	followCamera_->Initialize();
	// 追従カメラのターゲット設定
	followCamera_->SetTarget(&player_->GetWorldTransform());
	// プレイヤーにビュープロジェクションを設定
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	p_bullet_model_.reset((Model::Create()));
	std::vector<Model*> p_bullet_models = {p_bullet_model_.get()};


	player_->SetBulletModel(p_bullet_models);

#pragma endregion
}

void GameScene::Update() { 


	if (!isPoseMode_) {
		ImGui::Begin("GameScene");
		ImGui::Text("press esc -> pose");
		ImGui::End();


		if (input_->TriggerKey(DIK_ESCAPE)) {
			isPoseMode_ = true;
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


		// ビュープロジェクションを転送する
		viewProjection_.TransferMatrix();
	} 
	else if (isPoseMode_) {
		ImGui::Begin("GameScene");
		ImGui::Text("press esc -> game");
		ImGui::Text("press 1 -> Title");
		ImGui::End();

		if (input_->TriggerKey(DIK_ESCAPE)) {
			isPoseMode_ = false;
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

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	p_heart_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//void GameScene::AddPlayerBullet(PlayerBullet* bullet) {
	//
	//std::unique_ptr<PlayerBullet> newbullet;
	//newbullet.reset(bullet);
	//p_bullets_.push_back(newbullet);
//}
