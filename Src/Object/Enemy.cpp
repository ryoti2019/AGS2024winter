#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "Player.h"
#include "Enemy.h"

Enemy::Enemy(void)
{
}

Enemy::~Enemy(void)
{
}

void Enemy::InitAnimation(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::ENEMY_IDLE));

	idleAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_IDLE);

	attackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_ATTACK);

	float scale = 2.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 200.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(180), AsoUtility::AXIS_Y));
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	animNo_ = 0;

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// アニメーション総時間の取得
	//animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 0.0f;

	//// モデルに指定時間のアニメーションを設定する
	//MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Enemy::Init(void)
{

	// アニメーションの初期設定
	InitAnimation();

	// プレイヤーのパラメーター
	SetParam();

	// 待機アニメーション
	SetIdleAnimation();

}

void Enemy::Draw(void)
{

	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(transform_.modelId);

	// 衝突判定のカプセルの描画
	DrawCapsule3D(cPosDown_, cPosUp_, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

	// デバッグ描画
	DrawDebug();

}

void Enemy::Release(void)
{

	// モデルの開放
	MV1DeleteModel(transform_.modelId);

}

const Transform& Enemy::GetTransform(void) const
{
	return transform_;
}

VECTOR Enemy::GetCPosDown(void)
{
	return cPosDown_;
}

VECTOR Enemy::GetCPosUP(void)
{
	return cPosUp_;
}

int Enemy::GetHP(void)
{
	return hp_;
}

void Enemy::SetHP(int hp)
{
	hp_ += hp;
}

void Enemy::SetFollow(const Transform* follow)
{

	followTransform_ = follow;

}

void Enemy::Move(void)
{

	preState_ = state_;

	// 衝突判定用
	Collision();

	// 方向(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	VECTOR vec = VSub(followTransform_->pos,transform_.pos);
	auto veca = AsoUtility::Magnitude(vec);

	// 敵とプレイヤーの距離が一定距離になったら攻撃する
	if (veca < 500.0f && AsoUtility::EqualsVZero(dir))
	{
		ChangeState(STATE::ATTACK);
	}

	// 待機状態
	if (!AsoUtility::EqualsVZero(dir))
	{
		ChangeState(STATE::IDLE);
	}

	// アニメーションの変更
	ChangeAnimation();

}

void Enemy::Collision(void)
{

	// 敵から当たり判定の下までの相対座標
	VECTOR cPosDOWN = transform_.quaRot.PosAxis(LOCAL_C_DOWN_POS);
	// 敵から当たり判定の上までの相対座標
	VECTOR cPosUP = transform_.quaRot.PosAxis(LOCAL_C_UP_POS);

	// 敵の位置の更新
	cPosDown_ = VAdd(transform_.pos, cPosDOWN);
	cPosUp_ = VAdd(transform_.pos, cPosUP);

}

void Enemy::ChangeState(STATE state)
{

	// 状態の更新
	state_ = state;

}

void Enemy::SetIdleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, idleAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 20.0f;

	// モデルに指定時間のアニメーションを設定する
	//MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Enemy::SetWalkAnimation(void)
{
}

void Enemy::SetRunAnimation(void)
{
}

void Enemy::SetAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 20.0f;

	// モデルに指定時間のアニメーションを設定する
	//MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Enemy::ChangeAnimation(void)
{

	if (state_ == preState_) return;

	stepAnim_ = 0.0f;
	switch (state_)
	{
	case Enemy::STATE::IDLE:
		SetIdleAnimation();
		break;
	case Enemy::STATE::WALK:
		SetWalkAnimation();
		break;
	case Enemy::STATE::RUN:
		SetRunAnimation();
		break;
	case Enemy::STATE::ATTACK:
		SetAttackAnimation();
		break;
	}

}

void Enemy::LazyRotation(float goalRot)
{
}

void Enemy::DrawDebug(void)
{

	// HPバー
	int hpLength = 300;
	int H;
	int hpGauge;
	H = hp_ * (512.0f / hpMax_) - 100;
	int R = min(max((384 - H), 0), 0xff);
	int G = min(max((H + 64), 0), 0xff);
	int B = max((H - 384), 0);
	hpGauge = hpLength * hp_ / hpMax_;

	if (hp_ >= 0)
	{
		DrawBox(700, 0, 700 + hpGauge, 30, GetColor(R, G, B), true);
	}

}

void Enemy::SetParam(void)
{

	// HPの最大値
	hpMax_ = 100;

	// HP
	hp_ = hpMax_;

}

void Enemy::Animation(void)
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

		if (state_ == STATE::ATTACK)
		{
			ChangeState(STATE::IDLE);
			SetIdleAnimation();
		}

	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}