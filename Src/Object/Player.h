#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"

class Player : public UnitBase
{

public:

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 0.0f;

	// ��]��(deg)
	static constexpr float SPEED_ROT = 300.0f;

	// ��]��(rad)
	static constexpr float SPEED_ROT_RAD = SPEED_ROT * (DX_PI_F / 180.0f);

	// �v���C���[�̏��
	enum class STATE
	{
		WALK,
		RUN,
		ATTACK
	};

	// �R���X�g���N�^
	Player(void);

	// �f�X�g���N�^
	~Player(void);

	void Init(void) override;

	void InitAnimation(void)override;

	//void Update(void);

	void Draw(void);

	void Release(void);

	// �v���C���[��Transform�̎擾
	const Transform& GetTransform(void) const;

protected:

	//// �A�j���[�V�����̃A�^�b�`�ԍ�
	//int animAttachNo_;

	//// �A�j���[�V�����̑��Đ�����
	//float animTotalTime_;

	//// �Đ����̃A�j���[�V��������
	//float stepAnim_;

	//// �A�j���[�V�������x
	//float speedAnim_;

	//// ���f���̒��ɂ���A�j���[�V�����̔ԍ�
	//int animNo_;

	// �����A�j���[�V����
	int walkAnim_;

	// ����A�j���[�V����
	int runAnim_;

	//// ���f������̊�{���
	//Transform transform_;

	// �v���C���[�̏��
	STATE state_;

	// ��]
	Quaternion quaRot_;

	// �ړ�����
	void Move(void) override;

	// �J�����̈ړ�����
	
	// ��ԑJ��
	void ChangeState(STATE state);

	// �A�j���[�V����
	void Animation(void);

	// �A�j���[�V�����̏����ݒ�
	void SetWalkAnimation(void);
	void SetRunAnimation(void);

	// �x����]
	void LazyRotation(float goalRot);

	// �f�o�b�O�`��
	void DrawDebug(void);

	// �p�����[�^�ݒ�
	void SetParam(void) override;

};

