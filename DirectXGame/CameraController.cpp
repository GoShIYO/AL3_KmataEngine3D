#include "CameraController.h"
#include "Easing.hpp"
#include "Player.h"
#include <algorithm>
#undef min
#undef max
void CameraController::Initialize(Camera* camera) { camera_ = camera; }

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	targetPos_ = targetWorldTransform.translation_ + targetOffset_ + target_->GetVelocity() * kVelocityBias;
	camera_->translation_ = Lerp(camera_->translation_, targetPos_ + targetOffset_, kInterpolationRate);
	
	
	camera_->translation_.x = std::clamp(camera_->translation_.x, targetPos_.x + margin.left, targetPos_.x + margin.right);
	camera_->translation_.y = std::clamp(camera_->translation_.y, targetPos_.y + margin.bottom, targetPos_.y + margin.top);

	camera_->UpdateMatrix();
}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}
