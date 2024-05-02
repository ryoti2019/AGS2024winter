#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationContorller.h"
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

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::ENEMY_IDLE));

	auto& model = ResourceManager::GetInstance();

	std::string path = Application::PATH_MODEL + "Enemy/";
	animationController_ = new AnimationController(transform_.modelId, 0);
	animationController_->Add("THINK", path + "idle.mv1", 0.0f, 121.0f, IDLE_ANIM_SPEED,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_IDLE), true, false);
	animationController_->Add("IDLE", path + "idle.mv1", 0.0f, 121.0f, IDLE_ANIM_SPEED,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_IDLE),true,false);
	animationController_->Add("WALK", path + "walk.mv1", 0.0f, 43.0f, WALK_ANIM_SPEED,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_WALK), true, false);
	animationController_->Add("ATTACK", path + "attack.mv1", 0.0f, 80.0f, ATTACK_ANIM_SPEED,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_ATTACK), false, false);
	animationController_->Add("JUMP_ATTACK", path + "jumpAttack.mv1", 0.0f, 111.0f, JUMP_ATTACK_ANIM_SPEED,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_JUMP_ATTACK), false, false);
	animationController_->Add("BEFORE_TACKLE", path + "idle.mv1", 0.0f, 120.0f, IDLE_ANIM_SPEED,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_IDLE), true, false);
	animationController_->Add("TACKLE", path + "tackle.mv1", 0.0f, 38.0f, TACKLE_ANIM_SPEED,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_IDLE), true, false);
	animationController_->Add("CREATE", path + "create.mv1", 15.0f, 45.0f, 30.0f,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_SHOT_CREATE), false, false);
	animationController_->Add("SHOT", path + "shot.mv1", SHOT_START_TIME, SHOT_END_TIME, SHOT_ANIM_SPEED,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_SHOT_ATTACK), false, false);
	animationController_->Add("HIT", path + "hit.mv1", 0.0f, 81.0f, HIT_ANIM_SPEED,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_HIT), false, false);
	animationController_->Add("DEATH", path + "death.mv1", 0.0f, 81.0,30.0f,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_DEATH), false, false);
	animationController_->Add("TURN_LEFT", path + "turnLeft.mv1", 0.0f, 40.0f, 30.0f,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_TURN_LEFT), false, false);
	animationController_->Add("TURN_RIGHT", path + "turnRight.mv1", 0.0f, 40.0f, 30.0f,
		model.LoadModelDuplicate(ResourceManager::SRC::ENEMY_TURN_RIGHT), false, false);

	animationController_->ChangeAnimation("THINK");

	key_ = "THINK";

	// ���b�N�I����Ԃɏo��J�[�\��
	lockOnCursorImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CURSOR).handleIds_;

	// transform�̏�����
	float scale = 2.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 1000.0f };
	transform_.Update();

	// �ҋ@��Ԃ̒e�̐�
	shotNum_ = 0;

}

void Enemy::Init(void)
{

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

	// �G�t�F�N�g�̏����ݒ�
	InitEffect();

	// ���̏����ݒ�
	InitMusic();

	// �v���C���[�̃p�����[�^�[
	SetParam();

	// �ҋ@�A�j���[�V����
	//SetIdleAnimation();

	// HP�o�[�̉摜
	imgHPBar_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_HP_BAR).handleId_;

	// �J�v�Z�����A�^�b�`����t���[���̔ԍ�������
	enemyAttachFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:Spine2");

	// ������A�^�b�`����t���[���̔ԍ�������
	weponAttachFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:LeftHand");

	// �t���[���ԍ����t���[�����Ŏ擾����
	enemyPosFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:Hips");

	// �U�������ǂ���
	attack_ = false;

	// �_�b�V���A�^�b�N�̃t���O
	jumpAttack_ = false;

	// ��]�̊J�n
	startRotation_ = false;

	// ��]�̏I���̃t���O
	isRotation_ = false;

	// �s���I���̃t���O
	isAction_ = false;

	isNoPlay_ = false;

	// �e�𔭎˂������ǂ���
	isShot_ = false;

	// �����������̃t���O
	hit_ = false;

	// �ŏ��̕����̃A�j���[�V�����̃J�E���^
	walkCnt_ = 0.0f;

	noPlayTime_ = 0.0f;

	lockOnCursorCnt_ = 0;

	lockOnCursorTime_ = 0.0f;

	shotCnt_ = 0.0f;

	beforeTackleCnt_ = 0.0f;

	// �������
	ChangeState(STATE::THINK);

}

void Enemy::InitEffect(void)
{

	// �e�����Ƃ��̃G�t�F�N�g
	effectCreateResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CREATE_EFFECT).handleId_;

	// �^�b�N���̃G�t�F�N�g
	effectTackleResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TACKLE_EFFECT).handleId_;

	// �^�b�N���̃G�t�F�N�g
	effectTackleRangeResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TACKLE_RANGE_EFFECT).handleId_;

	// �W�����v�A�^�b�N�̃G�t�F�N�g
	effectJumpAttackResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::JUMPATTACK_EFFECT).handleId_;

	// �W�����v�A�^�b�N�G�t�F�N�g�̃J�E���^
	isEffectJumpAttack_ = true;

	// �W�����v�A�^�b�N�̍U���͈͂̃G�t�F�N�g
	effectJumpAttackRangeResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::JUMPATTACK_RANGE_EFFECT).handleId_;

}

void Enemy::CreatePlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectCreatePlayId_ = PlayEffekseer3DEffect(effectCreateResId_);

	float SCALE = 100.0f;
	// �傫��
	SetScalePlayingEffekseer3DEffect(effectCreatePlayId_, SCALE, SCALE, SCALE);

	// �ʒu
	CreateSyncEffect();

}

void Enemy::TacklePlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectTacklePlayId_ = PlayEffekseer3DEffect(effectTackleResId_);

	float SCALE = 100.0f;
	// �傫��
	SetScalePlayingEffekseer3DEffect(effectTacklePlayId_, SCALE, SCALE, SCALE);

	// �ʒu
	TackleSyncEffect();

}

void Enemy::TackleRangePlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectTackleRangePlayId_ = PlayEffekseer3DEffect(effectTackleRangeResId_);

	// �傫��
	SetScalePlayingEffekseer3DEffect(effectTackleRangePlayId_, 100.0f, 0.0f, 700.0f);

	// �ʒu
	TackleRangeSyncEffect();

}

void Enemy::JumpAttackPlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectJumpAttackPlayId_ = PlayEffekseer3DEffect(effectJumpAttackResId_);

	float SCALE = 500.0f;
	// �傫��
	SetScalePlayingEffekseer3DEffect(effectJumpAttackPlayId_, SCALE, SCALE, SCALE);

	// �ʒu
	JumpAttackSyncEffect();

}

void Enemy::JumpAttackRangePlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectJumpAttackRangePlayId_ = PlayEffekseer3DEffect(effectJumpAttackRangeResId_);

	float SCALE = 200.0f;
	// �傫��
	SetScalePlayingEffekseer3DEffect(effectJumpAttackRangePlayId_, SCALE, SCALE, SCALE);

	// �ʒu
	JumpAttackRangeSyncEffect();

}

void Enemy::CreateSyncEffect(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = transform_.pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis({ 0.0f,0.0f,0.0f });

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectCreatePos_ = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectCreatePlayId_, effectCreatePos_.x, effectCreatePos_.y, effectCreatePos_.z);
	SetRotationPlayingEffekseer3DEffect(effectCreatePlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void Enemy::TackleSyncEffect(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = transform_.pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis({ 0.0f,0.0f,0.0f });

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectTacklePos_ = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectTacklePlayId_, effectTacklePos_.x, effectTacklePos_.y, effectTacklePos_.z);
	SetRotationPlayingEffekseer3DEffect(effectTacklePlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void Enemy::TackleRangeSyncEffect(void)
{

	//// �Ǐ]�Ώۂ̈ʒu
	//VECTOR followPos = transform_.pos;

	//// �Ǐ]�Ώۂ̌���
	//Quaternion followRot = transform_.quaRot;

	//VECTOR rot = Quaternion::ToEuler(followRot);

	//// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	//VECTOR effectPos = followRot.PosAxis({ 0.0f,10.0f,-3800.0f });

	//// �G�t�F�N�g�̈ʒu�̍X�V
	//effectTackleRangePos_ = VAdd(followPos, effectPos);

	//// �ʒu�̐ݒ�
	//SetPosPlayingEffekseer3DEffect(effectTackleRangePlayId_, effectTackleRangePos_.x, effectTackleRangePos_.y, effectTackleRangePos_.z);
	//SetRotationPlayingEffekseer3DEffect(effectTackleRangePlayId_, rot.x, rot.y, rot.z);


	// New
	auto atkPos = attackPlayerPos_;
	atkPos.y = 0.0f;

	auto nowPos = transform_.pos;
	nowPos.y = 0.0f;

	auto dirXZ = VSub(atkPos, nowPos);
	auto rotXZ = Quaternion::LookRotation(dirXZ);
	auto rot = rotXZ.ToEuler();

	SetRotationPlayingEffekseer3DEffect(effectTackleRangePlayId_, rot.x, rot.y, rot.z);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = rotXZ.PosAxis({ 0.0f,10.0f,3800.0f });

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectTackleRangePos_ = VAdd(nowPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(
		effectTackleRangePlayId_, effectTackleRangePos_.x, effectTackleRangePos_.y, effectTackleRangePos_.z);

	transform_.Update();

}

void Enemy::JumpAttackSyncEffect(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = attackPlayerPos_;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis({ 0.0f,0.0f,0.0f });

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectJumpAttackPos_ = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectJumpAttackPlayId_, effectJumpAttackPos_.x, effectJumpAttackPos_.y, effectJumpAttackPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJumpAttackPlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void Enemy::JumpAttackRangeSyncEffect(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = attackPlayerPos_;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis({ 0.0f,10.0f,0.0f });

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectJumpAttackRangePos_ = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectJumpAttackRangePlayId_, effectJumpAttackRangePos_.x, effectJumpAttackRangePos_.y, effectJumpAttackRangePos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJumpAttackRangePlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void Enemy::InitMusic(void)
{

	// �e����鉹
	musicCreateId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CREATE_MUSIC).handleId_;

	// �^�b�N����
	musicTackleId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TACKLE_MUSIC).handleId_;

	// ����
	musicFootStepsId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_FOOTSTEPS_MUSIC).handleId_;

	// �W�����v�A�^�b�N�̉�
	musicJumpAttackId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::JUMPATTACK_MUSIC).handleId_;

	// �n�����̉�
	musicEarthQuakeId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::EARTHQUAKE_MUSIC).handleId_;

	// �U���̃{�C�X�P
	musicAttackVoice1Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_ATTACK_VOICE_MUSIC1).handleId_;

	// �U���̃{�C�X�Q
	musicAttackVoice2Id_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_ATTACK_VOICE_MUSIC2).handleId_;

	// �U���̃{�C�X�̃t���O
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

	int number = GetRand(1);

	if (number == 0)
	{
		PlaySoundMem(musicAttackVoice1Id_, DX_PLAYTYPE_BACK);
	}
	else if (number == 1)
	{
		PlaySoundMem(musicAttackVoice2Id_, DX_PLAYTYPE_BACK);
	}

}

void Enemy::Update(void)
{

	//// ��]�������Ă��Ȃ������珈�����Ȃ�
	//if (isRotation_)
	//{
	//	return;
	//}

	// �A�j���[�V��������
	Animation();

	if (noPlayTime_ > 0.0f || hp_ <= 0)
	{
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

		// �Đ����x�̐ݒ�
		SetFrequencySoundMem(30000, musicFootStepsId_);

		// ����
		FootStepsMusic();
		break;
	case Enemy::STATE::ATTACK:
		if (animationController_->GetAnimData("ATTACK").stepAnim >= ATTACK_COLLISION_START_TIME
			&& animationController_->GetAnimData("ATTACK").stepAnim <= ATTACK_COLLISION_END_TIME
			&& !hit_)
		{
			attack_ = true;
		}
		UpdateAttack();
		break;
	case Enemy::STATE::JUMP_ATTACK:
		if (animationController_->GetAnimData("JUMP_ATTACK").stepAnim >= JUMP_ATTACK_COLLISION_START_TIME
			&& animationController_->GetAnimData("JUMP_ATTACK").stepAnim <= JUMP_ATTACK_COLLISION_END_TIME
			&& !hit_)
		{
			attack_ = true;
		}
		UpdateJumpAttack();

		if (animationController_->GetAnimData("JUMP_ATTACK").stepAnim >= 40.0f && isEffectJumpAttack_)
		{
			// �W�����v�A�^�b�N�̃G�t�F�N�g
			JumpAttackPlayEffect();
			isEffectJumpAttack_ = false;
			PlaySoundMem(musicJumpAttackId_, DX_PLAYTYPE_BACK);
			PlaySoundMem(musicEarthQuakeId_, DX_PLAYTYPE_BACK);
		}

		// �G�t�F�N�g�̈ʒu
		JumpAttackSyncEffect();
		JumpAttackRangeSyncEffect();

		break;
	case Enemy::STATE::BEFORE_TACKLE:
		UpdateBeforeTackle();
		break;
	case Enemy::STATE::TACKLE:
		UpdateTackle();

		// �Đ����x�̐ݒ�
		SetFrequencySoundMem(120000, musicFootStepsId_);

		// ����
		FootStepsMusic();

		// �G�t�F�N�g�̈ʒu
		TackleSyncEffect();

		// �����̃J�E���^���Z�b�g
		if (musicFootStepsCnt_ >= 2.2f)
		{
			musicFootStepsCnt_ = 0.0f;
		}

		break;
	case Enemy::STATE::CREATE:
		UpdateCreate();

		// �G�t�F�N�g�̈ʒu
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
		// �Đ����x�̐ݒ�
		SetFrequencySoundMem(10000, musicFootStepsId_);

		// ����
		FootStepsMusic();
		break;
	case Enemy::STATE::TURN_RIGHT:
		// �Đ����x�̐ݒ�
		SetFrequencySoundMem(10000, musicFootStepsId_);

		// ����
		FootStepsMusic();
		break;
	}

	// ��]����
	Rotation();

	// �Փ˔���p
	Collision();

	transform_.pos.y = 0.0f;

	transform_.Update();

	walkCnt_ += SceneManager::GetInstance().GetDeltaTime();

	// �A�j���[�V��������
	animationController_->Update();

	for (auto v : shots_)
	{
		v->Update();
	}

}

void Enemy::Draw(void)
{

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);

	// �f�o�b�O�`��
	DrawDebug();

	// HP�o�[
	int hpLength = HP_LENGTH;
	int H;
	int hpGauge;
	H = hp_ * (512.0f / hpMax_) - 100;
	int R = min(max((384 - H), 0), 0xff);
	int G = min(max((H + 64), 0), 0xff);
	int B = max((H - 384), 0);
	hpGauge = hpLength * hp_ / hpMax_;

	int x = Application::SCREEN_SIZE_X / 2;

	// HP�̕`��
	if (hp_ >= 0)
	{
		DrawBox(x - 390, Application::SCREEN_SIZE_Y - 100, x - 390 + hpGauge, Application::SCREEN_SIZE_Y - 80, GetColor(R, G, B), true);
	}

	// HP�o�[�̕`��
	DrawExtendGraph(x - 400, Application::SCREEN_SIZE_Y - 100, x + 400, Application::SCREEN_SIZE_Y - 80, imgHPBar_, true);

	for (auto v : shots_)
	{
		v->Draw();
	}

	// �G�̖��O�̕\��
	std::string msg = "���E�̉��n�f�X";
	SetFontSize(30);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(x - 400, Application::SCREEN_SIZE_Y - 150, 0xffffff, msg.c_str());

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

	Camera* camera = SceneManager::GetInstance().GetCamera();

	if (camera->GetMode() == Camera::MODE::LOCKON)
	{
		if (lockOnCursorTime_ <= 0.0f)
		{
			lockOnCursorCnt_ += 1;
		}

		lockOnCursorTime_ -= SceneManager::GetInstance().GetDeltaTime();

		// ���b�N�I���J�[�\���̃r���{�[�h
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

	DeleteGraph(imgHPBar_);

	// ���f���̊J��
	MV1DeleteModel(transform_.modelId);
	MV1DeleteModel(idleAnim_);
	MV1DeleteModel(walkAnim_);
	MV1DeleteModel(tackleAnim_);
	MV1DeleteModel(attackAnim_);
	MV1DeleteModel(jumpAttackAnim_);
	MV1DeleteModel(createAnim_);
	MV1DeleteModel(shotAnim_);
	MV1DeleteModel(hitAnim_);
	MV1DeleteModel(deathAnim_);
	MV1DeleteModel(turnLeftAnim_);
	MV1DeleteModel(turnRightAnim_);


	for (auto& s : shots_)
	{
		s->Release();
		delete s;
	}
	shots_.clear();


	DeleteEffekseerEffect(effectCreateResId_);
	DeleteEffekseerEffect(effectCreatePlayId_);
	DeleteEffekseerEffect(effectTackleResId_);
	DeleteEffekseerEffect(effectTacklePlayId_);
	DeleteEffekseerEffect(effectTackleRangeResId_);
	DeleteEffekseerEffect(effectTackleRangePlayId_);
	DeleteEffekseerEffect(effectJumpAttackResId_);
	DeleteEffekseerEffect(effectJumpAttackPlayId_);
	DeleteEffekseerEffect(effectJumpAttackRangeResId_);
	DeleteEffekseerEffect(effectJumpAttackRangePlayId_);

	DeleteSoundMem(musicCreateId_);
	DeleteSoundMem(musicTackleId_);
	DeleteSoundMem(musicFootStepsId_);
	DeleteSoundMem(musicJumpAttackId_);
	DeleteSoundMem(musicEarthQuakeId_);
	DeleteSoundMem(musicAttackVoice1Id_);
	DeleteSoundMem(musicAttackVoice2Id_);

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

	// �x�N�g������
	VECTOR vec = { 0.0f,0.0f,0.0f };

	// ����
	float length = 0.0f;

	// �U���̑I��
	//attackNumber_ = GetRand(3);
	attackNumber_ = 0;

	// �U���������������ǂ���
	hit_ = false;

	// �ړ� --------------------------------------------------

	for (auto& animData : animationController_->GetAnimDatas())
	{
		if (state_ != STATE::ATTACK && state_ != STATE::JUMP_ATTACK || state_ != STATE::TACKLE && animData.second.stepAnim == 0.0f)
		{
			ChangeState(STATE::WALK);
		}
	}
	if (walkCnt_ <= FIRST_WALK_TIME)
	{
		return;
	}

	// �ʏ�U�� ----------------------------------------------

	// �v���C���[�̕��������߂�
	vec = VSub(followTransform_->pos, transform_.pos);
	length = AsoUtility::Magnitude(vec);

	// �G�ƃv���C���[�̋�������苗���ɂȂ�����U������
	if (attackNumber_ == 0 && length < ATTACK_RANGE)
	{
		ChangeState(STATE::ATTACK);
	}

	// �W�����v�U�� ------------------------------------------

	if (attackNumber_ == 1)
	{

		// �v���C���[���������W����
		attackPlayerPos_ = followTransform_->pos;

		// �v���C���[�̕��������߂�
		vec = VSub(attackPlayerPos_, transform_.pos);
		length = AsoUtility::Magnitude(vec);

		// ���K��
		pDirection_ = VNorm(vec);

		// �W�����v�U��
		if ((length > JUMP_ATTACK_RANGE_MIN && length < JUMP_ATTACK_RANGE_MAX) || length < ATTACK_RANGE)
		{
			ChangeState(STATE::JUMP_ATTACK);
		}
	}

	// �^�b�N���U�� ------------------------------------------

	if (attackNumber_ == 2)
	{

		// �v���C���[���������W����
		attackPlayerPos_ = followTransform_->pos;

		// �v���C���[�̕��������߂�
		vec = VSub(attackPlayerPos_, transform_.pos);
		length = AsoUtility::Magnitude(vec);

		beforeTackleCnt_ = BEOFRE_TACKLE_TIME;

		// ���K��
		pDirection_ = VNorm(vec);
		ChangeState(STATE::BEFORE_TACKLE);
		//SetIdleAnimation();

	}

	// �V���b�g�U��-------------------------------------------

	if (attackNumber_ == 3)
	{
		ChangeState(STATE::CREATE);
	}

}

void Enemy::Rotation(void)
{

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);

	vec = { -vec.x,vec.y,-vec.z };

	// ���K��
	VECTOR Vdirection = VNorm(vec);

	// �������p�x�ɕϊ�����(XZ���� Y��)
	float angle = atan2f(Vdirection.x, Vdirection.z);

	// ��]
	if (state_ != STATE::IDLE && state_ != STATE::ATTACK && state_ != STATE::JUMP_ATTACK
		&& state_ != STATE::TACKLE && state_ != STATE::BEFORE_TACKLE)
	{
		LazyRotation(angle);
	}

}

void Enemy::AfterRotation(void)
{

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);

	vec = { -vec.x,-vec.y,-vec.z };

	// ���K��
	VECTOR Vdirection = VNorm(vec);

	// �������p�x�ɕϊ�����(XZ���� Y��)
	float angle = atan2f(Vdirection.x, Vdirection.z);

	auto goal = Quaternion::Euler(0.0f, angle, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.02f);

	// ���W�A������f�O���[
	float goalDeg = AsoUtility::Rad2DegF(angle);
	goalDeg = AsoUtility::DegIn360(goalDeg);

}

void Enemy::UpdateIdle(void)
{
}

void Enemy::UpdateWalk(void)
{

	// �v���C���[�̕��������߂�
	auto vec = VSub(followTransform_->pos, transform_.pos);

	// ���K��
	pDirection_ = VNorm(vec);

	// �ړ���
	movePow_ = VScale(pDirection_, WALK_SPEED);

	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	// �ړ�����
	transform_.pos = movedPos_;

}

void Enemy::UpdateAttack(void)
{

	attack_ = false;

	// �U�����肪����A�j���[�V�����̕b��
	if (animationController_->GetAnimData("ATTACK").stepAnim >= ATTACK_COLLISION_START_TIME &&
		animationController_->GetAnimData("ATTACK").stepAnim <= ATTACK_COLLISION_END_TIME && !hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateJumpAttack(void)
{

	// �v���C���[�Ƃ̋��������߂�
	VECTOR vec = VSub(attackPlayerPos_, transform_.pos);
	float length = AsoUtility::Magnitude(vec);
	attack_ = false;

	// �ړ���
	movePow_ = VScale(pDirection_, JUMP_ATTACK_SPEED);

	if (animationController_->GetAnimData("JUMP_ATTACK").stepAnim >= 20.0f && animationController_->GetAnimData("JUMP_ATTACK").stepAnim <= 80.0f)
	{
		// �v���C���[�Ƃ̋�����10.0f�����ɂȂ�܂ňړ�
		if (animationController_->GetAnimData("JUMP_ATTACK").stepAnim <= JUMP_ATTACK_END_TIME)
		{
			if (length >= JUMP_ATTACK_RANGE_MIN)
			{

				// ���ݍ��W���N�_�Ɉړ�����W�����߂�
				movedPos_ = VAdd(transform_.pos, movePow_);

				// �ړ�����
				transform_.pos = movedPos_;

			}
		}
	}

	// �U�����肪����A�j���[�V�����̕b��
	if (animationController_->GetAnimData("JUMP_ATTACK").stepAnim >= JUMP_ATTACK_COLLISION_START_TIME &&
		animationController_->GetAnimData("JUMP_ATTACK").stepAnim <= JUMP_ATTACK_COLLISION_END_TIME && !hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateBeforeTackle(void)
{
	// �^�b�N������O�ɍU���͈͂�\������
	beforeTackleCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	if (beforeTackleCnt_ <= 0.0f)
	{
		ChangeState(STATE::TACKLE);
	}
}

void Enemy::UpdateTackle(void)
{

	// �^�b�N�����鎞�Ԃ��v�Z
	tackleCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	// �^�b�N����������Ԃ͍��W�𓮂���
	if (tackleCnt_ > 0.0f && beforeTackleCnt_ <= 0.0f)
	{
		// �ړ���
		movePow_ = VScale(pDirection_, TACKLE_SPEED);

		// ���ݍ��W���N�_�Ɉړ�����W�����߂�
		movedPos_ = VAdd(transform_.pos, movePow_);

		// �ړ�����
		transform_.pos = movedPos_;
	}

	// �U���t���O
	if (!hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateCreate(void)
{

	// �e�̐���
	CreateShot();

}

void Enemy::UpdateShot(void)
{

	// �e�̔��ˏ���
	ProcessShot();

	// �U���t���O
	if (!hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateHit(void)
{

	// �v���C���[�Ƃ̋��������߂�
	VECTOR vec = VSub(attackPlayerPos_, transform_.pos);
	pDirection_ = VNorm(vec);

	if (animationController_->GetAnimData("HIT").stepAnim <= HIT_END_TIME)
	{
		transform_.pos = VAdd(transform_.pos, VScale(pDirection_, -HIT_SPEED));
	}

}

void Enemy::Collision(void)
{

	// �G���g�̓����蔻��
	EnemyBodyCollision();

	// �G�̕���̓����蔻��
	WeponCollision();

	// �X�e�[�W�Ƃ̏Փ˔���
	CollisionStage();

	transform_.Update();

}

void Enemy::CollisionStage(void)
{


	auto vec = VSub({ 0.0f,-300.0f, 500.0f }, transform_.pos);

	float length = AsoUtility::Magnitude(vec);

	auto dir = VNorm(vec);

	if (length >= 3500.0f)
	{
		// �@���̕����ɂ�����Ƃ����ړ�������
		movedPos_ = VAdd(movedPos_, VScale(dir, 100.0f));

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
				movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));

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

void Enemy::EnemyBodyCollision(void)
{

	// �G�̒��S�ɃJ�v�Z����Ǐ]������
	// �J�v�Z�����A�^�b�`����t���[���̃��[�J�������[���h�ϊ��s����擾����
	MATRIX EnemyFrameMatrix = MV1GetFrameLocalWorldMatrix(transform_.modelId, enemyAttachFrameNum_);

	// �s�񂩂烉�W�A��
	auto rot = MGetRotElem(EnemyFrameMatrix);
	auto pos = MGetTranslateElem(EnemyFrameMatrix);

	// �s�񂩂�N�H�[�^�j�I��
	Quaternion qua = Quaternion::GetRotation(rot);

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = qua;

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR cPosDOWN = followRot.PosAxis(LOCAL_BODY_C_DOWN_POS);
	VECTOR cPosUP = followRot.PosAxis(LOCAL_BODY_C_UP_POS);

	// �G�̈ʒu�̍X�V
	cBodyPosDown_ = VAdd(pos, cPosDOWN);
	cBodyPosUp_ = VAdd(pos, cPosUP);

}

void Enemy::WeponCollision(void)
{

	// �G�l�~�[����ɃJ�v�Z����Ǐ]������
	// �J�v�Z�����A�^�b�`����t���[���̃��[�J�������[���h�ϊ��s����擾����
	MATRIX WeponFrameMatrix = MV1GetFrameLocalWorldMatrix(transform_.modelId, weponAttachFrameNum_);

	// �s�񂩂烉�W�A��
	auto rot = MGetRotElem(WeponFrameMatrix);
	auto pos = MGetTranslateElem(WeponFrameMatrix);

	// �s�񂩂�N�H�[�^�j�I��
	Quaternion qua = Quaternion::GetRotation(rot);

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = qua;

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR cPosDOWN = followRot.PosAxis(LOCAL_WEPON_C_DOWN_POS);
	VECTOR cPosUP = followRot.PosAxis(LOCAL_WEPON_C_UP_POS);

	// �G�̈ʒu�̍X�V
	//cWeponPosDown_ = VAdd(transform_.pos, cPosDOWN);
	//cWeponPosUp_ = VAdd(transform_.pos, cPosUP);
	cWeponPosDown_ = VAdd(pos, cPosDOWN);
	cWeponPosUp_ = VAdd(pos, cPosUP);

}

void Enemy::ChangeState(STATE state)
{

	if (state_ == state && state != STATE::THINK)
	{
		return;
	}

	state_ = state;

	stateHiss_.emplace_back(ANIM_DATA_KEY[(int)state]);

	preKey_ = key_;

	key_ = ANIM_DATA_KEY[(int)state];

	animationController_->ChangeAnimation(key_);

	if (state_ == STATE::THINK)
	{

		// ��]�̃t���O��߂�
		isRotation_ = false;

		// ���ꂩ��̍s�����l����
		Think();

	}

	//// ��ԑJ�ڎ��̏���������
	//switch (state_)
	//{
	//case Enemy::STATE::THINK:

	//	break;
	//case Enemy::STATE::IDLE:
	//	SetIdleAnimation();

	//	// �����~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;

	//	isEffectJumpAttack_ = true;
	//	break;
	//case Enemy::STATE::WALK:
	//	SetWalkAnimation();
	//	break;
	//case Enemy::STATE::ATTACK:
	//	SetAttackAnimation();

	//	// �����~�߂�
	//	StopSoundMem(musicFootStepsId_);

	//	// ���̍Đ�
	//	AttackMusic();

	//	break;
	//case Enemy::STATE::JUMP_ATTACK:
	//	SetJumpAttackAnimation();

	//	// �G�t�F�N�g�̍Đ�
	//	JumpAttackRangePlayEffect();

	//	// �����~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;

	//	// ���̍Đ�
	//	AttackMusic();

	//	break;
	//case Enemy::STATE::BEFORE_TACKLE:
	//	// �G�t�F�N�g�Đ�
	//	TackleRangePlayEffect();
	//	break;
	//case Enemy::STATE::TACKLE:

	//	SetTackleAnimation();
	//	tackleCnt_ = 4.0f;

	//	// �G�t�F�N�g�̍Đ�
	//	TacklePlayEffect();

	//	// ���̍Đ�
	//	PlaySoundMem(musicTackleId_, DX_PLAYTYPE_BACK);

	//	// ���̍Đ�
	//	AttackMusic();
	//	break;
	//case Enemy::STATE::CREATE:
	//	SetCreateAnimation();

	//	// �����~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;

	//	// �G�t�F�N�g�̍Đ�
	//	CreatePlayEffect();

	//	// ���̍Đ�
	//	PlaySoundMem(musicCreateId_, DX_PLAYTYPE_BACK);

	//	// ���̍Đ�
	//	AttackMusic();

	//	break;
	//case Enemy::STATE::SHOT:
	//	delayShot_ = TIME_DELAY_SHOT;
	//	SetShotAnimation();

	//	// �����~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	StopSoundMem(musicCreateId_);

	//	// ���̍Đ�
	//	AttackMusic();

	//	break;
	//case Enemy::STATE::HIT:
	//	attackPlayerPos_ = followTransform_->pos;
	//	SetHitAnimation();

	//	// �����~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	break;
	//case Enemy::STATE::DEATH:
	//	SetDeathAnimation();

	//	// �����~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	break;
	//case Enemy::STATE::TURN_LEFT:
	//	SetTurnLeftAnimation();
	//	break;
	//case Enemy::STATE::TURN_RIGHT:
	//	SetTurnRightAnimation();
	//	break;
	//}

}

void Enemy::LazyRotation(float goalRot)
{

	// �ړI�̊p�x�܂ŉ�]������
	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.05f);

	// ���W�A������f�O���[
	float goalDeg = AsoUtility::Rad2DegF(goalRot);
	goalDeg = AsoUtility::DegIn360(goalDeg);

	auto rad = transform_.quaRot.ToEuler();
	auto deg = AsoUtility::Rad2DegF(rad.y);

	deg = AsoUtility::DegIn360(deg);

	// �ړI�̊p�x�Ǝ����̊p�x�̍��𑪂�
	float sub = goalDeg - deg;

	sub = abs(sub);

	// ����10�x������������true�ɂ���
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

	//// �G�l�~�[���g�̏Փ˔���̃J�v�Z���̕`��
	//DrawCapsule3D(cBodyPosDown_, cBodyPosUp_, COLLISION_BODY_RADIUS, 10, 0xff0000, 0xff0000, false);

	//// �G�l�~�[����̏Փ˔���̃J�v�Z���̕`��
	//DrawCapsule3D(cWeponPosDown_, cWeponPosUp_, COLLISION_WEPON_RADIUS, 10, 0xff0000, 0xff0000, false);

	//// �W�����v�A�^�b�N�̋��̂̕`��
	//if (state_ == STATE::JUMP_ATTACK)
	//{
	//	DrawCone3D(VAdd(attackPlayerPos_, { 0.0f,0.0f,0.0f }), VAdd(attackPlayerPos_, { 0.0f,10.0f,0.0f }), 1000, 10, 0x0000ff, 0x0000ff, true);
	//}

	// �A�^�b�`����Ă��鐔
	DrawFormatString(900, 180, 0xff0000, "list:%d", animationController_->GetAttachNum());

	int y = 200;
	const auto& animDatas = animationController_->GetAnimDatas();
	for (const auto& anim : animDatas)
	{
		//if (anim.second.blendRate > 0.0f)
		{
			DrawFormatString(900, y, 0xff0000, "type:%s, step:%.2f, rate:%.2f",
				anim.second.state.c_str(),
				anim.second.stepAnim,
				anim.second.blendRate
			);
			y += 20;
		}
	}

	y = 100;
	for (auto& s : stateHiss_)
	{
		DrawFormatString(750, y, 0xff0000, "state:%s", s.c_str());
		y += 20;
	}

}

void Enemy::SetParam(void)
{

	// HP�̍ő�l
	hpMax_ = HP_MAX;

	// HP
	hp_ = hpMax_;

}

void Enemy::Animation(void)
{

	// �s����ɓ����Ȃ����Ԃ����
	if (isAction_)
	{
		noPlayTime_ -= SceneManager::GetInstance().GetDeltaTime();
	}

	// �V���b�g�̑S�̂̎��Ԃ̃J�E���^
	if (state_ == STATE::SHOT)
	{
		shotCnt_ += SceneManager::GetInstance().GetDeltaTime();
	}

	if (state_ != STATE::SHOT || (shotNum_ == 0 && shotCnt_ >= SHOT_ATTACK_TIME))
	{
		isShot_ = false;
		if (animationController_->GetAnimData(key_).stepAnim >= animationController_->GetAnimData(key_).animTotalTime - 1)
		{
			int i = 0;
			if (state_ != STATE::TACKLE || tackleCnt_ < 0.0f)
			{

				// �ҋ@��Ԃɂ���
				ChangeState(STATE::IDLE);

				// �A�j���[�V�������I�������true�ɂ���
				isAction_ = true;

				//// �N�[���^�C�������Z�b�g����
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

	if (noPlayTime_ >= 0.0f)
	{
		return;
	}

	if (state_ == STATE::SHOT && animationController_->GetAnimData("SHOT").stepAnim >= SHOT_END_TIME)
	{
		isShot_ = false;
	}

	float goalDeg = 0.0f;
	// �s����v���C���[�����Ɋp�x��ς���
	if ((state_ == STATE::IDLE || state_ == STATE::TURN_LEFT || state_ == STATE::TURN_RIGHT) && isAction_)
	{
		// �������Ȃ��悤�ɃA�j���[�V�����̍Đ����Ԃ��I�������false�ɂ���
		isAction_ = false;

		// �v���C���[�̕��������߂�
		VECTOR vec = VSub(followTransform_->pos, transform_.pos);

		vec = { -vec.x,vec.y,-vec.z };

		// ���K��
		VECTOR Vdirection = VNorm(vec);

		// �������p�x�ɕϊ�����(XZ���� Y��)
		float angle = atan2f(Vdirection.x, Vdirection.z);

		auto goal = Quaternion::Euler(0.0f, angle, 0.0f);
		transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.02f);

		// �N�H�[�^�j�I�����烉�W�A��
		VECTOR rad = transform_.quaRot.ToEuler();

		// ���W�A������f�O���[
		float deg = AsoUtility::Rad2DegF(rad.y);
		deg = AsoUtility::DegIn360(deg);

		// ���W�A������f�O���[
		float goalDeg = AsoUtility::Rad2DegF(angle);
		goalDeg = AsoUtility::DegIn360(goalDeg);

		// �ړI�̊p�x�ɋ߂������ɐ���
		if (goalDeg <= deg)
		{
			ChangeState(STATE::TURN_LEFT);
		}
		else if (goalDeg >= deg)
		{
			ChangeState(STATE::TURN_RIGHT);
		}
	}

	// �s����I��
	if (isRotation_)
	{
		ChangeState(STATE::THINK);
		isNoPlay_ = true;
	}

	// �A�j���[�V�����̌Œ�
	AnimationFrame();

}

void Enemy::AnimationFrame(void)
{

	// �Ώۃt���[���̃��[�J���s��������l�Ƀ��Z�b�g����
	MV1ResetFrameUserLocalMatrix(transform_.modelId, enemyPosFrameNum_);

	// �W�����v�U�����ɍ��W���Œ肷��
	if (state_ == STATE::JUMP_ATTACK || state_ == STATE::HIT)
	{

		// �Ώۃt���[���̃��[�J���s��(�傫���A��]�A�ʒu)���擾����
		auto mat = MV1GetFrameLocalMatrix(transform_.modelId, enemyPosFrameNum_);

		auto scl = MGetSize(mat); // �s�񂩂�傫�������o��
		auto rot = MGetRotElem(mat); // �s�񂩂��]�����o��
		pos_ = MGetTranslateElem(mat); // �s�񂩂�ړ��l�����o��

		// �傫���A��]�A�ʒu�����[�J���s��ɖ߂�
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // �傫��
		mix = MMult(mix, rot); // ��]

		// �����Ń��[�J�����W���s��ɁA���̂܂ܖ߂����A
		// �����������[�J�����W��ݒ肷��
		mix = MMult(mix, MGetTranslate({ 0.0f, pos_.y, 0.0f }));

		// ���������s���Ώۃt���[���ɃZ�b�g�������āA
		// �A�j���[�V�����̈ړ��l�𖳌���
		MV1SetFrameUserLocalMatrix(transform_.modelId, enemyPosFrameNum_, mix);

	}

}

void Enemy::ProcessShot(void)
{

	auto& ins = InputManager::GetInstance();

	delayShot_ -= SceneManager::GetInstance().GetDeltaTime();

	// �e�����Ԃ����炵�Ĕ�΂�
	if (animationController_->GetAnimData("SHOT").stepAnim >= 35.0f && !isShot_)
	{
		Shot();
		isShot_ = true;
	}

}

void Enemy::Shot(void)
{

	if (shotNum_ < 0) return;
	// �w���ɂ���e���猂�e(�ҋ@��Ԃ̒e)�I��Ŕ���
	for (auto& shot : shots_)
	{
		if (shot->IsIdle())
		{

			// �e�̕���
			auto vec = VSub(VAdd(followTransform_->pos, { 0.0f,100.0f,0.0f }), shot->GetPos());
			auto dir = VNorm(vec);

			// ���ˏ���
			shot->Shot(dir);
			shotNum_--;

			AttackMusic();
			break;

		}
	}

}

void Enemy::CreateShot(void)
{

	// 8�̒e��G�̔w��ɍ��(8�񃋁[�v)
	if (shotNum_ < 10 && delayCreate_ <= 0.0f)
	{

		// ���p�\�Ȃ��̂�T��
		ShotEnemy* shot = GetAvailableShot();

		// �f�O���[�����W�A���ɕϊ�
		shotCreateRad_ = shotCreateDeg_ * DX_PI_F / 180.0f;

		// 45�x���e�̈ʒu��ς���
		shotCreateDeg_ += (360 / 10);

		// �p�x�������(�x�N�g��)�����߂�
		auto qua = Quaternion::Euler({ 0.0f,0.0f,shotCreateRad_ });

		// ���΍��W
		auto rPos = qua.PosAxis(LOCAL_SHOT_POS);

		// �e�̍쐬
		shot->Create(rPos, &transform_);

		// �e�𐶐������瑝�₷
		shotNum_++;

		// ����������܂����Ԃ𑝂₷
		delayCreate_ = TIME_DELAY_SHOT_CREATE;

	}

	delayCreate_ -= SceneManager::GetInstance().GetDeltaTime();

	// �A�j���[�V�������I�������V���b�g�Ɉڍs����
	if (animationController_->GetAnimData("CREATE").speedAnim >= SHOT_CREATE_END_TIME - 1.0f)
	{
		ChangeState(STATE::SHOT);
	}

}

ShotEnemy* Enemy::GetAvailableShot(void)
{

	// �ė��p�T��
	for (auto& v : shots_)
	{
		if (!v->IsAlive())
		{
			return v;
		}
	}

	// �Ȃ������ꍇ�쐬
	ShotEnemy* newShot = new ShotEnemy();
	//newShot->Create({ transform_.pos.x,
	//	transform_.pos.y + 150 ,
	//	transform_.pos.z },
	//	transform_.GetForward());

	// �e�̊Ǘ��z��ɒǉ�
	shots_.push_back(newShot);
	return newShot;

}