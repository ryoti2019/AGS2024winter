#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"

class Player : public UnitBase
{

public:

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

protected:

	// �v���C���[�̏��
	STATE state_;
	STATE preState_;

	// �A�j���[�V����
	int idleAnim_;
	int walkAnim_;
	int runAnim_;
	int attackAnim_;

	// �ړ�����
	void Move(void) override;

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

