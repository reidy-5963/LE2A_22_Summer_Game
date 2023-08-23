#include "MyMath.h"
#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = MyMath::MakeAffineMatrix(scale_, rotation_, translation_);

	if (parent_) {
		matWorld_ = MyMath::Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}
