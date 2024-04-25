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

	// ���߂�G�t�F�N�g
	effectChargeResId_ = -1;
	effectChargePlayId_ = -1;

}

Player::~Player(void)
{
}

void Player::InitAnimation(void)
{

	// ���f������̊�{���
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

	key_ = "IDLE";

//#pragma region �A�j���[�V����
//
	// �ҋ@
	//animData_[(int)STATE::IDLE].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
		//ResourceManager::SRC::PLAYER_IDLE);

//	// ����
//	animData_[(int)STATE::WALK].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_WALK);
//
//	// ���߂Ȃ������
//	animData_[(int)STATE::CHARGE_WALK].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_CHARGE_WALK);
//	
//	// ����
//	animData_[(int)STATE::RUN].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_RUN);
//
//	// �U��
//	animData_[(int)STATE::ATTACK].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//			ResourceManager::SRC::PLAYER_ATTACK1);
//
//	// �U��2
//	animData_[(int)STATE::ATTACK2].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_ATTACK2);
//
//	// �U��3
//	animData_[(int)STATE::ATTACK3].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_ATTACK3);
//
//	// ���ߍU��
//	animData_[(int)STATE::CHARGE_ATTACK].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_CHARGEATTACK);
//
//	// �_���[�W�q�b�g
//	animData_[(int)STATE::HIT].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_HIT);
//
//	// ���S
//	animData_[(int)STATE::DEATH].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_DEATH);
//
//	// ���
//	animData_[(int)STATE::ROLL].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_ROLL);
//	
//	// ��ꂽ
//	animData_[(int)STATE::
// 
// ].animHandle_ = ResourceManager::GetInstance().LoadModelDuplicate(
//		ResourceManager::SRC::PLAYER_TIRED);
//
//#pragma endregion
//
//#pragma region �Đ����x
//
//	// �ҋ@
//	animData_[(int)STATE::IDLE].speedAnim_ = IDLE_ANIM_SPEED;
//
//	// ����
//	animData_[(int)STATE::WALK].speedAnim_ = IDLE_ANIM_SPEED;
//
//	// ���߂Ȃ������
//	animData_[(int)STATE::CHARGE_WALK].speedAnim_ = CHARGE_WALK_ANIM_SPEED;
//
//	// ����
//	animData_[(int)STATE::RUN].speedAnim_ = RUN_ANIM_SPEED;
//
//	// �U��
//	animData_[(int)STATE::ATTACK].speedAnim_ = ATTACK_ANIM_SPEED;
//
//	// �U��2
//	animData_[(int)STATE::ATTACK2].speedAnim_ = ATTACK_ANIM_SPEED;
//
//	// �U��3
//	animData_[(int)STATE::ATTACK3].speedAnim_ = ATTACK_ANIM_SPEED;
//
//	// ���ߍU��
//	animData_[(int)STATE::CHARGE_ATTACK].speedAnim_ = CHARGE_ATTACK_ANIM_SPEED;
//
//	// �_���[�W�q�b�g
//	animData_[(int)STATE::HIT].speedAnim_ = HIT_ANIM_SPEED;
//
//	// ���S
//	animData_[(int)STATE::DEATH].speedAnim_ = 20.0f;
//
//	// ���
//	animData_[(int)STATE::ROLL].speedAnim_ = ROLL_ANIM_SPEED;
//
//	// ��ꂽ
//	animData_[(int)STATE::TIRED].speedAnim_ = 50.0f;
//
//#pragma endregion
//
//#pragma region �u�����h�^�C��
//
//	// �ҋ@
//	animData_[(int)STATE::IDLE].blendTime_ = 0.5f;
//
//	// ����
//	animData_[(int)STATE::WALK].blendTime_ = 0.5f;
//
//	// ���߂Ȃ������
//	animData_[(int)STATE::CHARGE_WALK].blendTime_ = 0.5f;
//
//	// ����
//	animData_[(int)STATE::RUN].blendTime_ = 0.5f;
//
//	// �U��
//	animData_[(int)STATE::ATTACK].blendTime_ = 0.2f;
//
//	// �U��2
//	animData_[(int)STATE::ATTACK2].blendTime_ = 0.2f;
//
//	// �U��3
//	animData_[(int)STATE::ATTACK3].blendTime_ = 0.2f;
//
//	// ���ߍU��
//	animData_[(int)STATE::CHARGE_ATTACK].blendTime_ = 0.5f;
//
//	// �_���[�W�q�b�g
//	animData_[(int)STATE::HIT].blendTime_ = 0.5f;
//
//	// ���S
//	animData_[(int)STATE::DEATH].blendTime_ = 0.5f;
//
//	// ���
//	animData_[(int)STATE::ROLL].blendTime_ = 0.5f;
//
//	// ��ꂽ
//	animData_[(int)STATE::TIRED].blendTime_ = 0.5f;
//
//#pragma endregion
//
//#pragma region �A�j���[�V�������n�܂鎞��
//
//	// �ҋ@
//	animData_[(int)STATE::IDLE].stepAnim_ = 0.0f;
//
//	// ����
//	animData_[(int)STATE::WALK].stepAnim_ = 0.0f;
//
//	// ���߂Ȃ������
//	animData_[(int)STATE::CHARGE_WALK].stepAnim_ = 0.0f;
//
//	// ����
//	animData_[(int)STATE::RUN].stepAnim_ = 0.0f;
//
//	// �U��
//	animData_[(int)STATE::ATTACK].stepAnim_ = 0.0f;
//
//	// �U��2
//	animData_[(int)STATE::ATTACK2].stepAnim_ = 0.0f;
//
//	// �U��3
//	animData_[(int)STATE::ATTACK3].stepAnim_ = 0.0f;
//
//	// ���ߍU��
//	animData_[(int)STATE::CHARGE_ATTACK].stepAnim_ = 0.0f;
//
//	// �_���[�W�q�b�g
//	animData_[(int)STATE::HIT].stepAnim_ = 0.0f;
//
//	// ���S
//	animData_[(int)STATE::DEATH].stepAnim_ = 0.0f;
//
//	// ���
//	animData_[(int)STATE::ROLL].stepAnim_ = 0.0f;
//
//	// ��ꂽ
//	animData_[(int)STATE::TIRED].stepAnim_ = 0.0f;
//
//#pragma endregion
//
//#pragma region �A�j���[�V�����̏��
//
//	// �ҋ@
//	animData_[(int)STATE::IDLE].state_ = STATE::IDLE;
//
//	// ����
//	animData_[(int)STATE::WALK].state_ = STATE::WALK;
//
//	// ���߂Ȃ������
//	animData_[(int)STATE::CHARGE_WALK].state_ = STATE::CHARGE_WALK;
//
//	// ����
//	animData_[(int)STATE::RUN].state_ = STATE::RUN;
//
//	// �U��
//	animData_[(int)STATE::ATTACK].state_ = STATE::ATTACK;
//
//	// �U��2
//	animData_[(int)STATE::ATTACK2].state_ = STATE::ATTACK2;
//
//	// �U��3
//	animData_[(int)STATE::ATTACK3].state_ = STATE::ATTACK3;
//
//	// ���ߍU��
//	animData_[(int)STATE::CHARGE_ATTACK].state_ = STATE::CHARGE_ATTACK;
//
//	// �_���[�W�q�b�g
//	animData_[(int)STATE::HIT].state_ = STATE::HIT;
//
//	// ���S
//	animData_[(int)STATE::DEATH].state_ = STATE::DEATH;
//
//	// ���
//	animData_[(int)STATE::ROLL].state_ = STATE::ROLL;
//
//	// ��ꂽ
//	animData_[(int)STATE::TIRED].state_ = STATE::TIRED;
//
//#pragma endregion

	// �A�j���[�V��������ԍ�
	animNo_ = 1;

	//// �ҋ@�A�j���[�V����
	//ChangeAnimation(STATE::IDLE,0);

	//// �Đ�����A�j���[�V�����̐ݒ�
	//preAnimAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, walkAnim_);

	// transform�̏�����
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

	//// �Đ�����A�j���[�V�����̐ݒ�
	//animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_,idleAnim_);

	//preAnimAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_,idleAnim_);

	//// �Đ����̃A�j���[�V��������
	//stepAnim_ = 0.0f;

	//// �A�j���[�V�������x
	//speedAnim_ = 0.0f;

}

void Player::InitEffect(void)
{

	// ���߂̃G�t�F�N�g
	effectChargeResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGE_EFFECT).handleId_;

}

void Player::InitMusic(void)
{

	// ���߂鉹
	musicChargeId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_CHARGE_MUSIC).handleId_;

	// ����؂鉹
	musicSlash1Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SLASH_MUSIC1).handleId_;

	// ����؂鉹
	musicSlash2Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SLASH_MUSIC2).handleId_;

	// ����؂鉹�̃t���O
	isMusicSlash_ = true;

	// ����
	musicFootStepsId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_FOOTSTEPS_MUSIC).handleId_;

	// �����̃J�E���^
	musicFootStepsCnt_ = 0.0f;

	// �v���C���[�̍U���{�C�X�P
	musicSlashVoice1Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SLASH_VOICE_MUSIC1).handleId_;

	// �v���C���[�̍U���{�C�X�Q
	musicSlashVoice2Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SLASH_VOICE_MUSIC2).handleId_;

	// �v���C���[�̗��ߍU���{�C�X
	musicChargeSlashVoiceId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGEATTACK_VOICE_MUSIC).handleId_;

	// �����
	musicRollId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ROLL_MUSIC).handleId_;
	ChangeVolumeSoundMem(255 * 70 / 100, musicRollId_);

	// ����{�C�X�P
	musicRollVoice1Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ROLL_VOICE_MUSIC1).handleId_;

	// ����{�C�X�Q
	musicRollVoice2Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ROLL_VOICE_MUSIC2).handleId_;

	// ������̃t���O
	isMusicRoll_ = true;

}

void Player::Init(void)
{

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

	// �G�t�F�N�g�̏����ݒ�
	InitEffect();

	// ���̏����ݒ�
	InitMusic();

	// �v���C���[�̃p�����[�^�[
	SetParam();

	// �A�j���[�V�������x
	speedAnim_ = WALK_ANIM_SPEED;

	// �J�v�Z�����A�^�b�`����t���[���̔ԍ�������
	playerAttachFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:Hips");

	// �U���������������ǂ���
	hit_ = false;

	// �U���������ǂ���
	attack_ = false;

	// �U���P�i�K��
	attack1_ = false;

	// �U���Q�i�K��
	attack2_ = false;

	// �U���R�i�K��
	attack3_ = false;

	// �U���S
	chargeAttack_ = false;

	// ��𒆂͖��G
	isInvincible_ = false;

	// ��ꂽ���̃t���O
	isTired_ = false;

	// �ړ�����
	moveDir_ = AsoUtility::VECTOR_ZERO;

	staminaCnt_ = 0.0f;

	limitCnt_ = 0.0f;

	blendTime_ = 0.5;

	stepBlend_ = 0.0f;

	list_ = 0;

	slowCnt_ = 60;
	//AttatchNum((int)STATE::IDLE);

}

void Player::Update(void)
{












	//auto& ins = InputManager::GetInstance();

	//// ���N���b�N�ōU��
	//if (ins.IsClickMouseLeft())
	//{
	//	animationController_->ChangeAnimation(ANIM_DATA_KEY[(int)STATE::ATTACK]);
	//}



	//// �U�����[�V�������I������玩���I��IDLE
	//auto atk = ANIM_DATA_KEY[(int)STATE::ATTACK];
	//if (atk == animationController_->GetPlayType())
	//{
	//	if (animationController_->IsEndPlayAnimation())
	//	{
	//		animationController_->ChangeAnimation(ANIM_DATA_KEY[(int)STATE::CHARGE_WALK]);
	//		animationController_->ChangeAnimation(ANIM_DATA_KEY[(int)STATE::IDLE]);
	//	}
	//}

	//// �E�N���b�N��IDLE
	//if (ins.IsClickMouseRight())
	//{
	//	animationController_->ChangeAnimation(ANIM_DATA_KEY[(int)STATE::IDLE]);
	//}


	//// 2�i�K�ڂɐi�܂Ȃ��Ƃ��̓��Z�b�g
	//float step = animationController_->GetAnimData("ATTACK").stepAnim;
	//float time = animationController_->GetAnimData("ATTACK").animTotalTime;
	//if (state_ == STATE::ATTACK
	//	&& step >= time
	//	&& !attack2_)
	//{
	//	hit_ = false;
	//	ChangeState(STATE::IDLE);
	//	chargeCnt_ = 0.0f;
	//}



	//KeyboardContoroller();

	//// �A�j���[�V�����X�V
	//animationController_->Update();














	//// �A�j���[�V�����̌Œ�
	////AnimationFrame();

	////
	////// �A�j���[�V�����Đ�
	////if (animationController_->GetAnimData(key_).stepAnim >= animationController_->GetAnimData(key_).animTotalTime)
	////{

	////	if (state_ == STATE::ATTACK || state_ == STATE::ATTACK2
	////		|| state_ == STATE::ATTACK3 || state_ == STATE::CHARGE_ATTACK
	////		|| state_ == STATE::HIT || state_ == STATE::ROLL)
	////	{
	////		if (state_ == STATE::ATTACK2)
	////		{
	////			animationController_->SetStartStepAnim("ATTACK2", ATTACK_START_TIME2);
	////		}
	////		else if (state_ == STATE::ATTACK3)
	////		{
	////			animationController_->SetStartStepAnim("ATTACK3", ATTACK_START_TIME3);
	////		}
	////		else
	////		{
	////			animationController_->SetStartStepAnim(key_, 0.0f);
	////		}
	////		attack1_ = false;
	////		attack2_ = false;
	////		attack3_ = false;
	////		chargeAttack_ = false;
	////		isMusicSlash_ = true;
	////		isMusicRoll_ = true;
	////		hit_ = false;
	////		ChangeState(STATE::IDLE);
	////		chargeCnt_ = 0.0f;
	////	}

	////}







	////// 3�i�K�ڂɐi�܂Ȃ��Ƃ��̓��Z�b�g
	////if (state_ == STATE::ATTACK2 && animationController_->GetAnimData("ATTACK2").stepAnim >=
	////	animationController_->GetAnimData("ATTACK2").animTotalTime && !attack3_)
	////{
	////	animationController_->SetStartStepAnim("ATTACK2", ATTACK_START_TIME2);
	////	hit_ = false;
	////	ChangeState(STATE::IDLE);
	////	chargeCnt_ = 0.0f;
	////}




	//// ���̏�������؍s��Ȃ�
	//return;

































	//// �X���[
	//if (slowCnt_ > 0)
	//{
	//	slowCnt_--;
	//	if (slowCnt_ % 5 != 0)
	//	{
	//		return;
	//	}
	//}

	slowCnt_ = 60;

	// ��Ԃ̑J��
	switch (state_)
	{
	case Player::STATE::IDLE:
		break;
	case Player::STATE::WALK:

		// �����̃J�E���^���Z�b�g
		if (musicFootStepsCnt_ >= 10.0f)
		{
			musicFootStepsCnt_ = 0.0f;
		}

		// �Đ����x�̐ݒ�
		SetFrequencySoundMem(35000, musicFootStepsId_);

		if (state_ != STATE::CHARGE_WALK)
		{
			// ����
			FootStepsMusic();
		}

		break;
	case Player::STATE::CHARGE_WALK:
		ChargeSyncEffect();
		break;
	case Player::STATE::RUN:

		// �����̃J�E���^���Z�b�g
		if (musicFootStepsCnt_ >= 7.0f)
		{
			musicFootStepsCnt_ = 0.0f;
		}

		// �Đ����x�̐ݒ�
		SetFrequencySoundMem(50000, musicFootStepsId_);

		if (state_ != STATE::CHARGE_WALK)
		{
			// ����
			FootStepsMusic();
		}

		if (staminaCnt_ >= 0.1f)
		{
			stamina_ -= 1.0f;
			staminaCnt_ = 0.0f;
		}

		break;
	case Player::STATE::ATTACK:
		// ����؂鉹�̍Đ�
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
		// ����؂鉹�̍Đ�
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
		// ����؂鉹�̍Đ�
		SlashMusic();
		if (animationController_->GetAnimData("ATTACK3").stepAnim >= animationController_->GetAnimData("ATTACK3").startTime
			&& animationController_->GetAnimData("ATTACK3").stepAnim <= animationController_->GetAnimData("ATTACK").animTotalTime
			&& !hit_)
		{
			attack_ = true;
		}
		break;
	case Player::STATE::CHARGE_ATTACK:
		// ����؂鉹�̍Đ�
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
			// �ړ���
			movePow_ = VScale(moveDir_, speed_);

			// ���ݍ��W���N�_�Ɉړ�����W�����߂�
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
		// �X�^�~�i���񕜂����瓮����
		if (stamina_ >= 100)
		{
			isTired_ = false;
			ChangeState(STATE::IDLE);
			stamina_ = 100.0f;
		}

		// �X�^�~�i���񕜂���
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

	// �X�^�~�i���؂ꂽ���ꂽ�A�j���[�V�����Ɉڍs
	if (stamina_ <= 0.0f)
	{
		isTired_ = true;
		ChangeState(STATE::TIRED);
	}

	// HP��0�ɂȂ����瑀��ł��Ȃ��悤�ɂ���
	if (hp_ > 0)
	{
		limitCnt_ += SceneManager::GetInstance().GetDeltaTime();
		if (limitCnt_ <= 0.5f)
		{
			return;
		}

		// �L�[�{�[�h�ł̑���
		if (!SceneManager::GetInstance().GetGamePad())
		{
			KeyboardContoroller();
		}

		// �Q�[���p�b�h�ł̑���
		if (SceneManager::GetInstance().GetGamePad())
		{
			GamePadController();
		}
	}

	//// �ҋ@�A�j���[�V����
	//ChangeAnimation(state_);

	Animation();


	// �Փ˔���
	Collision();






	transform_.pos.y = 0.0f;

	transform_.Update();

	// �A�j���[�V��������
	animationController_->Update();

}

void Player::Draw(void)
{

	// ���f���̕`��
	UnitBase::Draw();

	// �f�o�b�O�`��
	//DrawDebug();

	// �A�^�b�`����Ă��鐔
	DrawFormatString(0, 180, 0xff0000, "list:%d", animationController_->GetAttachNum());


	int y = 200;
	const auto& animDatas = animationController_->GetAnimDatas();
	for (const auto& anim : animDatas)
	{
		//if (anim.second.blendRate > 0.0f)
		{
			DrawFormatString(0, y, 0xff0000, "type:%s, step:%.2f, rate:%.2f",
				anim.second.state.c_str(),
				anim.second.stepAnim,
				anim.second.blendRate
			);
			y += 20;
		}
	}


	//if (animationController_->GetIsPriority())
	//{
	//	auto data = animationController_->GetAnimData(key_);
	//	DrawFormatString(0, 200, 0xff0000, "stepAnim_:%f, animType:%s", 
	//		data.stepAnim,
	//		data.state.c_str()
	//	);
	//}

}

void Player::ChargePlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectChargePlayId_ = PlayEffekseer3DEffect(effectChargeResId_);

	float SCALE = 100.0f;
	// �傫��
	SetScalePlayingEffekseer3DEffect(effectChargePlayId_, SCALE, SCALE, SCALE);

	// �ʒu
	ChargeSyncEffect();

}

void Player::ChargeSyncEffect(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = transform_.pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectLPos = followRot.PosAxis(LOCAL_CHRAGE_POS);

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectChargePos_ = VAdd(followPos, effectLPos);

	// �ʒu�̐ݒ�
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

	// ���f���̊J��
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

	// �G���g�̓����蔻��
	PlayerBodyCollision();

	// �X�e�[�W�̓����蔻��
	CollisionStage();

}

void Player::PlayerBodyCollision(void)
{

	// �G�̒��S�ɃJ�v�Z����Ǐ]������
	// �J�v�Z�����A�^�b�`����t���[���̃��[�J�������[���h�ϊ��s����擾����
	MATRIX PlayerFrameMatrix = MV1GetFrameLocalWorldMatrix(transform_.modelId, 0);

	// �s�񂩂烉�W�A��
	auto rot = MGetRotElem(PlayerFrameMatrix);
	auto pos = MGetTranslateElem(PlayerFrameMatrix);

	// �s�񂩂�N�H�[�^�j�I��
	Quaternion qua = Quaternion::GetRotation(rot);

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = qua;

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR cPosDOWN = followRot.PosAxis(LOCAL_BODY_C_DOWN_POS);
	VECTOR cPosUP = followRot.PosAxis(LOCAL_BODY_C_UP_POS);

	// �v���C���[�̈ʒu�̍X�V
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
		// �@���̕����ɂ�����Ƃ����ړ�������
		movedPos_ = VAdd(movedPos_, VScale(dir, 20.0f));

		// �J�v�Z�����ꏏ�Ɉړ�������
		transform_.pos = movedPos_;
		transform_.Update();
	}

	// �J�v�Z���Ƃ̏Փ˔���
	auto hits = MV1CollCheck_Capsule(
		stageId_, -1,
		cBodyPosUp_, cBodyPosDown_, COLLISION_BODY_RADIUS);

	// �Փ˂��������̃|���S���ƏՓˉ������܂ŁA
	// �v���C���[�̈ʒu���ړ�������
	for (int i = 0; i < hits.HitNum; i++)
	{

		auto hit = hits.Dim[i];

		// �n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
		// ���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
		// �Փ˂����|���S���̖@�������������肷��
		for (int tryCnt = 0; tryCnt < 10; tryCnt++)
		{

			// �ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
			// �ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������
			int pHit = HitCheck_Capsule_Triangle(
				cBodyPosUp_, cBodyPosDown_, COLLISION_BODY_RADIUS,
				hit.Position[0], hit.Position[1], hit.Position[2]);

			if (pHit)
			{

				// �@���̕����ɂ�����Ƃ����ړ�������
				movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.2f));

				// �J�v�Z�����ꏏ�Ɉړ�������
				transform_.pos = movedPos_;
				transform_.Update();
				continue;

			}

			break;

		}

	}

	// ���o�����n�ʃ|���S�����̌�n��
	MV1CollResultPolyDimTerminate(hits);

}

void Player::KeyboardContoroller(void)
{

	// �ړ�����
	KeyboardMove();

	//�U������
	KeyboardAttack();

	// �v���C���[�����ɃJ�����������鏈��
	KeyBoardCamera();

	// �G�����b�N�I�����鏈��
	KeyBoardLockOn();

}

void Player::KeyboardMove(void)
{

	// �J�����̎擾
	Camera* camera = SceneManager::GetInstance().GetCamera();

	// �J�����̊p�x���擾
	VECTOR cameraAngles = camera->GetAngles();

	auto& ins = InputManager::GetInstance();

	// ����(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// WASD�Ńv���C���[�̈ʒu��ς���
	if (ins.IsNew(KEY_INPUT_W)) { dir = VAdd(dir, { 0.0f, 0.0f, 1.0f }); }
	if (ins.IsNew(KEY_INPUT_A)) { dir = VAdd(dir, { -1.0f, 0.0f, 0.0f }); }
	if (ins.IsNew(KEY_INPUT_S)) { dir = VAdd(dir, { 0.0f, 0.0f, -1.0f }); }
	if (ins.IsNew(KEY_INPUT_D)) { dir = VAdd(dir, { 1.0f, 0.0f, 0.0f }); }

	// �ړ�
	if (state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{
		// ����
		if (ins.IsNew(KEY_INPUT_LSHIFT) && !AsoUtility::EqualsVZero(dir)
			&& state_ != STATE::CHARGE_WALK && state_ != STATE::TIRED && stamina_ >= 1.0f)
		{
			ChangeState(STATE::RUN);
			speed_ = MOVE_POW_RUN;
		}
		// ����
		else if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::CHARGE_WALK)
		{
			ChangeState(STATE::WALK);
			// �ړ���
			speed_ = MOVE_POW_WALK;
		}
		// �ҋ@���
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

	//���߂Ȃ������
	if (ins.IsClickMouseLeft() && (ins.IsNew(KEY_INPUT_W) || ins.IsNew(KEY_INPUT_A) ||
		ins.IsNew(KEY_INPUT_S) || ins.IsNew(KEY_INPUT_D)) &&
		!AsoUtility::EqualsVZero(dir) && state_ != STATE::HIT &&state_ != STATE::ROLL && state_ != STATE::TIRED)
	{

		// �����𐳋K��
		dir = VNorm(dir);

		// Y���̍s��
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// ��]�s����g�p���āA�x�N�g������]������
		moveDir_ = VTransform(dir, mat);

		// �ړ���
		speed_ = MOVE_POW_CHRAGE_WALK;

		// �������p�x�ɕϊ�����(XZ���� Y��)
		float angle = atan2f(dir.x, dir.z);

	}

	// ���
	if (ins.IsTrgDown(KEY_INPUT_SPACE) && !AsoUtility::EqualsVZero(dir) &&
		state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED && stamina_ >= 10.0f)
	{

		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);

		ChangeState(STATE::ROLL);

		// �����𐳋K��
		dir = VNorm(dir);

		// Y���̍s��
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// ��]�s����g�p���āA�x�N�g������]������
		moveDir_ = VTransform(dir, mat);

		// �ړ���
		speed_ = MOVE_POW_ROLL;

		// �������p�x�ɕϊ�����(XZ���� Y��)
		float angle = atan2f(dir.x, dir.z);

		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		LazyRotation(cameraAngles.y + angle);

	}

	// ��ꂽ��
	if (state_ == STATE::TIRED)
	{
		speed_ = 0;
	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{

		// �����𐳋K��
		dir = VNorm(dir);

		// Y���̍s��
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// ��]�s����g�p���āA�x�N�g������]������
		moveDir_ = VTransform(dir, mat);

		// ���b�N�I�����͑���ɋ߂Â��̂ɐ���������
		if (camera->GetMode() == Camera::MODE::LOCKON)
		{
			LockOn();
		}

		// �������p�x�ɕϊ�����(XZ���� Y��)
		float angle = atan2f(dir.x, dir.z);

		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		LazyRotation(cameraAngles.y + angle);

	}

	if (state_ != STATE::HIT && state_ != STATE::ATTACK && state_ != STATE::ATTACK2 &&
		state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK && state_ != STATE::ROLL)
	{
		// �ړ���
		movePow_ = VScale(moveDir_, speed_);

		// ���ݍ��W���N�_�Ɉړ�����W�����߂�
		movedPos_ = VAdd(transform_.pos, movePow_);
	}

	// �J�����̒����_
	auto cameraTargetPos = followTransform_->pos;

	float y = movedPos_.y;

	// XZ���ʂ̈ړ�����W
	auto movedPosXZ = movedPos_;

	// XZ���ʂ̃J�����̒����_
	auto cameraTargetPosXZ = cameraTargetPos;

	// XZ���ʂ̃J�������W
	auto cameraPosXZ = camera->GetPos();

	// �ړ�����W�ƃJ�����̒����_�ƃJ�������W��0�ɂ��邱�Ƃ�XZ���ʍ��W�ɂ��Ă���
	movedPosXZ.y = cameraTargetPosXZ.y = cameraPosXZ.y = 0.0f;

	// �����_����̃v���C���[�̃x�N�g��
	auto target2Player = VNorm(VSub(movedPosXZ, cameraTargetPosXZ));

	// �ړ�����W�ƈړ��O���W��0�ȏ�̎�
	if (!AsoUtility::EqualsVZero(moveDiff_))
	{

		// �����_�ƈړ�����W�̋���
		target2PlayerDis_ = AsoUtility::Distance(cameraTargetPos, movedPos_);

		// �G�Ƃ̍ŏ����̋���
		enemyMinDis_ = 100.0f;

		// �����_�ƓG�Ƃ̍ŏ����̋�����100�����̎�
		if (target2PlayerDis_ < enemyMinDis_)
		{

			// �����_����ړ�����W�̃x�N�g�����N�H�[�^�j�I����
			auto rot = Quaternion::LookRotation(target2Player);

			// �ړ�����W���X�V
			movedPos_ = VAdd(cameraTargetPos, VScale(rot.GetForward(), enemyMinDis_ + 0.5f));
			movedPos_.y = y;

		}

	}

	// �G�ƏՓ˂��Ă�������W��߂�
	moveDiff_ = VSub(movedPos_, transform_.pos);
	transform_.pos = movedPos_;

}

void Player::KeyboardAttack(void)
{

	auto& insInput = InputManager::GetInstance();
	auto& insScene = SceneManager::GetInstance();

	// �U������
	// �{�^�����N���b�N���ꂽ���ǂ������m�F
	if (chargeCnt_ >= 0.5 && state_ != STATE::CHARGE_ATTACK && state_ != STATE::CHARGE_WALK
		&& state_ != STATE::ATTACK && state_ != STATE::ATTACK2 && state_ != STATE::ATTACK3 
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{
		ChangeState(STATE::CHARGE_WALK);
	}

	if (insInput.IsTrgUpMouseLeft() && chargeCnt_ <= CHARGE_TIME && state_ != STATE::HIT)
	{

		//�{�^���������ꂽ��A�j���[�V������؂�ւ���
		//�P�i�K��
		if (state_ == STATE::IDLE || state_ == STATE::RUN || state_ == STATE::WALK || state_ == STATE::CHARGE_WALK)
		{
			attack1_ = true;
			ChangeState(STATE::ATTACK);
		}
		// �Q�i�K��
		else if (state_ == STATE::ATTACK && !attack2_)
		{
			attack2_ = true;
		}
		// 3�i�K��
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

	// �P�i�K�ڂ��I�������J�ڂ���
	if (attack2_ && !attack1_ && state_ == STATE::ATTACK)
	{
		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);
		ChangeState(STATE::ATTACK2);
	}

	// �Q�i�K�ڂ��I�������J�ڂ���
	if (attack3_ && !attack2_ && state_ == STATE::ATTACK2)
	{
		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);
		ChangeState(STATE::ATTACK3);
	}

	// ���ߎa��
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

	// �v���C���[�������Ă�������ɃJ������������
	auto rad = transform_.quaRot.ToEuler();
	if (ins.IsTrgDown(KEY_INPUT_Q))
	{
		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		SceneManager::GetInstance().GetCamera()->SetLazyAngles(rad);
	}

}

void Player::KeyBoardLockOn(void)
{

	auto& ins = InputManager::GetInstance();

	auto camera = SceneManager::GetInstance().GetCamera();

	// �v���C���[�̕��������߂�
	auto length = AsoUtility::Distance(followTransform_->pos, transform_.pos);
	// �L�[���������烍�b�N�I������
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

	// �ړ�����
	GamePadMove();

	// �U������
	GamePadAttack();

	// �v���C���[�����ɃJ�����������鏈��
	GamePadCamera();

	// �G�����b�N�I�����鏈��
	GamePadLockOn();

}

void Player::GamePadMove(void)
{

	// �J�����̎擾
	Camera* camera = SceneManager::GetInstance().GetCamera();

	// �J�����̊p�x���擾
	VECTOR cameraAngles = camera->GetAngles();

	auto& ins = InputManager::GetInstance();

	// ����(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// �ړ���
	float movePow = MOVE_POW_WALK;

	// �Q�[���p�b�h�̔ԍ����擾
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// �p�b�h�̕�����dir�ɒ���
	dir.x = pad.AKeyLX;
	dir.z = -pad.AKeyLZ;

	// �ړ�
	if (state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{

		// ����
		if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN)
			&& !AsoUtility::EqualsVZero(dir)
			&& state_ != STATE::CHARGE_WALK && state_ != STATE::TIRED && stamina_ >= 1.0f)
		{
			ChangeState(STATE::RUN);
			speed_ = MOVE_POW_RUN;
		}
		// ����
		else if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::CHARGE_WALK)
		{
			ChangeState(STATE::WALK);
			// �ړ���
			speed_ = MOVE_POW_WALK;
		}
		// �ҋ@���
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

	//���߂Ȃ������
	if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)
		/*&& (pad.AKeyLX != 0.0f || pad.AKeyLZ != 0.0f)*/ && !AsoUtility::EqualsVZero(dir)
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{
		// �����𐳋K��
		dir = VNorm(dir);

		// Y���̍s��
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// ��]�s����g�p���āA�x�N�g������]������
		moveDir_ = VTransform(dir, mat);

		// �ړ���
		speed_ = MOVE_POW_CHRAGE_WALK;

		// �������p�x�ɕϊ�����(XZ���� Y��)
		float angle = atan2f(dir.x, dir.z);
	}

	// ���
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_BOTTON)
		&& !AsoUtility::EqualsVZero(dir) &&
		state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED && stamina_ >= 10.0f)
	{

		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);

		ChangeState(STATE::ROLL);

		// �����𐳋K��
		dir = VNorm(dir);

		// Y���̍s��
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// ��]�s����g�p���āA�x�N�g������]������
		moveDir_ = VTransform(dir, mat);

		// �ړ���
		speed_ = MOVE_POW_ROLL;

		// �������p�x�ɕϊ�����(XZ���� Y��)
		float angle = atan2f(dir.x, dir.z);

		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		LazyRotation(cameraAngles.y + angle);

	}

	// ��ꂽ��
	if (state_ == STATE::TIRED)
	{
		speed_ = 0;
	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{

		// �����𐳋K��
		dir = VNorm(dir);

		// Y���̍s��
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// ��]�s����g�p���āA�x�N�g������]������
		moveDir_ = VTransform(dir, mat);

		// ���b�N�I�����͑���ɋ߂Â��̂ɐ���������
		if (camera->GetMode() == Camera::MODE::LOCKON)
		{
			LockOn();
		}

		// �������p�x�ɕϊ�����(XZ���� Y��)
		float angle = atan2f(dir.x, dir.z);

		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		LazyRotation(cameraAngles.y + angle);

	}

	if (state_ != STATE::HIT && state_ != STATE::ATTACK && state_ != STATE::ATTACK2 &&
		state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK && state_ != STATE::ROLL)
	{
		// �ړ���
		movePow_ = VScale(moveDir_, speed_);

		// ���ݍ��W���N�_�Ɉړ�����W�����߂�
		movedPos_ = VAdd(transform_.pos, movePow_);
	}

	// �J�����̒����_
	auto cameraTargetPos = followTransform_->pos;

	float y = movedPos_.y;

	// XZ���ʂ̈ړ�����W
	auto movedPosXZ = movedPos_;

	// XZ���ʂ̃J�����̒����_
	auto cameraTargetPosXZ = cameraTargetPos;

	// XZ���ʂ̃J�������W
	auto cameraPosXZ = camera->GetPos();

	// �ړ�����W�ƃJ�����̒����_�ƃJ�������W��0�ɂ��邱�Ƃ�XZ���ʍ��W�ɂ��Ă���
	movedPosXZ.y = cameraTargetPosXZ.y = cameraPosXZ.y = 0.0f;

	// �����_����̃v���C���[�̃x�N�g��
	auto target2Player = VNorm(VSub(movedPosXZ, cameraTargetPosXZ));

	// �ړ�����W�ƈړ��O���W��0�ȏ�̎�
	if (!AsoUtility::EqualsVZero(moveDiff_))
	{

		// �����_�ƈړ�����W�̋���
		target2PlayerDis_ = AsoUtility::Distance(cameraTargetPos, movedPos_);

		// �G�Ƃ̍ŏ����̋���
		enemyMinDis_ = 100.0f;

		// �����_�ƓG�Ƃ̍ŏ����̋�����100�����̎�
		if (target2PlayerDis_ < enemyMinDis_)
		{

			// �����_����ړ�����W�̃x�N�g�����N�H�[�^�j�I����
			auto rot = Quaternion::LookRotation(target2Player);

			// �ړ�����W���X�V
			movedPos_ = VAdd(cameraTargetPos, VScale(rot.GetForward(), enemyMinDis_ + 0.5f));
			movedPos_.y = y;

		}

	}

	// �G�ƏՓ˂��Ă�������W��߂�
	moveDiff_ = VSub(movedPos_, transform_.pos);
	transform_.pos = movedPos_;


}

void Player::GamePadAttack(void)
{

	auto& insInput = InputManager::GetInstance();
	auto& insScene = SceneManager::GetInstance();

	// �U������
	// �{�^�����N���b�N���ꂽ���ǂ������m�F
	if (chargeCnt_ >= 0.1 && state_ != STATE::CHARGE_ATTACK && state_ != STATE::CHARGE_WALK
		&& state_ != STATE::ATTACK && state_ != STATE::ATTACK2 && state_ != STATE::ATTACK3 
		&& state_ != STATE::HIT && state_ != STATE::ROLL && state_ != STATE::TIRED)
	{
		ChangeState(STATE::CHARGE_WALK);
	}

	if (insInput.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) &&
		chargeCnt_ <= CHARGE_TIME && state_ != STATE::HIT)
	{

		//�{�^���������ꂽ��A�j���[�V������؂�ւ���
		//�P�i�K��
		if (state_ == STATE::IDLE || state_ == STATE::RUN || state_ == STATE::WALK || state_ == STATE::CHARGE_WALK)
		{
			attack1_ = true;
			ChangeState(STATE::ATTACK);
		}
		// �Q�i�K��
		else if (state_ == STATE::ATTACK && !attack2_)
		{
			attack2_ = true;
		}
		// 3�i�K��
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

	// �P�i�K�ڂ��I�������J�ڂ���
	if (attack2_ && !attack1_ && state_ == STATE::ATTACK)
	{
		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);
		ChangeState(STATE::ATTACK2);
	}

	// �Q�i�K�ڂ��I�������J�ڂ���
	if (attack3_ && !attack2_ && state_ == STATE::ATTACK2)
	{
		chargeCnt_ = 0.0f;
		StopEffekseer3DEffect(effectChargePlayId_);
		ChangeState(STATE::ATTACK3);
	}

	// ���ߎa��
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

	// �v���C���[�������Ă�������ɃJ������������
	auto rad = transform_.quaRot.ToEuler();
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		SceneManager::GetInstance().GetCamera()->SetLazyAngles(rad);
	}

}

void Player::GamePadLockOn(void)
{

	auto& ins = InputManager::GetInstance();

	auto camera = SceneManager::GetInstance().GetCamera();

	// �v���C���[�̕��������߂�
	auto length = AsoUtility::Distance(followTransform_->pos, transform_.pos);

	// �L�[���������烍�b�N�I������
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

	// �G�ɋ߂Â��G�̎���𑖂葱����ƃJ�������v���C���[��ǂ�Ȃ��Ȃ�

	Camera* camera = SceneManager::GetInstance().GetCamera();

	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	// �@�J���������b�N�I���̎��́A������]���s��Ȃ�
	if (camera->GetMode() != Camera::MODE::LOCKON)
	{
		// �ړ������ɉ�������]
		Rotate();
	}

	//// �A�ړ������O�ɁA�^�[�Q�b�g�Ƃ̓��ʏՓ˔���
	//moveDiff_ = VSub(movedPos_, transform_.pos);

	//// ���b�N�I�����[�h�̎�
	//if (camera->GetMode() == Camera::MODE::LOCKON)
	//{

	//	// �J�����̒����_
	//	auto cameraTargetPos = camera->GetTargetPos();

	//	float y = movedPos_.y;

	//	// XZ���ʂ̈ړ�����W
	//	auto movedPosXZ = movedPos_;

	//	// XZ���ʂ̃J�����̒����_
	//	auto cameraTargetPosXZ = cameraTargetPos;

	//	// XZ���ʂ̃J�������W
	//	auto cameraPosXZ = camera->GetPos();

	//	// �ړ�����W�ƃJ�����̒����_�ƃJ�������W��0�ɂ��邱�Ƃ�XZ���ʍ��W�ɂ��Ă���
	//	movedPosXZ.y = cameraTargetPosXZ.y = cameraPosXZ.y = 0.0f;

	//	// �����_����̃v���C���[�̃x�N�g��
	//	auto target2Player = VNorm(VSub(movedPosXZ, cameraTargetPosXZ));

	//	//// �ړI�̊p�x�܂ł̍��𑪂�
	//	//SetGoalRotate(Quaternion::LookRotation(VScale(target2Player, -1.0)));

	//	//// ��������]������
	//	//Rotate();

	//	// �ړ�����W�ƈړ��O���W��0�ȏ�̎�
	//	if (!AsoUtility::EqualsVZero(moveDiff_))
	//	{

	//		// �����_�ƈړ�����W�̋���
	//		target2PlayerDis_ = AsoUtility::Distance(cameraTargetPos, movedPos_);

	//		// �G�Ƃ̍ŏ����̋���
	//		enemyMinDis_ = 100.0f;

	//		// �����_�ƓG�Ƃ̍ŏ����̋�����100�����̎�
	//		if (target2PlayerDis_ < enemyMinDis_)
	//		{

	//			// �����_����ړ�����W�̃x�N�g�����N�H�[�^�j�I����
	//			auto rot = Quaternion::LookRotation(target2Player);

	////			// �E����
	////			auto r = rot.GetRight();

	////			// ������
	////			auto l = rot.GetLeft();

	////			// �E�����̓���
	////			auto dotR = VDot(r, target2Player);

	////			// �������̓���
	////			auto dotL = VDot(l, target2Player);

	////			float deg = 2.0f;


	////			// �E��������I�ԏ���

	////			if (dotR + 0.01f > dotL)

	////			{

	////				deg *= -1.0f;

	////				// �L�������E�ɉ��

	////				rot = rot.Mult(

	////					Quaternion::AngleAxis(-deg * DX_PI_F / 180.0f, AsoUtility::AXIS_Y));

	////				// ���ς̑傫���ق��Ɋp�x�𑫂�
	////				camera->AddLockOnAnglesY(deg * DX_PI_F / 180.0f);


	////			}

	////			else

	////			{

	////				// �L���������ɉ��

	////				rot = rot.Mult(

	////					Quaternion::AngleAxis(deg * DX_PI_F / 180.0f, AsoUtility::AXIS_Y));

	////				camera->AddLockOnAnglesY(deg * DX_PI_F / 180.0f);

	////			}


	//			movedPos_ = VAdd(cameraTargetPos, VScale(rot.GetForward(), enemyMinDis_ + 0.5f));
	//			movedPos_.y = y;

	//		}

	//	}

	//}


	//// �ړ�

	//moveDiff_ = VSub(movedPos_, transform_.pos);

	//transform_.pos = movedPos_;


	// �B��]�����܂ł̎��ԒZ�k


}

// �⑫�֐�
void Player::SetGoalRotate(Quaternion rot)
{

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(rot, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1)
	{
		// ��]�����̎���
		stepRotTime_ = TIME_ROT;
	}

	// �ړI�̊p�x
	goalQuaRot_ = rot;

}

void Player::Rotate(void)
{

	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

	// ��]�̋��ʕ��
	transform_.quaRot = Quaternion::Slerp(
		transform_.quaRot, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

}

//void Player::ChangeAnimation(STATE state)
//{
//
//	// ������Ԃ����������Ȃ�
//	if (state_ == preState_) return;
//
//	// �O�̏�Ԃ̗D��x���Ȃ���
//	animData_[(int)preState_].isPriority_ = false;
//
//	// �Đ�����A�j���[�V�����̐ݒ�
//	AttatchNum((int)state_);
//
//	// ��ԑJ�ڎ��̏���������
//	switch (state_)
//	{
//	case Player::STATE::IDLE:
//		animationController_->Play("IDLE", true, false, true);
//		// �������~�߂�
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::WALK:
//		animationController_->Play("WALK", true, false, true);
//		break;
//	case Player::STATE::CHARGE_WALK:
//		animationController_->Play("CHARGE_WALK", true, false, true);
//		// �G�t�F�N�g�̍Đ�
//		ChargePlayEffect();
//
//		// ���߂鉹�̍Đ�
//		PlaySoundMem(musicChargeId_,DX_PLAYTYPE_BACK);
//
//		// �������~�߂�
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::RUN:
//		animationController_->Play("RUN", true, false, true);
//		break;
//	case Player::STATE::ATTACK:
//		animationController_->Play("ATTACK", true, false, true);
//		hit_ = false;
//		// �G�t�F�N�g���~�߂�
//		StopEffekseer3DEffect(effectChargePlayId_);
//		// �������~�߂�
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::ATTACK2:
//		animationController_->Play("ATTACK2", true, false, true);
//		hit_ = false;
//		// �������~�߂�
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::ATTACK3:
//		animationController_->Play("ATTACK3", true, false, true);
//		hit_ = false;
//		// �������~�߂�
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::CHARGE_ATTACK:
//		animationController_->Play("CHARGE_ATTACK", true, false, true);
//		hit_ = false;
//		// �������~�߂�
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::HIT:
//		animationController_->Play("HIT", true, false, true);
//		// �G�t�F�N�g���~�߂�
//		StopEffekseer3DEffect(effectChargePlayId_);
//		// �������~�߂�
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::DEATH:
//		animationController_->Play("DEATH", true, false, true);
//		// �������~�߂�
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		break;
//	case Player::STATE::ROLL:
//		animationController_->Play("ROLL", true, false, true);
//		// �������~�߂�
//		StopSoundMem(musicFootStepsId_);
//		musicFootStepsCnt_ = 0.0f;
//		// �X�^�~�i�����炷
//		stamina_ -= 10.0f;
//		break;
//	case Player::STATE::TIRED:
//		animationController_->Play("TIRED", true, false, true);
//		// �������~�߂�
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
//	// ��O�̏�Ԃ�ۑ�
//	preState_ = state_;
//
//}

void Player::ChangeState(STATE state)
{

	if (state_ == state)return;

	state_ = state;

	preKey_ = key_;

	key_ = ANIM_DATA_KEY[(int)state];

	animationController_->ChangeAnimation(key_);

}

void Player::LazyRotation(float goalRot)
{

	//// ��]����
	//// �N�H�[�^�j�I�����烉�W�A��
	//float radNowAngleY = transform_.quaRot.ToEuler().y;
	//float degNowAngleY = AsoUtility::Rad2DegF(radNowAngleY);
	//float degGoalAngleY = AsoUtility::Rad2DegF(goalRot);

	//// 0�x�`360�x�ȓ��Ɋp�x�������߂�
	////degGoalAngleY = static_cast<float>(AsoUtility::DegIn360(degGoalAngleY));

	//// ��]�����Ȃ����̉�]�������擾����(���v���:1�A�����v���:-1)
	//int aroundDir = AsoUtility::DirNearAroundDeg(degNowAngleY, degGoalAngleY);

	//// ��]�ʂ̍쐬
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
	//	// ���W�A������N�H�[�^�j�I��
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
	DrawFormatString(0, 70, 0xffffff, "�v���C���[���W : (%.1f, %.1f, %.1f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);
	DrawFormatString(0, 90, 0xffffff, "�v���C���[�p�xdeg : (%.1f, %.1f, %.1f)", pDeg.x, pDeg.y, pDeg.z);
	DrawFormatString(0, 110, 0xffffff, "�v���C���[�p�xrad : (%.5f, %.5f, %.5f)", rad.x, rad.y, rad.z);

	// �G�l�~�[���g�̏Փ˔���̃J�v�Z���̕`��
	DrawCapsule3D(cBodyPosDown_, cBodyPosUp_, COLLISION_BODY_RADIUS, 10, 0xff0000, 0xff0000, false);

}

void Player::SetParam(void)
{

	// HP�̍ő�l
	hpMax_ = HP_MAX;

	// HP
	hp_ = hpMax_;

	// �X�^�~�i�̍ő�l
	staminaMax_ = STAMINA_MAX;

	// �X�^�~�i
	stamina_ = staminaMax_;

}

void Player::Animation(void)
{

	// �A�j���[�V�����̌Œ�
	AnimationFrame();

	if (state_ == STATE::ROLL)
	{
		int a = 0;
	}
	// �A�j���[�V�����Đ�
	if (animationController_->GetAnimData(key_).stepAnim >= animationController_->GetAnimData(key_).animTotalTime)
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

	// 2�i�K�ڂɐi�܂Ȃ��Ƃ��̓��Z�b�g
	if (state_ == STATE::ATTACK && animationController_->GetAnimData("ATTACK").stepAnim >= 
		animationController_->GetAnimData("ATTACK").animTotalTime && !attack2_)
	{
		hit_ = false;
		ChangeState(STATE::IDLE);
		chargeCnt_ = 0.0f;
	}

	// 3�i�K�ڂɐi�܂Ȃ��Ƃ��̓��Z�b�g
	if (state_ == STATE::ATTACK2 && animationController_->GetAnimData("ATTACK2").stepAnim >= 
		animationController_->GetAnimData("ATTACK2").animTotalTime && !attack3_)
	{
		animationController_->SetStartStepAnim("ATTACK2", ATTACK_START_TIME2);
		hit_ = false;
		ChangeState(STATE::IDLE);
		chargeCnt_ = 0.0f;
	}

}

void Player::AnimationFrame(void)
{

	// �Ώۃt���[���̃��[�J���s��������l�Ƀ��Z�b�g����
	MV1ResetFrameUserLocalMatrix(transform_.modelId, playerAttachFrameNum_);

	// �W�����v�U�����ɍ��W���Œ肷��
	if (state_ == STATE::ATTACK || state_ == STATE::ATTACK2 || state_ == STATE::ATTACK3 || state_ == STATE::CHARGE_ATTACK || state_ == STATE::ROLL)
	{

		// �Ώۃt���[���̃��[�J���s��(�傫���A��]�A�ʒu)���擾����
		auto mat = MV1GetFrameLocalMatrix(transform_.modelId, playerAttachFrameNum_);

		auto scl = MGetSize(mat); // �s�񂩂�傫�������o��
		auto rot = MGetRotElem(mat); // �s�񂩂��]�����o��
		auto pos = MGetTranslateElem(mat); // �s�񂩂�ړ��l�����o��

		// �傫���A��]�A�ʒu�����[�J���s��ɖ߂�
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // �傫��
		mix = MMult(mix, rot); // ��]

		// �����Ń��[�J�����W���s��ɁA���̂܂ܖ߂����A
		// �����������[�J�����W��ݒ肷��
		mix = MMult(mix, MGetTranslate({ pos.x, pos.y, 0.0f }));

		// ���������s���Ώۃt���[���ɃZ�b�g�������āA
		// �A�j���[�V�����̈ړ��l�𖳌���
		MV1SetFrameUserLocalMatrix(transform_.modelId, playerAttachFrameNum_, mix);

	}

}