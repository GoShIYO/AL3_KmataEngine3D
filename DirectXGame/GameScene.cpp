#include "GameScene.h"
#include <cassert>

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelSkydome_;
	delete player_;
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
	model_ = Model::Create();
    modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	textureHandle_ = TextureManager::Load("uvChecker.png");
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &camera_);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);
	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	const float kWallWidth = kBlockWidth * kNumBlockHorizontal;
	const float kWallHeight = kBlockHeight * kNumBlockVirtical;
	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVirtical);

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (j % (rand() % 5 + 1) == 0 || i % (rand() % 3 + 1) == 0) {
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				// ブロックを中央に調整する
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j - kWallWidth / 2.0f;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i - kWallHeight / 2.0f;
			} else {
				worldTransformBlocks_[i][j] = nullptr;
			}
		}
	}
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
			    1.0f
			};
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
	ImGui::Begin("frame");
    ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
    ImGui::End();
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
			model_->Draw(*worldTransformBlock, camera_);
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
