#pragma once
#include "BaseCharacter.h"
#include <optional>

#include "input/Input.h"

#include "PlayerBullet.h"

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
		Body,
		Head,
		L_Arm,
		R_Arm,
		Wepon
	};

public: // メンバ関数
	Player() {}
	~Player() {
		for (PlayerBullet* bullet : bullets_) {
			delete bullet;
		}
	}

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
	/// 通常状態の更新処理
	/// </summary>
	void RootUpdate();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection">ViewProjection</param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// ViewProjectionの設定
	/// </summary>
	/// <param name="viewProjection">ViewProjection</param>
	void SetViewProjection(const ViewProjection* viewProjection);

	/// <summary>
	/// 各部位のモデルの初期化処理
	/// </summary>
	void ModelInitialize(); 

	/// <summary>
	/// 
	/// </summary>
	void Attack();

	void MauseUpdate();

	//void SetGameScene(GameScene* gameScene) { gameScene_.reset(gameScene); }
	void BulletIsDead() {
		bullets_.remove_if([](PlayerBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		}); 
	
	}

	void SetBulletModel(const std::vector<Model*>& bulletModels) { bulletModels_ = bulletModels; }

	private: // メンバ変数
	// 入力関係
	Input* input_ = nullptr;
	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 各部位のワールド変換データ
	WorldTransform worldTransform_parts_[5];

	//WorldTransform worldTransform_body_;
	//WorldTransform worldTransform_head_;
	//WorldTransform worldTransform_l_arm_;
	//WorldTransform worldTransform_r_arm_;
	//WorldTransform worldTransform_wepon_;

	//
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;

	bool isAttack_ = false;
	bool isJump_ = false;
	static const int kBulletoffset = 15;

	int timer_ = 1;

	//std::unique_ptr<GameScene> gameScene_;
	std::vector<Model*> bulletModels_;


};
