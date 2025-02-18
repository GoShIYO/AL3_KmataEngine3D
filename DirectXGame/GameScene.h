#pragma once
#include <KamataEngine.h>

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	uint32_t textureHandle_;				// テクスチャハンドル
	Sprite* sprite_ = nullptr;				// スプライト
	Model* model_ = nullptr;				// モデル
	WorldTransform worldTransform_;			// ワールド変換
	Camera camera_;							// カメラ
	uint32_t soundDataHandle_;				// 音声データハンドル
	uint32_t voiceHandle_;					// 音声ハンドル
	DebugCamera* debugCamera_ = nullptr;	// デバッグカメラ

	float inputFloat3[3] = {0};
};

} // namespace KamataEngine
