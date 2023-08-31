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
public:
	enum joyStateSelectButton {
		BACKPLAY,
		BACKTITLE,
	};

	enum Tutorial {
		Base,
		Joy_Base,
		Mouse_Base,
		Joy_Left,
		Joy_Right,
		Joy_RB,
		Joy_Start,
		Mouse_W,
		Mouse_A,
		Mouse_S,
		Mouse_D,
		Mouse_ESC,
		Mouse_Left,

	};

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
	/// グローバル変数の初期化処理
	/// </summary>
	void InitializeGlobalVariavles();

	/// <summary>
	/// グローバル変数の取得
	/// </summary>
	void ApplyGlobalVariavles();

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
	bool ButtonToCurSor(Vector2 buttonPos, Vector2 buttonSize);

	void KeyMouse_ButtonUpdate(
	    Vector2 buttonPos, Vector2 buttonSize, bool& isSelect, bool& whatDO, Sprite* sprite,
	    uint32_t texture[2]);

	void JoyStateUpdate();
	void JoyState_ButtonUpdate(
	    int selectState, Sprite* start, Sprite* end, uint32_t startTex, uint32_t endTex,
	    bool& whatDO);

private
	    : // メンバ変数
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
	uint32_t hp_root_tex_ = 0u;
	uint32_t hp_gauge_root_tex_ = 0u;
	uint32_t hp_out_tex_ = 0u;
	uint32_t hp_gauge_out_tex_ = 0u;
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

	uint32_t damageEffectTex_ = 0u;


#pragma endregion
	// ポーズモードフラグ
	bool isNotPose_ = false;
	
	// マウス操作
	bool controlMouse = true;
	
	// 敵が最後かどうか
	bool isEnemyEnd_ = false;
	
	// 敵ポップ停止フラグ
	bool isWait_ = false;

	bool isStart_ = false;
	int i = 0;

	float bolume = 0.5f;
	uint32_t selectSEHandle_ = 0u;
	uint32_t BGMHandle_ = 0u;
#pragma region menu
	// メニュー[タイトルに戻る]
	std::unique_ptr<Sprite> menu_backTitle_ = nullptr;
	
	// メニュー[タイトルに戻る]
	uint32_t menu_backTitle_Tex_[2] = {0u};
	
	// メニュー[タイトルに戻る]の位置
	Vector2 menu_backTitle_pos_;

	// 
	bool isSelect_backTitle_ = false;

	std::unique_ptr<Sprite> menu_backPlay_ = nullptr;
	uint32_t menu_backPlay_Tex_[2] = {0u};
	Vector2 menu_backPlay_pos_;
	bool isSelect_backPlay_ = false;


	POINT mousePos_;
	bool isTitle_ = false;
	
	// 選択時間の設定値
	int timerSet_ = 20;
	// コントローラー時の選択時間
	int timer_ = timerSet_;
	int selectButton = 0;
	// もし選択していたらのフラグ
	bool isSelectButton_ = false;

#pragma endregion

	std::unique_ptr<Sprite> tutorial_[13];
	Vector2 tutorialPos_;

	const int tutorialNum = 13;
	uint32_t tutorial_tex_[13];

	bool isAnimation_ = false;
	int setTime_tutAnime_ = 60;
	int tutAnime_timer_ = setTime_tutAnime_;
		
};

