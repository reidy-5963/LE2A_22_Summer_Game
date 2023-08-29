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
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 全ての当たり判定処理
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾の追加
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵の追加
	/// </summary>
	/// <param name="enemy">敵</param>
	void AddEnemy(Enemy* enemy);

	/// <summary>
	/// 敵の初期化処理
	/// </summary>
	/// <param name="models">敵のモデル</param>
	/// <param name="position">敵の位置</param>
	void EnemyIni(const std::vector<Model*>& models, const Vector3 position, bool isLastEnemy);

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
	// プレイヤーの武器モデル
	std::unique_ptr<Model> p_gun_model_ = nullptr;
	std::unique_ptr<Model> p_bullet_model_ = nullptr;

	// レティクルのテクスチャ
	uint32_t reticleTex_;
	// 体力のテクスチャ
	uint32_t heartTex_;

#pragma endregion

#pragma region 追従カメラ
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
#pragma endregion

#pragma region 敵
	//敵
	std::list<Enemy*> enemys_;
	// 敵弾
	std::list<EnemyBullet*> enemyBullets_;
	
	// 敵のモデル
	std::unique_ptr<Model> e_body_model_ = nullptr;
	std::unique_ptr<Model> e_head_model_ = nullptr;
	std::unique_ptr<Model> e_l_arm_model_ = nullptr;
	std::unique_ptr<Model> e_r_arm_model_ = nullptr;

	// 敵弾のモデル
	std::unique_ptr<Model> e_bullet_model_ = nullptr;

	// 敵のモデル動的配列
	std::vector<Model*> enemyModels_;
	
	// 敵弾のモデル動的配列
	std::vector<Model*> e_bullet_models;

	// 敵ポップデータ
	std::stringstream enemyPopCommands;
	
	// 敵ポップ間のタイマー
	int32_t waitTimer_ = 0;

	std::unique_ptr<Sprite> blackout_ = nullptr;
	uint32_t blackoutTex_ = 0u;
	float blackoutAlpha_ = 0.0f;
	float blackoutAlpha_offset_ = 0.1f;

	std::unique_ptr<Sprite> damageEffect_ = nullptr;
	uint32_t damageEffectTex_ = 0u;
	float damageEffectAlpha_ = 0.0f;
	float damageEffectAlpha_offset_ = 0.01f;


#pragma endregion
	// ポーズモードフラグ
	bool isPoseMode_ = false;
	
	// マウス操作
	bool controlMouse = true;
	
	// 敵が最後かどうか
	bool isEnemyEnd_ = false;
	
	// 敵ポップ停止フラグ
	bool isWait_ = false;

	bool isStart_ = false;
};
