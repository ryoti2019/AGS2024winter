#include <EffekseerForDXLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationContorller.h"
#include "Stage.h"
#include "Enemy.h"
#include "Player.h"

Player::Player(void)
{

	// 溜めるエフェクト
	effectChargeResId_ = -1;
	effectChargePlayId_ = -1;

}

Player::~Player(void)
{
}

void Player::InitAnimation(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_MODEL));

	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = new AnimationController(transform_.modelId);
	animationController_->Add("IDLE", path + "Idle.mv1", 0.0f, 300.0f, IDLE_ANIM_SPEED, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_IDLE), true, false);
	animationController_->Add("WALK", path + "walk.mv1", 0.0f, 21.0f, WALK_ANIM_SPEED, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_WALK), true, false);
	animationController_->Add("CHARGE_WALK", path + "chargeWalk.mv1", 0.0f, 33.0f, CHARGE_WALK_ANIM_SPEED, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_CHARGE_WALK), true, false);
	animationController_->Add("RUN", path + "run.mv1", 0.0f, 22.0f, RUN_ANIM_SPEED, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_RUN), true, false);
	animationController_->Add("ATTACK", path + "attack1.mv1", 0.0f, 45.0f, ATTACK_ANIM_SPEED, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_ATTACK1), false, false);
	animationController_->Add("ATTACK2", path + "attack2.mv1", 0.0f, 50.0f, ATTACK_ANIM_SPEED, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_ATTACK2), false, false);
	animationController_->Add("ATTACK3", path + "attack3.mv1", 0.0f, 73.0f, ATTACK_ANIM_SPEED, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_ATTACK3), false, false);
	animationController_->Add("CHARGE_ATTACK", path + "chargeAttack", 52.0f, 0.0f, CHARGE_ATTACK_ANIM_SPEED, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_CHARGEATTACK), false, false);
	animationController_->Add("HIT", path + "hit.mv1", 0.0f, 21.0f, HIT_ANIM_SPEED, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_HIT), false, false);
	animationController_->Add("DEATH", path + "death.mv1", 0.0f, 117.0f, 20.0f, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_DEATH), false, false);
	animationController_->Add("ROLL", path + "roll.mv1", 0.0f, 71.0f, ROLL_ANIM_SPEED, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_ROLL), false, false);
	animationController_->Add("TIRED", path + "tired.mv1", 0.0f, 80.0f, 50.0f, ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_TIRED), true, false);

	animationController_->ChangeAnimation("IDLE");

//#pragma region アニメーション
//
	// 待機
	//animData_[(int)STATE::IDLE].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
		//ResourceManager::SRC::PLAYER_IDLE);

//	// 歩く
//	animData_[(int)STATE::WALK].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_WALK);
//
//	// 溜めながら歩く
//	animData_[(int)STATE::CHARGE_WALK].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_CHARGE_WALK);
//	
//	// 走る
//	animData_[(int)STATE::RUN].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_RUN);
//
//	// 攻撃
//	animData_[(int)STATE::ATTACK].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//			ResourceManager::SRC::PLAYER_ATTACK1);
//
//	// 攻撃2
//	animData_[(int)STATE::ATTACK2].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_ATTACK2);
//
//	// 攻撃3
//	animData_[(int)STATE::ATTACK3].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_ATTACK3);
//
//	// 溜め攻撃
//	animData_[(int)STATE::CHARGE_ATTACK].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_CHARGEATTACK);
//
//	// ダメージヒット
//	animData_[(int)STATE::HIT].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_HIT);
//
//	// 死亡
//	animData_[(int)STATE::DEATH].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_DEATH);
//
//	// 回避
//	animData_[(int)STATE::ROLL].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_ROLL);
//	
//	// 疲れた
//	animData_[(int)STATE::
// 
// ].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_TIRED);
//
//#pragma endregion
//
//#pragma region 再生速度
//
//	// 待機
//	animData_[(int)STATE::IDLE].speedAnim_ = IDLE_ANIM_SPEED;
//
//	// 歩く
//	animData_[(int)STATE::WALK].speedAnim_ = IDLE_ANIM_SPEED;
//
//	// 溜めながら歩く
//	animData_[(int)STATE::CHARGE_WALK].speedAnim_ = CHARGE_WALK_ANIM_SPEED;
//
//	// 走る
//	animData_[(int)STATE::RUN].speedAnim_ = RUN_ANIM_SPEED;
//
//	// 攻撃
//	animData_[(int)STATE::ATTACK].speedAnim_ = ATTACK_ANIM_SPEED;
//
//	// 攻撃2
//	animData_[(int)STATE::ATTACK2].speedAnim_ = ATTACK_ANIM_SPEED;
//
//	// 攻撃3
//	animData_[(int)STATE::ATTACK3].speedAnim_ = ATTACK_ANIM_SPEED;
//
//	// 溜め攻撃
//	animData_[(int)STATE::CHARGE_ATTACK].speedAnim_ = CHARGE_ATTACK_ANIM_SPEED;
//
//	// ダメージヒット
//	animData_[(int)STATE::HIT].speedAnim_ = HIT_ANIM_SPEED;
//
//	// 死亡
//	animData_[(int)STATE::DEATH].speedAnim_ = 20.0f;
//
//	// 回避
//	animData_[(int)STATE::ROLL].speedAnim_ = ROLL_ANIM_SPEED;
//
//	// 疲れた
//	animData_[(int)STATE::TIRED].speedAnim_ = 50.0f;
//
//#pragma endregion
//
//#pragma region ブレンドタイム
//
//	// 待機
//	animData_[(int)STATE::IDLE].blendTime_ = 0.5f;
//
//	// 歩く
//	animData_[(int)STATE::WALK].blendTime_ = 0.5f;
//
//	// 溜めながら歩く
//	animData_[(int)STATE::CHARGE_WALK].blendTime_ = 0.5f;
//
//	// 走る
//	animData_[(int)STATE::RUN].blendTime_ = 0.5f;
//
//	// 攻撃
//	animData_[(int)STATE::ATTACK].blendTime_ = 0.2f;
//
//	// 攻撃2
//	animData_[(int)STATE::ATTACK2].blendTime_ = 0.2f;
//
//	// 攻撃3
//	animData_[(int)STATE::ATTACK3].blendTime_ = 0.2f;
//
//	// 溜め攻撃
//	animData_[(int)STATE::CHARGE_ATTACK].blendTime_ = 0.5f;
//
//	// ダメージヒット
//	animData_[(int)STATE::HIT].blendTime_ = 0.5f;
//
//	// 死亡
//	animData_[(int)STATE::DEATH].blendTime_ = 0.5f;
//
//	// 回避
//	animData_[(int)STATE::ROLL].blendTime_ = 0.5f;
//
//	// 疲れた
//	animData_[(int)STATE::TIRED].blendTime_ = 0.5f;
//
//#pragma endregion
//
//#pragma region アニメーションが始まる時間
//
//	// 待機
//	animData_[(int)STATE::IDLE].stepAnim_ = 0.0f;
//
//	// 歩く
//	animData_[(int)STATE::WALK].stepAnim_ = 0.0f;
//
//	// 溜めながら歩く
//	animData_[(int)STATE::CHARGE_WALK].stepAnim_ = 0.0f;
//
//	// 走る
//	animData_[(int)STATE::RUN].stepAnim_ = 0.0f;
//
//	// 攻撃
//	animData_[(int)STATE::ATTACK].stepAnim_ = 0.0f;
//
//	// 攻撃2
//	animData_[(int)STATE::ATTACK2].stepAnim_ = 0.0f;
//
//	// 攻撃3
//	animData_[(int)STATE::ATTACK3].stepAnim_ = 0.0f;
//
//	// 溜め攻撃
//	animData_[(int)STATE::CHARGE_ATTACK].stepAnim_ = 0.0f;
//
//	// ダメージヒット
//	animData_[(int)STATE::HIT].stepAnim_ = 0.0f;
//
//	// 死亡
//	animData_[(int)STATE::DEATH].stepAnim_ = 0.0f;
//
//	// 回避
//	animData_[(int)STATE::ROLL].stepAnim_ = 0.0f;
//
//	// 疲れた
//	animData_[(int)STATE::TIRED].stepAnim_ = 0.0f;
//
//#pragma endregion
//
//#pragma region アニメーションの状態
//
//	// 待機
//	animData_[(int)STATE::IDLE].state_ = STATE::IDLE;
//
//	// 歩く
//	animData_[(int)STATE::WALK].state_ = STATE::WALK;
//
//	// 溜めながら歩く
//	animData_[(int)STATE::CHARGE_WALK].state_ = STATE::CHARGE_WALK;
//
//	// 走る
//	animData_[(int)STATE::RUN].state_ = STATE::RUN;
//
//	// 攻撃
//	animData_[(int)STATE::ATTACK].state_ = STATE::ATTACK;
//
//	// 攻撃2
//	animData_[(int)STATE::ATTACK2].state_ = STATE::ATTACK2;
//
//	// 攻撃3
//	animData_[(int)STATE::ATTACK3].state_ = STATE::ATTACK3;
//
//	// 溜め攻撃
//	animData_[(int)STATE::CHARGE_ATTACK].state_ = STATE::CHARGE_ATTACK;
//
//	// ダメージヒット
//	animData_[(int)STATE::HIT].state_ = STATE::HIT;
//
//	// 死亡
//	animData_[(int)STATE::DEATH].state_ = STATE::DEATH;
//
//	// 回避
//	animData_[(int)STATE::ROLL].state_ = STATE::ROLL;
//
//	// 疲れた
//	animData_[(int)STATE::TIRED].state_ = STATE::TIRED;
//
//#pragma endregion

	// アニメーションする番号
	animNo_ = 1;

	//// 待機アニメーション
	//ChangeAnimation(STATE::IDLE,0);

	//// 再生するアニメーションの設定
	//preAnimAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, walkAnim_);

	// transformの初期化
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(180), AsoUtility::AXIS_Y));
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	//// 再生するアニメーションの設定
	//animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_,idleAnim_);

	//preAnimAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_,idleAnim_);

	//// 再生中のアニメーション時間
	//stepAnim_ = 0.0f;

	//// アニメーション速度
	//speedAnim_ = 0.0f;

}

void Player::InitEffect(void)
{

	// 溜めのエフェクト
	effectChargeResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGE_EFFECT).handleId_;

}

void Player::InitMusic(void)
{

	// 溜める音
	musicChargeId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_CHARGE_MUSIC).handleId_;

	// 風を切る音
	musicSlash1Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SLASH_MUSIC1).handleId_;

	// 風を切る音
	musicSlash2Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SLASH_MUSIC2).handleId_;

	// 風を切る音のフラグ
	isMusicSlash_ = true;

	// 足音
	musicFootStepsId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_FOOTSTEPS_MUSIC).handleId_;

	// 足音のカウンタ
	musicFootStepsCnt_ = 0.0f;

	// プレイヤーの攻撃ボイス１
	musicSlashVoice1Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SLASH_VOICE_MUSIC1).handleId_;

	// プレイヤーの攻撃ボイス２
	musicSlashVoice2Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SLASH_VOICE_MUSIC2).handleId_;

	// プレイヤーの溜め攻撃ボイス
	musicChargeSlashVoiceId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGEATTACK_VOICE_MUSIC).handleId_;

	// 回避音
	musicRollId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ROLL_MUSIC).handleId_;
	ChangeVolumeSoundMem(255 * 70 / 100, musicRollId_);

	// 回避ボイス１
	musicRollVoice1Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ROLL_VOICE_MUSIC1).handleId_;

	// 回避ボイス２
	musicRollVoice2Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ROLL_VOICE_MUSIC2).handleId_;

	// 回避音のフラグ
	isMusicRoll_ = true;

}

void Player::Init(void)
{

	// アニメーションの初期設定
	InitAnimation();

	// エフェクトの初期設定
	InitEffect();

	// 音の初期設定
	InitMusic();

	// プレイヤーのパラメーター
	SetParam();

	// アニメーション速度
	speedAnim_ = WALK_ANIM_SPEED;

	// カプセルをアタッチするフレームの番号を検索
	playerAttachFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:Hips");

	// 攻撃が当たったかどうか
	hit_ = false;

	// 攻撃したかどうか
	attack_ = false;

	// 攻撃１段階目
	attack1_ = false;

	// 攻撃２段階目
	attack2_ = false;

	// 攻撃３段階目
	attack3_ = false;

	// 攻撃４
	chargeAttack_ = false;

	// 回避中は無敵
	isInvincible_ = false;

	// 疲れた時のフラグ
	isTired_ = false;

	// 移動方向
	moveDir_ = AsoUtility::VECTOR_ZERO;

	staminaCnt_ = 0.0f;

	limitCnt_ = 0.0f;

	blendTime_ = 0.5;

	stepBlend_ = 0.0f;

	list_ = 0;

	//AttatchNum((int)STATE::IDLE);

}

void Player::Update(void)
{

	// 状態の遷移
	switch (state_)
	{
	case Player::STATE::IDLE:
		break;
	case Player::STATE::WALK:

		// 足音のカウンタリセット
		if (musicFootStepsCnt_ >= 10.0f)
		{
			musicFootStepsCnt_ = 0.0f;
		}

		// 再生速度の設定
		SetFrequencySoundMem(35000, musicFootStepsId_);

		if (state_ != STATE::CHARGE_WALK)
		{
			// 足音
			FootStepsMusic();
		}

		break;
	case Player::STATE::CHARGE_WALK:
		ChargeSyncEffect();
		break;
	case Player::STATE::RUN:

		// 足音のカウンタリセット
		if (musicFootStepsCnt_ >= 7.0f)
		{
			musicFootStepsCnt_ = 0.0f;
		}

		// 再生速度の設定
		SetFrequencySoundMem(50000, musicFootStepsId_);

		if (state_ != STATE::CHARGE_WALK)
		{
			// 足音
			FootStepsMusic();
		}

		if (staminaCnt_ >= 0.1f)
		{
			stamina_ -= 1.0f;
			staminaCnt_ = 0.0f;
		}

		break;
	case Player::STATE::ATTACK:
		// 風を切る音の再生
		SlashMusic();
		if (animationController_->GetAnimData("ATTACK").stepAnim >= animationController_->GetAnimData("ATTACK").startTime
			&& animationController_->GetAnimData("ATTACK").stepAnim <= ATTACK_COLLISION_END_TIME1
			&& !hit_)
		{
			attack_ = true;
		}
		if (animationController_->GetAnimData("ATTACK").stepAnim >= animationController_->GetAnimData("ATTACK").animTotalTime - 1.0f)
		{
			attack1_ = false;
			isMusicSlash_ = true;
		}
		break;
	case Player::STATE::ATTACK2:
		// 風を切る音の再生
		SlashMusic();
		if (animationController_->GetAnimData("ATTACK2").stepAnim >= animationController_->GetAnimData("ATTACK2").startTime
			&& animationController_->GetAnimData("ATTACK2").stepAnim <= animationController_->GetAnimData("ATTACK2").animTotalTime
			&& !hit_)
		{
			attack_ = true;
		}
		if (animationController_->GetAnimData("ATTACK2").stepAnim >= animationController_->GetAnimData("ATTACK2").animTotalTime - 1.0f)
		{
			attack2_ = false;
			isMusicSlash_ = true;
		}
		break;
	case Player::STATE::ATTACK3:
		// 風を切る音の再生
		SlashMusic();
		if (animationController_->GetAnimData("ATTACK3").stepAnim >= animationController_->GetAnimData("ATTACK3").startTime
			&& animationController_->GetAnimData("ATTACK3").stepAnim <= animationController_->GetAnimData("ATTACK").animTotalTime
			&& !hit_)
		{
			attack_ = true;
		}
		break;
	case Player::STATE::CHARGE_ATTACK:
		// 風を切る音の再生
		SlashMusic();
		if (animationController_->GetAnimData("CHARGE_ATTACK").stepAnim >= animationController_->GetAnimData("CHARGE_ATTACK").startTime
			&& animationController_->GetAnimData("CHARGE_ATTACK").stepAnim <= animationController_->GetAnimData("CHARGE_ATTACK").animTotalTime
			&& !hit_)
		{
			attack_ = true;
		}
		break;
	case Player::STATE::HIT:
		if (preState_ == STATE::TIRED && animationController_->GetAnimData("HIT").stepAnim >= 20.0f)
		{
			ChangeState(STATE::TIRED);
		}
		break;
	case Player::STATE::DEATH:
		break;
	case Player::STATE::ROLL:
		RollMusic();
		if (animationController_->GetAnimData("ROLL").stepAnim >= 5.0f && animationController_->GetAnimData("ROLL").stepAnim <= 45.0f)
		{
			// 移動量
			movePow_ = VScale(moveDir_, speed_);

			// 現在座標を起点に移動後座標を決める
			movedPos_ = VAdd(transform_.pos, movePow_);

			isInvincible_ = true;

		}

		if ((animationController_->GetAnimData("ROLL").stepAnim >= 0.0f && animationController_->GetAnimData("ROLL").stepAnim <= 9.9f)
			|| (animationController_->GetAnimData("ROLL").stepAnim >= 45.1f && animationController_->GetAnimData("ROLL").stepAnim <= 71.0f))
		{
			isInvincible_ = false;
		}

		if (animationController_->GetAnimData("ROLL").stepAnim >= 45.0f)
		{
			speed_ = 0.0f;
		}

		break;
	case Player::STATE::TIRED:
		// スタミナが回復したら動ける
		if (stamina_ >= 100)
		{
			isTired_ = false;
			ChangeState(STATE::IDLE);
			stamina_ = 100.0f;
		}

		// スタミナを回復する
		if (state_ == STATE::TIRED && isTired_ && staminaCnt_ >= 0.1f)
		{
			stamina_ += 1.0f;
			staminaCnt_ = 0.0f;
		}

		break;
	}

	staminaCnt_ += SceneManager::GetInstance().GetDeltaTime();

	if ((state_ != STATE::RUN && state_ != STATE::ROLL && state_ != STATE::TIRED) 
		&& staminaCnt_ >= 0.1f && stamina_ < staminaMax_)
	{
		stamina_ += 0.5f;
		staminaCnt_ = 0.0f;
	}

	// スタミナが切れたら疲れたアニメーションに移行
	if (stamina_ <= 0.0f)
	{
		isTired_ = true;
		ChangeState(STATE::TIRED);
	}

	// HPが0になったら操作できないようにする
	if (hp_ > 0)
	{
		limitCnt_ += SceneManager::GetInstance().GetDeltaTime();
		if (limitCnt_ <= 0.5f)
		{
			return;
		}

		// キーボードでの操作
		if (!SceneManager::GetInstance().GetGamePad())
		{
			KeyboardContoroller();
		}

		// ゲームパッドでの操作
		if (SceneManager::GetInstance().GetGamePad())
		{
			GamePadController();
		}
	}

	//// 待機アニメーション
	//ChangeAnimation(state_);

	// アニメーション処理
	animationController_->Update();
	Animation();

	// 衝突判定
	Collision();




	transform_.pos.y = 0.0f;

	transform_.Update();

}

void Player::Draw(void)
{

	// モデルの描画
	UnitBase::Draw();

	// デバッグ描画
	//DrawDebug();

	// アタッチされている数
	DrawFormatString(0, 0, 0xff0000, "list:%d", animationController_->GetAttachNum());

}

void Player::ChargePlayEffect(void)
{

	// エフェクト再生
	effectChargePlayId_ = PlayEffekseer3DEffect(effectChargeResId_);

	float SCALE = 100.0f;
	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectChargePlayId_, SCALE, SCALE, SCALE);

	// 位置
	ChargeSyncEffect();

}

void Player::ChargeSyncEffect(void)
{

	// 追従対象の位置
	VECTOR followPos = transform_.pos;

	// 追従対象の向き
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectLPos = followRot.PosAxis(LOCAL_CHRAGE_POS);

	// エフェクトの位置の更新
	effectChargePos_ = VAdd(followPos, effectLPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectChargePlayId_, effectChargePos_.x, effectChargePos_.y, effectChargePos_.z);
	SetRotationPlayingEffekseer3DEffect(effectChargePlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void Player::SlashMusic(void)
{

	int number = GetRand(1);
	if (animationController_->GetAnimData("ATTACK").stepAnim >= animationController_->GetAnimData("ATTACK").startTime && isMusicSlash_ && state_ == STATE::ATTACK)
	{
		if (number == 0)
		{
			PlaySoundMem(musicSlash1Id_, DX_PLAYTYPE_BACK);
			PlaySoundMem(musicSlashVoice1Id_, DX_PLAYTYPE_BACK);
			isMusicSlash_ = false;
		}
		else if (number == 1)
		{
			PlaySoundMem(musicSlash2Id_, DX_PLAYTYPE_BACK);
			PlaySoundMem(musicSlashVoice2Id_, DX_PLAYTYPE_BACK);
			isMusicSlash_ = false;
		}

	}

	if (animationController_->GetAnimData("ATTACK2").stepAnim >= animationController_->GetAnimData("ATTACK").startTime && isMusicSlash_ && state_ == STATE::ATTACK2)
	{
		if (number == 0)
		{
			PlaySoundMem(musicSlash1Id_, DX_PLAYTYPE_BACK);
			PlaySoundMem(musicSlashVoice1Id_, DX_PLAYTYPE_BACK);
			isMusicSlash_ = false;
		}
		else if (number == 1)
		{
			PlaySoundMem(musicSlash2Id_, DX_PLAYTYPE_BACK);
			PlaySoundMem(musicSlashVoice2Id_, DX_PLAYTYPE_BACK);
			isMusicSlash_ = false;
		}
	}

	if (animationController_->GetAnimData("ATTACK3").stepAnim >= animationController_->GetAnimData("ATTACK3").startTime && isMusicSlash_ && state_ == STATE::ATTACK3)
	{
		if (number == 0)
		{
			PlaySoundMem(musicSlash1Id_, DX_PLAYTYPE_BACK);
			PlaySoundMem(musicSlashVoice1Id_, DX_PLAYTYPE_BACK);
			isMusicSlash_ = false;
		}
		else if (number == 1)
		{
			PlaySoundMem(musicSlash2Id_, DX_PLAYTYPE_BACK);
			PlaySoundMem(musicSlashVoice2Id_, DX_PLAYTYPE_BACK);
			isMusicSlash_ = false;
		}
	}

	if (animationController_->GetAnimData("CHARGE_ATTACK").stepAnim >= animationController_->GetAnimData("CHARGE_ATTACK").startTime && isMusicSlash_ && state_ == STATE::CHARGE_ATTACK)
	{
		if (number == 0)
		{
			PlaySoundMem(musicSlash1Id_, DX_PLAYTYPE_BACK);
			PlaySoundMem(musicChargeSlashVoiceId_, DX_PLAYTYPE_BACK);
			isMusicSlash_ = false;
		}
		else if (number == 1)
		{
			PlaySoundMem(musicSlash2Id_, DX_PLAYTYPE_BACK);
			PlaySoundMem(musicChargeSlashVoiceId_, DX_PLAYTYPE_BACK);
			isMusicSlash_ = false;
		}
	}
}

void Player::FootStepsMusic(void)
{

	if (musicFootStepsCnt_ <= 0.0f)
	{
		PlaySoundMem(musicFootStepsId_, DX_PLAYTYPE_BACK);
	}

	musicFootStepsCnt_ += SceneManager::GetInstance().GetDeltaTime();

}

void Player::RollMusic(void)
{

	int number = GetRand(1);
	if (animationController_->GetAnimData("ROLL").stepAnim >= animationController_->GetAnimData("ROLL").startTime && isMusicRoll_ && state_ == STATE::ROLL)
	{
		if (number == 0)
		{
			PlaySoundMem(musicRollId_, DX_PLAYTYPE_BACK);
			PlaySoundMem(musicRollVoice1Id_, DX_PLAYTYPE_BACK);
			isMusicRoll_ = false;
		}
		else if (number == 1)
		{
			PlaySoundMem(musicRollId_, DX_PLAYTYPE_BACK);
			PlaySoundMem(musicRollVoice2Id_, DX_PLAYTYPE_BACK);
			isMusicRoll_ = false;
		}
	}

}

//void Player::Blend(void)
//{
//
//}

void Player::Release(void)
{

	// モデルの開放
	MV1DeleteModel(transform_.modelId);

	MV1DeleteModel(idleAnim_);
	MV1DeleteModel(walkAnim_);
	MV1DeleteModel(chargeWalkAnim_);
	MV1DeleteModel(runAnim_);
	MV1DeleteModel(attackAnim_);
	MV1DeleteModel(chargeAttackAnim_);
	MV1DeleteModel(hitAnim_);
	MV1DeleteModel(deathAnim_);
	MV1DeleteModel(rollAnim_);

	DeleteEffekseerEffect(effectChargeResId_);
	DeleteEffekseerEffect(effectChargePlayId_);

	DeleteSoundMem(musicChargeId_);
	DeleteSoundMem(musicSlash1Id_);
	DeleteSoundMem(musicSlash2Id_);
	DeleteSoundMem(musicFootStepsId_);
	DeleteSoundMem(musicSlashVoice1Id_);
	DeleteSoundMem(musicSlashVoice2Id_);
	DeleteSoundMem(musicChargeSlashVoiceId_);
	DeleteSoundMem(musicRollId_);
	DeleteSoundMem(musicRollVoice1Id_);
	DeleteSoundMem(musicRollVoice2Id_);

}

const Transform& Player::GetTransform(void) const
{
	return transform_;
}

Player::STATE Player::GetState(void)
{
	return state_;
}

void Player::SetState(Player::STATE state)
{
	ChangeState(state);
}

VECTOR Player::GetCPosDown(void)
{
	return cBodyPosDown_;
}

VECTOR Player::GetCPosUP(void)
{
	return cBodyPosUp_;
}

void Player::SetPos(VECTOR pos)
{
	transform_.pos = VAdd(transform_.pos, pos);
}

bool Player::GetAttack(void)
{
	return attack_;
}

void Player::SetAttack(bool attack)
{
	attack_ = attack;
}

int Player::GetHP(void)
{
	return hp_;
}

void Player::SetHP(int hp)
{
	hp_ += hp;
}

float Player::GetStamina(void)
{
	return stamina_;
}

bool Player::GetHit(void)
{
	return hit_;
}

void Player::SetHit(bool hit)
{
	hit_ = hit;
}

void Player::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

void Player::SetStageID(const int modelId)
{
	stageId_ = modelId;
}

bool Player::GetInvincible(void)
{
	return isInvincible_;
}

void Player::Collision(void)
{

	// 敵自身の当たり判定
	PlayerBodyCollision();

	// ステージの当たり判定
	CollisionStage();

}

void Player::PlayerBodyCollision(void)
{

	// 敵の中心にカプセルを追従させる
	// カプセルをアタッチするフレームのローカル→ワールド変換行列を取得する
	MATRIX PlayerFrameMatrix = MV1GetFrameLocalWorldMatrix(transform_.modelId, 0);

	// 行列からラジアン
	auto rot = MGetRotElem(PlayerFrameMatrix);
	auto pos = MGetTranslateElem(PlayerFrameMatrix);

	// 行列からクォータニオン
	Quaternion qua = Quaternion::GetRotation(rot);

	// 追従対象の向き
	Quaternion followRot = qua;

	// 追従対象から自機までの相対座標
	VECTOR cPosDOWN = followRot.PosAxis(LOCAL_BODY_C_DOWN_POS);
	VECTOR cPosUP = followRot.PosAxis(LOCAL_BODY_C_UP_POS);

	// プレイヤーの位置の更新
	cBodyPosDown_ = VAdd(pos, cPosDOWN);
	cBodyPosUp_ = VAdd(pos, cPosUP);

}

void Player::CollisionStage(void)
{

	auto vec = VSub({ 0.0f,-300.0f, 500.0f },transform_.pos);

	float length = AsoUtility::Magnitude(vec);

	auto dir = VNorm(vec);

	if (length >= 3500.0f && state_ != STATE::ATTACK && state_ != STATE::ATTACK2 && state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK)
	{
		// 法線の方向にちょっとだけ移動させる
		movedPos_ = VAdd(movedPos_, VScale(dir, 20.0f));

		// カプセルも一緒に移動させる
		transform_.pos = movedPos_;
		transform_.Update();
	}

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
				movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.2f));

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

void Player::KeyboardContoroller(void)
{

	// 移動処理
	KeyboardMove();

	//攻撃処理
	KeyboardAttack();

	// プレイヤー方向にカメラを向ける処理
	KeyBoardCamera();

	// 敵をロックオンする処理
	KeyBoardLockOn();

}

void Player::KeyboardMove(void)
{

	// カメラの取得
	Camera* camera = SceneManager::GetInstance().GetCamera();

	// カメラの角度を取得
	VECTOR cameraAngles = camera->GetAngles();

	auto& ins = InputManager::GetInstance();

	// 方向(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// WASDでプレイヤーの位置を変える
	if (ins.IsNew(KEY_INPUT_W)) { dir = VAdd(dir, { 0.0f, 0.0f, 1.0f }); }
	if (ins.IsNew(KEY_INPUT_A)) { dir = VAdd(dir, { -1.0f, 0.0f, 0.0f }); }
	if (ins.IsNew(KEY_INPUT_S)) { dir = VAdd(dir, { 0.0f, 0.0f, -1.0f }); }
	if (ins.IsNew(KEY_INPUT_D)) { dir = VAdd(dir, { 1.0f, 0.0f, 0.0f }); }

	// 移動
	if (state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{
		// 走る
		if (ins.IsNew(KEY_INPUT_LSHIFT) && !AsoUtility::EqualsVZero(dir)
			&& state_ != STATE::CHARGE_WALK && state_ != STATE::TIRED && stamina_ >= 1.0f)
		{
			ChangeState(STATE::RUN);
			speed_ = MOVE_POW_RUN;
		}
		// 歩く
		else if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::CHARGE_WALK)
		{
			ChangeState(STATE::WALK);
			// 移動量
			speed_ = MOVE_POW_WALK;
		}
		// 待機状態
		else if (AsoUtility::EqualsVZero(dir) && state_ != STATE::CHARGE_WALK)
		{
			ChangeState(STATE::IDLE);
			speed_ = 0.0f;
		}
		else if (AsoUtility::EqualsVZero(dir) && state_ != STATE::TIRED && stamina_ <= 0.0f)
		{
			speed_ = 0.0f;
		}
	}

	//溜めながら歩く
	if (ins.IsClickMouseLeft() && (ins.IsNew(KEY_INPUT_W) || ins.IsNew(KEY_INPUT_A) ||
		ins.IsNew(KEY_INPUT_S) || ins.IsNew(KEY_INPUT_D)) &&
		!AsoUtility::EqualsVZero(dir) && state_ != STATE::HIT &&state_ != STATE::ROLL && state_ != STATE::TIRED)
	{

		// 方向を正規化
		dir = VNorm(dir);

		// Y軸の行列
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(dir, mat);

		// 移動量
		speed_ = MOVE_POW_CHRAGE_WALK;

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(dir.x, dir.z);

	}

	// 回避
	if (ins.IsTrgDown(KEY_INPUT_SPACE) && !AsoUtility::EqualsVZero(dir) &&
		state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED && stamina_ >= 10.0f)
	{

		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);

		ChangeState(STATE::ROLL);

		// 方向を正規化
		dir = VNorm(dir);

		// Y軸の行列
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(dir, mat);

		// 移動量
		speed_ = MOVE_POW_ROLL;

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(dir.x, dir.z);

		// カメラの角度を基準とし、方向分の角度を加える
		LazyRotation(cameraAngles.y + angle);

	}

	// 疲れた時
	if (state_ == STATE::TIRED)
	{
		speed_ = 0;
	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{

		// 方向を正規化
		dir = VNorm(dir);

		// Y軸の行列
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(dir, mat);

		// ロックオン時は相手に近づくのに制限をつける
		if (camera->GetMode() == Camera::MODE::LOCKON)
		{
			LockOn();
		}

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(dir.x, dir.z);

		// カメラの角度を基準とし、方向分の角度を加える
		LazyRotation(cameraAngles.y + angle);

	}

	if (state_ != STATE::HIT && state_ != STATE::ATTACK && state_ != STATE::ATTACK2 &&
		state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK && state_ != STATE::ROLL)
	{
		// 移動量
		movePow_ = VScale(moveDir_, speed_);

		// 現在座標を起点に移動後座標を決める
		movedPos_ = VAdd(transform_.pos, movePow_);
	}

	// カメラの注視点
	auto cameraTargetPos = followTransform_->pos;

	float y = movedPos_.y;

	// XZ平面の移動後座標
	auto movedPosXZ = movedPos_;

	// XZ平面のカメラの注視点
	auto cameraTargetPosXZ = cameraTargetPos;

	// XZ平面のカメラ座標
	auto cameraPosXZ = camera->GetPos();

	// 移動後座標とカメラの注視点とカメラ座標を0にすることでXZ平面座標にしている
	movedPosXZ.y = cameraTargetPosXZ.y = cameraPosXZ.y = 0.0f;

	// 注視点からのプレイヤーのベクトル
	auto target2Player = VNorm(VSub(movedPosXZ, cameraTargetPosXZ));

	// 移動後座標と移動前座標が0以上の時
	if (!AsoUtility::EqualsVZero(moveDiff_))
	{

		// 注視点と移動後座標の距離
		target2PlayerDis_ = AsoUtility::Distance(cameraTargetPos, movedPos_);

		// 敵との最小限の距離
		enemyMinDis_ = 100.0f;

		// 注視点と敵との最小限の距離が100未満の時
		if (target2PlayerDis_ < enemyMinDis_)
		{

			// 注視点から移動後座標のベクトルをクォータニオンに
			auto rot = Quaternion::LookRotation(target2Player);

			// 移動後座標を更新
			movedPos_ = VAdd(cameraTargetPos, VScale(rot.GetForward(), enemyMinDis_ + 0.5f));
			movedPos_.y = y;

		}

	}

	// 敵と衝突していたら座標を戻す
	moveDiff_ = VSub(movedPos_, transform_.pos);
	transform_.pos = movedPos_;

}

void Player::KeyboardAttack(void)
{

	auto& insInput = InputManager::GetInstance();
	auto& insScene = SceneManager::GetInstance();

	// 攻撃処理
	// ボタンがクリックされたかどうかを確認
	if (chargeCnt_ >= 0.1 && state_ != STATE::CHARGE_ATTACK && state_ != STATE::CHARGE_WALK
		&& state_ != STATE::ATTACK && state_ != STATE::ATTACK2 && state_ != STATE::ATTACK3 
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{
		ChangeState(STATE::CHARGE_WALK);
	}

	if (insInput.IsTrgUpMouseLeft() && chargeCnt_ <= CHARGE_TIME && state_ != STATE::HIT)
	{

		//ボタンが押されたらアニメーションを切り替える
		//１段階目
		if (state_ == STATE::IDLE || state_ == STATE::RUN || state_ == STATE::WALK || state_ == STATE::CHARGE_WALK)
		{
			attack1_ = true;
			ChangeState(STATE::ATTACK);
		}
		// ２段階目
		else if (state_ == STATE::ATTACK && !attack2_)
		{
			attack2_ = true;
		}
		// 3段階目
		else if (state_ == STATE::ATTACK && attack2_)
		{
			attack3_ = true;
		}
		else if (state_ == STATE::ATTACK2)
		{
			attack3_ = true;
		}
	}

	if (insInput.IsClickMouseLeft() && chargeCnt_ <= CHARGE_TIME && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::ATTACK && state_ != STATE::ATTACK2 && state_ != STATE::ATTACK3 
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{
		chargeCnt_ += insScene.GetDeltaTime();
	}

	// １段階目が終わったら遷移する
	if (attack2_ && !attack1_ && state_ == STATE::ATTACK)
	{
		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);
		ChangeState(STATE::ATTACK2);
	}

	// ２段階目が終わったら遷移する
	if (attack3_ && !attack2_ && state_ == STATE::ATTACK2)
	{
		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);
		ChangeState(STATE::ATTACK3);
	}

	// 溜め斬り
	if (chargeCnt_ >= CHARGE_TIME)
	{
		chargeAttack_ = true;
		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);
		ChangeState(STATE::CHARGE_ATTACK);
	}

}

void Player::KeyBoardCamera(void)
{

	auto& ins = InputManager::GetInstance();

	// プレイヤーが向いている方向にカメラを向ける
	auto rad = transform_.quaRot.ToEuler();
	if (ins.IsTrgDown(KEY_INPUT_Q))
	{
		// カメラの角度を基準とし、方向分の角度を加える
		SceneManager::GetInstance().GetCamera()->SetLazyAngles(rad);
	}

}

void Player::KeyBoardLockOn(void)
{

	auto& ins = InputManager::GetInstance();

	auto camera = SceneManager::GetInstance().GetCamera();

	// プレイヤーの方向を求める
	auto length = AsoUtility::Distance(followTransform_->pos, transform_.pos);
	// キーを押したらロックオンする
	if (ins.IsTrgDown(KEY_INPUT_V) && length <= 3000)
	{
		camera->ChangeLockOnFlag();
	}

	if (length >= 3000 && camera->GetLockOn())
	{
		camera->ChangeMode(Camera::MODE::FOLLOW);
	}

}

void Player::GamePadController(void)
{

	// 移動処理
	GamePadMove();

	// 攻撃処理
	GamePadAttack();

	// プレイヤー方向にカメラを向ける処理
	GamePadCamera();

	// 敵をロックオンする処理
	GamePadLockOn();

}

void Player::GamePadMove(void)
{

	// カメラの取得
	Camera* camera = SceneManager::GetInstance().GetCamera();

	// カメラの角度を取得
	VECTOR cameraAngles = camera->GetAngles();

	auto& ins = InputManager::GetInstance();

	// 方向(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// 移動量
	float movePow = MOVE_POW_WALK;

	// ゲームパッドの番号を取得
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// パッドの方向をdirに直す
	dir.x = pad.AKeyLX;
	dir.z = -pad.AKeyLZ;

	// 移動
	if (state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{

		// 走る
		if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN)
			&& !AsoUtility::EqualsVZero(dir)
			&& state_ != STATE::CHARGE_WALK && state_ != STATE::TIRED && stamina_ >= 1.0f)
		{
			ChangeState(STATE::RUN);
			speed_ = MOVE_POW_RUN;
		}
		// 歩く
		else if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::CHARGE_WALK)
		{
			ChangeState(STATE::WALK);
			// 移動量
			speed_ = MOVE_POW_WALK;
		}
		// 待機状態
		else if (AsoUtility::EqualsVZero(dir) && state_ != STATE::CHARGE_WALK)
		{
			ChangeState(STATE::IDLE);
			speed_ = 0.0f;
		}
		else if (AsoUtility::EqualsVZero(dir) && state_ != STATE::TIRED && stamina_ <= 0.0f)
		{
			speed_ = 0.0f;
		}
	}

	//溜めながら歩く
	if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)
		/*&& (pad.AKeyLX != 0.0f || pad.AKeyLZ != 0.0f)*/ && !AsoUtility::EqualsVZero(dir)
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{
		// 方向を正規化
		dir = VNorm(dir);

		// Y軸の行列
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(dir, mat);

		// 移動量
		speed_ = MOVE_POW_CHRAGE_WALK;

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(dir.x, dir.z);
	}

	// 回避
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_BOTTON)
		&& !AsoUtility::EqualsVZero(dir) &&
		state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED && stamina_ >= 10.0f)
	{

		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);

		ChangeState(STATE::ROLL);

		// 方向を正規化
		dir = VNorm(dir);

		// Y軸の行列
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(dir, mat);

		// 移動量
		speed_ = MOVE_POW_ROLL;

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(dir.x, dir.z);

		// カメラの角度を基準とし、方向分の角度を加える
		LazyRotation(cameraAngles.y + angle);

	}

	// 疲れた時
	if (state_ == STATE::TIRED)
	{
		speed_ = 0;
	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{

		// 方向を正規化
		dir = VNorm(dir);

		// Y軸の行列
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(dir, mat);

		// ロックオン時は相手に近づくのに制限をつける
		if (camera->GetMode() == Camera::MODE::LOCKON)
		{
			LockOn();
		}

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(dir.x, dir.z);

		// カメラの角度を基準とし、方向分の角度を加える
		LazyRotation(cameraAngles.y + angle);

	}

	if (state_ != STATE::HIT && state_ != STATE::ATTACK && state_ != STATE::ATTACK2 &&
		state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK && state_ != STATE::ROLL)
	{
		// 移動量
		movePow_ = VScale(moveDir_, speed_);

		// 現在座標を起点に移動後座標を決める
		movedPos_ = VAdd(transform_.pos, movePow_);
	}

	// カメラの注視点
	auto cameraTargetPos = followTransform_->pos;

	float y = movedPos_.y;

	// XZ平面の移動後座標
	auto movedPosXZ = movedPos_;

	// XZ平面のカメラの注視点
	auto cameraTargetPosXZ = cameraTargetPos;

	// XZ平面のカメラ座標
	auto cameraPosXZ = camera->GetPos();

	// 移動後座標とカメラの注視点とカメラ座標を0にすることでXZ平面座標にしている
	movedPosXZ.y = cameraTargetPosXZ.y = cameraPosXZ.y = 0.0f;

	// 注視点からのプレイヤーのベクトル
	auto target2Player = VNorm(VSub(movedPosXZ, cameraTargetPosXZ));

	// 移動後座標と移動前座標が0以上の時
	if (!AsoUtility::EqualsVZero(moveDiff_))
	{

		// 注視点と移動後座標の距離
		target2PlayerDis_ = AsoUtility::Distance(cameraTargetPos, movedPos_);

		// 敵との最小限の距離
		enemyMinDis_ = 100.0f;

		// 注視点と敵との最小限の距離が100未満の時
		if (target2PlayerDis_ < enemyMinDis_)
		{

			// 注視点から移動後座標のベクトルをクォータニオンに
			auto rot = Quaternion::LookRotation(target2Player);

			// 移動後座標を更新
			movedPos_ = VAdd(cameraTargetPos, VScale(rot.GetForward(), enemyMinDis_ + 0.5f));
			movedPos_.y = y;

		}

	}

	// 敵と衝突していたら座標を戻す
	moveDiff_ = VSub(movedPos_, transform_.pos);
	transform_.pos = movedPos_;


}

void Player::GamePadAttack(void)
{

	auto& insInput = InputManager::GetInstance();
	auto& insScene = SceneManager::GetInstance();

	// 攻撃処理
	// ボタンがクリックされたかどうかを確認
	if (chargeCnt_ >= 0.1 && state_ != STATE::CHARGE_ATTACK && state_ != STATE::CHARGE_WALK
		&& state_ != STATE::ATTACK && state_ != STATE::ATTACK2 && state_ != STATE::ATTACK3 
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{
		ChangeState(STATE::CHARGE_WALK);
	}

	if (insInput.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) &&
		chargeCnt_ <= CHARGE_TIME && state_ != STATE::HIT)
	{

		//ボタンが押されたらアニメーションを切り替える
		//１段階目
		if (state_ == STATE::IDLE || state_ == STATE::RUN || state_ == STATE::WALK || state_ == STATE::CHARGE_WALK)
		{
			attack1_ = true;
			ChangeState(STATE::ATTACK);
		}
		// ２段階目
		else if (state_ == STATE::ATTACK && !attack2_)
		{
			attack2_ = true;
		}
		// 3段階目
		else if (state_ == STATE::ATTACK && attack2_)
		{
			attack3_ = true;
		}
		else if (state_ == STATE::ATTACK2)
		{
			attack3_ = true;
		}

	}

	if (insInput.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) &&
		chargeCnt_ <= CHARGE_TIME && state_ != STATE::CHARGE_ATTACK && state_ != STATE::ATTACK &&
		state_ != STATE::ATTACK2 && state_ != STATE::ATTACK3 && state_ != STATE::HIT &&
		state_ != STATE::ROLL && state_ != STATE::TIRED)
	{
		chargeCnt_ += insScene.GetDeltaTime();
	}

	// １段階目が終わったら遷移する
	if (attack2_ && !attack1_ && state_ == STATE::ATTACK)
	{
		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);
		ChangeState(STATE::ATTACK2);
	}

	// ２段階目が終わったら遷移する
	if (attack3_ && !attack2_ && state_ == STATE::ATTACK2)
	{
		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);
		ChangeState(STATE::ATTACK3);
	}

	// 溜め斬り
	if (chargeCnt_ >= CHARGE_TIME)
	{
		chargeAttack_ = true;
		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);
		ChangeState(STATE::CHARGE_ATTACK);
	}

}

void Player::GamePadCamera(void)
{

	auto& ins = InputManager::GetInstance();

	// プレイヤーが向いている方向にカメラを向ける
	auto rad = transform_.quaRot.ToEuler();
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		// カメラの角度を基準とし、方向分の角度を加える
		SceneManager::GetInstance().GetCamera()->SetLazyAngles(rad);
	}

}

void Player::GamePadLockOn(void)
{

	auto& ins = InputManager::GetInstance();

	auto camera = SceneManager::GetInstance().GetCamera();

	// プレイヤーの方向を求める
	auto length = AsoUtility::Distance(followTransform_->pos, transform_.pos);

	// キーを押したらロックオンする
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN:: L_BOTTON) && length <= 3000)
	{
		camera->ChangeLockOnFlag();
	}

	if (length >= 3000 && camera->GetLockOn())
	{
		camera->ChangeMode(Camera::MODE::FOLLOW);
	}

}

void Player::LockOn(void)
{

	// 敵に近づき敵の周りを走り続けるとカメラがプレイヤーを追わなくなる

	Camera* camera = SceneManager::GetInstance().GetCamera();

	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	// �@カメラがロックオンの時は、自動回転を行わない
	if (camera->GetMode() != Camera::MODE::LOCKON)
	{
		// 移動方向に応じた回転
		Rotate();
	}

	//// �A移動処理前に、ターゲットとの特別衝突判定
	//moveDiff_ = VSub(movedPos_, transform_.pos);

	//// ロックオンモードの時
	//if (camera->GetMode() == Camera::MODE::LOCKON)
	//{

	//	// カメラの注視点
	//	auto cameraTargetPos = camera->GetTargetPos();

	//	float y = movedPos_.y;

	//	// XZ平面の移動後座標
	//	auto movedPosXZ = movedPos_;

	//	// XZ平面のカメラの注視点
	//	auto cameraTargetPosXZ = cameraTargetPos;

	//	// XZ平面のカメラ座標
	//	auto cameraPosXZ = camera->GetPos();

	//	// 移動後座標とカメラの注視点とカメラ座標を0にすることでXZ平面座標にしている
	//	movedPosXZ.y = cameraTargetPosXZ.y = cameraPosXZ.y = 0.0f;

	//	// 注視点からのプレイヤーのベクトル
	//	auto target2Player = VNorm(VSub(movedPosXZ, cameraTargetPosXZ));

	//	//// 目的の角度までの差を測る
	//	//SetGoalRotate(Quaternion::LookRotation(VScale(target2Player, -1.0)));

	//	//// 少しずつ回転させる
	//	//Rotate();

	//	// 移動後座標と移動前座標が0以上の時
	//	if (!AsoUtility::EqualsVZero(moveDiff_))
	//	{

	//		// 注視点と移動後座標の距離
	//		target2PlayerDis_ = AsoUtility::Distance(cameraTargetPos, movedPos_);

	//		// 敵との最小限の距離
	//		enemyMinDis_ = 100.0f;

	//		// 注視点と敵との最小限の距離が100未満の時
	//		if (target2PlayerDis_ < enemyMinDis_)
	//		{

	//			// 注視点から移動後座標のベクトルをクォータニオンに
	//			auto rot = Quaternion::LookRotation(target2Player);

	////			// 右方向
	////			auto r = rot.GetRight();

	////			// 左方向
	////			auto l = rot.GetLeft();

	////			// 右方向の内積
	////			auto dotR = VDot(r, target2Player);

	////			// 左方向の内積
	////			auto dotL = VDot(l, target2Player);

	////			float deg = 2.0f;


	////			// 右か左かを選ぶ処理

	////			if (dotR + 0.01f > dotL)

	////			{

	////				deg *= -1.0f;

	////				// キャラが右に回る

	////				rot = rot.Mult(

	////					Quaternion::AngleAxis(-deg * DX_PI_F / 180.0f, AsoUtility::AXIS_Y));

	////				// 内積の大きいほうに角度を足す
	////				camera->AddLockOnAnglesY(deg * DX_PI_F / 180.0f);


	////			}

	////			else

	////			{

	////				// キャラが左に回る

	////				rot = rot.Mult(

	////					Quaternion::AngleAxis(deg * DX_PI_F / 180.0f, AsoUtility::AXIS_Y));

	////				camera->AddLockOnAnglesY(deg * DX_PI_F / 180.0f);

	////			}


	//			movedPos_ = VAdd(cameraTargetPos, VScale(rot.GetForward(), enemyMinDis_ + 0.5f));
	//			movedPos_.y = y;

	//		}

	//	}

	//}


	//// 移動

	//moveDiff_ = VSub(movedPos_, transform_.pos);

	//transform_.pos = movedPos_;


	// �B回転完了までの時間短縮


}

// 補足関数
void Player::SetGoalRotate(Quaternion rot)
{

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(rot, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		// 回転完了の時間
		stepRotTime_ = TIME_ROT;
	}

	// 目的の角度
	goalQuaRot_ = rot;

}

void Player::Rotate(void)
{

	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

	// 回転の球面補間
	transform_.quaRot = Quaternion::Slerp(
		transform_.quaRot, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

}

//void Player::ChangeAnimation(STATE state)
//{
//
//	// 同じ状態だったら入らない
//	if (state_ == preState_) return;
//
//	// 前の状態の優先度をなくす
//	animData_[(int)preState_].isPriority_ = false;
//
//	// 再生するアニメーションの設定
//	AttatchNum((int)state_);
//
//	// 状態遷移時の初期化処理
//	switch (state_)
//	{
//	case Player::STATE::IDLE:
//		animationController_->Play("IDLE", true, false, true);
//		// 足音を止める
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::WALK:
//		animationController_->Play("WALK", true, false, true);
//		break;
//	case Player::STATE::CHARGE_WALK:
//		animationController_->Play("CHARGE_WALK", true, false, true);
//		// エフェクトの再生
//		ChargePlayEffect();
//
//		// 溜める音の再生
//		PlaySoundMem(musicChargeId_,DX_PLAYTYPE_BACK);
//
//		// 足音を止める
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::RUN:
//		animationController_->Play("RUN", true, false, true);
//		break;
//	case Player::STATE::ATTACK:
//		animationController_->Play("ATTACK", true, false, true);
//		hit_ = false;
//		// エフェクトを止める
//		StopEffekseer3DEffect(effectChargePlayId_);
//		// 足音を止める
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::ATTACK2:
//		animationController_->Play("ATTACK2", true, false, true);
//		hit_ = false;
//		// 足音を止める
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::ATTACK3:
//		animationController_->Play("ATTACK3", true, false, true);
//		hit_ = false;
//		// 足音を止める
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::CHARGE_ATTACK:
//		animationController_->Play("CHARGE_ATTACK", true, false, true);
//		hit_ = false;
//		// 足音を止める
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::HIT:
//		animationController_->Play("HIT", true, false, true);
//		// エフェクトを止める
//		StopEffekseer3DEffect(effectChargePlayId_);
//		// 足音を止める
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::DEATH:
//		animationController_->Play("DEATH", true, false, true);
//		// 足音を止める
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::ROLL:
//		animationController_->Play("ROLL", true, false, true);
//		// 足音を止める
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		// スタミナを減らす
//		stamina_ -= 10.0f;
//		break;
//	case Player::STATE::TIRED:
//		animationController_->Play("TIRED", true, false, true);
//		// 足音を止める
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	}
//
//	if (state_ != STATE::CHARGE_WALK)
//	{
//		StopSoundMem(musicChargeId_);
//	}
//
//	// 一個前の状態を保存
//	preState_ = state_;
//
//}

void Player::ChangeState(STATE state)
{
	if (state_ == state)return;


	state_ = state;
	key_ = ANIM_DATA_KEY[(int)state];

	animationController_->ChangeAnimation(key_);

}

void Player::LazyRotation(float goalRot)
{

	//// 回転処理
	//// クォータニオンからラジアン
	//float radNowAngleY = transform_.quaRot.ToEuler().y;
	//float degNowAngleY = AsoUtility::Rad2DegF(radNowAngleY);
	//float degGoalAngleY = AsoUtility::Rad2DegF(goalRot);

	//// 0度〜360度以内に角度をおさめる
	////degGoalAngleY = static_cast<float>(AsoUtility::DegIn360(degGoalAngleY));

	//// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	//int aroundDir = AsoUtility::DirNearAroundDeg(degNowAngleY, degGoalAngleY);

	//// 回転量の作成
	//Quaternion rotPow = Quaternion::Identity();
	//rotPow = Quaternion::Mult(
	//	rotPow,
	//	Quaternion::AngleAxis(AsoUtility::Deg2RadF(3.0f * aroundDir), AsoUtility::AXIS_Y));

	////auto localRotRad = transform_.quaRot.ToEuler();
	////auto localRotDeg = AsoUtility::Rad2DegF(localRotRad.y);
	////auto goalRad = goalRot;

	//if (fabs(degGoalAngleY - degNowAngleY) >= 5)
	//{
	//	transform_.quaRot = Quaternion::Mult(transform_.quaRot, rotPow);
	//}
	//else
	//{
	//	// ラジアンからクォータニオン
	//	Quaternion qua = Quaternion::Euler(0, goalRot, 0);
	//	transform_.quaRot.y = qua.y;
	//}

	//auto* camera = SceneManager::GetInstance().GetCamera();

	//auto cameraRotY = camera->GetRotY();
	//transform_.quaRot = Quaternion::Slerp(transform_.quaRot, cameraRotY, 0.1f);
	if (state_ == STATE::ROLL)
	{
		auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
		transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 1.0);
	}
	else
	{
		auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
		transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, ROTATION_POW);
	}
}

void Player::DrawDebug(void)
{

	auto rad = transform_.quaRot.ToEuler();

	VECTOR pDeg = { AsoUtility::Rad2DegF(rad.x),
					AsoUtility::Rad2DegF(rad.y),
					AsoUtility::Rad2DegF(rad.z) };
	DrawFormatString(0, 70, 0xffffff, "プレイヤー座標 : (%.1f, %.1f, %.1f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);
	DrawFormatString(0, 90, 0xffffff, "プレイヤー角度deg : (%.1f, %.1f, %.1f)", pDeg.x, pDeg.y, pDeg.z);
	DrawFormatString(0, 110, 0xffffff, "プレイヤー角度rad : (%.5f, %.5f, %.5f)", rad.x, rad.y, rad.z);

	// エネミー自身の衝突判定のカプセルの描画
	DrawCapsule3D(cBodyPosDown_, cBodyPosUp_, COLLISION_BODY_RADIUS, 10, 0xff0000, 0xff0000, false);

}

void Player::SetParam(void)
{

	// HPの最大値
	hpMax_ = HP_MAX;

	// HP
	hp_ = hpMax_;

	// スタミナの最大値
	staminaMax_ = STAMINA_MAX;

	// スタミナ
	stamina_ = staminaMax_;

}

void Player::Animation(void)
{

	int a = 0;
	// アニメーション再生
	if (animationController_->GetAnimData(key_).stepAnim > animationController_->GetAnimData(key_).animTotalTime)
	{

		if (state_ == STATE::ATTACK || state_ == STATE::ATTACK2
			|| state_ == STATE::ATTACK3 || state_ == STATE::CHARGE_ATTACK
			|| state_ == STATE::HIT || state_ == STATE::ROLL)
		{
			if (state_ == STATE::ATTACK2)
			{
				animationController_->SetStartStepAnim("ATTACK2", ATTACK_START_TIME2);
			}
			else if (state_ == STATE::ATTACK3)
			{
				animationController_->SetStartStepAnim("ATTACK3", ATTACK_START_TIME3);
			}
			else
			{
				animationController_->SetStartStepAnim(key_, 0.0f);
			}
			attack1_ = false;
			attack2_ = false;
			attack3_ = false;
			chargeAttack_ = false;
			isMusicSlash_ = true;
			isMusicRoll_ = true;
			hit_ = false;
			ChangeState(STATE::IDLE);
			chargeCnt_ = 0.0f;
		}

	}

	// 2段階目に進まないときはリセット
	if (state_ == STATE::ATTACK && animationController_->GetAnimData("ATTACK").stepAnim >= 
		animationController_->GetAnimData("ATTACK").animTotalTime && !attack2_)
	{
		hit_ = false;
		ChangeState(STATE::IDLE);
		chargeCnt_ = 0.0f;
	}

	// 3段階目に進まないときはリセット
	if (state_ == STATE::ATTACK2 && animationController_->GetAnimData("ATTACK2").stepAnim >= 
		animationController_->GetAnimData("ATTACK2").animTotalTime && !attack3_)
	{
		animationController_->SetStartStepAnim("ATTACK2", ATTACK_START_TIME2);
		hit_ = false;
		ChangeState(STATE::IDLE);
		chargeCnt_ = 0.0f;
	}

	// アニメーションの固定
	AnimationFrame();

}

void Player::AnimationFrame(void)
{

	// 対象フレームのローカル行列を初期値にリセットする
	MV1ResetFrameUserLocalMatrix(transform_.modelId, playerAttachFrameNum_);

	// ジャンプ攻撃時に座標を固定する
	if (state_ == STATE::ATTACK || state_ == STATE::ATTACK2 || state_ == STATE::ATTACK3 || state_ == STATE::CHARGE_ATTACK || state_ == STATE::ROLL)
	{

		// 対象フレームのローカル行列(大きさ、回転、位置)を取得する
		auto mat = MV1GetFrameLocalMatrix(transform_.modelId, playerAttachFrameNum_);

		auto scl = MGetSize(mat); // 行列から大きさを取り出す
		auto rot = MGetRotElem(mat); // 行列から回転を取り出す
		auto pos = MGetTranslateElem(mat); // 行列から移動値を取り出す

		// 大きさ、回転、位置をローカル行列に戻す
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // 大きさ
		mix = MMult(mix, rot); // 回転

		// ここでローカル座標を行列に、そのまま戻さず、
		// 調整したローカル座標を設定する
		mix = MMult(mix, MGetTranslate({ pos.x, pos.y, 0.0f }));

		// 合成した行列を対象フレームにセットし直して、
		// アニメーションの移動値を無効化
		MV1SetFrameUserLocalMatrix(transform_.modelId, playerAttachFrameNum_, mix);

	}

}