#include <EffekseerForDXLib.h>
#include "../Manager/SceneManager.h"
#include "ShotEnemy.h"
#include "Enemy.h"
#include "Player.h"



ShotEnemy::ShotEnemy()
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
	return state_ != STATE::END;
}

bool ShotEnemy::IsIdle(void)
{
	return state_ == STATE::IDLE;
}

void ShotEnemy::Shot(VECTOR dir)
{
	dir_ = dir;
	ChangeState(STATE::SHOT);
}

void ShotEnemy::Create(VECTOR relPos, Transform* follow)
{

	// �p�����[�^�ݒ�
	SetParam();

	// �e�̔����ʒu
	// �G��Transform
	enemyTransform_ = follow;

	// ���΍��W
	rPos_ = relPos;
	
	// �e�̈ʒu�̍X�V
	transform_.pos = VAdd(VAdd(enemyTransform_->pos,{0.0f,200.0f,0.0f}), enemyTransform_->quaRot.PosAxis(rPos_));

	// �e���f���̌���(�p�x)���w������ɍ��킹��
	transform_.quaRot = Quaternion::LookRotation(dir_);

	// �����t���O�A���Ԃ̏�����
	stepAlive_ = timeAlive_;

	// ���f������̊�{���X�V
	transform_.Update();

	// ��ԑJ��
	ChangeState(STATE::IDLE);

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

void ShotEnemy::UpdateIdle(void)
{

	// �e�̈ʒu�̍X�V
	transform_.pos = VAdd(enemyTransform_->pos, transform_.quaRot.PosAxis(rPos_));

	// �e�̉�]
	transform_.quaRot = enemyTransform_->quaRot;

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
