#include <EffekseerForDXLib.h>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "ShotBase.h"

ShotBase::ShotBase(void)
{

	// ������
	stepAlive_ = 0.0f;
	dir_ = Utility::VECTOR_ZERO;

	// ���f������̊�{���
	ResourceManager& rem = ResourceManager::GetInstance();
	transform_.modelId =
		rem.LoadModelDuplicate(ResourceManager::SRC::SHOT_MODEL);
	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = Utility::VECTOR_ZERO;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler(Utility::Deg2RadF(90.0f), 0.0f, 0.0f);

	transform_.Update();

}

ShotBase::~ShotBase(void)
{
}

void ShotBase::Create(VECTOR relPos, Transform* follow)
{

	// �p�����[�^�ݒ�
	SetParam();

	// �e�̔����ʒu
	relPos;
	follow;

	// �e���f���̌���(�p�x)���w������ɍ��킹��
	transform_.quaRot = Quaternion::LookRotation(dir_);

	// �����t���O�A���Ԃ̏�����
	stepAlive_ = timeAlive_;

	// ���f������̊�{���X�V
	transform_.Update();

	// ��ԑJ��
	ChangeState(STATE::IDLE);

}

void ShotBase::Blast(void)
{

	ChangeState(STATE::BLAST);

}

void ShotBase::Update(void)
{

	switch (state_)
	{
	case ShotBase::STATE::NONE:
		break;
	case ShotBase::STATE::IDLE:
		UpdateIdle();
		break;
	case ShotBase::STATE::SHOT:
		UpdateShot();
		break;
	case ShotBase::STATE::BLAST:
		UpdateBlast();
		break;
	case ShotBase::STATE::END:
		break;
	}

	// ���f������̊�{���X�V
	transform_.Update();

}

void ShotBase::CheckAlive(void)
{

	// �������Ԃ����炷
	stepAlive_ -= SceneManager::GetInstance().GetDeltaTime();
	if (stepAlive_ < 0.0f)
	{
		ChangeState(STATE::END);

	}

}

void ShotBase::Draw(void)
{

	switch (state_)
	{
	case ShotBase::STATE::NONE:
		break;
	case ShotBase::STATE::IDLE:
		break;
	case ShotBase::STATE::SHOT:
		break;
	case ShotBase::STATE::BLAST:
		break;
	case ShotBase::STATE::END:
		break;
	}

}

void ShotBase::Release(void)
{
	MV1DeleteModel(transform_.modelId);
}

VECTOR ShotBase::GetPos(void) const
{
	return transform_.pos;
}

void ShotBase::SetPos(VECTOR pos)
{
	transform_.pos = pos;
}

Quaternion ShotBase::GetQuaRot(void) const
{
	return transform_.quaRot;
}

float ShotBase::GetSpeed(void) const
{
	return speed_;
}

float ShotBase::GetTimeAlive(void) const
{
	return timeAlive_;
}

float ShotBase::GetCollisionRadius(void) const
{
	return collisionRadius_;
}

void ShotBase::Move(void)
{
	// �ړ�
	VECTOR velocity = VScale(dir_, speed_);
	transform_.pos = VAdd(transform_.pos, velocity);
}

void ShotBase::UpdateIdle(void)
{
}

void ShotBase::UpdateShot(void)
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

}

void ShotBase::UpdateBlast(void)
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

void ShotBase::ChangeState(STATE state)
{
	if (state_ == state)
	{
		// ������ԂȂ珈�����Ȃ�
		return;
	}
	// ��ԍX�V
	state_ = state;
	switch (state)
	{
	case ShotBase::STATE::NONE:
		break;
	case ShotBase::STATE::IDLE:
		break;
	case ShotBase::STATE::SHOT:
		break;
	case ShotBase::STATE::BLAST:
		// �����G�t�F�N�g���Đ�����
		PlayBlastEffect();
		break;
	case ShotBase::STATE::END:
		break;
	}

}

void ShotBase::PlayBlastEffect(void)
{
	// �����G�t�F�N�g���Đ�����
	effectBlastPlayId_ = PlayEffekseer3DEffect(effectBlastResId_);
	SetScalePlayingEffekseer3DEffect(
		effectBlastPlayId_, BLAST_SCALE, BLAST_SCALE, BLAST_SCALE);
	SetRotationPlayingEffekseer3DEffect(effectBlastPlayId_, 0.0f, 0.0f, 0.0f);
	VECTOR pos = transform_.pos;
	SetPosPlayingEffekseer3DEffect(effectBlastPlayId_, pos.x, pos.y, pos.z);
}

void ShotBase::DrawDebug(void)
{
	DrawSphere3D(transform_.pos,50, 10, 0xff0000, 0xff0000, true);
}
