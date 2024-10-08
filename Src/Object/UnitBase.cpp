#include "../Manager/SceneManager.h"
#include "../Utility/Utility.h"
#include "../Application.h"
#include "Common/AnimationContorller.h"
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

	// 移動処理
	Move();

	transform_.Update();
}

void UnitBase::Draw(void)
{

	// ロードされた３Ｄモデルを画面に描画
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
