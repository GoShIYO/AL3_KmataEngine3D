#include "Player.h"
#include <cassert>
void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {
	assert(model);
	worldTransform_.Initialize();
    model_ = model;
	textureHandle_ = textureHandle;
    camera_ = camera;
}

void Player::Update() { 

	worldTransform_.TransferMatrix(); 

}

void Player::Draw() {

	model_->Draw(worldTransform_, *camera_, textureHandle_);

}
