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

}

void UnitBase::Update(void)
{

	// �ړ�����
	Move();

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

float UnitBase::GetStepAnim(void)
{
	return 0.0f;
}

void UnitBase::Move(void)
{
}

void UnitBase::Animation(void)
{
}
