#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"

class Player : public UnitBase
{

public:

	// �v���C���[�̃J�v�Z���̉��̑��΍��W
	static constexpr VECTOR LOCAL_BODY_C_DOWN_POS = { 0.0f,-60.0f,0.0f };

	// �v���C���[�̃J�v�Z���̏�̑��΍��W
	static constexpr VECTOR LOCAL_BODY_C_UP_POS = { 0.0f,50.0f,0.0f };

	// �v���C���[�̏Փ˔���̋��̔��a
	static constexpr float COLLISION_BODY_RADIUS = 40.0f;

	// �U����1�i�K�ڂ��I��鎞��
	static constexpr float ATTACK_END_TIME1 = 40.0f;

	// �U����1�i�K�ڂ̓����蔻�肪�n�܂鎞��
	static constexpr float ATTACK_COLLISION_START_TIME1 = 27.0f;

	// �U����1�i�K�ڂ̓����蔻�肪�I��鎞��
	static constexpr float ATTACK_COLLISION_END_TIME1 = 37.0f;

	// �U����2�i�K�ڂ��n�܂鎞��
	static constexpr float ATTACK_START_TIME2 = 40.0f;

	// �U����2�i�K�ڂ��I��鎞��
	static constexpr float ATTACK_END_TIME2 = 60.0f;

	// �U����2�i�K�ڂ̓����蔻�肪�n�܂鎞��
	static constexpr float ATTACK_COLLISION_START_TIME2 = 51.0f;

	// �U����2�i�K�ڂ̓����蔻�肪�I��鎞��
	static constexpr float ATTACK_COLLISION_END_TIME2 = 58.0f;

	// �U����3�i�K�ڂ��n�܂鎞��
	static constexpr float ATTACK_START_TIME3 = 60.0f;

	// �U����3�i�K�ڂ��I��鎞��
	static constexpr float ATTACK_END_TIME3 = 40.0f;

	// �U����3�i�K�ڂ̓����蔻�肪�n�܂鎞��
	static constexpr float ATTACK_COLLISION_START_TIME3 = 100.0f;

	// �U����3�i�K�ڂ̓����蔻�肪�I��鎞��
	static constexpr float ATTACK_COLLISION_END_TIME3 = 110.0f;

	// �U����3�i�K�ڂ̓����蔻�肪�n�܂鎞��
	static constexpr float CHARGE_ATTACK_COLLISION_START_TIME = 20.0f;

	// �U����3�i�K�ڂ̓����蔻�肪�I��鎞��
	static constexpr float CHARGE_ATTACK_COLLISION_END_TIME = 30.0f;

	// �ړ���(����)
	static constexpr float MOVE_POW_WALK = 10.0f;

	// �ړ���(���߂Ȃ������)
	static constexpr float MOVE_POW_CHRAGE_WALK = 5.0f;

	// �ړ���(����)
	static constexpr float MOVE_POW_RUN = 20.0f;

	// �ҋ@�A�j���[�V�����̍Đ����x
	static constexpr float IDLE_ANIM_SPEED = 20.0f;

	// �����A�j���[�V�����̍Đ����x
	static constexpr float WALK_ANIM_SPEED = 30.0f;

	// ���߂Ȃ�������A�j���[�V�����̍Đ����x
	static constexpr float CHARGE_WALK_ANIM_SPEED = 20.0f;

	// ����A�j���[�V�����̍Đ����x
	static constexpr float RUN_ANIM_SPEED = 40.0f;

	// �U���A�j���[�V�����̍Đ����x
	static constexpr float ATTACK_ANIM_SPEED = 60.0f;

	// ���ߍU���A�j���[�V�����̍Đ����x
	static constexpr float CHARGE_ATTACK_ANIM = 30.0f;

	// �U�����󂯂����̃A�j���[�V�����̍Đ����x
	static constexpr float HIT_ANIM_SPEED = 30.0f;

	// ����̃A�j���[�V�����̍Đ����x
	static constexpr float ROLL_ANIM_SPEED = 50.0f;

	// ��]���鋭��
	static constexpr float ROTATION_POW = 0.1f;

	// HP�o�[�̒���
	static constexpr int HP_LENGTH = 800;

	// HP�̍ő�l
	static constexpr int HP_MAX = 100;

	// ���ߎa��̃{�^���������b��
	static constexpr float CHARGE_TIME = 1.0f;

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 0.2f;

	// �v���C���[�̏��
	enum class STATE
	{
		IDLE,
		WALK,
		CHARGE_WALK,
		RUN,
		ATTACK,
		ATTACK2,
		ATTACK3,
		CHARGE_ATTACK,
		HIT,
		ROLL
	};

	enum class SPECIAL_STATE
	{
		IDLE,
	};

	// �R���X�g���N�^
	Player(void);

	// �f�X�g���N�^
	~Player(void);

	void Init(void) override;

	void Update(void) override;

	void Draw(void);

	void Release(void);

	// �v���C���[��Transform�̎擾
	const Transform& GetTransform(void) const;

	// �v���C���[�̏�Ԃ��擾
	Player::STATE GetState(void);

	// �v���C���[�̏�Ԃ�ݒ�
	void SetState(Player::STATE state);

	// �Փ˔���̉��̍��W�̎擾
	VECTOR GetCPosDown(void);

	// �Փ˔���̏�̍��W�̎擾
	VECTOR GetCPosUP(void);

	// �U���t���O�̎擾
	bool GetAttack(void);

	// �U���t���O�̐ݒ�
	void SetAttack(bool attack);

	// �v���C���[��HP�擾
	int GetHP(void);

	// �v���C���[��HP�̐ݒ�
	void SetHP(int hp);

	// �U�����������������擾
	bool GetHit(void);

	// �U����������������ݒ�
	void SetHit(bool hit);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

protected:

	// �v���C���[�̏��
	STATE state_;

	// �K�E�Z�̃v���C���[�̏��
	SPECIAL_STATE specialState_;

	// �Ǐ]�Ώ�
	const Transform* followTransform_;

	// �U���t���O
	bool attack_;

	// �U�����q�b�g������
	bool hit_;

	// �U���P�i�K��
	bool attack1_;

	// �U���Q�i�K��
	bool attack2_;

	// �U���R�i�K��
	bool attack3_;

	// ���ߎa��
	bool chargeAttack_;

	// ���b�N�I���̑O�̏��
	bool preLockOn_;
	
	// �ҋ@�A�j���[�V����
	int idleAnim_;

	// �����A�j���[�V����
	int walkAnim_;

	// ���߂Ȃ�������A�j���[�V����
	int chargeWalkAnim_;

	// ����A�j���[�V����
	int runAnim_;

	// �U���A�j���[�V����
	int attackAnim_;

	// ���ߍU���A�j���[�V����
	int chargeAttackAnim_;

	// �_���[�W�q�b�g�A�j���[�V����
	int hitAnim_;
	
	// ����A�j���[�V����
	int rollAnim_;

	// ���ߎa��̃J�E���^
	float chargeCnt;

	// �J�v�Z�����A�^�b�`����t���[���̔ԍ�
	int playerAttachFrameNum_;

	// �v���C���[�̏Փ˔���̍��W
	VECTOR cBodyPosUp_;
	VECTOR cBodyPosDown_;

	// �ړ���
	VECTOR moveDiff_;

	// �ړ�����W
	VECTOR movedPos_;

	// �X�s�[�h
	float speed_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ���
	VECTOR movePow_;

	// �ړI�̊p�x
	Quaternion goalQuaRot_;

	// ��]���鎞��
	float stepRotTime_;

	// �����_�ƈړ�����W�̋���
	float target2PlayerDis_;

	// �G�Ƃ̍ŏ����̋���
	float enemyMinDis_;

	// �K�E�Z�̃��[�r�[�J�E���^
	float specialCnt_;

	// �ړ�����
	void KeyboardMove(void);
	void GamePadMove(void);

	// �U������
	void KeyboardAttack(void);
	void GamePadAttack(void);

	// �v���C���[�����ɃJ�����������鏈��
	void KeyBoardCamera(void);
	void GamePadCamera(void);

	// �G�����b�N�I�����鏈��
	void KeyBoardLockOn(void);
	void GamePadLockOn(void);
	void LockOn(void);
	void SetGoalRotate(Quaternion rot);
	void Rotate(void);

	// �Փ˔���
	void Collision(void);

	// �G���g�̏Փ˔���
	void PlayerBodyCollision(void);

	// ��ԑJ��
	void ChangeState(STATE state);

	// �K�E�Z�̏�ԑJ��
	void SpecialChangeState(SPECIAL_STATE state);

	// �A�j���[�V����
	void Animation(void)override;

	// �ҋ@�A�j���[�V�����̐ݒ�
	void SetIdleAnimation(void);

	// �����A�j���[�V�����̐ݒ�
	void SetWalkAnimation(void);

	// ���߂Ȃ�������A�j���[�V�����̐ݒ�
	void SetChargeWalkAnimation(void);

	// ����A�j���[�V�����̐ݒ�
	void SetRunAnimation(void);

	// �U���P�i�K�ڂ̃A�j���[�V�����̐ݒ�
	void SetAttackAnimation(void);

	// �U���Q�i�K�ڂ̃A�j���[�V�����̐ݒ�
	void SetAttackAnimation2(void);

	// �U���R�i�K�ڂ̃A�j���[�V�����̐ݒ�
	void SetAttackAnimation3(void);

	// ���ߍU���̃A�j���[�V�����̐ݒ�
	void SetChargeAttackAnimation(void);

	// �_���[�W�q�b�g�A�j���[�V�����̐ݒ�
	void SetHitAnimation(void);

	// ����A�j���[�V�����̐ݒ�
	void SetRollAnimation(void);

	// �x����]
	void LazyRotation(float goalRot);

	// �f�o�b�O�`��
	void DrawDebug(void);

	// �p�����[�^�ݒ�
	void SetParam(void) override;

	// �A�j���[�V�����̏�����
	void InitAnimation(void) override;

	// �L�[�{�[�h�̑���
	void KeyboardContoroller(void);

	// �Q�[���p�b�h�̑���
	void GamePadController(void);

	// �A�j���[�V�����̃t���[���̌Œ�
	void AnimationFrame(void);

	// HP�o�[�̕`��
	void DrawHPBar(void);

	// �K�E�Z
	void SpecialMoveUpdate(void);

};

