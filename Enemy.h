#pragma once

#include "BaseCharacter.h"
class GameScene;

class Player;

class Enemy : public BaseCharacter {
public: // サブクラス
	/// <summary>
	/// モデルの配列番号
	/// </summary>
	enum modelIndex { 
		Body,
		L_Wepon,
		R_Wepon
 };

public:
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

	void SetPosition(Vector3 velosity);

	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	// 
	WorldTransform worldTransform_parts_[3];

	GameScene* gameScene_ = nullptr;
	Player* player_ = nullptr;
	
	bool isDead_ = false;

};