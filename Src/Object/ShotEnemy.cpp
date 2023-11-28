#include <EffekseerForDXLib.h>
#include "../Manager/SceneManager.h"
#include "ShotEnemy.h"
#include "Enemy.h"
#include "Player.h"

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

void ShotEnemy::SetDir(VECTOR dir)
{
	dir_ = dir;
}

bool ShotEnemy::IsAlive(void)
{
	return isAlive_;
}

void ShotEnemy::SetParam(void)
{

	// �e�̑��x
	speed_ = 30.0f;

	// ��������
	timeAlive_ = 5.0f;

	// �Փ˗p���̔��a
	collisionRadius_ = 50.0f;

}

void ShotEnemy::Move(void)
{
	transform_.pos = VAdd(transform_.pos, VScale(dir_, 10.0f));
}

void ShotEnemy::UpdateBlast(void)
{

	// �G�t�F�N�g�Đ��`�F�b�N
	// 0:�Đ����A-1:�Đ�����Ă��Ȃ��A�������͍Đ��I��
	int ret = IsEffekseer3DEffectPlaying(effectBlastPlayId_);
	if (ret == -1)
	{
		// �I����Ԃ�
		ChangeState(STATE::END);
	}

}

void ShotEnemy::CheckAlive(void)
{

	// �������Ԃ����炷
	stepAlive_ -= SceneManager::GetInstance().GetDeltaTime();
	if (stepAlive_ < 0.0f)
	{
		ChangeState(STATE::END);
	}

}
