#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
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

	idleAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_IDLE);

	attackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_ATTACK);

	float scale = 2.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 200.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(180), AsoUtility::AXIS_Y));
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	animNo_ = 0;

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// �A�j���[�V���������Ԃ̎擾
	//animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 0.0f;

	//// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	//MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Enemy::Init(void)
{

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

	// �v���C���[�̃p�����[�^�[
	SetParam();

	// �ҋ@�A�j���[�V����
	SetIdleAnimation();

}

void Enemy::Draw(void)
{

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);

	// �Փ˔���̃J�v�Z���̕`��
	DrawCapsule3D(cPosDown_, cPosUp_, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

	// �f�o�b�O�`��
	DrawDebug();

}

void Enemy::Release(void)
{

	// ���f���̊J��
	MV1DeleteModel(transform_.modelId);

}

const Transform& Enemy::GetTransform(void) const
{
	return transform_;
}

VECTOR Enemy::GetCPosDown(void)
{
	return cPosDown_;
}

VECTOR Enemy::GetCPosUP(void)
{
	return cPosUp_;
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

void Enemy::Move(void)
{

	preState_ = state_;

	// �Փ˔���p
	Collision();

	// ����(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	VECTOR vec = VSub(followTransform_->pos,transform_.pos);
	auto veca = AsoUtility::Magnitude(vec);

	// �G�ƃv���C���[�̋�������苗���ɂȂ�����U������
	if (veca < 500.0f && AsoUtility::EqualsVZero(dir))
	{
		ChangeState(STATE::ATTACK);
	}

	// �ҋ@���
	if (!AsoUtility::EqualsVZero(dir))
	{
		ChangeState(STATE::IDLE);
	}

	// �A�j���[�V�����̕ύX
	ChangeAnimation();

}

void Enemy::Collision(void)
{

	// �G���瓖���蔻��̉��܂ł̑��΍��W
	VECTOR cPosDOWN = transform_.quaRot.PosAxis(LOCAL_C_DOWN_POS);
	// �G���瓖���蔻��̏�܂ł̑��΍��W
	VECTOR cPosUP = transform_.quaRot.PosAxis(LOCAL_C_UP_POS);

	// �G�̈ʒu�̍X�V
	cPosDown_ = VAdd(transform_.pos, cPosDOWN);
	cPosUp_ = VAdd(transform_.pos, cPosUP);

}

void Enemy::ChangeState(STATE state)
{

	// ��Ԃ̍X�V
	state_ = state;

}

void Enemy::SetIdleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, idleAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	//MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Enemy::SetWalkAnimation(void)
{
}

void Enemy::SetRunAnimation(void)
{
}

void Enemy::SetAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	//MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Enemy::ChangeAnimation(void)
{

	if (state_ == preState_) return;

	stepAnim_ = 0.0f;
	switch (state_)
	{
	case Enemy::STATE::IDLE:
		SetIdleAnimation();
		break;
	case Enemy::STATE::WALK:
		SetWalkAnimation();
		break;
	case Enemy::STATE::RUN:
		SetRunAnimation();
		break;
	case Enemy::STATE::ATTACK:
		SetAttackAnimation();
		break;
	}

}

void Enemy::LazyRotation(float goalRot)
{
}

void Enemy::DrawDebug(void)
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
		DrawBox(700, 0, 700 + hpGauge, 30, GetColor(R, G, B), true);
	}

}

void Enemy::SetParam(void)
{

	// HP�̍ő�l
	hpMax_ = 100;

	// HP
	hp_ = hpMax_;

}

void Enemy::Animation(void)
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

		if (state_ == STATE::ATTACK)
		{
			ChangeState(STATE::IDLE);
			SetIdleAnimation();
		}

	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}