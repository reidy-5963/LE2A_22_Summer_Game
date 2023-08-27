#pragma once
#include "BaseCharacter.h"
#include <optional>

#include "input/Input.h"

#include "PlayerBullet.h"

#include "Sprite.h"

//class GameScene;

/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseCharacter {
public: // サブクラス
	/// <summary>
	/// モデルの配列番号
	/// </summary>
	enum modelIndex {
		Body,	// 体
		Head,	// 頭
		L_Arm,	// 左腕
		R_Arm,	// 右腕
		Wepon	// 
	};

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化処理 
	/// </summary>
	/// <param name="models">モデルデータ</param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection">ViewProjection</param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// もし当たっていた場合の処理
	/// </summary>
	void OnCollision(float damage) override;

	/// <summary>
	/// グローバル変数の取得
	/// </summary>
	void ApplyGlobalVariavles() override;

	/// <summary>
	///
	/// </summary>
	void InitializeGlobalVariavles() override;

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatingGimmick() override;

	/// <summary>
	/// 腕ふりギミック更新
	/// </summary>
	void UpdateArmGimmick() override;

	/// <summary>
	/// 攻撃を食らったときの動き
	/// </summary>
	void DamageGimmick() override;


	/// <summary>
	/// 各部位のモデルの初期化処理
	/// </summary>
	void ModelInitialize(); 

	/// <summary>
	/// 通常状態の更新処理
	/// </summary>
	void RootUpdate();

	/// <summary>
	/// 攻撃の更新処理
	/// </summary>
	void Attack();

	/// <summary>
	/// 2D,3Dレティクルの更新処理
	/// </summary>
	void ReticleUpdate();

	/// <summary>
	/// 弾が有効かどうか
	/// </summary>
	void BulletIsDead();

	/// <summary>
	/// 2dのUI関係の描画処理
	/// </summary>
	void DrawUI();

	/// <summary>
	/// ViewProjectionの設定
	/// </summary>
	/// <param name="viewProjection">ViewProjection</param>
	void SetViewProjection(const ViewProjection* viewProjection);

	/// <summary>
	/// 弾のモデル設定
	/// </summary>
	/// <param name="bulletModels"></param>
	void SetBulletModel(const std::vector<Model*>& bulletModels) { bulletModels_ = bulletModels; }

	/// <summary>
	/// レティクルの設定
	/// </summary>
	/// <param name="texture"></param>
	void SetReticle(uint32_t texture) { texReticle_ = texture; }

	/// <summary>
	/// 体力の設定
	/// </summary>
	/// <param name="texture"></param>
	void SetHeart(uint32_t texture) { heartTex_ = texture; }

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

private: // メンバ変数

	// モデルの数
	const int kModelNo = 5;

	// 入力関係
	Input* input_ = nullptr;

	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 弾
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;

	// 弾のモデル
	std::vector<Model*> bulletModels_;
	
	// 弾を撃てるかどうか
	bool isAttack_ = false;

	// ジャンプできるかどうか
	bool isJump_ = false;

	// 弾のタイマー
	int timer_ = 1;


	// 3dレティクルのワールド変換データ
	WorldTransform worldTransform_3dReticle_;

	// レティクルのテクスチャ
	uint32_t texReticle_ = 0u;

	// 体力のテクスチャ
	uint32_t heartTex_ = 0u;

	// 2dレティクルのスプライト
	std::unique_ptr<Sprite> sprite_2dReticle_ = nullptr;

	// 体力のスプライト
	std::unique_ptr<Sprite> p_heart_ = nullptr;



private: // 外部から書き換え可能な値
	// 各部位のワールド変換データ
	WorldTransform worldTransform_parts_[5];

};
