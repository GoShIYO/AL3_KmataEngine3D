#define NOMINMAX
#include "Player.h"
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
	bool landing = false;

	if (velocity_.y < 0.0f) {
		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		}
	}
	
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRdirection::kRight) {
					lrDirection_ = LRdirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRdirection::kLeft) {
					lrDirection_ = LRdirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
			velocity_ += acceleration;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			velocity_ += Vector3(0.0f, kJumpAcceleration, 0.0f);
		}
	} else {
		velocity_ += Vector3(0.0f, -kGravityAcceleration , 0.0f);
		velocity_.y = std::clamp(velocity_.y, -kLimitFallSpeed, 0.0f);
		if (landing) {
			worldTransform_.translation_.y = 2.0f;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}

	ImGui::Begin("Player");
	ImGui::SliderFloat("x", &worldTransform_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat("y", &worldTransform_.translation_.y, -100.0f, 100.0f);
	ImGui::SliderFloat("z", &worldTransform_.translation_.z, -100.0f, 100.0f);
	ImGui::End();

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

void Player::Draw() { model_->Draw(worldTransform_, *camera_); }
