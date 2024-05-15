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
			ResourceManager::SRC::SWORD_MODEL1));
	float scale = 0.15f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(-270), AsoUtility::AXIS_Z));
	rotPow = rotPow.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(0), AsoUtility::AXIS_X));
	//VECTOR localRot = { AsoUtility::Deg2RadF(90),AsoUtility::Deg2RadF(0), AsoUtility::Deg2RadF(60) };
	//transform_.quaRotLocal = Quaternion::Euler(localRot);
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	// 敵と剣の当たり判定
	EnemyCheckHit();

	cPosUp_ = { 0.0f,0.0f,0.0f };
	cPosDown_ = { 0.0f,0.0f,0.0f };

	Update();

}

void Sword::EnemyCheckHit(void)
{

}

void Sword::Init(void)
{

	//武器をアタッチするフレームの番号を検索
	WeponAttachFrameNum_ = MV1SearchFrame(followTransform_->modelId, "mixamorig:RightHand");

	// アニメーションの初期設定
	InitAnimation();

}

const Transform& Sword::GetTransform(void) const
{
	return transform_;
}

VECTOR Sword::GetCPosDown(void)
{
	return cPosDown_;
}

VECTOR Sword::GetCPosUP(void)
{
	return cPosUp_;
}

void Sword::Update(void)
{

	// プレイヤーの右手に追従させる
	// 追従対象(プレイヤー)の位置

	//武器をアタッチするフレームのローカル→ワールド変換行列を取得する
	MATRIX WeponFrameMatrix = MV1GetFrameLocalWorldMatrix(followTransform_->modelId, WeponAttachFrameNum_);

	// 行列からラジアン
	auto rot = MGetRotElem(WeponFrameMatrix);
	auto pos = MGetTranslateElem(WeponFrameMatrix);

	// 行列からクォータニオン
	Quaternion qua = Quaternion::GetRotation(rot);

	// 追従対象の向き
	Quaternion followRot = qua;

	// 追従対象から自機までの相対座標
	VECTOR swordPos = followRot.PosAxis(LOCAL_SWORD_POS);

	// 剣の位置の更新
	transform_.pos = VAdd(pos, swordPos);

	transform_.quaRot = qua;

	// 剣から当たり判定の下までの相対座標
	VECTOR cPosDOWN = transform_.quaRot.PosAxis(LOCAL_C_DOWN_POS);
	// 剣から当たり判定の上までの相対座標
	VECTOR cPosUP = transform_.quaRot.PosAxis(LOCAL_C_UP_POS);

	// 剣の位置の更新
	cPosDown_ = VAdd(transform_.pos, cPosDOWN);
	cPosUp_ = VAdd(transform_.pos, cPosUP);

	transform_.Update();

}

void Sword::Draw(void)
{

	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(transform_.modelId);

	//collisionUPPos_ = VAdd(collisionUPPos_, { 0.0f,5.0f,0.0f });
	//collisionDOWNPos_ = VAdd(collisionDOWNPos_, { 0.0f,-5.0f,0.0f });

	// 当たり判定の描画
	//DrawCapsule3D(cPosDown_, cPosUp_, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

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