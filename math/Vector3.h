#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3 operator+(const Vector3& v1) {
		Vector3 result{};
		result.x = x + v1.x;
		result.y = y + v1.y;
		result.z = z + v1.z;
		return result;
	}

	Vector3 operator-(const Vector3& v1) {
		Vector3 result{};
		result.x = x - v1.x;
		result.y = y - v1.y;
		result.z = z - v1.z;
		return result;
	}

	bool operator<(const Vector3& v1) {
		if (x < v1.x && y < v1.y && z < v1.z) {
			return true;
		}

		return false;
	};

	bool operator>(const Vector3& v1) {
		if (x > v1.x && y > v1.y && z > v1.z) {
			return true;
		}

		return false;
	};
};