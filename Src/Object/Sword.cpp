#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "Sword.h"

//Sword::Sword(const Transform& follow) : follow_(follow)
//{
//}

Sword::Sword(void)
{
}

Sword::~Sword(void)
{
}

void Sword::InitAnimation(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SWORD));
	float scale = 0.1f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 400.0f, 300.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(-90), AsoUtility::AXIS_X));
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	//animNo_ = 1;

	//// 再生するアニメーションの設定
	//animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	//// アニメーション総時間の取得
	//animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	//stepAnim_ = 0.0f;

	//// アニメーション速度
	//speedAnim_ = 20.0f;

	//// モデルに指定時間のアニメーションを設定する
	//MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Sword::Init(void)
{

	// アニメーションの初期設定
	InitAnimation();

}

const Transform& Sword::GetTransform(void) const
{
	return transform_;
}

void Sword::Update(void)
{

	// プレイヤーの右手に追従させる
	// 追従対象(プレイヤー)の位置
	
	//武器をアタッチするフレームの番号を検索
	int WeponAttachFrameNum = MV1SearchFrame(followTransform_->modelId, "mixamorig:RightHandMiddle1");

	//武器をアタッチするフレームのローカル→ワールド変換行列を取得する
	MATRIX WeponFrameMatrix = MV1GetFrameLocalWorldMatrix(followTransform_->modelId, WeponAttachFrameNum);

	// 行列からクォータニオン
	Quaternion qua = Quaternion::GetRotation(WeponFrameMatrix);

	//クォータニオンからラジアン
	VECTOR rad = qua.ToEuler();

	// 追従対象の向き
	Quaternion followRot = followTransform_->quaRot;

	// 追従対象から自機までの相対座標
	VECTOR swordPos = followRot.PosAxis(transform_.pos);

	// 剣の位置の更新
	transform_.pos = VAdd(rad, swordPos);

	// ラジアンから行列
	MATRIX matPos = MGetTranslate(transform_.pos);

	MV1SetMatrix(transform_.modelId, matPos);

	//MV1SetPosition(followTransform_->modelId,transform_.pos);

	//transform_.pos = followPos;

	//transform_.Update();

}

void Sword::Draw(void)
{

	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(transform_.modelId);

}

void Sword::Release(void)
{

	// モデルの開放
	MV1DeleteModel(transform_.modelId);

}

void Sword::SetFollow(const Transform* follow)
{

	followTransform_ = follow;

}
