#include "MyMath.h"
#include "WorldTransform.h"

/// <summary>
/// ワールド変換データの更新処理
/// </summary>
void WorldTransform::UpdateMatrix() {
	// アフィン変換
	matWorld_ = MyMath::MakeAffineMatrix(scale_, rotation_, translation_);

	// もし親になる行列があれば
	if (parent_) {
		// 乗算
		matWorld_ = MyMath::Multiply(matWorld_, parent_->matWorld_);
	}

	// 行列の転送
	TransferMatrix();
}
