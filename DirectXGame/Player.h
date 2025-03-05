#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

enum class LRDirection { 
	kRight, 
	kLeft 
};
enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner // 要素数
};
struct CollisionMapInfo {
	bool top = false;
	bool bottom = false;
	bool wall = false;
	Vector3 offset;
};
class MapChipField;
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
	void Initialize(Model* model, Camera* camera, const Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
	void OnCollisionByMapChip(const CollisionMapInfo& info);
	
	void CeilingCollision(const CollisionMapInfo& info);
	Vector3 CornerPosition(const Vector3& center, Corner corner);
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	WorldTransform& GetWorldTransform() { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }

private:
	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	/// <summary>
	/// マップチップと衝突判定
	/// </summary>
	/// <param name="info"></param>
	void MapChipCollision(CollisionMapInfo& info);

	void IsCollisionTop(CollisionMapInfo& info);

	//void IsCollisionBottom(CollisionMapInfo& info);

	//void IsCollisionLeft(CollisionMapInfo& info);

	//void IsCollisionRight(CollisionMapInfo& info);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Camera* camera_ = nullptr;
	Vector3 velocity_ = {};
	MapChipField* mapChipField_ = nullptr;
	// 移動用変数
	static inline const float kAcceleration = 0.05f;
	const float kAttenuation = 0.2f;
	const float kLimitRunSpeed = 0.2f;
	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;
	// ジャンプ用変数
	bool onGround_ = true;
	static inline const float kGravityAcceleration = 0.05f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kJumpAcceleration = 0.5f;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 1.8f;

	static inline const float kBlank = 0.1f;
};
