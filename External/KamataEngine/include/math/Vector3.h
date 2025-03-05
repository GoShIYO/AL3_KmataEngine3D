#pragma once

namespace KamataEngine {

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x, y, z;

	 Vector3 operator+(const Vector3& v) const { return Vector3{x + v.x, y + v.y, z + v.z}; }

	 Vector3 operator-(const Vector3& v) const { return Vector3{x - v.x, y - v.y, z - v.z}; }

	 Vector3& operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	 Vector3& operator-=(const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	 Vector3 operator*(float scalar) const { return Vector3{x * scalar, y * scalar, z * scalar}; }

	 Vector3& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	 friend Vector3 operator*(float scalar, const Vector3& v) {
		 return v * scalar;
	 }
};

} // namespace KamataEngine
