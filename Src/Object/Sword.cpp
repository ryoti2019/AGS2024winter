#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "Sword.h"

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
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(-90), AsoUtility::AXIS_Z));
	rotPow = rotPow.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(90), AsoUtility::AXIS_X));
	//VECTOR localRot = { AsoUtility::Deg2RadF(90),AsoUtility::Deg2RadF(0), AsoUtility::Deg2RadF(60) };
	//transform_.quaRotLocal = Quaternion::Euler(localRot);
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	// 敵と剣の当たり判定
	EnemyCheckHit();

}

void Sword::EnemyCheckHit(void)
{

}

void Sword::Init(void)
{
	//武器をアタッチするフレームの番号を検索
	WeponAttachFrameNum = MV1SearchFrame(followTransform_->modelId, "mixamorig:RightHand");

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

	//武器をアタッチするフレームのローカル→ワールド変換行列を取得する
	MATRIX WeponFrameMatrix = MV1GetFrameLocalWorldMatrix(followTransform_->modelId, WeponAttachFrameNum);

	// 行列からラジアン
	auto rot = MGetRotElem(WeponFrameMatrix);
	auto pos = MGetTranslateElem(WeponFrameMatrix);

	// 行列からクォータニオン
	Quaternion qua = Quaternion::GetRotation(rot);

	// 追従対象の向き
	Quaternion followRot = qua;

	// 追従対象から自機までの相対座標
	VECTOR swordPos = followRot.PosAxis({60.0f,0.0f,0.0f});

	// 剣の位置の更新
	transform_.pos = VAdd(pos, swordPos);

	transform_.quaRot = qua;

	//// ラジアンから行列
	//MATRIX matPos = MGetTranslate(transform_.pos);

	//MV1SetMatrix(transform_.modelId, matPos);

	//MV1SetPosition(followTransform_->modelId,transform_.pos);

	//transform_.pos = followPos;

	transform_.Update();

}

void Sword::Draw(void)
{

	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(transform_.modelId);

	// 当たり判定の描画
	DrawCapsule3D(transform_.pos,transform_.pos, 10, 10, 0xff0000, 0xff0000, false);

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
