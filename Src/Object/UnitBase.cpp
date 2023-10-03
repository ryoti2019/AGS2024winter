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

VECTOR UnitBase::GetPos(void) const
{
	return pos_;
}

VECTOR UnitBase::GetRot(void) const
{
	return rot_;
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

void UnitBase::SetRotation(void)
{

	// 回転行列を使用した角度設定
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(rot_.x));
	mat = MMult(mat, MGetRotY(rot_.y));
	mat = MMult(mat, MGetRotZ(rot_.z));

	// 調整用の回転行列
	MATRIX localMat = MGetIdent();
	localMat = MMult(localMat, MGetRotX(rotLocal_.x));
	localMat = MMult(localMat, MGetRotY(rotLocal_.y));
	localMat = MMult(localMat, MGetRotZ(rotLocal_.z));

	// 行列の合成
	mat = MMult(mat, localMat);

	// 行列を使用してモデルの角度を設定 行列にて、モデルの角度
	MV1SetRotationMatrix(modelId_, mat);

}

void UnitBase::LazyRotation(float goalRot)
{

	// 回転処理
	float degNowAngleY = AsoUtility::Rad2DegF(rot_.y);
	float degGoalAngleY = AsoUtility::Rad2DegF(goalRot);

	// 0度～360度以内に角度をおさめる
	degGoalAngleY = static_cast<float>(AsoUtility::DegIn360(degGoalAngleY));

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	int aroundDir = AsoUtility::DirNearAroundDeg(degNowAngleY, degGoalAngleY);

	// 到達したい角度に回転を加える
	if (fabs(degGoalAngleY - degNowAngleY) >= 5)
	{
		rot_.y += SPEED_ROT_RAD * static_cast<float>(aroundDir);
	}
	else
	{
		rot_.y = goalRot;
	}

	// 0度～360度以内に角度をおさめる
	rot_.y = static_cast<float>(AsoUtility::RadIn2PI(rot_.y));

}

void UnitBase::InitAnimation(void)
{

}
