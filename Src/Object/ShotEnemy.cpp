#include <EffekseerForDXLib.h>
#include "../Manager/SceneManager.h"
#include "ShotEnemy.h"
#include "Enemy.h"
#include "Player.h"



ShotEnemy::ShotEnemy()
{
}

ShotEnemy::~ShotEnemy(void)
{
}

ShotEnemy::STATE ShotEnemy::GetState(void)
{
	return state_;
}

void ShotEnemy::SetState(ShotEnemy::STATE state)
{
	ChangeState(state);
}

void ShotEnemy::SetDir(VECTOR dir)
{
	dir_ = dir;
}

bool ShotEnemy::IsAlive(void)
{
	return state_ != STATE::END;
}

bool ShotEnemy::IsIdle(void)
{
	return state_ == STATE::IDLE;
}

void ShotEnemy::Shot(VECTOR dir)
{
	dir_ = dir;
	ChangeState(STATE::SHOT);
}

void ShotEnemy::Create(VECTOR relPos, Transform* follow)
{

	// パラメータ設定
	SetParam();

	// 弾の発生位置
	// 敵のTransform
	enemyTransform_ = follow;

	// 相対座標
	rPos_ = relPos;
	
	// 弾の位置の更新
	transform_.pos = VAdd(VAdd(enemyTransform_->pos,{0.0f,200.0f,0.0f}), enemyTransform_->quaRot.PosAxis(rPos_));

	// 弾モデルの向き(角度)を指定方向に合わせる
	transform_.quaRot = Quaternion::LookRotation(dir_);

	// 生存フラグ、時間の初期化
	stepAlive_ = timeAlive_;

	// モデル制御の基本情報更新
	transform_.Update();

	// 状態遷移
	ChangeState(STATE::IDLE);

}

void ShotEnemy::SetParam(void)
{

	// 弾の速度
	speed_ = 30.0f;

	// 生存時間
	timeAlive_ = 5.0f;

	// 衝突用球体半径
	collisionRadius_ = 50.0f;

}

void ShotEnemy::Move(void)
{
	transform_.pos = VAdd(transform_.pos, VScale(dir_, 10.0f));
}

void ShotEnemy::UpdateIdle(void)
{

	// 弾の位置の更新
	transform_.pos = VAdd(enemyTransform_->pos, transform_.quaRot.PosAxis(rPos_));

	// 弾の回転
	transform_.quaRot = enemyTransform_->quaRot;

}

void ShotEnemy::UpdateBlast(void)
{

	// エフェクト再生チェック
	// 0:再生中、-1:再生されていない、もしくは再生終了
	int ret = IsEffekseer3DEffectPlaying(effectBlastPlayId_);
	if (ret == -1)
	{
		// 終了状態へ
		ChangeState(STATE::END);
	}

}

void ShotEnemy::CheckAlive(void)
{

	// 生存時間を減らす
	stepAlive_ -= SceneManager::GetInstance().GetDeltaTime();
	if (stepAlive_ < 0.0f)
	{
		ChangeState(STATE::END);
	}

}
