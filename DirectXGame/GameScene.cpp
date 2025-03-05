#include "GameScene.h"
#include <cassert>

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelSkydome_;
	delete player_;
	delete mapChipField_;
	for (auto& worldTransformLine : worldTransformBlocks_) {
		for (auto worldTransformBlock : worldTransformLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	srand(unsigned int(time(nullptr)));
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	camera_.Initialize();
	// モデル
	model_ = Model::CreateFromOBJ("player", true);
	modelBlock_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// マップチップ
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	// テクスチャ
	playerTextureHandle_ = TextureManager::Load("./Resources/player/head.png");
	blockTextureHandle_ = TextureManager::Load("block.png");
	// プレイヤー
	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 17);
	player_->Initialize(model_, &camera_, playerPosition);
	// スカイドーム
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);
	// ブロック
	GenerateBlocks();
}

void GameScene::Update() {
	// ブロックの更新
	for (auto& worldTransformLine : worldTransformBlocks_) {
		for (auto worldTransformBlock : worldTransformLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->matWorld_ = {
			    1.0f, 0.0f, 0.0f, 0.0f, 
				0.0f, 1.0f, 0.0f, 0.0f, 
				0.0f, 0.0f, 1.0f, 0.0f, 
				worldTransformBlock->translation_.x, worldTransformBlock->translation_.y, worldTransformBlock->translation_.z,
			    1.0f};
			worldTransformBlock->TransferMatrix();
		}
	}

	player_->Update();
	skydome_->Update();
#ifdef _DEBUG
	// デバッグカメラの有効/無効
	if (input_->TriggerKey(DIK_Z)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	ImGui::Begin("frame");
	ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
	ImGui::End();
#endif // _DEBUG

	// カメラの更新
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	skydome_->Draw();
	for (auto& worldTransformLine : worldTransformBlocks_) {
		for (auto worldTransformBlock : worldTransformLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, camera_, blockTextureHandle_);
		}
	}

	player_->Draw();
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	worldTransformBlocks_.resize(numBlockVirtical);

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}
