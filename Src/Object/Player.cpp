#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

}

void Player::InitAnimation(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_WALK));
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 200.0f, 0.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	animNo_ = 1;

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Player::Update(void)
{

	// �ړ�����
	Move();

	// �A�j���[�V��������
	Animation();

	//// ���f���̈ړ��ݒ�
	//MV1SetPosition(transform_.modelId, transform_.pos);

	transform_.Update();
}

void Player::Draw(void)
{

	MV1DrawModel(transform_.modelId);

}

void Player::Release(void)
{

	MV1DeleteModel(transform_.modelId);

}

const Transform& Player::GetTransform(void) const
{
	return transform_;
}

void Player::Move(void)
{

	// �J�����̊p�x���擾
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	auto& ins = InputManager::GetInstance();
	float movePow = 10.0f;

	// ���̃V�t�g�L�[�ő���
	if (ins.IsNew(KEY_INPUT_SPACE))
	{
		ChangeState(STATE::RUN);
		movePow = 20.0f;
	}
	else if (!ins.IsNew(KEY_INPUT_SPACE))
	{
		ChangeState(STATE::WALK);
	}

	switch (state_)
	{
	case Player::STATE::WALK:
		SetWalkAnimation();
		break;
	case Player::STATE::RUN:
		SetRunAnimation();
		break;
	}

	// ����(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	// WASD�ŃJ�����̈ʒu��ς���
	if (ins.IsNew(KEY_INPUT_W)) { dir = { 0.0f, 0.0f, -1.0f }; }
	if (ins.IsNew(KEY_INPUT_A)) { dir = { 1.0f, 0.0f, 0.0f }; }
	if (ins.IsNew(KEY_INPUT_S)) { dir = { 0.0f, 0.0f, 1.0f }; }
	if (ins.IsNew(KEY_INPUT_D)) { dir = { -1.0f, 0.0f, 0.0f }; }

	
	float deg = 0.0f;
	if (ins.IsNew(KEY_INPUT_C)) { deg = -10.0f;}
	if (ins.IsNew(KEY_INPUT_V)) { deg = 10.0f;}



	if (!/*AsoUtility::EqualsVZero(rot)*/deg == 0.0f)
	{
		// ��]�ʂ̍쐬
		Quaternion rotPow = Quaternion::Identity();
		rotPow = Quaternion::Mult(
			rotPow,
			Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), AsoUtility::AXIS_Y));

		transform_.quaRot = Quaternion::Mult(transform_.quaRot,rotPow);
	}

	if (!AsoUtility::EqualsVZero(dir))
	{

		// XYZ�̉�]�s��
		// XZ���ʈړ��ɂ���ꍇ�́AXZ�̉�]���l�����Ȃ��悤�ɂ���
		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotX(angles_.x));
		mat = MMult(mat, MGetRotY(cameraAngles.y));
		//mat = MMult(mat, MGetRotZ(angles_.z));

		// ��]�s����g�p���āA�x�N�g������]������
		VECTOR moveDir = VTransform(dir, mat);

		// �����~�X�s�[�h�ňړ��ʂ�����āA���W�ɑ����Ĉړ�
		transform_.pos = VAdd(transform_.pos, VScale(moveDir, movePow));

		// �������p�x�ɕϊ�����(XZ���� Y��)
		float angle = atan2f(dir.x, dir.z);

		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		//transform_.rot.y = AsoUtility::Deg2RadF(cameraAngles.y + angle);

		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		//LazyRotation(cameraAngles.y + angle);

	}

}

void Player::ChangeState(STATE state)
{

	// ��Ԃ̍X�V
	state_ = state;

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
	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_WALK), animAttachNo_, stepAnim_);

}

void Player::SetWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
	//stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Player::SetRunAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
	//stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Player::LazyRotation(float goalRot)
{

	// ��]����
	float degNowAngleY = AsoUtility::Rad2DegF(transform_.rot.y);
	float degGoalAngleY = AsoUtility::Rad2DegF(goalRot);

	// 0�x�`360�x�ȓ��Ɋp�x�������߂�
	degGoalAngleY = static_cast<float>(AsoUtility::DegIn360(degGoalAngleY));

	// ��]�����Ȃ����̉�]�������擾����(���v���:1�A�����v���:-1)
	int aroundDir = AsoUtility::DirNearAroundDeg(degNowAngleY, degGoalAngleY);

	// ���B�������p�x�ɉ�]��������
	if (fabs(degGoalAngleY - degNowAngleY) >= 5)
	{
		transform_.rot.y += SPEED_ROT_RAD * static_cast<float>(aroundDir);
	}
	else
	{
		transform_.rot.y = goalRot;
	}

	// 0�x�`360�x�ȓ��Ɋp�x�������߂�
	transform_.rot.y = static_cast<float>(AsoUtility::RadIn2PI(transform_.rot.y));

}
