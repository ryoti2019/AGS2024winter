#include <tuple>
#include "../Utility/Utility.h"
#include "IntVector3.h"

// �R���X�g���N�^
IntVector3::IntVector3()
	:
	x(0),
	y(0),
	z(0)

{
}

// �R���X�g���N�^
IntVector3::IntVector3(const int vX, const int vY, const int vZ)
	:
	x(vX),
	y(vY),
	z(vZ)

{
}

IntVector3::IntVector3(const VECTOR& v)
	:
	x(Utility::Round(v.x)),
	y(Utility::Round(v.y)),
	z(Utility::Round(v.z))
{

}