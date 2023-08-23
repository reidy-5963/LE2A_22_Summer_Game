#pragma once

//３次元ベクトル
struct Vector3 final {
	float x;	//!< x
	float y;	//!< y
	float z;	//!< z
};

//4x4行列
struct Matrix4x4 final {
	float m[4][4];	//!< 4x4の配列
};

//球
struct Sphere final {
	Vector3 center;//!< 中心点
	float radius;//!< 半径
};

//直線
struct Line final {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

//半直線
struct Ray final {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

//線分
struct Segment final {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

//平面
struct Plane final {
	Vector3 normal;	//!< 法線
	float distance;	//!< 距離
};

struct Triangle final {
	Vector3 vertices[3]; //!< 頂点(3個)
};

struct AABB final {
	Vector3 min;	//!< 最少点
	Vector3 max;	//!< 最大点

};

