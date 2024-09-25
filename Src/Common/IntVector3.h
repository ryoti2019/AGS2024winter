#pragma once
#include <DxLib.h>
class IntVector3
{

public:

	int x;
	int y;
	int z;

	// �R���X�g���N�^
	IntVector3();

	// �R���X�g���N�^
	IntVector3(const int vX, const int vY, const int vZ);

	// �R���X�g���N�^
	IntVector3(const VECTOR& v);

	// �f�X�g���N�^
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

