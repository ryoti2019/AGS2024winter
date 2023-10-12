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

	// パラメータ設定
	SetParam();

}

void UnitBase::Update(void)
{

	// 移動処理
	Move();

	// アニメーション処理
	Animation();

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

void UnitBase::SetParam(void)
{
}

void UnitBase::Move(void)
{
}

void UnitBase::Animation(void)
{

	// アニメーション再生
	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// アニメーション時間の進行
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		// ループ再生
		stepAnim_ = 0.0f;
	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(modelId_, animAttachNo_, stepAnim_);

}
