#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "UnitBase.h"

UnitBase::UnitBase(void)
{
}

UnitBase::~UnitBase(void)
{
}

void UnitBase::Init(void)
{

	// �p�����[�^�ݒ�
	SetParam();

}

void UnitBase::Update(void)
{

	// �ړ�����
	Move();

	// �A�j���[�V��������
	Animation();

	transform_.Update();
}

void UnitBase::Draw(void)
{

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);

}

void UnitBase::Release(void)
{

	MV1DeleteModel(modelId_);

}

const Transform& UnitBase::GetTransform(void) const
{
	return transform_;
}

void UnitBase::SetParam(void)
{
}

void UnitBase::Move(void)
{
}

void UnitBase::Animation(void)
{

	// �A�j���[�V�����Đ�
	// �o�ߎ��Ԃ̎擾
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// �A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		// ���[�v�Đ�
		stepAnim_ = 0.0f;
	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(modelId_, animAttachNo_, stepAnim_);

}
