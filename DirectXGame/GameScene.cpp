#include "GameScene.h"
#include <cassert>

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");
	// スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, { 100,50});
	// 3Dモデルの生成
	model_ = Model::Create();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();
	// サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("fanfare.wav");
	// 音声再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true,0.5f);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth,WinApp::kWindowHeight);
	// ライン描画が参照するカメラを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetCamera());
	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸表示するカメラを指定する
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
}

void GameScene::Update() { 
	// スプライトの移動
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;
	sprite_->SetPosition(position);
	// スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(voiceHandle_);
	}
#ifdef _DEBUG
	// デバッグテキストの表示
	ImGui::Begin("Debug1");
	ImGui::Text("Kmata Taro %d.%d.%d", 2025, 12, 31);
	ImGui::End();
	ImGui::Begin("Debug2");
	// float3入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();
	ImGui::ShowDemoWindow();
#endif // _DEBUG
	debugCamera_->Update();	
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
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region ライン描画
	float gridSize = 1.0f;
	float gridCount = 20.0f;

	float length = gridSize * gridCount;

	// X軸方向のグリッド線（Z固定）
	for (float x = -gridCount; x <= gridCount; x+= 1.0f) {
		PrimitiveDrawer::GetInstance()->DrawLine3d({x, 0, -length}, {x, 0, length}, {1.0f, 0.0f, 0.0f, 1.0f});
	}

	// Z軸方向のグリッド線（X固定）
	for (float z = -gridCount; z <= gridCount; z += 1.0f) {
		PrimitiveDrawer::GetInstance()->DrawLine3d({-length, 0, z}, {length, 0, z}, {0.0f, 0.0f, 1.0f, 1.0f});
	}

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
