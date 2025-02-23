#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

enum class LRdirection {
	kRight,
	kLeft
};
class Player {
public:
	Player() {};
	~Player() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="camera">カメラ</param>
	void Initialize(Model* model,Camera* camera,const Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Camera* camera_ = nullptr;
	Vector3 velocity_ = {};
	//移動用変数
	static inline const float kAcceleration = 0.1f;
	const float kAttenuation = 0.2f;
	const float kLimitRunSpeed = 0.5f;
	LRdirection lrDirection_ = LRdirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;
	//ジャンプ用変数
	bool onGround_ = true;
	static inline const float kGravityAcceleration = 0.0098f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kJumpAcceleration = 1.0f;

};
