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
	camera_->translation_ = Lerp(camera_->translation_, targetPos_, kInterpolationRate);

	ImGui::Begin("margin");
	ImGui::DragFloat("left", &margin.left, 1.0f);
    ImGui::DragFloat("right", &margin.right, 1.0f);
    ImGui::DragFloat("bottom", &margin.bottom, 1.0f);
    ImGui::DragFloat("top", &margin.top, 1.0f);
    ImGui::End();
	camera_->translation_.x = std::clamp(camera_->translation_.x, target_->GetWorldTransform().translation_.x + margin.left, target_->GetWorldTransform().translation_.x + margin.right);
	camera_->translation_.y = std::clamp(camera_->translation_.y, target_->GetWorldTransform().translation_.y + margin.bottom, target_->GetWorldTransform().translation_.y + margin.top);

	camera_->translation_.x = std::max(camera_->translation_.x, movebleArea_.left);
	camera_->translation_.x = std::min(camera_->translation_.x, movebleArea_.right);

	camera_->translation_.y = std::max(camera_->translation_.y, movebleArea_.bottom);
	camera_->translation_.y = std::min(camera_->translation_.y, movebleArea_.top);

	camera_->UpdateMatrix();
}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}
