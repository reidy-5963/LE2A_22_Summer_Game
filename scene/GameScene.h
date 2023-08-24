#pragma once

#include "BaseScene.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "FollowCamera.h"
#include "skydome.h"
#include "Ground.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	//void AddPlayerBullet(PlayerBullet* bullet);

private: // メンバ変数
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// ビュープロジェクション
	ViewProjection viewProjection_;

#pragma region 天球
	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	// 天球のモデル
	std::unique_ptr<Model> skydomeModel_ = nullptr;
#pragma endregion

#pragma region 地面
	// 地面
	std::unique_ptr<Ground> ground_ = nullptr;
	// 地面のモデル
	std::unique_ptr<Model> groundModel_ = nullptr;
#pragma endregion

#pragma region プレイヤー

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	// プレイヤーのモデル
	std::unique_ptr<Model> p_body_model_ = nullptr;
	std::unique_ptr<Model> p_head_model_ = nullptr;
	std::unique_ptr<Model> p_l_arm_model_ = nullptr;
	std::unique_ptr<Model> p_r_arm_model_ = nullptr;
	std::unique_ptr<Model> p_wepon_model_ = nullptr;
#pragma endregion

#pragma region プレイヤーの武器

	// 
	//std::list<std::unique_ptr<PlayerBullet>> p_bullets_;
	// プレイヤーの武器モデル
	std::unique_ptr<Model> p_gun_model_ = nullptr;
	std::unique_ptr<Model> p_bullet_model_ = nullptr;

	
	uint32_t reticleTex_;
	uint32_t heartTex_;

#pragma endregion

#pragma region 追従カメラ
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
#pragma endregion

	
#pragma region 敵
	//
	std::unique_ptr<Enemy> enemy_ = nullptr;
	//
	std::unique_ptr<Model> e_body_model_ = nullptr;
	std::unique_ptr<Model> e_l_wepon_model_ = nullptr;
	std::unique_ptr<Model> e_r_wepon_model_ = nullptr;


#pragma endregion
	bool isPoseMode_ = false;
	bool controlMouse = true;
};
