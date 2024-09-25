#pragma once
#include <DxLib.h>
#include <iostream>
#include <algorithm>
class Quaternion
{

public:
	
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	double w;
	double x;
	double y;
	double z;

	Quaternion();
	Quaternion(const VECTOR& rad);
	Quaternion(const double w, const double x, const double y, const double z);

	~Quaternion() = default;

	// オイラー角からクォータニオンへ変換
	static const Quaternion Euler(const VECTOR& rad) { return Euler(rad.x, rad.y, rad.z); };

	static Quaternion Euler(double radX, double radY, double radZ)
	{

		Quaternion ret = Quaternion();

		radX = Utility::RadIn2PI(radX);
		radY = Utility::RadIn2PI(radY);
		radZ = Utility::RadIn2PI(radZ);

		double cosZ = cos(radZ / 2.0f);
		double sinZ = sin(radZ / 2.0f);
		double cosX = cos(radX / 2.0f);
		double sinX = sin(radX / 2.0f);
		double cosY = cos(radY / 2.0f);
		double sinY = sin(radY / 2.0f);

		ret.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
		ret.x = sinX * cosY * cosZ + cosX * sinY * sinZ;
		ret.y = cosX * sinY * cosZ - sinX * cosY * sinZ;
		ret.z = cosX * cosY * sinZ - sinX * sinY * cosZ;

		return ret;

	};

	// クォータニオンの合成
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2)
	{

		Quaternion ret = Quaternion();
		double d1, d2, d3, d4;

		// wの計算 
		d1 = q1.w * q2.w;
		d2 = -q1.x * q2.x;
		d3 = -q1.y * q2.y;
		d4 = -q1.z * q2.z;
		ret.w = d1 + d2 + d3 + d4;

		// xの計算 
		d1 = q1.w * q2.x;
		d2 = q2.w * q1.x;
		d3 = q1.y * q2.z;
		d4 = -q1.z * q2.y;
		ret.x = d1 + d2 + d3 + d4;

		// yの計算
		d1 = q1.w * q2.y;
		d2 = q2.w * q1.y;
		d3 = q1.z * q2.x;
		d4 = -q1.x * q2.z;
		ret.y = d1 + d2 + d3 + d4;

		// zの計算
		d1 = q1.w * q2.z;
		d2 = q2.w * q1.z;
		d3 = q1.x * q2.y;
		d4 = -q1.y * q2.x;
		ret.z = d1 + d2 + d3 + d4;

		return ret;

	};

	Quaternion Mult(const Quaternion& q) const { return Mult(Quaternion(w, x, y, z), q); };

	// 指定軸を指定角分、回転させる
	static Quaternion AngleAxis(const double rad, VECTOR axis)
	{

		Quaternion ret = Quaternion();

		double norm;
		double c, s;

		// Unityに合わせる
		//ret.w = ret.x = ret.y = ret.z = 0.0;
		ret.w = 1.0;
		ret.x = ret.y = ret.z = 0.0;

		norm = (double)axis.x * (double)axis.x + (double)axis.y * (double)axis.y + (double)axis.z * (double)axis.z;
		if (norm <= 0.0f)
		{
			return ret;
		}

		norm = 1.0 / sqrt(norm);
		axis.x = (float)(axis.x * norm);
		axis.y = (float)(axis.y * norm);
		axis.z = (float)(axis.z * norm);

		c = cos(0.5f * rad);
		s = sin(0.5f * rad);

		ret.w = c;
		ret.x = s * axis.x;
		ret.y = s * axis.y;
		ret.z = s * axis.z;

		return ret;

	};

	// 座標を回転させる
	static VECTOR PosAxis(const Quaternion& q, const VECTOR& pos)
	{

		// 位置情報に回転情報を反映させる
		// pos' = q・pos・q(-1)
		Quaternion tmp = Quaternion();
		tmp = tmp.Mult(q);
		tmp = tmp.Mult(Quaternion(0.0f, pos.x, pos.y, pos.z));
		tmp = tmp.Mult(q.Inverse());
		return { (float)tmp.x, (float)tmp.y, (float)tmp.z };

	};

	VECTOR PosAxis(const VECTOR& pos) const { return PosAxis(Quaternion(w, x, y, z), pos); };

	// クォータニオンからオイラー角へ変換
	static VECTOR ToEuler(const Quaternion& q);
	VECTOR ToEuler() const;

	// クォータニオンから行列へ変換
	static MATRIX ToMatrix(const Quaternion& q);
	MATRIX ToMatrix() const;

	// ベクトルからクォータニオンに変換
	static Quaternion LookRotation(VECTOR dir);
	static Quaternion LookRotation(VECTOR dir, VECTOR up);

	// 行列からクォータニオンに変換
	static Quaternion GetRotation(MATRIX mat);

	// 基本ベクトルを取得
	VECTOR GetForward() const;
	VECTOR GetBack() const;
	VECTOR GetRight() const;
	VECTOR GetLeft() const;
	VECTOR GetUp() const;
	VECTOR GetDown() const;

	// 内積
	static double Dot(const Quaternion& q1, const Quaternion& q2);
	double Dot(const Quaternion& b) const;

	// 正規化
	static Quaternion Normalize(const Quaternion& q);
	Quaternion Normalized() const;
	void Normalize();

	// 逆クォータニオン
	Quaternion Inverse() const;

	// 球面補間
	static Quaternion Slerp(Quaternion from, Quaternion to, double t);

	// ２つのベクトル間の回転量を取得する
	static Quaternion FromToRotation(VECTOR fromDir, VECTOR toDir);
	static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);
	static double Angle(const Quaternion& q1, const Quaternion& q2);
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);
	static Quaternion Identity();

	double Length() const;
	double LengthSquared() const;
	VECTOR xyz() const;

	// 対象方向の回転
	void ToAngleAxis(float* angle, VECTOR* axis);

private:

	// 基本ベクトルを取得
	VECTOR GetDir(VECTOR dir) const;

	Quaternion operator*(float& rhs);
	const Quaternion operator*(const float& rhs);
	Quaternion operator+(Quaternion& rhs);
	const Quaternion operator+(const Quaternion& rhs);
	
};
