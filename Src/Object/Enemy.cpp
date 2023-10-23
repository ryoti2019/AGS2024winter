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

	// �ҋ@�A�j���[�V����
	idleAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_IDLE);

	// �U���A�j���[�V����
	attackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_ATTACK);

	// �_�b�V���U���A�j���[�V����
	dashAttackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_DASH_ATTACK);

	float scale = 2.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(180), AsoUtility::AXIS_Y));
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	// �Đ�����A�j���[�V�����̔ԍ�
	animNo_ = 0;

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// �Đ����̃A�j���[�V��������
	stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 0.0f;

	// �_�b�V���A�^�b�N�̃t���O
	dashAttack_ = false;

}

void Enemy::Init(void)
{

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

	// �v���C���[�̃p�����[�^�[
	SetParam();

	// �ҋ@�A�j���[�V����
	SetIdleAnimation();

	// �J�v�Z�����A�^�b�`����t���[���̔ԍ�������
	enemyAttachFrameNum_ = MV1SearchFrame(followTransform_->modelId, "mixamorig:Spine2");

	// ������A�^�b�`����t���[���̔ԍ�������
	weponAttachFrameNum_ = MV1SearchFrame(followTransform_->modelId, "mixamorig:LeftHand");

	// �t���[���ԍ����t���[�����Ŏ擾����
	enemyPosFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:Hips");
	//EnemyPosFrameNum_ = MV1SearchFrame(transform_.modelId, "MutantMesh");

}

void Enemy::Draw(void)
{

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);

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

Enemy::STATE Enemy::GetState(void)
{
	return state_;
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
	if (veca < 500.0f && AsoUtility::EqualsVZero(dir) && state_ != STATE::DASH_ATTACK)
	{
		ChangeState(STATE::ATTACK);
	}

	// �ҋ@���
	if (!AsoUtility::EqualsVZero(dir))
	{
		ChangeState(STATE::IDLE);
	}

	// �G���v���C���[�������Ă�������ɉ�]����
	VECTOR Vdirection = VNorm(vec);

	// �������p�x�ɕϊ�����(XZ���� Y��)
	float angle = atan2f(Vdirection.x, Vdirection.z);

	// ��]
	LazyRotation(angle);

	// �ړ�
	if (state_ != STATE::ATTACK && state_ != STATE::DASH_ATTACK)
	{
		transform_.pos = VAdd(transform_.pos, VScale(Vdirection, 2.0f));
	}

	// �_�b�V���U��
	if (veca > 1000.0f && stepAnim_ == 0.0f)
	{
		dashAttack_ = true;
	}

	if (dashAttack_)
	{
		ChangeState(STATE::DASH_ATTACK);
		transform_.pos = VAdd(transform_.pos, VScale(Vdirection, 10.0f));
	}

	// �A�j���[�V�����̕ύX
	ChangeAnimation();

}

void Enemy::Collision(void)
{

	// �G���g�̓����蔻��
	EnemyBodyCollision();

	// �G�̕���̓����蔻��
	WeponCollision();

	transform_.Update();

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

}

void Enemy::SetDashAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, dashAttackAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

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
		attack_ = true;
		break;
	case Enemy::STATE::DASH_ATTACK:
		SetDashAttackAnimation();
		attack_ = true;
		break;
	}

	//// �U���̃t���O��true�ɒ���
	//if (Enemy::STATE::IDLE == state_ ||
	//	Enemy::STATE::RUN == state_ ||
	//	Enemy::STATE::WALK == state_)
	//{
	//	attack_ = true;
	//}

}

void Enemy::LazyRotation(float goalRot)
{

	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.01f);

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

	// �G�l�~�[���g�̏Փ˔���̃J�v�Z���̕`��
	DrawCapsule3D(cBodyPosDown_, cBodyPosUp_, COLLISION_BODY_RADIUS, 10, 0xff0000, 0xff0000, false);

	// �G�l�~�[����̏Փ˔���̃J�v�Z���̕`��
	DrawCapsule3D(cWeponPosDown_, cWeponPosUp_, COLLISION_WEPON_RADIUS, 10, 0xff0000, 0xff0000, false);

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

		if (state_ == STATE::ATTACK || state_ == STATE::DASH_ATTACK)
		{
			ChangeState(STATE::IDLE);
			SetIdleAnimation();
		}

	}

	if (state_ == STATE::DASH_ATTACK && stepAnim_ >= 40.0f)
	{
		dashAttack_ = false;
	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

	// �A�j���[�V�����̌Œ�
	AnimationFrame();

}

void Enemy::AnimationFrame(void)
{

	// �Ώۃt���[���̃��[�J���s��������l�Ƀ��Z�b�g����
	MV1ResetFrameUserLocalMatrix(transform_.modelId, enemyPosFrameNum_);

	if (state_ == STATE::DASH_ATTACK)
	{

		// �Ώۃt���[���̃��[�J���s��(�傫���A��]�A�ʒu)���擾����
		auto mat = MV1GetFrameLocalMatrix(transform_.modelId, enemyPosFrameNum_);

		auto scl = MGetSize(mat); // �s�񂩂�傫�������o��
		auto rot = MGetRotElem(mat); // �s�񂩂��]�����o��
		auto pos = MGetTranslateElem(mat); // �s�񂩂�ړ��l�����o��

		// �傫���A��]�A�ʒu�����[�J���s��ɖ߂�
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // �傫��
		mix = MMult(mix, rot); // ��]

		// �����Ń��[�J�����W���s��ɁA���̂܂ܖ߂����A
		// �����������[�J�����W��ݒ肷��
		mix = MMult(mix, MGetTranslate({ 0.0f, pos.y, 0.0f }));

		// ���������s���Ώۃt���[���ɃZ�b�g�������āA
		// �A�j���[�V�����̈ړ��l�𖳌���
		MV1SetFrameUserLocalMatrix(transform_.modelId, enemyPosFrameNum_, mix);
	}
}
