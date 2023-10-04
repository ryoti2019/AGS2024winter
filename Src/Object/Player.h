#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"

class Player : public UnitBase
{

public:

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

protected:

	// �v���C���[�̏��
	STATE state_;

	// ��]
	Quaternion quaRot_;

	// �ړ�����
	void Move(void) override;

	// �J�����̈ړ�����
	
	// ��ԑJ��
	void ChangeState(STATE state);

	// �A�j���[�V�����̏����ݒ�
	void SetIdleAnimation(void);
	void SetWalkAnimation(void);
	void SetRunAnimation(void);

	// �x����]
	void LazyRotation(float goalRot);

	// �f�o�b�O�`��
	void DrawDebug(void);

	// �p�����[�^�ݒ�
	void SetParam(void) override;

	// �A�j���[�V�����̏�����
	void InitAnimation(void) override;

};

