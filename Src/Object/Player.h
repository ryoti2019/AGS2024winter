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
	static constexpr float COLLISION_BODY_RADIUS = 30.0f;

	// �U������
	static constexpr float ATTACK_TIME = 0.5f;

	// �v���C���[�̏��
	enum class STATE
	{
		IDLE,
		WALK,
		RUN,
		ATTACK
	};

	// �R���X�g���N�^
	Player(void);

	// �f�X�g���N�^
	~Player(void);

	void Init(void) override;

	void Draw(void);

	void Release(void);

	// �v���C���[��Transform�̎擾
	const Transform& GetTransform(void) const;

	// �v���C���[�̏�Ԃ��擾
	Player::STATE GetState(void);

	// �Փ˔���̉��̍��W�̎擾
	VECTOR GetCPosDown(void);

	// �Փ˔���̏�̍��W�̎擾
	VECTOR GetCPosUP(void);

	// �U���t���O�̎擾
	bool GetAttack(void);

	// �U���t���O�̐ݒ�
	void SetAttack(bool attack);

	// �G��HP�擾
	int GetHP(void);

	// �G��HP�̐ݒ�
	void SetHP(int hp);

protected:

	// �v���C���[�̏��
	STATE state_;
	STATE preState_;

	// �U���t���O
	bool attack_;

	// �A�j���[�V�������Ƃɕϐ��ɑ��
	int idleAnim_;
	int walkAnim_;
	int runAnim_;
	int attackAnim_;

	// �J�v�Z�����A�^�b�`����t���[���̔ԍ�
	int playerAttachFrameNum_;

	// �v���C���[�̏Փ˔���̍��W
	VECTOR cBodyPosUp_;
	VECTOR cBodyPosDown_;

	// �ړ�����
	void Move(void) override;

	// �Փ˔���
	void Collision(void);

	// �G���g�̏Փ˔���
	void PlayerBodyCollision(void);

	// ��ԑJ��
	void ChangeState(STATE state);

	// �A�j���[�V����
	void Animation(void)override;

	// �A�j���[�V�����̏����ݒ�
	void SetIdleAnimation(void);
	void SetWalkAnimation(void);
	void SetRunAnimation(void);
	void SetAttackAnimation(void);

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

	// �L�[�{�[�h�̑���
	void KeybordContoroller(void);

	// �Q�[���p�b�h�̑���
	void GamePadController(void);

};

