#include "ShotEnemy.h"

ShotEnemy::ShotEnemy(void)
{
}

ShotEnemy::~ShotEnemy(void)
{
}

ShotEnemy::STATE ShotEnemy::GetState(void)
{
	return state_;
}

void ShotEnemy::SetState(ShotEnemy::STATE state)
{
	ChangeState(state);
}

void ShotEnemy::SetParam(void)
{

	// �e�̑��x
	speed_ = 30.0f;

	// ��������
	timeAlive_ = 5.0f;

	// �Փ˗p���̔��a
	collisionRadius_ = 10.0f;

}
