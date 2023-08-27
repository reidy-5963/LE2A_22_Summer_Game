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

	/// <summary>
	/// 全ての当たり判定処理
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾の追加
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵の追加
	/// </summary>
	/// <param name="enemy"></param>
	void AddEnemy(Enemy* enemy);

	/// <summary>
	/// 敵の初期化処理
	/// </summary>
	/// <param name="models"></param>
	/// <param name="position"></param>
	void EnemyIni(const std::vector<Model*>& models, const Vector3 position);

	/// <summary>
	/// 敵ポップデータの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵ポップデータの更新処理
	/// </summary>
	void UpdateEnemyPopCommands();

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

	std::stringstream enemyPopCommands;
	bool isWait_ = false;
	int32_t waitTimer_ = 0;

#pragma region 敵
	//
	std::list<Enemy*> enemys_;
	std::list<EnemyBullet*> enemyBullets_;
	//
	std::unique_ptr<Model> e_body_model_ = nullptr;
	std::unique_ptr<Model> e_head_model_ = nullptr;
	std::unique_ptr<Model> e_l_arm_model_ = nullptr;
	std::unique_ptr<Model> e_r_arm_model_ = nullptr;

	std::unique_ptr<Model> e_bullet_model_ = nullptr;
	std::vector<Model*> enemyModels_;
	std::vector<Model*> e_bullet_models;
#pragma endregion
	bool isPoseMode_ = false;
	bool controlMouse = true;
};
