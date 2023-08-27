#include "PlayerBullet.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include <cassert>
#include "GlobalVariables.h"

/// <summary>
/// 
/// </summary>
/// <param name="models"></param>
/// <param name="position"></param>
/// <param name="velosity"></param>
void PlayerBullet::Initialize(
    const std::vector<Model*>& models, const Vector3& position, const Vector3& velosity) {

	BaseBullet::Initialize(models, position);

	velosity_ = velosity;
	worldTransform_.translation_.y = 5.0f;

	InitializeGlobalVariavles();
}

/// <summary>
/// 
/// </summary>
void PlayerBullet::Update() {
	ApplyGlobalVariavles();

	BaseBullet::Update();
}

/// <summary>
/// 
/// </summary>
/// <param name="viewProjection"></param>
void PlayerBullet::Draw(const ViewProjection& viewProjection) { 
	//
	BaseBullet::Draw(viewProjection);
}

/// <summary>
/// 
/// </summary>
void PlayerBullet::OnCollision() { isDead_ = true; }

/// <summary>
/// 
/// </summary>
void PlayerBullet::ApplyGlobalVariavles() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "P_Bullet";
	// 作ったグループにあるアイテムから値を取得
	damage_ = gloVars->GetFloatValue(groupName, "Damage");
	radius_ = gloVars->GetFloatValue(groupName, "radius");
}

/// <summary>
/// 
/// </summary>
void PlayerBullet::InitializeGlobalVariavles() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "P_Bullet";
	// 作ったグループにそれぞれアイテムを追加
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "Damage", damage_);
	gloVars->AddItem(groupName, "radius", radius_);
}
