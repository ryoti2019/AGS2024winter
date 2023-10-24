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
	static constexpr float COLLISION_WEPON_RADIUS = 50.0f;

	// �ːi�̕b��
	static constexpr float TACKLE_TIME = 4.0f;

	// �v���C���[�̏��
	enum class STATE
	{
		IDLE,
		WALK,
		TACKLE,
		ATTACK,
		JUMP_ATTACK,
	};

	// �R���X�g���N�^
	Enemy(void);

	// �f�X�g���N�^
	~Enemy(void);

	void Init(void)override;

	void Draw(void)override;

	void Release(void);

	// �G��Transform�̎擾
	const Transform& GetTransform(void) const;

	// �v���C���[�̏�Ԃ��擾
	Enemy::STATE GetState(void);

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

protected:

	// �v���C���[�̎擾
	Player* player_;

	// �v���C���[�̏��
	STATE state_;
	STATE preState_;

	// �A�j���[�V�������Ƃɕϐ��ɑ��
	int idleAnim_;
	int walkAnim_;
	int tackleAnim_;
	int attackAnim_;
	int jumpAttackAnim_;

	// �U���̃t���O
	bool attack_;

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

	// �ړ�����
	void Move(void) override;

	// ��]����
	void Rotation(void);

	// �ҋ@
	void Idle(void);

	// �ړ�
	void Walk(void);

	// �ʏ�U��
	void Attack(void);

	// �_�b�V���U��
	void DashAttack(void);

	// �^�b�N���U��
	void Tackle(void);

	// �Փ˔���
	void Collision(void);

	// �G���g�̏Փ˔���
	void EnemyBodyCollision(void);

	// �G�̕���̏Փ˔���
	void WeponCollision(void);

	// ��ԑJ��
	void ChangeState(STATE state);

	// �A�j���[�V�����̏����ݒ�
	void SetIdleAnimation(void);
	void SetWalkAnimation(void);
	void SetTackleAnimation(void);
	void SetAttackAnimation(void);
	void SetJumpAttackAnimation(void);

	// �A�j���[�V�����̕ύX
	void ChangeAnimation(void);

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

