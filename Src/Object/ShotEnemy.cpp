#include <EffekseerForDXLib.h>
#include "../Manager/SceneManager.h"
#include "ShotEnemy.h"
#include "Enemy.h"
#include "Player.h"

ShotEnemy::ShotEnemy(void)
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
	return isAlive_;
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
