#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"

class Enemy : public UnitBase
{

public:

	// �����玝����̓����蔻��܂ł̑��΍��W
	static constexpr VECTOR LOCAL_C_DOWN_POS = { 0.0f,250.0f,40.0f };

	// �����猕��̓����蔻��܂ł̑��΍��W
	static constexpr VECTOR LOCAL_C_UP_POS = { 0.0f,50.0f,40.0f };

	// �Փ˔���̋��̔��a
	static constexpr float COLLISION_RADIUS = 100.0f;

	// �v���C���[�̏��
	enum class STATE
	{
		IDLE,
		WALK,
		RUN,
		ATTACK
	};

	// �R���X�g���N�^
	Enemy(void);

	// �f�X�g���N�^
	~Enemy(void);

	void Init(void)override;

	void Draw(void)override;

	void Release(void);

	// �v���C���[��Transform�̎擾
	const Transform& GetTransform(void) const;

	// �Փ˔���̉��̍��W�̎擾
	VECTOR GetCPosDown(void);

	// �Փ˔���̏�̍��W�̎擾
	VECTOR GetCPosUP(void);

protected:

	// �v���C���[�̏��
	STATE state_;

	// �ړ�����
	void Move(void) override;

	// �Փ˔���
	void Collision(void);

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

	// �Փ˔���̍��W
	VECTOR cPosUp_;
	VECTOR cPosDown_;

	// �Փ˔���̉�]
	Quaternion cQuaUP_;
	Quaternion cQuaDOWN_;

};

