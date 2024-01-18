#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "ShotEnemy.h"
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

	// 待機アニメーション
	idleAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_IDLE);

	// 歩くアニメーション
	walkAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_WALK);

	// タックルアニメーション
	tackleAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_TACKLE);

	// 攻撃アニメーション
	attackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_ATTACK);

	// ダッシュ攻撃アニメーション
	jumpAttackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_DASH_ATTACK);

	// 弾を生成するアニメーション
	createAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_SHOT_CREATE);

	// ショットアニメーション
	shotAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_SHOT_ATTACK);

	// 攻撃を食らったときのアニメーション
	hitAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_HIT);

	// 死亡アニメーション
	deathAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_DEATH);

	// 左旋回のアニメーション
	turnLeftAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_TURN_LEFT);

	// 右旋回のアニメーション
	turnRightAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_TURN_RIGHT);

	// ロックオン状態に出るカーソル
	lockOnCursorImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CURSOR).handleIds_;

	// transformの初期化
	float scale = 2.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 1000.0f };
	transform_.Update();

	// 再生するアニメーションの番号
	animNo_ = 0;

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// 再生中のアニメーション時間
	stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 0.0f;

	// 待機状態の弾の数
	shotNum_ = 0;

}

void Enemy::Init(void)
{

	// アニメーションの初期設定
	InitAnimation();

	// エフェクトの初期設定
	InitEffect();

	// 音の初期設定
	InitMusic();

	// プレイヤーのパラメーター
	SetParam();

	// 待機アニメーション
	SetIdleAnimation();

	// カプセルをアタッチするフレームの番号を検索
	enemyAttachFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:Spine2");

	// 武器をアタッチするフレームの番号を検索
	weponAttachFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:LeftHand");

	// フレーム番号をフレーム名で取得する
	enemyPosFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:Hips");

	// 攻撃中かどうか
	attack_ = false;

	// ダッシュアタックのフラグ
	jumpAttack_ = false;

	// タックルアタックのフラグ
	tackleAttack_ = false;

	// 回転の開始
	startRotation_ = false;

	// 回転の終了のフラグ
	isRotation_ = false;

	// 行動終了のフラグ
	isAction_ = false;

	isNoPlay_ = false;

	// 弾を発射したかどうか
	isShot_ = false;

	// 当たったかのフラグ
	hit_ = false;

	// 最初の歩きのアニメーションのカウンタ
	walkCnt_ = 0.0f;

	noPlayTime_ = 0.0f;

	lockOnCursorCnt_ = 0;

	lockOnCursorTime_ = 0.0f;

	// 初期状態
	ChangeState(STATE::THINK);

}

void Enemy::InitEffect(void)
{

	// 弾を作るときのエフェクト
	effectCreateResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CREATE_EFFECT).handleId_;

	// タックルのエフェクト
	effectTackleResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TACKLE_EFFECT).handleId_;

	// ジャンプアタックのエフェクト
	effectJumpAttackResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::JUMPATTACK_EFFECT).handleId_;

	// ジャンプアタックエフェクトのカウンタ
	isEffectJumpAttack_ = true;

	// ジャンプアタックの攻撃範囲のエフェクト
	effectJumpAttackRangeResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::JUMPATTACK_RANGE_EFFECT).handleId_;

}

void Enemy::CreatePlayEffect(void)
{

	// エフェクト再生
	effectCreatePlayId_ = PlayEffekseer3DEffect(effectCreateResId_);

	float SCALE = 100.0f;
	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectCreatePlayId_, SCALE, SCALE, SCALE);

	// 位置
	CreateSyncEffect();

}

void Enemy::TacklePlayEffect(void)
{

	// エフェクト再生
	effectTacklePlayId_ = PlayEffekseer3DEffect(effectTackleResId_);

	float SCALE = 100.0f;
	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectTacklePlayId_, SCALE, SCALE, SCALE);

	// 位置
	TackleSyncEffect();

}

void Enemy::JumpAttackPlayEffect(void)
{

	// エフェクト再生
	effectJumpAttackPlayId_ = PlayEffekseer3DEffect(effectJumpAttackResId_);

	float SCALE = 500.0f;
	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectJumpAttackPlayId_, SCALE, SCALE, SCALE);

	// 位置
	JumpAttackSyncEffect();

}

void Enemy::JumpAttackRangePlayEffect(void)
{

	// エフェクト再生
	effectJumpAttackRangePlayId_ = PlayEffekseer3DEffect(effectJumpAttackRangeResId_);

	float SCALE = 200.0f;
	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectJumpAttackRangePlayId_, SCALE, SCALE, SCALE);

	// 位置
	JumpAttackRangeSyncEffect();

}

void Enemy::CreateSyncEffect(void)
{

	// 追従対象の位置
	VECTOR followPos = transform_.pos;

	// 追従対象の向き
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectPos = followRot.PosAxis({ 0.0f,0.0f,0.0f });

	// エフェクトの位置の更新
	effectCreatePos_ = VAdd(followPos, effectPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectCreatePlayId_, effectCreatePos_.x, effectCreatePos_.y, effectCreatePos_.z);
	SetRotationPlayingEffekseer3DEffect(effectCreatePlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void Enemy::TackleSyncEffect(void)
{

	// 追従対象の位置
	VECTOR followPos = transform_.pos;

	// 追従対象の向き
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectPos = followRot.PosAxis({ 0.0f,0.0f,0.0f });

	// エフェクトの位置の更新
	effectTacklePos_ = VAdd(followPos, effectPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectTacklePlayId_, effectTacklePos_.x, effectTacklePos_.y, effectTacklePos_.z);
	SetRotationPlayingEffekseer3DEffect(effectTacklePlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void Enemy::JumpAttackSyncEffect(void)
{

	// 追従対象の位置
	VECTOR followPos = attackPlayerPos_;

	// 追従対象の向き
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectPos = followRot.PosAxis({ 0.0f,0.0f,.0f });

	// エフェクトの位置の更新
	effectJumpAttackPos_ = VAdd(followPos, effectPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectJumpAttackPlayId_, effectJumpAttackPos_.x, effectJumpAttackPos_.y, effectJumpAttackPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJumpAttackPlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void Enemy::JumpAttackRangeSyncEffect(void)
{

	// 追従対象の位置
	VECTOR followPos = attackPlayerPos_;

	// 追従対象の向き
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectPos = followRot.PosAxis({ 0.0f,10.0f,0.0f });

	// エフェクトの位置の更新
	effectJumpAttackRangePos_ = VAdd(followPos, effectPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectJumpAttackRangePlayId_, effectJumpAttackRangePos_.x, effectJumpAttackRangePos_.y, effectJumpAttackRangePos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJumpAttackRangePlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void Enemy::InitMusic(void)
{

	// 弾を作る音
	musicCreateId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CREATE_MUSIC).handleId_;

	// タックル音
	musicTackleId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TACKLE_MUSIC).handleId_;

	// 足音
	musicFootStepsId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_FOOTSTEPS_MUSIC).handleId_;

	// ジャンプアタックの音
	musicJumpAttackId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::JUMPATTACK_MUSIC).handleId_;

	// 地響きの音
	musicEarthQuakeId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::EARTHQUAKE_MUSIC).handleId_;

	// 攻撃のボイス１
	musicAttackVoice1Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_ATTACK_VOICE_MUSIC1).handleId_;

	// 攻撃のボイス２
	musicAttackVoice2Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_ATTACK_VOICE_MUSIC2).handleId_;

	// 攻撃のボイス３
	musicAttackVoice3Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_ATTACK_VOICE_MUSIC3).handleId_;

	// 攻撃のボイスのフラグ
	isMusicAttackVoice_ = true;

}

void Enemy::FootStepsMusic(void)
{

	if (musicFootStepsCnt_ <= 0.0f)
	{
		PlaySoundMem(musicFootStepsId_, DX_PLAYTYPE_BACK);
	}

	musicFootStepsCnt_ += SceneManager::GetInstance().GetDeltaTime();

}

void Enemy::AttackMusic(void)
{

	int number = GetRand(2);

	if (number == 0)
	{
		PlaySoundMem(musicAttackVoice1Id_, DX_PLAYTYPE_BACK);
	}
	else if (number == 1)
	{
		PlaySoundMem(musicAttackVoice2Id_, DX_PLAYTYPE_BACK);
	}
	else if (number == 2)
	{
		PlaySoundMem(musicAttackVoice3Id_, DX_PLAYTYPE_BACK);
	}

}

void Enemy::Update(void)
{

	//// 回転しきっていなかったら処理しない
	//if (isRotation_)
	//{
	//	return;
	//}

		// アニメーション処理
	Animation();

	if (noPlayTime_ > 0.0f || hp_ <= 0)
	{
		return;
	}

	// 必殺技になったら必殺技以外の処理を止める
	if (SceneManager::GetInstance().GetSceneID() == SceneManager::SCENE_ID::SPECIALMOVE)
	{
		// 必殺技の更新
		//SpecialMoveUpdate();
		return;
	}

	switch (state_)
	{
	case Enemy::STATE::THINK:
		break;
	case Enemy::STATE::IDLE:
		UpdateIdle();

		StopSoundMem(musicFootStepsId_);
		StopSoundMem(musicEarthQuakeId_);
		break;
	case Enemy::STATE::WALK:
		UpdateWalk();

		// 再生速度の設定
		SetFrequencySoundMem(30000, musicFootStepsId_);

		// 足音
		FootStepsMusic();
		break;
	case Enemy::STATE::ATTACK:
		if (stepAnim_ >= ATTACK_COLLISION_START_TIME
			&& stepAnim_ <= ATTACK_COLLISION_END_TIME
			&& !hit_)
		{
			attack_ = true;
		}
		UpdateAttack();
		break;
	case Enemy::STATE::JUMP_ATTACK:
		if (stepAnim_ >= JUMP_ATTACK_COLLISION_START_TIME
			&& stepAnim_ <= JUMP_ATTACK_COLLISION_END_TIME
			&& !hit_)
		{
			attack_ = true;
		}
		UpdateJumpAttack();

		if (stepAnim_ >= 40.0f && isEffectJumpAttack_)
		{
			// ジャンプアタックのエフェクト
			JumpAttackPlayEffect();
			isEffectJumpAttack_ = false;
			PlaySoundMem(musicJumpAttackId_,DX_PLAYTYPE_BACK);
			PlaySoundMem(musicEarthQuakeId_, DX_PLAYTYPE_BACK);
		}

		// エフェクトの位置
		JumpAttackSyncEffect();
		JumpAttackRangeSyncEffect();

		break;
	case Enemy::STATE::TACKLE:
		UpdateTackle();

		// エフェクトの位置
		TackleSyncEffect();

		// 足音のカウンタリセット
		if (musicFootStepsCnt_ >= 2.2f)
		{
			musicFootStepsCnt_ = 0.0f;
		}

		// 再生速度の設定
		SetFrequencySoundMem(120000, musicFootStepsId_);

		// 足音
		FootStepsMusic();

		break;
	case Enemy::STATE::CREATE:
		UpdateCreate();

		// エフェクトの位置
		CreateSyncEffect();
		break;
	case Enemy::STATE::SHOT:
		UpdateShot();
		break;
	case Enemy::STATE::HIT:
		//UpdateHit();
		break;
	case Enemy::STATE::DEATH:
		break;
	case Enemy::STATE::TURN_LEFT:
		// 再生速度の設定
		SetFrequencySoundMem(10000, musicFootStepsId_);

		// 足音
		FootStepsMusic();
		break;
	case Enemy::STATE::TURN_RIGHT:
		// 再生速度の設定
		SetFrequencySoundMem(10000, musicFootStepsId_);

		// 足音
		FootStepsMusic();
		break;
	}

	// 回転処理
	Rotation();

	// 衝突判定用
	Collision();

	transform_.pos.y = 0.0f;

	transform_.Update();

	walkCnt_ += SceneManager::GetInstance().GetDeltaTime();

	for (auto v : shots_)
	{
		v->Update();
	}

}

void Enemy::Draw(void)
{

	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(transform_.modelId);

	// デバッグ描画
	DrawDebug();

	// HPバー
	int hpLength = HP_LENGTH;
	int H;
	int hpGauge;
	H = hp_ * (512.0f / hpMax_) - 100;
	int R = min(max((384 - H), 0), 0xff);
	int G = min(max((H + 64), 0), 0xff);
	int B = max((H - 384), 0);
	hpGauge = hpLength * hp_ / hpMax_;

	// HPの描画
	if (hp_ >= 0)
	{
		DrawBox(700, 0, 700 + hpGauge, 30, GetColor(R, G, B), true);
	}

	for (auto v : shots_)
	{
		v->Draw();
	}

	Camera* camera = SceneManager::GetInstance().GetCamera();

	if (camera->GetMode() == Camera::MODE::LOCKON)
	{
		if (lockOnCursorTime_ <= 0.0f)
		{
			lockOnCursorCnt_ += 1;
		}

		lockOnCursorTime_ -= SceneManager::GetInstance().GetDeltaTime();

		// ロックオンカーソルのビルボード
		DrawBillboard3D(VAdd(transform_.pos, { 0.0f,500.0f + pos_.y,0.0f }), 0.5, 0.5, 256, 0.0f, lockOnCursorImg_[lockOnCursorCnt_], true);

		if (lockOnCursorCnt_ >= 58)
		{
			lockOnCursorCnt_ = 0;
		}
	}
	else
	{
		lockOnCursorCnt_ = 0;
		lockOnCursorTime_ = 0.0f;
	}

}

void Enemy::Release(void)
{

	// モデルの開放
	MV1DeleteModel(transform_.modelId);

	for (auto& s : shots_)
	{
		s->Release();
		delete s;
	}
	shots_.clear();

	StopEffekseer3DEffect(effectCreatePlayId_);
	StopEffekseer3DEffect(effectJumpAttackRangePlayId_);
	StopEffekseer3DEffect(effectJumpAttackPlayId_);
	StopEffekseer3DEffect(effectTacklePlayId_);

}

const Transform& Enemy::GetTransform(void) const
{
	return transform_;
}

Enemy::STATE Enemy::GetState(void)
{
	return state_;
}

void Enemy::SetState(Enemy::STATE state)
{
	ChangeState(state);
}

VECTOR Enemy::GetCBodyPosDown(void)
{
	return cBodyPosDown_;
}

VECTOR Enemy::GetCBodyPosUP(void)
{
	return cBodyPosUp_;
}

VECTOR Enemy::GetCWeponPosDown(void)
{
	return cWeponPosDown_;
}

VECTOR Enemy::GetCWeponPosUP(void)
{
	return cWeponPosUp_;
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

bool Enemy::GetAttack(void)
{
	return attack_;
}

void Enemy::SetAttack(bool attack)
{
	attack_ = attack;
}

bool Enemy::GetHit(void)
{
	return hit_;
}

void Enemy::SetHit(bool hit)
{
	hit_ = hit;
}

void Enemy::SetDeathCnt(int cnt)
{
	shotNum_ += cnt;
}

std::vector<ShotEnemy*>& Enemy::GetShots(void)
{
	return shots_;
}

void Enemy::SetStageID(const int modelId)
{
	stageId_ = modelId;
}

VECTOR Enemy::GetAttackPlayerPos(void)
{
	return attackPlayerPos_;
}

void Enemy::Think(void)
{

	// ベクトル方向
	VECTOR vec = { 0.0f,0.0f,0.0f };

	// 距離
	float length = 0.0f;

	// 攻撃の選択
	attackNumber_ = GetRand(3);
	//attackNumber_ = 1;
	
	// 攻撃が当たったかどうか
	hit_ = false;

	// 移動 --------------------------------------------------

	if (state_ != STATE::ATTACK && state_ != STATE::JUMP_ATTACK || state_ != STATE::TACKLE && stepAnim_ == 0.0f)
	{
		ChangeState(STATE::WALK);
	}

	if (walkCnt_ <= FIRST_WALK_TIME)
	{
		return;
	}

	// 通常攻撃 ----------------------------------------------

	// プレイヤーの方向を求める
	vec = VSub(followTransform_->pos, transform_.pos);
	length = AsoUtility::Magnitude(vec);

	// 敵とプレイヤーの距離が一定距離になったら攻撃する
	if (attackNumber_ == 0 && length < ATTACK_RANGE)
	{
		ChangeState(STATE::ATTACK);
	}

	// ジャンプ攻撃 ------------------------------------------

	if (attackNumber_ == 1)
	{

		// プレイヤーがいた座標を代入
		attackPlayerPos_ = followTransform_->pos;

		// プレイヤーの方向を求める
		vec = VSub(attackPlayerPos_, transform_.pos);
		length = AsoUtility::Magnitude(vec);

		// 正規化
		pDirection_ = VNorm(vec);

		// ジャンプ攻撃
		if ((length > JUMP_ATTACK_RANGE_MIN && length < JUMP_ATTACK_RANGE_MAX) || length < ATTACK_RANGE)
		{
			ChangeState(STATE::JUMP_ATTACK);
		}
	}

	// タックル攻撃 ------------------------------------------

	if (attackNumber_ == 2)
	{
		// プレイヤーがいた座標を代入
		attackPlayerPos_ = followTransform_->pos;

		// プレイヤーの方向を求める
		vec = VSub(attackPlayerPos_, transform_.pos);
		length = AsoUtility::Magnitude(vec);

		// 正規化
		pDirection_ = VNorm(vec);
		ChangeState(STATE::TACKLE);
	}

	// ショット攻撃-------------------------------------------

	if (attackNumber_ == 3)
	{
		ChangeState(STATE::CREATE);
	}

}

void Enemy::Rotation(void)
{

	// プレイヤーの方向を求める
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);

	vec = { -vec.x,vec.y,-vec.z };

	// 正規化
	VECTOR Vdirection = VNorm(vec);

	// 方向を角度に変換する(XZ平面 Y軸)
	float angle = atan2f(Vdirection.x, Vdirection.z);

	// 回転
	if (state_ != STATE::IDLE && state_ != STATE::ATTACK && state_ != STATE::JUMP_ATTACK && state_ != STATE::TACKLE)
	{
		LazyRotation(angle);
	}

}

void Enemy::AfterRotation(void)
{

	// プレイヤーの方向を求める
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);

	vec = { -vec.x,-vec.y,-vec.z };

	// 正規化
	VECTOR Vdirection = VNorm(vec);

	// 方向を角度に変換する(XZ平面 Y軸)
	float angle = atan2f(Vdirection.x, Vdirection.z);

	auto goal = Quaternion::Euler(0.0f, angle, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.02f);

	// ラジアンからデグリー
	float goalDeg = AsoUtility::Rad2DegF(angle);
	goalDeg = AsoUtility::DegIn360(goalDeg);

}

void Enemy::UpdateIdle(void)
{
}

void Enemy::UpdateWalk(void)
{

	// プレイヤーの方向を求める
	auto vec = VSub(followTransform_->pos, transform_.pos);

	// 正規化
	pDirection_ = VNorm(vec);

	// 移動量
	movePow_ = VScale(pDirection_, WALK_SPEED);

	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	// 移動処理
	transform_.pos = movedPos_;

}

void Enemy::UpdateAttack(void)
{

	attack_ = false;

	// 攻撃判定が入るアニメーションの秒数
	if (stepAnim_ >= ATTACK_COLLISION_START_TIME && stepAnim_ <= ATTACK_COLLISION_END_TIME && !hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateJumpAttack(void)
{

	// プレイヤーとの距離を求める
	VECTOR vec = VSub(attackPlayerPos_, transform_.pos);
	float length = AsoUtility::Magnitude(vec);
	attack_ = false;

	// 移動量
	movePow_ = VScale(pDirection_, JUMP_ATTACK_SPEED);

	if (stepAnim_ >= 20.0f && stepAnim_ <= 80.0f)
	{
		// プレイヤーとの距離が10.0f未満になるまで移動
		if (stepAnim_ <= JUMP_ATTACK_END_TIME)
		{
			if (length >= JUMP_ATTACK_RANGE_MIN)
			{

				// 現在座標を起点に移動後座標を決める
				movedPos_ = VAdd(transform_.pos, movePow_);

				// 移動処理
				transform_.pos = movedPos_;

			}
		}
	}

	// 攻撃判定が入るアニメーションの秒数
	if (stepAnim_ >= JUMP_ATTACK_COLLISION_START_TIME && stepAnim_ <= JUMP_ATTACK_COLLISION_END_TIME && !hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateTackle(void)
{

	// タックルする時間を計算
	tackleCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	// タックルし続ける間は座標を動かす
	if (tackleCnt_ > 0.0f)
	{
		// 移動量
		movePow_ = VScale(pDirection_, TACKLE_SPEED);

		// 現在座標を起点に移動後座標を決める
		movedPos_ = VAdd(transform_.pos, movePow_);

		// 移動処理
		transform_.pos = movedPos_;
	}

	// 終わったらIDLEに戻す
	if (tackleCnt_ < 0.0f)
	{
		ChangeState(STATE::IDLE);
	}

	// 攻撃フラグ
	if (!hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateCreate(void)
{

	// 弾の生成
	CreateShot();

}

void Enemy::UpdateShot(void)
{

	// 弾の発射処理
	ProcessShot();

	// 攻撃フラグ
	if (!hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateHit(void)
{

	// プレイヤーとの距離を求める
	VECTOR vec = VSub(attackPlayerPos_, transform_.pos);
	pDirection_ = VNorm(vec);

	if (stepAnim_ <= HIT_END_TIME)
	{
		transform_.pos = VAdd(transform_.pos, VScale(pDirection_, -HIT_SPEED));
	}

}

void Enemy::Collision(void)
{

	// 敵自身の当たり判定
	EnemyBodyCollision();

	// 敵の武器の当たり判定
	WeponCollision();

	// ステージとの衝突判定
	CollisionStage();

	transform_.Update();

}

void Enemy::CollisionStage(void)
{

	// カプセルとの衝突判定
	auto hits = MV1CollCheck_Capsule(
		stageId_, -1,
		cBodyPosUp_, cBodyPosDown_, COLLISION_BODY_RADIUS);

	// 衝突した複数のポリゴンと衝突回避するまで、
	// プレイヤーの位置を移動させる
	for (int i = 0; i < hits.HitNum; i++)
	{

		auto hit = hits.Dim[i];

		// 地面と異なり、衝突回避位置が不明なため、何度か移動させる
		// この時、移動させる方向は、移動前座標に向いた方向であったり、
		// 衝突したポリゴンの法線方向だったりする
		for (int tryCnt = 0; tryCnt < 10; tryCnt++)
		{

			// 再度、モデル全体と衝突検出するには、効率が悪過ぎるので、
			// 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
			int pHit = HitCheck_Capsule_Triangle(
				cBodyPosUp_, cBodyPosDown_, COLLISION_BODY_RADIUS,
				hit.Position[0], hit.Position[1], hit.Position[2]);

			if (pHit)
			{

				// 法線の方向にちょっとだけ移動させる
				movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));

				// カプセルも一緒に移動させる
				transform_.pos = movedPos_;
				transform_.Update();
				continue;

			}

			break;

		}

	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);

}

void Enemy::EnemyBodyCollision(void)
{

	// 敵の中心にカプセルを追従させる
	// カプセルをアタッチするフレームのローカル→ワールド変換行列を取得する
	MATRIX EnemyFrameMatrix = MV1GetFrameLocalWorldMatrix(transform_.modelId, enemyAttachFrameNum_);

	// 行列からラジアン
	auto rot = MGetRotElem(EnemyFrameMatrix);
	auto pos = MGetTranslateElem(EnemyFrameMatrix);

	// 行列からクォータニオン
	Quaternion qua = Quaternion::GetRotation(rot);

	// 追従対象の向き
	Quaternion followRot = qua;

	// 追従対象から自機までの相対座標
	VECTOR cPosDOWN = followRot.PosAxis(LOCAL_BODY_C_DOWN_POS);
	VECTOR cPosUP = followRot.PosAxis(LOCAL_BODY_C_UP_POS);

	// 敵の位置の更新
	cBodyPosDown_ = VAdd(pos, cPosDOWN);
	cBodyPosUp_ = VAdd(pos, cPosUP);

}

void Enemy::WeponCollision(void)
{

	// エネミー武器にカプセルを追従させる
	// カプセルをアタッチするフレームのローカル→ワールド変換行列を取得する
	MATRIX WeponFrameMatrix = MV1GetFrameLocalWorldMatrix(transform_.modelId, weponAttachFrameNum_);

	// 行列からラジアン
	auto rot = MGetRotElem(WeponFrameMatrix);
	auto pos = MGetTranslateElem(WeponFrameMatrix);

	// 行列からクォータニオン
	Quaternion qua = Quaternion::GetRotation(rot);

	// 追従対象の向き
	Quaternion followRot = qua;

	// 追従対象から自機までの相対座標
	VECTOR cPosDOWN = followRot.PosAxis(LOCAL_WEPON_C_DOWN_POS);
	VECTOR cPosUP = followRot.PosAxis(LOCAL_WEPON_C_UP_POS);

	// 敵の位置の更新
	//cWeponPosDown_ = VAdd(transform_.pos, cPosDOWN);
	//cWeponPosUp_ = VAdd(transform_.pos, cPosUP);
	cWeponPosDown_ = VAdd(pos, cPosDOWN);
	cWeponPosUp_ = VAdd(pos, cPosUP);

}

void Enemy::ChangeState(STATE state)
{

	// 前の状態を保持する
	preState_ = state_;

	// 状態の更新
	state_ = state;

	// 状態遷移時の初期化処理
	switch (state_)
	{
	case Enemy::STATE::THINK:
		// 回転のフラグを戻す
		isRotation_ = false;
		// これからの行動を考える
		Think();
		break;
	case Enemy::STATE::IDLE:
		SetIdleAnimation();

		// 音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;

		isEffectJumpAttack_ = true;
		break;
	case Enemy::STATE::WALK:
		SetWalkAnimation();
		break;
	case Enemy::STATE::ATTACK:
		SetAttackAnimation();

		// 音を止める
		StopSoundMem(musicFootStepsId_);

		// 音の再生
		AttackMusic();

		break;
	case Enemy::STATE::JUMP_ATTACK:
		SetJumpAttackAnimation();

		// エフェクトの再生
		JumpAttackRangePlayEffect();

		// 音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;

		// 音の再生
		AttackMusic();

		break;
	case Enemy::STATE::TACKLE:
		tackleCnt_ = 4.0f;
		SetTackleAnimation();

		// エフェクトの再生
		TacklePlayEffect();

		// 音の再生
		PlaySoundMem(musicTackleId_, DX_PLAYTYPE_BACK);

		// 音の再生
		AttackMusic();

		break;
	case Enemy::STATE::CREATE:
		SetCreateAnimation();

		// 音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;

		// エフェクトの再生
		CreatePlayEffect();

		// 音の再生
		PlaySoundMem(musicCreateId_, DX_PLAYTYPE_BACK);

		// 音の再生
		AttackMusic();

		break;
	case Enemy::STATE::SHOT:
		delayShot_ = TIME_DELAY_SHOT;
		SetShotAnimation();

		// 音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		StopSoundMem(musicCreateId_);

		// 音の再生
		AttackMusic();

		break;
	case Enemy::STATE::HIT:
		attackPlayerPos_ = followTransform_->pos;
		SetHitAnimation();

		// 音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Enemy::STATE::DEATH:
		SetDeathAnimation();

		// 音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Enemy::STATE::TURN_LEFT:
		SetTurnLeftAnimation();
		break;
	case Enemy::STATE::TURN_RIGHT:
		SetTurnRightAnimation();
		break;
	}

}

void Enemy::SetIdleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, idleAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = IDLE_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Enemy::SetWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, walkAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = WALK_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Enemy::SetAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = ATTACK_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Enemy::SetJumpAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, jumpAttackAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = JUMP_ATTACK_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Enemy::SetTackleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, tackleAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = TACKLE_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Enemy::SetCreateAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, createAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = SHOT_CREATE_END_TIME;

	// アニメーション速度
	speedAnim_ = SHOT_CREATE_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = SHOT_CREATE_START_TIME;

}

void Enemy::SetShotAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, shotAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = SHOT_END_TIME;

	// アニメーション速度
	speedAnim_ = SHOT_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = SHOT_START_TIME;

}

void Enemy::SetHitAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, hitAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = HIT_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 10.0f;

}

void Enemy::SetDeathAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, deathAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 20.0f;

	// アニメーション時間の初期化
	stepAnim_ = 15.0f;

}

void Enemy::SetTurnLeftAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, turnLeftAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = HIT_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Enemy::SetTurnRightAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, turnRightAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = HIT_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Enemy::LazyRotation(float goalRot)
{

	// 目的の角度まで回転させる
	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.05f);

	// ラジアンからデグリー
	float goalDeg = AsoUtility::Rad2DegF(goalRot);
	goalDeg = AsoUtility::DegIn360(goalDeg);

	auto rad = transform_.quaRot.ToEuler();
	auto deg = AsoUtility::Rad2DegF(rad.y);

	deg = AsoUtility::DegIn360(deg);

	// 目的の角度と自分の角度の差を測る
	float sub = goalDeg - deg;

	sub = abs(sub);

	// 差が10度未満だったらtrueにする
	if (state_ == STATE::TURN_LEFT || state_ == STATE::TURN_RIGHT)
	{
		if (sub <= GOAL_DEG && sub >= -GOAL_DEG)
		{
			isRotation_ = true;
		}
	}
}

void Enemy::DrawDebug(void)
{

	//// エネミー自身の衝突判定のカプセルの描画
	//DrawCapsule3D(cBodyPosDown_, cBodyPosUp_, COLLISION_BODY_RADIUS, 10, 0xff0000, 0xff0000, false);

	//// エネミー武器の衝突判定のカプセルの描画
	//DrawCapsule3D(cWeponPosDown_, cWeponPosUp_, COLLISION_WEPON_RADIUS, 10, 0xff0000, 0xff0000, false);

	//// ジャンプアタックの球体の描画
	//if (state_ == STATE::JUMP_ATTACK)
	//{
	//	DrawCone3D(VAdd(attackPlayerPos_, { 0.0f,0.0f,0.0f }), VAdd(attackPlayerPos_, { 0.0f,10.0f,0.0f }), 1000, 10, 0x0000ff, 0x0000ff, true);
	//}

}

void Enemy::SetParam(void)
{

	// HPの最大値
	hpMax_ = HP_MAX;

	// HP
	hp_ = hpMax_;

}

void Enemy::Animation(void)
{

	// 行動後に動かない時間を作る
	if (isAction_)
	{
		noPlayTime_ -= SceneManager::GetInstance().GetDeltaTime();
	}

	// アニメーション再生
	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// アニメーション時間の進行
	stepAnim_ += (speedAnim_ * deltaTime);
	if (state_ != STATE::SHOT || shotNum_ == 0)
	{
		isShot_ = false;
		if (stepAnim_ > animTotalTime_)
		{
			// ループ再生
			stepAnim_ = 0.0f;

			if (state_ != STATE::TACKLE)
			{

				// 待機状態にする
				ChangeState(STATE::IDLE);

				// アニメーションが終わったらtrueにする
				isAction_ = true;

				//// クールタイムをリセットする
				//noPlayTime_ = 3.0f;

				startRotation_ = true;

			}
			if (isNoPlay_ && state_ == STATE::IDLE && preState_ != STATE::WALK)
			{
				noPlayTime_ = COOL_TIME;
				isNoPlay_ = false;
			}
		}
	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);
	if (noPlayTime_ >= 0.0f)
	{
		return;
	}

	if (state_ == STATE::SHOT && stepAnim_ >= SHOT_END_TIME)
	{
		stepAnim_ = SHOT_START_TIME;
		isShot_ = false;
	}

	float goalDeg = 0.0f;
	// 行動後プレイヤー方向に角度を変える
	if ((state_ == STATE::IDLE || state_ == STATE::TURN_LEFT || state_ == STATE::TURN_RIGHT) && isAction_)
	{
		// 毎回入らないようにアニメーションの再生時間が終わったらfalseにする
		isAction_ = false;

		// プレイヤーの方向を求める
		VECTOR vec = VSub(followTransform_->pos, transform_.pos);

		vec = { -vec.x,vec.y,-vec.z };

		// 正規化
		VECTOR Vdirection = VNorm(vec);

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(Vdirection.x, Vdirection.z);

		auto goal = Quaternion::Euler(0.0f, angle, 0.0f);
		transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.02f);

		// クォータニオンからラジアン
		VECTOR rad = transform_.quaRot.ToEuler();

		// ラジアンからデグリー
		float deg = AsoUtility::Rad2DegF(rad.y);
		deg = AsoUtility::DegIn360(deg);

		// ラジアンからデグリー
		float goalDeg = AsoUtility::Rad2DegF(angle);
		goalDeg = AsoUtility::DegIn360(goalDeg);

		// 目的の角度に近い方向に旋回
		if (goalDeg <= deg)
		{
			ChangeState(STATE::TURN_LEFT);
		}
		else if (goalDeg >= deg)
		{
			ChangeState(STATE::TURN_RIGHT);
		}

	}

	// 行動を選択
	if (isRotation_)
	{
		ChangeState(STATE::THINK);
		isNoPlay_ = true;
	}

	// アニメーションの固定
	AnimationFrame();

}

void Enemy::AnimationFrame(void)
{

	// 対象フレームのローカル行列を初期値にリセットする
	MV1ResetFrameUserLocalMatrix(transform_.modelId, enemyPosFrameNum_);

	// ジャンプ攻撃時に座標を固定する
	if (state_ == STATE::JUMP_ATTACK || state_ == STATE::HIT)
	{

		// 対象フレームのローカル行列(大きさ、回転、位置)を取得する
		auto mat = MV1GetFrameLocalMatrix(transform_.modelId, enemyPosFrameNum_);

		auto scl = MGetSize(mat); // 行列から大きさを取り出す
		auto rot = MGetRotElem(mat); // 行列から回転を取り出す
		pos_ = MGetTranslateElem(mat); // 行列から移動値を取り出す

		// 大きさ、回転、位置をローカル行列に戻す
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // 大きさ
		mix = MMult(mix, rot); // 回転

		// ここでローカル座標を行列に、そのまま戻さず、
		// 調整したローカル座標を設定する
		mix = MMult(mix, MGetTranslate({ 0.0f, pos_.y, 0.0f }));

		// 合成した行列を対象フレームにセットし直して、
		// アニメーションの移動値を無効化
		MV1SetFrameUserLocalMatrix(transform_.modelId, enemyPosFrameNum_, mix);

	}

}

void Enemy::ProcessShot(void)
{

	auto& ins = InputManager::GetInstance();

	delayShot_ -= SceneManager::GetInstance().GetDeltaTime();

	// 弾を時間をずらして飛ばす
	if (stepAnim_ >= 0.1f && !isShot_)
	{
		Shot();
		isShot_ = true;
	}

}

void Enemy::Shot(void)
{

	if (shotNum_ < 0) return;
	// 背中にある弾から撃つ弾(待機状態の弾)選んで発射
	for (auto& shot : shots_)
	{
		if (shot->IsIdle())
		{

			// 弾の方向
			auto vec = VSub(VAdd(followTransform_->pos, { 0.0f,100.0f,0.0f }), shot->GetPos());
			auto dir = VNorm(vec);

			// 発射処理
			shot->Shot(dir);
			shotNum_--;

			AttackMusic();
			break;

		}
	}

}


void Enemy::CreateShot(void)
{

	// 8個の弾を敵の背後に作る(8回ループ)
	if (shotNum_ < 10 && delayCreate_ <= 0.0f)
	{

		// 利用可能なものを探す
		ShotEnemy* shot = GetAvailableShot();

		// デグリーをラジアンに変換
		shotCreateRad_ = shotCreateDeg_ * DX_PI_F / 180.0f;

		// 45度ずつ弾の位置を変える
		shotCreateDeg_ += (360 / 10);

		// 角度から方向(ベクトル)を求める
		auto qua = Quaternion::Euler({ 0.0f,0.0f,shotCreateRad_ });

		// 相対座標
		auto rPos = qua.PosAxis(LOCAL_SHOT_POS);

		// 弾の作成
		shot->Create(rPos, &transform_);

		// 弾を生成したら増やす
		shotNum_++;

		// 生成したらまた時間を増やす
		delayCreate_ = TIME_DELAY_SHOT_CREATE;

	}

	delayCreate_ -= SceneManager::GetInstance().GetDeltaTime();

	// アニメーションが終わったらショットに移行する
	if (stepAnim_ >= SHOT_CREATE_END_TIME - 1.0f)
	{
		ChangeState(STATE::SHOT);
	}

}

ShotEnemy* Enemy::GetAvailableShot(void)
{

	// 再利用探索
	for (auto& v : shots_)
	{
		if (!v->IsAlive())
		{
			return v;
		}
	}

	// なかった場合作成
	ShotEnemy* newShot = new ShotEnemy();
	//newShot->Create({ transform_.pos.x,
	//	transform_.pos.y + 150 ,
	//	transform_.pos.z },
	//	transform_.GetForward());

	// 弾の管理配列に追加
	shots_.push_back(newShot);
	return newShot;

}