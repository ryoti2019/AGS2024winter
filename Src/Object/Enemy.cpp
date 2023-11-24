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

	// �����A�j���[�V����
	walkAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_WALK);

	// �^�b�N���A�j���[�V����
	tackleAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_TACKLE);

	// �U���A�j���[�V����
	attackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_ATTACK);

	// �_�b�V���U���A�j���[�V����
	jumpAttackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_DASH_ATTACK);

	// �V���b�g�A�j���[�V����
	shotAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_SHOT_ATTACK);

	// �U����H������Ƃ��̃A�j���[�V����
	hitAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_HIT);

	// transform�̏�����
	float scale = 2.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 1000.0f };
	transform_.quaRot = Quaternion();
	//Quaternion rotPow = Quaternion::Identity();
	//rotPow = Quaternion::Mult(
	//	rotPow,
	//	Quaternion::AngleAxis(AsoUtility::Deg2RadF(180), AsoUtility::AXIS_Y));
	//transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	// �Đ�����A�j���[�V�����̔ԍ�
	animNo_ = 0;

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// �Đ����̃A�j���[�V��������
	stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 0.0f;

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

	// �U�������ǂ���
	attack_ = false;

	// �_�b�V���A�^�b�N�̃t���O
	jumpAttack_ = false;

	// �^�b�N���A�^�b�N�̃t���O
	tackleAttack_ = false;

	// ��]�̏I���̃t���O
	rotationEnd_ = false;

	// �����������̃t���O
	hit_ = false;

	// �ŏ��̕����̃A�j���[�V�����̃J�E���^
	walkCnt_ = 0.0f;

	// �������
	ChangeState(STATE::THINK);

}

void Enemy::Update(void)
{

	// ��]�������Ă��Ȃ������珈�����Ȃ�
	if (rotationEnd_)
	{
		return;
	}

	switch (state_)
	{
	case Enemy::STATE::THINK:
		break;
	case Enemy::STATE::IDLE:
		UpdateIdle();
		break;
	case Enemy::STATE::WALK:
		UpdateWalk();
		break;
	case Enemy::STATE::ATTACK:
		UpdateAttack();
		break;
	case Enemy::STATE::JUMP_ATTACK:
		UpdateJumpAttack();
		break;
	case Enemy::STATE::TACKLE:
		UpdateTackle();
		break;
	case Enemy::STATE::SHOT:
		break;
	case Enemy::STATE::HIT:
		/*UpdateHit();*/
		break;
	}

	// �A�j���[�V��������
	Animation();

	// ��]����
	Rotation();

	// �Փ˔���p
	Collision();

	transform_.Update();

	walkCnt_ += SceneManager::GetInstance().GetDeltaTime();

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

void Enemy::Think(void)
{

	// �x�N�g������
	VECTOR vec = { 0.0f,0.0f,0.0f };

	// ����
	float length = 0.0f;

	// �U���̑I��
	attackNumber_ = GetRand(2);
	
	// �U���������������ǂ���
	hit_ = false;

	// �ړ� --------------------------------------------------

	// �ړ�
	if (state_ != STATE::ATTACK && state_ != STATE::JUMP_ATTACK || state_ != STATE::TACKLE && stepAnim_ == 0.0f)
	{
		ChangeState(STATE::WALK);
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
	if (length < ATTACK_RANGE)
	{
		ChangeState(STATE::ATTACK);
	}

	// �W�����v�U�� ------------------------------------------

	if (attackNumber_ == 0)
	{

		// �v���C���[���������W����
		attackPlayerPos_ = followTransform_->pos;

		// �v���C���[�̕��������߂�
		vec = VSub(attackPlayerPos_, transform_.pos);
		length = AsoUtility::Magnitude(vec);

		// ���K��
		pDirection_ = VNorm(vec);

		// �W�����v�U��
		if (length > JUMP_ATTACK_RANGE_MIN && length < JUMP_ATTACK_RANGE_MAX)
		{
			ChangeState(STATE::JUMP_ATTACK);
		}

	}

	// �^�b�N���U�� ------------------------------------------

	if (attackNumber_ == 1)
	{

		// �v���C���[���������W����
		attackPlayerPos_ = followTransform_->pos;

		// �v���C���[�̕��������߂�
		vec = VSub(attackPlayerPos_, transform_.pos);
		length = AsoUtility::Magnitude(vec);

		// ���K��
		pDirection_ = VNorm(vec);

		// �^�b�N���������鎞��
		tackleCnt_ = TACKLE_TIME;

		// �^�b�N���U��
		if (length > TACKLE_RANGE_MIN)
		{
			ChangeState(STATE::TACKLE);
		}

	}

}

void Enemy::Rotation(void)
{

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);

	vec = { -vec.x,-vec.y,-vec.z };

	// ���K��
	VECTOR Vdirection = VNorm(vec);

	// �������p�x�ɕϊ�����(XZ���� Y��)
	float angle = atan2f(Vdirection.x, Vdirection.z);

	// ��]
	if (state_ == STATE::IDLE || state_ == STATE::WALK)
	{
		LazyRotation(angle );
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

	// �N�H�[�^�j�I�����烉�W�A��
	VECTOR rad = transform_.quaRot.ToEuler();

	// ���W�A������f�O���[
	float deg = AsoUtility::Rad2DegF(rad.y);
	deg = AsoUtility::DegIn360(deg);

	// ���W�A������f�O���[
	float goalDeg = AsoUtility::Rad2DegF(angle);
	goalDeg = AsoUtility::DegIn360(goalDeg);

	// �ړI�̊p�x�Ǝ����̊p�x�̍��𑪂�
	float sub = goalDeg - deg;

	// ����10�x������������true�ɂ���
	if (sub <= GOAL_DEG && sub >= -GOAL_DEG)
	{
		rotationEnd_ = true;
	}

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

	// �ړ�����
	transform_.pos = VAdd(transform_.pos, VScale(pDirection_, WALK_SPEED));

}

void Enemy::UpdateAttack(void)
{

	// �U�����肪����A�j���[�V�����̕b��
	if (stepAnim_ >= ATTACK_COLLISION_START_TIME && stepAnim_ <= ATTACK_COLLISION_END_TIME && !hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateJumpAttack(void)
{

	// �v���C���[�Ƃ̋��������߂�
	VECTOR vec = VSub(attackPlayerPos_, transform_.pos);
	float length = AsoUtility::Magnitude(vec);

	// �v���C���[�Ƃ̋�����10.0f�����ɂȂ�܂ňړ�
	if (stepAnim_ <= JUMP_ATTACK_END_TIME)
	{
		if (length >= JUMP_ATTACK_RANGE_MIN)
		{
			transform_.pos = VAdd(transform_.pos, VScale(pDirection_, JUMP_ATTACK_SPEED));
		}
	}

	// �U�����肪����A�j���[�V�����̕b��
	if (stepAnim_ >= JUMP_ATTACK_COLLISION_START_TIME && stepAnim_ <= JUMP_ATTACK_COLLISION_END_TIME && !hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateTackle(void)
{

	// �^�b�N�����鎞�Ԃ��v�Z
	tackleCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	// �^�b�N����������Ԃ͍��W�𓮂���
	if (tackleCnt_ > 0.0f)
	{
		// �ړ�����
		transform_.pos = VAdd(transform_.pos, VScale(pDirection_, TACKLE_SPEED));
	}

	// �I�������IDLE�ɖ߂�
	if (tackleCnt_ < 0.0f)
	{
		ChangeState(STATE::IDLE);
	}
	
	// �U���t���O
	if (!hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateShot(void)
{
}

void Enemy::UpdateHit(void)
{

	// �v���C���[�Ƃ̋��������߂�
	VECTOR vec = VSub(attackPlayerPos_, transform_.pos);
	pDirection_ = VNorm(vec);

	if (stepAnim_ <= HIT_END_TIME)
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

	// ��ԑJ�ڎ��̏���������
	switch (state_)
	{
	case Enemy::STATE::THINK:
		// ��]�̃t���O��߂�
		rotationEnd_ = false;
		// ���ꂩ��̍s�����l����
		Think();
		break;
	case Enemy::STATE::IDLE:
		SetIdleAnimation();
		break;
	case Enemy::STATE::WALK:
		SetWalkAnimation();
		break;
	case Enemy::STATE::ATTACK:
		SetAttackAnimation();
		break;
	case Enemy::STATE::JUMP_ATTACK:
		SetJumpAttackAnimation();
		break;
	case Enemy::STATE::TACKLE:
		SetTackleAnimation();
		break;
	case Enemy::STATE::SHOT:
		SetHitAnimation();
		break;
	case Enemy::STATE::HIT:
		attackPlayerPos_ = followTransform_->pos;
		SetHitAnimation();
		break;
	}

}

void Enemy::SetIdleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, idleAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = IDLE_ANIM_SPEED;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = 0.0f;

}

void Enemy::SetWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, walkAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = WALK_ANIM_SPEED;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = 0.0f;

}

void Enemy::SetAttackAnimation(void)
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

void Enemy::SetJumpAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, jumpAttackAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = JUMP_ATTACK_ANIM_SPEED;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = 0.0f;

}

void Enemy::SetTackleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, tackleAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = TACKLE_ANIM_SPEED;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = 0.0f;

}

void Enemy::SetShotAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, shotAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = SHOT_ANIM_SPEED;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = 0.0f;

}

void Enemy::SetHitAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, hitAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = HIT_ANIM_SPEED;

	// �A�j���[�V�������Ԃ̏�����
	stepAnim_ = 10.0f;

}

void Enemy::LazyRotation(float goalRot)
{

	// �ړI�̊p�x�܂ŉ�]������
	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.02f);

}

void Enemy::DrawDebug(void)
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

	// HP�̕`��
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
	hpMax_ = HP_MAX;

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

		if (state_ != STATE::TACKLE)
		{
			// �ҋ@��Ԃɂ���
			ChangeState(STATE::IDLE);
		}

	}

	// �s����v���C���[�����Ɋp�x��ς���
	if (state_ == STATE::IDLE)
	{
		AfterRotation();
	}

	// �s����I��
	if (rotationEnd_)
	{
		ChangeState(STATE::THINK);
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

	// �W�����v�U�����ɍ��W���Œ肷��
	if (state_ == STATE::JUMP_ATTACK || state_ == STATE::HIT)
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
