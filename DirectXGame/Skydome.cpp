#include "Skydome.h"

void Skydome::Initialize(Model* model, Camera* camera) {
	model_ = model;
    camera_ = camera;
	worldTransform_.Initialize();
}

void Skydome::Update() {}

void Skydome::Draw() {
	model_->Draw(worldTransform_, *camera_);
}
