#include <EffekseerForDXLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "ShotEnemy.h"
#include "Enemy.h"
#include "Player.h"



ShotEnemy::ShotEnemy()
{

	// エフェクトの初期設定
	InitEffect();

	// 音の初期設定
	InitMusic();

}

ShotEnemy::~ShotEnemy(void)
{
}

void ShotEnemy::InitEffect(void)
{

	// 溜めのエフェクト
	effectShotResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SHOT_EFFECT).handleId_;

}

void ShotEnemy::InitMusic(void)
{

	// 弾を作る音
	musicCreateId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CREATE_MUSIC).handleId_;

	// 弾を発射する音
	musicShotId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SHOT_MUSIC).handleId_;

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
	StopSoundMem(musicCreateId_);
	PlaySoundMem(musicShotId_, DX_PLAYTYPE_BACK);
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
	transform_.pos = VAdd(VAdd(enemyTransform_->pos, { 0.0f,200.0f,100.0f }), enemyTransform_->quaRot.PosAxis(rPos_));

	// 弾モデルの向き(角度)を指定方向に合わせる
	transform_.quaRot = Quaternion::LookRotation(dir_);

	// 生存フラグ、時間の初期化
	stepAlive_ = timeAlive_;

	// 弾のエフェクトの再生
	ShotPlayEffect();

	// モデル制御の基本情報更新
	transform_.Update();

	// 弾を作る音
	PlaySoundMem(musicCreateId_, DX_PLAYTYPE_BACK);

	// 状態遷移
	ChangeState(STATE::IDLE);

}

void ShotEnemy::SetParam(void)
{

	// 弾の速度
	speed_ = 150.0f;

	// 生存時間
	timeAlive_ = 5.0f;

	// 衝突用球体半径
	collisionRadius_ = 50.0f;

}

void ShotEnemy::Move(void)
{
	transform_.pos = VAdd(transform_.pos, VScale(dir_, speed_));
}

void ShotEnemy::UpdateIdle(void)
{

	// 弾の位置の更新
	transform_.pos = VAdd(VAdd(enemyTransform_->pos, { 0.0f,200.0f,0.0f }), transform_.quaRot.PosAxis(rPos_));

	// 弾の回転
	transform_.quaRot = enemyTransform_->quaRot;

	// エフェクトの位置
	ShotSyncEffect();

}

void ShotEnemy::UpdateShot(void)
{

	// 生存チェック
	CheckAlive();

	if (state_ != STATE::SHOT)
	{

		// 処理中断
		return;

	}

	// 移動処理
	Move();

	// エフェクトの位置
	ShotSyncEffect();

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
		StopEffekseer3DEffect(effectShotPlayId_);
		ChangeState(STATE::END);
	}

}

void ShotEnemy::ShotPlayEffect(void)
{

	// エフェクト再生
	effectShotPlayId_ = PlayEffekseer3DEffect(effectShotResId_);

	float SCALE = 100.0f;
	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectShotPlayId_, SCALE, SCALE, SCALE);

	// 位置
	ShotSyncEffect();

}

void ShotEnemy::ShotSyncEffect(void)
{

	// 追従対象の位置
	VECTOR followPos = transform_.pos;

	// 追従対象の向き
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	float degY = AsoUtility::Rad2DegF(rot.y);
	degY = degY + 180.0f;

	float radY = AsoUtility::Deg2RadF(degY);

	// 追従対象から自機までの相対座標
	VECTOR effectLPos = followRot.PosAxis({ 0.0f,0.0f,0.0f });

	// エフェクトの位置の更新
	effectShotPos_ = VAdd(followPos, effectLPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectShotPlayId_, effectShotPos_.x, effectShotPos_.y, effectShotPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectShotPlayId_, rot.x, radY, rot.z);

	transform_.Update();

}
