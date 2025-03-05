#pragma once
#include <KamataEngine.h>
#include<vector>
#include"Player.h"
#include"Skydome.h"
#include"MapChipField.h"
#include"CameraController.h"

namespace KamataEngine {
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void GenerateBlocks();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	Player* player_ = nullptr;
	Skydome* skydome_ = nullptr;
	uint32_t playerTextureHandle_ = 0;
	uint32_t blockTextureHandle_ = 0;

	Model* model_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelSkydome_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	DebugCamera* debugCamera_ = nullptr;
	Camera camera_;
	CameraController cameraController_;
	bool isDebugCameraActive_ = false;

	MapChipField* mapChipField_ = nullptr;
};

} // namespace KamataEngine
