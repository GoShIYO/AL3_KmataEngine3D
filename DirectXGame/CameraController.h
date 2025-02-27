#pragma once
#include<KamataEngine.h>
using namespace KamataEngine;

struct Rect {
	float left = 0.0f;
	float right = 1.0f;
	float bottom = 0.0f;
	float top = 1.0f;
};

class Player;
class CameraController {
public:

	void Initialize(Camera* camera);
	
	void Update();

	void Reset();

	void SetTarget(Player* target) { target_ = target; };

	void SetMovebleArea(Rect area) { movebleArea_ = area; }

private:
	Camera* camera_;
	Player* target_;
	Vector3 targetOffset_ = {0, 0, -20.0f};
	Rect movebleArea_ = {0, 100, 0, 100};
	Vector3 targetPos_ ;
	static inline const float kInterpolationRate = 0.1f;
	static inline const float kVelocityBias = 0.5f;
	static inline const Rect margin = {0, 50.0f, 0,50.0f};
};
