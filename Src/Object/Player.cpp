#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Enemy.h"
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

	// ���߂Ȃ�������A�j���[�V����
	chargeWalkAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_CHARGE_WALK);
	
	// ����A�j���[�V����
	runAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_RUN);

	// �U���A�j���[�V����
	attackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_ATTACK);

	// ���ߍU���A�j���[�V����
	chargeAttackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_CHARGE_ATTACK);

	// �_���[�W�q�b�g�A�j���[�V����
	hitAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_HIT);

	// ����A�j���[�V����
	rollAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_ROLL);
	
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

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

	// �v���C���[�̃p�����[�^�[
	SetParam();

	// �ҋ@�A�j���[�V����
	SetIdleAnimation();

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

	moveDir_ = { 0.0f,0.0f,0.0f };

}

void Player::Update(void)
{

	// �K�E�Z�ɂȂ�����K�E�Z�ȊO�̏������~�߂�
	if (SceneManager::GetInstance().GetSceneID() == SceneManager::SCENE_ID::SPECIALMOVE)
	{
		// �K�E�Z�̍X�V
  		SpecialMoveUpdate();
		return;
	}

	// ��Ԃ̑J��
	switch (state_)
	{
	case Player::STATE::IDLE:
		break;
	case Player::STATE::WALK:
		break;
	case Player::STATE::CHARGE_WALK:
		break;
	case Player::STATE::RUN:
		break;
	case Player::STATE::ATTACK:
		if (stepAnim_ >= ATTACK_COLLISION_START_TIME1
			&& stepAnim_ <= ATTACK_COLLISION_END_TIME1
			&& !hit_)
		{
			attack_ = true;
		}
		if (stepAnim_ >= ATTACK_END_TIME1 - 1.0f)
		{
			attack1_ = false;
		}
		break;
	case Player::STATE::ATTACK2:
		if (stepAnim_ >= ATTACK_COLLISION_START_TIME2
			&& stepAnim_ <= ATTACK_COLLISION_END_TIME2
			&& !hit_)
		{
			attack_ = true;
		}
		if (stepAnim_ >= ATTACK_END_TIME2 - 1.0f)
		{
			attack2_ = false;
		}
		break;
	case Player::STATE::ATTACK3:
		if (stepAnim_ >= ATTACK_COLLISION_START_TIME3
			&& stepAnim_ <= ATTACK_COLLISION_END_TIME3
			&& !hit_)
		{
			attack_ = true;
		}
		break;
	case Player::STATE::CHARGE_ATTACK:
		if (stepAnim_ >= CHARGE_ATTACK_COLLISION_START_TIME
			&& stepAnim_ <= CHARGE_ATTACK_COLLISION_END_TIME
			&& !hit_)
		{
			attack_ = true;
		}
		break;
	case Player::STATE::HIT:
		break;
	case Player::STATE::ROLL:
		if (stepAnim_ >= 45.0f)
		{
			speed_ = 0.0f;
		}
		break;
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

	// HP�o�[
	DrawHPBar();

}

void Player::DrawHPBar(void)
{
	// HP�o�[
	int hpLength = HP_LENGTH;
	int H;
	int hpGauge;
	H = hp_ * (512.0f / hpMax_) - 100;
	int R = min(max((384 - H), 0), 0xff);
	int G = min(max((H + 64), 0), 0xff);
	int B = max((H - 384), 0);
	hpGauge = hpLength * hp_ / hpMax_;

	if (hp_ >= 0)
	{
		DrawBox(0, Application::SCREEN_SIZE_Y - 30, hpGauge, Application::SCREEN_SIZE_Y, GetColor(R, G, B), true);
	}
}

void Player::SpecialMoveUpdate(void)
{

	//�@�K�E�Z���̏�Ԃ̑J��
	switch (specialState_)
	{
	case Player::SPECIAL_STATE::IDLE:
		break;
	}

	// �ҋ@��Ԃɂ���
	if (specialCnt_ <= 0.0f)
	{
		SpecialChangeState(SPECIAL_STATE::IDLE);
	}

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

void Player::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
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
	//if (camera->GetMode() == Camera::MODE::FOLLOW)
	//{
	if (ins.IsNew(KEY_INPUT_W))
	{
		dir = VAdd(dir, { 0.0f, 0.0f, 1.0f });
	}
	//}
	//else if (camera->GetMode() == Camera::MODE::LOCKON)
	//{
	//	if (ins.IsNew(KEY_INPUT_W) && target2PlayerDis_ > enemyMinDis_)
	//	{
	//		dir = VAdd(dir, { 0.0f, 0.0f, 1.0f });
	//	}
	//}
	if (ins.IsNew(KEY_INPUT_A)) { dir = VAdd(dir, { -1.0f, 0.0f, 0.0f }); }
	if (ins.IsNew(KEY_INPUT_S)) { dir = VAdd(dir, { 0.0f, 0.0f, -1.0f }); }
	if (ins.IsNew(KEY_INPUT_D)) { dir = VAdd(dir, { 1.0f, 0.0f, 0.0f }); }

	// �ړ�
	if (state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL)
	{
		// ����
		if (ins.IsNew(KEY_INPUT_LSHIFT) && !AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::RUN);
			speed_ = MOVE_POW_RUN;
		}
		// ����
		else if (!AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::WALK);
			// �ړ���
			speed_ = MOVE_POW_WALK;
		}
		// �ҋ@���
		else if (AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::IDLE);
			speed_ = 0.0f;
		}
	}

	//���߂Ȃ������
	if (ins.IsClickMouseLeft() && state_ != STATE::HIT)
	{
		speed_ = MOVE_POW_CHRAGE_WALK;
	}

	// ���
	if (ins.IsTrgDown(KEY_INPUT_SPACE) && !AsoUtility::EqualsVZero(dir) &&
		state_ != STATE::HIT && state_ != STATE::ROLL)
	{

		ChangeState(STATE::ROLL);

		// �����𐳋K��
		dir = VNorm(dir);

		// Y���̍s��
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// ��]�s����g�p���āA�x�N�g������]������
		moveDir_ = VTransform(dir, mat);

		// �ړ���
		speed_ = MOVE_POW_RUN;

	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL)
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

	// �ړ���
	movePow_ = VScale(moveDir_, speed_);

	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

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

	//  �ړ�
	moveDiff_ = VSub(movedPos_, transform_.pos);
	transform_.pos = movedPos_;

}

void Player::KeyboardAttack(void)
{

	auto& insInput = InputManager::GetInstance();
	auto& insScene = SceneManager::GetInstance();

	// �U������
	// �{�^�����N���b�N���ꂽ���ǂ������m�F
	if (insInput.IsClickMouseLeft() && chargeCnt <= CHARGE_TIME && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::ATTACK && state_ != STATE::ATTACK2 && state_ != STATE::ATTACK3 && state_ != STATE::HIT)
	{
		chargeCnt += insScene.GetDeltaTime();
		ChangeState(STATE::CHARGE_WALK);
	}

	if (insInput.IsTrgUpMouseLeft() && chargeCnt <= CHARGE_TIME && state_ != STATE::HIT)
	{

		//�{�^���������ꂽ��A�j���[�V������؂�ւ���
		//�P�i�K��
		if (state_ == STATE::IDLE || state_ == STATE::RUN || state_ == STATE::WALK)
		{
			attack_ = true;
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

	// �P�i�K�ڂ��I�������J�ڂ���
	if (attack2_ && !attack1_ && state_ == STATE::ATTACK)
	{
		chargeCnt = 0.0f;
		ChangeState(STATE::ATTACK2);
	}

	// �Q�i�K�ڂ��I�������J�ڂ���
	if (attack3_ && !attack2_ && state_ == STATE::ATTACK2)
	{
		chargeCnt = 0.0f;
		ChangeState(STATE::ATTACK3);
	}

	// ���ߎa��
	if (chargeCnt >= CHARGE_TIME)
	{
		chargeAttack_ = true;
		chargeCnt = 0.0f;
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

	// �J�����̊p�x���擾
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	auto& ins = InputManager::GetInstance();

	// ����(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// �ړ���
	float movePow = MOVE_POW_WALK;

	// �Q�[���p�b�h�̔ԍ����擾
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// ���̃X�e�B�b�N�̏����擾����
	auto isTrgLStick = ins.IsPadLStickTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER);

	// �p�b�h�̕�����dir�ɒ���
	dir.x = pad.AKeyLX;
	dir.z = -pad.AKeyLZ;

	// �ړ�
	if (state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL)
	{

		// ����
		if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) && !AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::RUN);
			movePow = MOVE_POW_RUN;
		}
		// ����
		else if (!AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::WALK);
		}
		// �ҋ@���
		else if (AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::IDLE);
		}

	}

	//���߂Ȃ������
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)
		&& state_ != STATE::HIT)
	{
		movePow = MOVE_POW_CHRAGE_WALK;
	}

	// ���
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN)
		&& state_ != STATE::HIT && state_ != STATE::ROLL)
	{
		ChangeState(STATE::ROLL);
	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL)
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

void Player::GamePadAttack(void)
{

	auto& insInput = InputManager::GetInstance();
	auto& insScene = SceneManager::GetInstance();

	// �U������
	// �{�^�����N���b�N���ꂽ���ǂ������m�F
	if (insInput.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)
		&& chargeCnt <= CHARGE_TIME && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::HIT)
	{
		chargeCnt += insScene.GetDeltaTime();
		ChangeState(STATE::CHARGE_WALK);
	}

	if (insInput.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)
		&& chargeCnt <= CHARGE_TIME && state_ != STATE::HIT)
	{

		//�{�^���������ꂽ��A�j���[�V������؂�ւ���
		//�P�i�K��
		if (state_ == STATE::IDLE || state_ == STATE::RUN || state_ == STATE::WALK)
		{
			attack_ = true;
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

	// �P�i�K�ڂ��I�������J�ڂ���
	if (attack2_ && !attack1_ && state_ == STATE::ATTACK)
	{
		chargeCnt = 0.0f;
		ChangeState(STATE::ATTACK2);
	}

	// �Q�i�K�ڂ��I�������J�ڂ���
	if (attack3_ && !attack2_ && state_ == STATE::ATTACK2)
	{
		chargeCnt = 0.0f;
		ChangeState(STATE::ATTACK3);
	}

	// ���ߎa��
	if (chargeCnt >= CHARGE_TIME)
	{
		chargeAttack_ = true;
		chargeCnt = 0.0f;
		ChangeState(STATE::CHARGE_ATTACK);
	}

}

void Player::GamePadCamera(void)
{

	auto& ins = InputManager::GetInstance();

	// �v���C���[�������Ă�������ɃJ������������
	auto rad = transform_.quaRot.ToEuler();
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_BOTTON))
	{
		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		SceneManager::GetInstance().GetCamera()->SetLazyAngles(rad);
	}

}

void Player::GamePadLockOn(void)
{

	auto& ins = InputManager::GetInstance();

	// �L�[���������烍�b�N�I������
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		
	}

	// �L�[���������烍�b�N�I������������
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		
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
	case Player::STATE::CHARGE_WALK:
		SetChargeWalkAnimation();
		break;
	case Player::STATE::RUN:
		SetRunAnimation();
		break;
	case Player::STATE::ATTACK:
		hit_ = false;
		SetAttackAnimation();
		break;
	case Player::STATE::ATTACK2:
		hit_ = false;
		SetAttackAnimation2();
		break;
	case Player::STATE::ATTACK3:
		hit_ = false;
		SetAttackAnimation3();
		break;
	case Player::STATE::CHARGE_ATTACK:
		hit_ = false;
		SetChargeAttackAnimation();
		break;
	case Player::STATE::HIT:
		SetHitAnimation();
		break;
	case Player::STATE::ROLL:
		SetRollAnimation();
		break;
	}

}

void Player::SpecialChangeState(SPECIAL_STATE state)
{
	// ��Ԃ̍X�V
	specialState_ = state;

	// ��ԑJ�ڎ��̏���������
	switch (specialState_)
	{
	case Player::SPECIAL_STATE::IDLE:
		SetIdleAnimation();
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
	speedAnim_ = IDLE_ANIM_SPEED;

}

void Player::SetWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, walkAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = WALK_ANIM_SPEED;

}

void Player::SetChargeWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, chargeWalkAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = CHARGE_WALK_ANIM_SPEED;

}

void Player::SetRunAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, runAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = RUN_ANIM_SPEED;

}

void Player::SetAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = ATTACK_ANIM_SPEED;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = 0.0f;

}

void Player::SetAttackAnimation2(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = ATTACK_ANIM_SPEED;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = ATTACK_START_TIME2;

}

void Player::SetAttackAnimation3(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = ATTACK_ANIM_SPEED;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = ATTACK_START_TIME3;

}

void Player::SetChargeAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, chargeAttackAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = CHARGE_ATTACK_ANIM;

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
	speedAnim_ = HIT_ANIM_SPEED;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = 0.0f;

}

void Player::SetRollAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, rollAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = ROLL_ANIM_SPEED;

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
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, ROTATION_POW);

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

		if (state_ == STATE::ATTACK || state_ == STATE::ATTACK2
			|| state_ == STATE::ATTACK3 || state_ == STATE::CHARGE_ATTACK 
			|| state_ == STATE::HIT || state_ == STATE::ROLL)
		{
			stepAnim_ = 0.0f;
			attack1_ = false;
			attack2_ = false;
			attack3_ = false;
			chargeAttack_ = false;

			hit_ = false;
			ChangeState(STATE::IDLE);
			chargeCnt = 0.0f;
		}
	}

	// 2�i�K�ڂɐi�܂Ȃ��Ƃ��̓��Z�b�g
	if (state_ == STATE::ATTACK && stepAnim_ >= ATTACK_END_TIME1 && !attack2_)
	{
		stepAnim_ = 0.0f;
		hit_ = false;
		ChangeState(STATE::IDLE);
		chargeCnt = 0.0f;
	}

	// 3�i�K�ڂɐi�܂Ȃ��Ƃ��̓��Z�b�g
	if (state_ == STATE::ATTACK2 && stepAnim_ >= ATTACK_END_TIME2 && !attack3_)
	{
		stepAnim_ = 0.0f;
		hit_ = false;
		ChangeState(STATE::IDLE);
		chargeCnt = 0.0f;
	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

	// �A�j���[�V�����̌Œ�
	AnimationFrame();

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