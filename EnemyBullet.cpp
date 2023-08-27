#include "EnemyBullet.h"
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
void EnemyBullet::Initialize(
    const std::vector<Model*>& models, const Vector3& position, const Vector3& velosity) {

    BaseBullet::Initialize(models, position);

	velosity_ = velosity;
	worldTransform_.translation_.y = 5.0f;

	InitializeGlobalVariavles();
}

/// <summary>
/// 
/// </summary>
void EnemyBullet::Update() {
	ApplyGlobalVariavles();

	BaseBullet::Update();
}

/// <summary>
/// 
/// </summary>
/// <param name="viewProjection"></param>
void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	//
	BaseBullet::Draw(viewProjection);
}

/// <summary>
/// 
/// </summary>
void EnemyBullet::OnCollision() { isDead_ = true; }

/// <summary>
/// 
/// </summary>
void EnemyBullet::ApplyGlobalVariavles() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "E_Bullet";
	// 作ったグループにあるアイテムから値を取得
	damage_ = gloVars->GetFloatValue(groupName, "Damage");
	radius_ = gloVars->GetFloatValue(groupName, "radius");
}

/// <summary>
/// 
/// </summary>
void EnemyBullet::InitializeGlobalVariavles() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "E_Bullet";
	// 作ったグループにそれぞれアイテムを追加
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "Damage", damage_);
	gloVars->AddItem(groupName, "radius", radius_);
}
