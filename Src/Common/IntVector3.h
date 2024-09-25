#pragma once
#include <DxLib.h>
class IntVector3
{

public:

	int x;
	int y;
	int z;

	// コンストラクタ
	IntVector3();

	// コンストラクタ
	IntVector3(const int vX, const int vY, const int vZ);

	// コンストラクタ
	IntVector3(const VECTOR& v);

	// デストラクタ
	~IntVector3() = default;

	bool operator<(const IntVector3& value) const { return std::tie(x, y, z) < std::tie(value.x, value.y, value.z); };
	
	void Add(const int v)
	{
		x += v;
		y += v;
		z += v;
	};

	void Sub(const int v)
	{
		x -= v;
		y -= v;
		z -= v;
	};

	void Scale(const int v)
	{
		x *= v;
		y *= v;
		z *= v;
	};

};

