#include <EffekseerForDXLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "ShotEnemy.h"
#include "Enemy.h"
#include "Player.h"



ShotEnemy::ShotEnemy()
{

	// �G�t�F�N�g�̏����ݒ�
	InitEffect();

	// ���̏����ݒ�
	InitMusic();

}

ShotEnemy::~ShotEnemy(void)
{
}

void ShotEnemy::InitEffect(void)
{

	// ���߂̃G�t�F�N�g
	effectShotResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SHOT_EFFECT).handleId_;

}

void ShotEnemy::InitMusic(void)
{

	// �e����鉹
	musicCreateId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CREATE_MUSIC).handleId_;

	// �e�𔭎˂��鉹
	musicShotId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SHOT_MUSIC).handleId_;

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
	StopSoundMem(musicCreateId_);
	PlaySoundMem(musicShotId_, DX_PLAYTYPE_BACK);
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
	transform_.pos = VAdd(VAdd(enemyTransform_->pos, { 0.0f,200.0f,100.0f }), enemyTransform_->quaRot.PosAxis(rPos_));

	// �e���f���̌���(�p�x)���w������ɍ��킹��
	transform_.quaRot = Quaternion::LookRotation(dir_);

	// �����t���O�A���Ԃ̏�����
	stepAlive_ = timeAlive_;

	// �e�̃G�t�F�N�g�̍Đ�
	ShotPlayEffect();

	// ���f������̊�{���X�V
	transform_.Update();

	// �e����鉹
	PlaySoundMem(musicCreateId_, DX_PLAYTYPE_BACK);

	// ��ԑJ��
	ChangeState(STATE::IDLE);

}

void ShotEnemy::SetParam(void)
{

	// �e�̑��x
	speed_ = 150.0f;

	// ��������
	timeAlive_ = 5.0f;

	// �Փ˗p���̔��a
	collisionRadius_ = 50.0f;

}

void ShotEnemy::Move(void)
{
	transform_.pos = VAdd(transform_.pos, VScale(dir_, speed_));
}

void ShotEnemy::UpdateIdle(void)
{

	// �e�̈ʒu�̍X�V
	transform_.pos = VAdd(VAdd(enemyTransform_->pos, { 0.0f,200.0f,0.0f }), transform_.quaRot.PosAxis(rPos_));

	// �e�̉�]
	transform_.quaRot = enemyTransform_->quaRot;

	// �G�t�F�N�g�̈ʒu
	ShotSyncEffect();

}

void ShotEnemy::UpdateShot(void)
{

	// �����`�F�b�N
	CheckAlive();

	if (state_ != STATE::SHOT)
	{

		// �������f
		return;

	}

	// �ړ�����
	Move();

	// �G�t�F�N�g�̈ʒu
	ShotSyncEffect();

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
		StopEffekseer3DEffect(effectShotPlayId_);
		ChangeState(STATE::END);
	}

}

void ShotEnemy::ShotPlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectShotPlayId_ = PlayEffekseer3DEffect(effectShotResId_);

	float SCALE = 100.0f;
	// �傫��
	SetScalePlayingEffekseer3DEffect(effectShotPlayId_, SCALE, SCALE, SCALE);

	// �ʒu
	ShotSyncEffect();

}

void ShotEnemy::ShotSyncEffect(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = transform_.pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	float degY = AsoUtility::Rad2DegF(rot.y);
	degY = degY + 180.0f;

	float radY = AsoUtility::Deg2RadF(degY);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectLPos = followRot.PosAxis({ 0.0f,0.0f,0.0f });

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectShotPos_ = VAdd(followPos, effectLPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectShotPlayId_, effectShotPos_.x, effectShotPos_.y, effectShotPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectShotPlayId_, rot.x, radY, rot.z);

	transform_.Update();

}
