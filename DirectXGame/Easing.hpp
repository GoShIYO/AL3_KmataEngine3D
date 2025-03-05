#pragma once
#include <cmath>
#include <algorithm>
#include <functional>

const float PI = 3.1415926535f;
const float c1 = 1.70158f;
const float c2 = c1 * 1.525f;
const float c3 = c1 + 1.0f;
const float c4 = (2 * PI) / 3;
const float c5 = (2 * PI) / 4.5f;

/// <summary>
/// 線形補間（Linear Interpolation）
/// 直線的に値を補間する関数
/// </summary>

template<typename T> 
inline T Lerp(T a, T b, float t) { 
	return static_cast<T>((1 - t) * a + b * t); 
}


/// <summary>
/// EaseInSine（加速するようなサインカーブの補間）
/// 緩やかに加速する動き
/// </summary>
template<typename T>
inline T EaseInSine(T a, T b, float t) {
	t = 1 - cos((t * PI) / 2);
	return Lerp(a, b, t);
}

/// <summary>
/// EaseOutSine（減速するようなサインカーブの補間）
/// 緩やかに減速する動き
/// </summary>
template<typename T>
inline T EaseOutSine(T a, T b, float t) {
	t = sin((t * PI) / 2);
	return Lerp(a, b, t);
}

/// <summary>
/// EaseInOutSine（加速と減速を組み合わせたサインカーブの補間）
/// 加速後に減速する自然な動き
/// </summary>
template<typename T>
inline T EaseInOutSine(T a, T b, float t) {
	t = -(cos(PI * t) - 1) / 2;
	return Lerp(a, b, t);
}

/// <summary>
/// EaseInQuad（2乗関数による加速補間）
/// 加速が徐々に強くなる動き
/// </summary>
template<typename T>
inline T EaseInQuad(T a, T b, float t) {
	t = t * t;
	return Lerp(a, b, t);
}

/// <summary>
/// EaseOutQuad（2乗関数による減速補間）
/// 減速が徐々に弱くなる動き
/// </summary>
template<typename T>
inline T EaseOutQuad(T a, T b, float t) {
	t = t * (2 - t);
	return Lerp(a, b, t);
}

/// <summary>
/// EaseInOutQuad（加速と減速を組み合わせた2乗関数補間）
/// スムーズな加速と減速
/// </summary>
template<typename T>
inline T EaseInOutQuad(T a, T b, float t) {
	t = (t < 0.5) ? (2 * t * t) : (-1 + (4 - 2 * t) * t);
	return Lerp(a, b, t);
}

/// <summary>
/// EaseInCubic（3乗関数による加速補間）
/// より急な加速
/// </summary>
template<typename T>
inline T EaseInCubic(T a, T b, float t) {
	t = t * t * t;
	return Lerp(a, b, t);
}

/// <summary>
/// EaseOutCubic（3乗関数による減速補間）
/// より急な減速
/// </summary>
template<typename T>
inline T EaseOutCubic(T a, T b, float t) {
	t = 1 - pow(1 - t, 3);
	return Lerp(a, b, t);
}

/// <summary>
/// EaseInOutCubic（加速と減速を組み合わせた3乗関数補間）
/// 滑らかな加速と減速
/// </summary>
template<typename T>
inline T EaseInOutCubic(T a, T b, float t) {
	t = (t < 0.5) ? (4 * t * t * t) : ((t - 1) * (2 * t - 2) * (2 * t - 2) + 1);
	return Lerp(a, b, t);
}

template<typename T>
inline T EaseInQuart(T a, T b, float t) {
	t = t * t * t * t;
	return Lerp(a, b, t);
}

template<typename T>
inline T EaseOutQuart(T a, T b, float t) {
	t = 1 - powf(1 - t, 4);
	return Lerp(a, b, t);
}
template<typename T>
inline T EaseInOutQuart(T a, T b, float t) {
	t = t < 0.5 ? 8 * t * t * t * t : 1 - pow(-2 * t + 2, 4) / 2;
	return Lerp(a, b, t);
}

template<typename T>
inline T EaseInBack(T a, T b, float t) {
	t = c3 * t * t * t - c1 * t * t;
	return Lerp(a, b, t);
}

template<typename T>
inline T EaseOutBack(T a, T b, float t) {
	t = 1 + c3 * pow(t - 1, 3) + c1 * pow(t - 1, 2);
	return Lerp(a, b, t);
}

template<typename T>
inline T EaseInOutBack(T a, T b, float t) {
	t = t < 0.5
		? (pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2
		: (pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
	return Lerp(a, b, t);
}

template<typename T>
inline T EaseInElastic(T a, T b, float t) {
	t = t == 0
		? 0
		: t == 1
		? 1
		: -powf(2, 10 * t - 10) * sinf((t * 10 - 10.75f) * c4);
	return Lerp(a, b, t);
}

template<typename T>
inline T EaseOutElastic(T a, T b, float t) {
	t = t == 0
		? 0
		: t == 1
		? 1
		: powf(2, -10 * t) * sinf((t * 10 - 0.75f) * c4) + 1;
	return Lerp(a, b, t);
}
template<typename T>
inline T EaseInOutElastic(T a, T b, float t) {
	t = t == 0
		? 0
		: t == 1
		? 1
		: t < 0.5
		? -(pow(2, 20 * t - 10) * sin((20 * t - 11.125) * c5)) / 2
		: (pow(2, -20 * t + 10) * sin((20 * t - 11.125) * c5)) / 2 + 1;
	return Lerp(a, b, t);
}
constexpr float bounceFactor1 = 1.0f / 2.75f;
constexpr float bounceFactor2 = 1.5f / 2.75f;
constexpr float bounceFactor3 = 2.25f / 2.75f;
constexpr float bounceFactor4 = 2.625f / 2.75f;
/// <summary>
/// EaseOutBounce（跳ねるような減速補間）
/// ボールがバウンドして止まる動き
/// </summary>
template<typename T>
inline T EaseOutBounce(T a, T b, float t) {
	if (t < (1 / 2.75f)) {
		t = 7.5625f * t * t;
	}
	else if (t < (2 / 2.75f)) {
		t -= (1.5f / 2.75f);
		t = 7.5625f * t * t + 0.75f;
	}
	else if (t < (2.5f / 2.75f)) {
		t -= (2.25f / 2.75f);
		t = 7.5625f * t * t + 0.9375f;
	}
	else {
		t -= (2.625f / 2.75f);
		t = 7.5625f * t * t + 0.984375f;
	}
	return Lerp(a, b, t);
}

/// <summary>
/// EaseInBounce（跳ねるような加速補間）
/// ボールが地面に落ちるような動き
/// </summary>
template<typename T>
inline T EaseInBounce(T a, T b, float t) {
	return b - EaseOutBounce(b, a, 1 - t);
}

/// <summary>
/// EaseInOutBounce（加速と減速を組み合わせた跳ねる補間）
/// バウンドしながらスムーズに加速・減速
/// </summary>
template<typename T>
inline T EaseInOutBounce(T a, T b, float t) {
	if (t < 0.5f) {
		return EaseInBounce(a, b, t * 2) * 0.5f;
	}
	else {
		return EaseOutBounce(a, b, t * 2 - 1) * 0.5f + b * 0.5f;
	}
}

using LerpCallBack = unsigned int(*)(unsigned int, unsigned int, float);
/// <summary>
/// 色をイージングで変える関数
/// </summary>
/// <param name="start">始める色</param>
/// <param name="end">目標色</param>
/// <param name="t">t</param>
/// <param name="lerpCallBack">イージング関数</param>
/// <returns>tによる色</returns>
inline unsigned int LerpColor(unsigned int start, unsigned int end, float t, LerpCallBack lerpCallBack) {
	t = std::clamp(t, 0.0f, 1.0f);

	unsigned int r1 = (start >> 24) & 0xFF;
	unsigned int g1 = (start >> 16) & 0xFF;
	unsigned int b1 = (start >> 8) & 0xFF;
	unsigned int a1 = start & 0xFF;

	unsigned int r2 = (end >> 24) & 0xFF;
	unsigned int g2 = (end >> 16) & 0xFF;
	unsigned int b2 = (end >> 8) & 0xFF;
	unsigned int a2 = end & 0xFF;

	unsigned int r = lerpCallBack(r1, r2, t);
	unsigned int g = lerpCallBack(g1, g2, t);
	unsigned int b = lerpCallBack(b1, b2, t);
	unsigned int a = lerpCallBack(a1, a2, t);
	return (r << 24) | (g << 16) | (b << 8) | a;
}