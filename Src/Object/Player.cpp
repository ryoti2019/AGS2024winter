#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::InitAnimation(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_IDLE));

	// �ҋ@�A�j���[�V����
	idleAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_IDLE);

	// �����A�j���[�V����
	walkAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_WALK);

	// ����A�j���[�V����
	runAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_RUN);

	// �U���A�j���[�V����
	attackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_ATTACK);

	// �_���[�W�q�b�g�A�j���[�V����
	hitAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_HIT);

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

	// �A�j���[�V��������ԍ�
	animNo_ = 1;

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// �Đ����̃A�j���[�V��������
	stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 0.0f;

}

void Player::Init(void)
{

	// �J�v�Z�����A�^�b�`����t���[���̔ԍ�������
	playerAttachFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:Hips");

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

	// �v���C���[�̃p�����[�^�[
	SetParam();

	// �ҋ@�A�j���[�V����
	SetIdleAnimation();

	// �U���������������ǂ���
	hit_ = false;

}

void Player::Update(void)
{

	// �L�[�{�[�h�ł̑���
	if (!SceneManager::GetInstance().GetGamePad())
	{
		KeybordContoroller();
	}

	// �Q�[���p�b�h�ł̑���
	if (SceneManager::GetInstance().GetGamePad())
	{
		GamePadController();
	}

	// �A�j���[�V��������
	Animation();

	// �Փ˔���
	Collision();

	transform_.Update();

}

void Player::Draw(void)
{

	// ���f���̕`��
	UnitBase::Draw();

	// �f�o�b�O�`��
	DrawDebug();

}

void Player::Release(void)
{

	// ���f���̊J��
	MV1DeleteModel(transform_.modelId);

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

bool Player::GetHit(void)
{
	return hit_;
}

void Player::SetHit(bool hit)
{
	hit_ = hit;
}

void Player::Collision(void)
{

	// �G���g�̓����蔻��
	PlayerBodyCollision();

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

void Player::KeybordContoroller(void)
{

	// �J�����̊p�x���擾
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	auto& ins = InputManager::GetInstance();

	// �ړ���
	float movePow = 10.0f;

	// ����(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// WASD�Ńv���C���[�̈ʒu��ς���
	if (ins.IsNew(KEY_INPUT_W)) { dir = VAdd(dir, { 0.0f, 0.0f, 1.0f }); }
	if (ins.IsNew(KEY_INPUT_A)) { dir = VAdd(dir, { -1.0f, 0.0f, 0.0f }); }
	if (ins.IsNew(KEY_INPUT_S)) { dir = VAdd(dir, { 0.0f, 0.0f, -1.0f }); }
	if (ins.IsNew(KEY_INPUT_D)) { dir = VAdd(dir, { 1.0f, 0.0f, 0.0f }); }

	// �X�y�[�X�L�[�ő���
	if (state_ != STATE::ATTACK && state_ != STATE::HIT)
	{

		// �ҋ@���
		if (AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::IDLE);
		}
		// ����
		else if (ins.IsNew(KEY_INPUT_SPACE) && !AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::RUN);
			movePow = 20.0f;
		}
		// ����
		else if (!AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::WALK);
		}
	}

	// �U������
	if (ins.IsTrgDown(KEY_INPUT_J) && !hit_)
	{
		ChangeState(STATE::ATTACK);
		hit_ = true;
	}

	// �v���C���[�������Ă�������ɃJ������������
	auto rad = transform_.quaRot.ToEuler();
	if (ins.IsTrgDown(KEY_INPUT_Q))
	{
		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		SceneManager::GetInstance().GetCamera()->SetLazyAngles(rad);
	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK && state_ != STATE::HIT)
	{

		// �����𐳋K��
		dir = VNorm(dir);

		// Y���̍s��
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// ��]�s����g�p���āA�x�N�g������]������
		VECTOR moveDir = VTransform(dir, mat);

		// �����~�X�s�[�h�ňړ��ʂ�����āA���W�ɑ����Ĉړ�
		transform_.pos = VAdd(transform_.pos, VScale(moveDir, movePow));

		// �������p�x�ɕϊ�����(XZ���� Y��)
		float angle = atan2f(dir.x, dir.z);

		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		LazyRotation(cameraAngles.y + angle);

	}

}

void Player::GamePadController(void)
{

	// �J�����̊p�x���擾
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	auto& ins = InputManager::GetInstance();

	// �ړ���
	float movePow = 10.0f;

	// ����(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// �Q�[���p�b�h�̔ԍ����擾
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// ���̃X�e�B�b�N�̏����擾����
	auto isTrgLStick = ins.IsPadLStickTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER);

	// �p�b�h�̕�����dir�ɒ���
	dir.x = pad.AKeyLX;
	dir.z = -pad.AKeyLZ;

	// �U������
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) && !attack_)
	{
		ChangeState(STATE::ATTACK);
	}

	// B�{�^���ő���
	if (state_ != STATE::ATTACK && state_ != STATE::HIT)
	{

		if (AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::IDLE);
		}
		else if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) && !AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::RUN);
			movePow = 20.0f;
		}
		else if (!AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::WALK);
		}

	}

	// �v���C���[�������Ă�������ɃJ������������
	auto rad = transform_.quaRot.ToEuler();
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_BOTTON))
	{
		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		SceneManager::GetInstance().GetCamera()->SetLazyAngles(rad);
	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK)
	{

		// �����𐳋K��
		dir = VNorm(dir);

		// Y���̍s��
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// ��]�s����g�p���āA�x�N�g������]������
		VECTOR moveDir = VTransform(dir, mat);

		// �����~�X�s�[�h�ňړ��ʂ�����āA���W�ɑ����Ĉړ�
		transform_.pos = VAdd(transform_.pos, VScale(moveDir, movePow));

		// �������p�x�ɕϊ�����(XZ���� Y��)
		float angle = atan2f(pad.AKeyLX, -pad.AKeyLZ);

		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		LazyRotation(cameraAngles.y + angle);

	}

}

void Player::ChangeState(STATE state)
{

	// ��Ԃ̍X�V
	state_ = state;

	// ��ԑJ�ڎ��̏���������
	switch (state_)
	{
	case Player::STATE::IDLE:
		SetIdleAnimation();
		break;
	case Player::STATE::WALK:
		SetWalkAnimation();
		break;
	case Player::STATE::RUN:
		SetRunAnimation();
		break;
	case Player::STATE::ATTACK:
		attack_ = true;
		SetAttackAnimation();
		break;
	case Player::STATE::HIT:
		SetHitAnimation();
		break;
	}

}

void Player::SetIdleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, idleAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

}

void Player::SetWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, walkAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = 30.0f;

}

void Player::SetRunAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, runAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = 40.0f;

}

void Player::SetAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = 60.0f;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = 0.0f;

}

void Player::SetHitAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, hitAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = 30.0f;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = 0.0f;

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


	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.1f);

}

void Player::DrawDebug(void)
{

	// HP�o�[
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
		DrawBox(0, Application::SCREEN_SIZE_Y - 30, 500 + hpGauge, Application::SCREEN_SIZE_Y, GetColor(R, G, B), true);
	}

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
	hpMax_ = 100;

	// HP
	hp_ = hpMax_;

}

void Player::Animation(void)
{

	// �A�j���[�V�����Đ�
	// �o�ߎ��Ԃ̎擾
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// �A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		// ���[�v�Đ�
		stepAnim_ = 0.0f;

		if (state_ == STATE::ATTACK || state_ == STATE::HIT)
		{
			ChangeState(STATE::IDLE);
		}
	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}