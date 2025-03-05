#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include "MatrixFunc.h"
#include <algorithm>
#include <cassert>
#include <numbers>

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {
	assert(model);
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.translation_.y = std::numbers::pi_v<float> / 2.0f;
	model_ = model;
	camera_ = camera;
}

void Player::Update() {

	Move();
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.offset = velocity_;
	MapChipCollision(collisionMapInfo);
	OnCollisionByMapChip(collisionMapInfo);

#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::SliderFloat("x", &worldTransform_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat("y", &worldTransform_.translation_.y, -100.0f, 100.0f);
	ImGui::SliderFloat("z", &worldTransform_.translation_.z, -100.0f, 100.0f);
	ImGui::End();
#endif
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_); }

void Player::Move() {

	if (onGround_) {
		// 移動入力
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
			velocity_ += acceleration;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
	} else {
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// 着地フラグ
	bool landing = false;

	// 下降中？
	if (velocity_.y < 0) {
		// y座標が地面いかになったら着地
		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		}
	}

	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めりこみ排斥
			worldTransform_.translation_.y = 2.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		float t = 1.0f - (turnTimer_ / kTimeTurn);
		worldTransform_.rotation_.y = turnFirstRotationY_ + (destinationRotationY - turnFirstRotationY_) * t;
	}

	worldTransform_.translation_ += velocity_;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Player::MapChipCollision(CollisionMapInfo& info) {
	IsCollisionTop(info);
	// IsCollisionBottom(info);
	// IsCollisionLeft(info);
	// IsCollisionRight(info);
}

void Player::IsCollisionTop(CollisionMapInfo& info) {
	if (info.offset.y <= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionNew;
	for (int i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.offset, static_cast<Corner>(i));
	}
	MapChipType mapChipType;

	// 真上の当たり判定を行う
	bool hit = false;

	// 左上点の判定
	IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
		// めり込み先ブロックの矩形範囲
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.offset.y = std::max(0.0f, velocity_.y + kBlank);
		info.top = true;
	}
	CeilingCollision(info);
}

// void Player::IsCollisionBottom(CollisionMapInfo& info) {}
//
// void Player::IsCollisionLeft(CollisionMapInfo& info) {}
//
// void Player::IsCollisionRight(CollisionMapInfo& info) {}

void Player::OnCollisionByMapChip(const CollisionMapInfo& info) { worldTransform_.translation_ += info.offset; }

void Player::CeilingCollision(const CollisionMapInfo& info) {
	if (info.top) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0.0f;
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {kWidth / 2.0f,  -kHeight / 2.0f, 0.0f},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f},
        {kWidth / 2.0f,  kHeight / 2.0f,  0.0f},
        {-kWidth / 2.0f, kHeight / 2.0f,  0.0f}
    };
	return center + offsetTable[static_cast<uint32_t>(corner)];
}
