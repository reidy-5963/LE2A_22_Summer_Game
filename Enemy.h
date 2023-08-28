#pragma once

#include "BaseCharacter.h"
#include <optional>
#include "EnemyBullet.h"

class GameScene;

class Player;

class Enemy : public BaseCharacter {
public: // サブクラス
	/// <summary>
	/// モデルの配列番号
	/// </summary>
	enum modelIndex { 
		Body,
		Head,
		L_Arm,
		R_Arm
 };

public:
	Enemy();
	~Enemy();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="models">モデルデータ</param>
	void Initialize(const std::vector<Model*>& models, const Vector3 position, bool isLast);

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
	/// 
	/// </summary>
	/// <param name="damage"></param>
	void OnCollision(float damage) override;

	/// <summary>
	/// 
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
	///
	/// </summary>
	void RootUpdate();

	/// <summary>
	///
	/// </summary>
	void Attack();


	/// <summary>
	///
	/// </summary>
	/// <param name="velosity"></param>
	void SetPosition(Vector3 velosity);

	/// <summary>
	///
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	///
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	/// 弾のモデル設定
	/// </summary>
	/// <param name="bulletModels"></param>
	void SetBulletModel(const std::vector<Model*>& bulletModels) { bulletModels_ = bulletModels; }

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	float GetDamage() { return damage_; }

	bool IsLast() { return isLast_; }

	private: //

	//
	const int kModelNo = 4;

	//
	GameScene* gameScene_ = nullptr;

	//
	Player* player_ = nullptr;

	// 弾のモデル
	std::vector<Model*> bulletModels_;

	// 弾を撃てるかどうか
	bool isAttack_ = false;

	// 弾のタイマー
	int timer_ = 1;

	bool isLast_ = false;

private: //

	//
	WorldTransform worldTransform_parts_[4];

	//
	float damage_ = 200.0f;
};