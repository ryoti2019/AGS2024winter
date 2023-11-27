#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"
class Player;

class Enemy : public UnitBase
{

public:

	// �G�l�~�[���g�̃J�v�Z���̉��̑��΍��W
	static constexpr VECTOR LOCAL_BODY_C_DOWN_POS = { 0.0f,50.0f,0.0f };

	// �G�l�~�[���g�̃J�v�Z���̏�̑��΍��W
	static constexpr VECTOR LOCAL_BODY_C_UP_POS = { 0.0f,-100.0f,-50.0f };

	// �G�l�~�[����̃J�v�Z���̉��̑��΍��W
	static constexpr VECTOR LOCAL_WEPON_C_DOWN_POS = { 0.0f,0.0f,0.0f };

	// �G�l�~�[����̃J�v�Z���̏�̑��΍��W
	static constexpr VECTOR LOCAL_WEPON_C_UP_POS = { 0.0f,150.0f,0.0f };

	// �G�l�~�[���g�̏Փ˔���̋��̔��a
	static constexpr float COLLISION_BODY_RADIUS = 100.0f;

	// �G�l�~�[����̏Փ˔���̋��̔��a
	static constexpr float COLLISION_WEPON_RADIUS = 70.0f;

	// �ːi�̕b��
	static constexpr float TACKLE_TIME = 4.0f;

	// �ŏ��̕���������b��
	static constexpr float FIRST_WALK_TIME = 2.0f;

	// �A�^�b�N���̍U���͈�
	static constexpr float ATTACK_RANGE = 400.0f;

	// �W�����v�A�^�b�N�̍U���͈�(�Œ�)
	static constexpr float JUMP_ATTACK_RANGE_MIN = 400.0f;

	// �W�����v�A�^�b�N�̍U���͈�(�ō�)
	static constexpr float JUMP_ATTACK_RANGE_MAX = 3000.0f;

	//// �^�b�N���̍U���͈�(�Œ�)
	//static constexpr float TACKLE_RANGE_MIN = 1000.0f;
	
	// �ړI�̊p�x�܂ł̍�
	static constexpr float GOAL_DEG = 10.0f;

	// �����X�s�[�h
	static constexpr float WALK_SPEED = 2.0f;

	// �W�����v�A�^�b�N�̃X�s�[�h
	static constexpr float JUMP_ATTACK_SPEED = 20.0f;

	// �W�����v�A�^�b�N�̃X�s�[�h
	static constexpr float TACKLE_SPEED = 30.0f;

	// �_���[�W�q�b�g�̃X�s�[�h
	static constexpr float HIT_SPEED = -5.0f;

	// �U���̓����蔻�肪�n�܂鎞��
	static constexpr float ATTACK_COLLISION_START_TIME = 30.0f;

	// �U���̓����蔻�肪�I��鎞��
	static constexpr float ATTACK_COLLISION_END_TIME = 50.0f;

	// �W�����v�A�^�b�N�̓����蔻�肪�n�܂鎞��
	static constexpr float JUMP_ATTACK_COLLISION_START_TIME = 40.0f;

	// �W�����v�A�^�b�N�̓����蔻�肪�I��鎞��
	static constexpr float JUMP_ATTACK_COLLISION_END_TIME = 60.0f;

	// �W�����v�A�^�b�N���I��鎞��
	static constexpr float JUMP_ATTACK_END_TIME = 300.0f;

	// �_���[�W�q�b�g���I��鎞��
	static constexpr float HIT_END_TIME = 300.0f;

	// �ҋ@�A�j���[�V�����̍Đ����x
	static constexpr float IDLE_ANIM_SPEED = 20.0f;

	// �����A�j���[�V�����̍Đ����x
	static constexpr float WALK_ANIM_SPEED = 20.0f;

	// ����A�j���[�V�����̍Đ����x
	static constexpr float RUN_ANIM_SPEED = 40.0f;

	// �U���A�j���[�V�����̍Đ����x
	static constexpr float ATTACK_ANIM_SPEED = 20.0f;

	// �W�����v�A�^�b�N�A�j���[�V�����̍Đ����x
	static constexpr float JUMP_ATTACK_ANIM_SPEED = 20.0f;

	// �^�b�N���A�j���[�V�����̍Đ����x
	static constexpr float TACKLE_ANIM_SPEED = 40.0f;

	// �V���b�g�A�j���[�V�����̍Đ����x
	static constexpr float SHOT_ANIM_SPEED = 20.0f;

	// �_���[�W�q�b�g�A�j���[�V�����̍Đ����x
	static constexpr float HIT_ANIM_SPEED = 20.0f;

	// HP�o�[�̒���
	static constexpr int HP_LENGTH = 300;

	// HP�̍ő�l
	static constexpr int HP_MAX = 100;

	// �v���C���[�̏��
	enum class STATE
	{
		THINK,
		IDLE,
		WALK,
		ATTACK,
		JUMP_ATTACK,
		TACKLE,
		SHOT,
		HIT,
	};

	// �R���X�g���N�^
	Enemy(void);

	// �f�X�g���N�^
	~Enemy(void);

	void Init(void)override;

	void Update(void)override;

	void Draw(void)override;

	void Release(void);

	// �G��Transform�̎擾
	const Transform& GetTransform(void) const;

	// �v���C���[�̏�Ԃ��擾
	Enemy::STATE GetState(void);

	// �v���C���[�̏�Ԃ��擾
	void SetState(Enemy::STATE state);

	// �G���g�̏Փ˔���̉��̍��W�̎擾
	VECTOR GetCBodyPosDown(void);

	// �G���g�̏Փ˔���̏�̍��W�̎擾
	VECTOR GetCBodyPosUP(void);

	// �G����̏Փ˔���̉��̍��W�̎擾
	VECTOR GetCWeponPosDown(void);

	// �G����̏�̍��W�̎擾
	VECTOR GetCWeponPosUP(void);

	// �G��HP�擾
	int GetHP(void);

	// �G��HP�̐ݒ�
	void SetHP(int hp);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

	// �U���t���O�̎擾
	bool GetAttack(void);

	// �U���t���O�̐ݒ�
	void SetAttack(bool attack);

	// �U���������������ǂ����̎擾
	bool GetHit(void);

	// �U���������������ǂ����̐ݒ�
	void SetHit(bool hit);

protected:

	// �e
	std::vector<ShotEnemy*> shots_;

	// �v���C���[�̎擾
	Player* player_;

	// �v���C���[�̏��
	STATE state_;
	STATE preState_;

	// �ҋ@�A�j���[�V����
	int idleAnim_;

	// �����A�j���[�V����
	int walkAnim_;

	// �^�b�N���A�j���[�V����
	int tackleAnim_;

	// �U���A�j���[�V����
	int attackAnim_;

	// �W�����v�A�^�b�N�̃A�j���[�V����
	int jumpAttackAnim_;

	// �V���b�g�A�j���[�V����
	int shotAnim_;

	// �_���[�W�q�b�g�A�j���[�V����
	int hitAnim_;

	// �ŏ��̕����̃A�j���[�V�����̃J�E���^
	float walkCnt_;
	
	// ��]�̏I���̃t���O
	bool rotationEnd_;

	// �U���̔ԍ�
	int attackNumber_;

	// �U���̃t���O
	bool attack_;

	// �U���������������ǂ���
	bool hit_;

	// �_�b�V���U���̃t���O
	bool jumpAttack_;

	// �^�b�N���̍U���t���O
	bool tackleAttack_;

	// �ːi��������b��
	float tackleCnt_;

	// �ړ�����������t���[���ԍ�
	int enemyPosFrameNum_;
	
	// �J�v�Z�����A�^�b�`����t���[���̔ԍ�
	int enemyAttachFrameNum_;

	// ������A�^�b�`����t���[���̔ԍ�
	int weponAttachFrameNum_;

	// �v���C���[�̕���
	VECTOR pDirection_;

	// �G�l�~�[���g�̏Փ˔���̍��W
	VECTOR cBodyPosUp_;
	VECTOR cBodyPosDown_;

	// �G�l�~�[����̏Փ˔���̍��W
	VECTOR cWeponPosUp_;
	VECTOR cWeponPosDown_;

	// �_�b�V���U�����鎞�̃v���C���[���W��ۊǂ�����W
	VECTOR attackPlayerPos_;

	// �Ǐ]�Ώ�
	const Transform* followTransform_;

	// �s���̑I��
	void Think(void);

	// ��]����
	void Rotation(void);
	
	// �s����̉�]����
	void AfterRotation(void);

	// �ҋ@
	void UpdateIdle(void);

	// �ړ�
	void UpdateWalk(void);

	// �ʏ�U��
	void UpdateAttack(void);

	// �_�b�V���U��
	void UpdateJumpAttack(void);

	// �^�b�N���U��
	void UpdateTackle(void);

	// �V���b�g�U��
	void UpdateShot(void);

	// �U���q�b�g
	void UpdateHit(void);

	// �Փ˔���
	void Collision(void);

	// �G���g�̏Փ˔���
	void EnemyBodyCollision(void);

	// �G�̕���̏Փ˔���
	void WeponCollision(void);

	// ��ԑJ��
	void ChangeState(STATE state);

	// �ҋ@�A�j���[�V�����̐ݒ�
	void SetIdleAnimation(void);

	// �����A�j���[�V�����̐ݒ�
	void SetWalkAnimation(void);

	// �^�b�N���̃A�j���[�V�����̐ݒ�
	void SetTackleAnimation(void);

	// �U���A�j���[�V�����̐ݒ�
	void SetAttackAnimation(void);

	// �W�����v�A�^�b�N�̐ݒ�
	void SetJumpAttackAnimation(void);

	// �V���b�g�̐ݒ�
	void SetShotAnimation(void);

	// �_���[�W�q�b�g�q�b�g�A�j���[�V�����̐ݒ�
	void SetHitAnimation(void);

	// �x����]
	void LazyRotation(float goalRot);

	// �f�o�b�O�`��
	void DrawDebug(void);

	// �p�����[�^�ݒ�
	void SetParam(void) override;

	// �A�j���[�V�����̏�����
	void InitAnimation(void) override;

	// �A�j���[�V����
	void Animation(void) override;

	// �A�j���[�V�����̃t���[���̌Œ�
	void AnimationFrame(void);

};

