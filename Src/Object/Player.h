#pragma once
#include "Common/Transform.h"

class Player
{

public:

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 0.0f;

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

	void Init(void);

	void InitAnimation(void);

	void Update(void);

	void Draw(void);

	void Release(void);

	// �v���C���[��Transform�̎擾
	const Transform& GetTransform(void) const;

protected:

	// �A�j���[�V�����̃A�^�b�`�ԍ�
	int animAttachNo_;

	// �A�j���[�V�����̑��Đ�����
	float animTotalTime_;

	// �Đ����̃A�j���[�V��������
	float stepAnim_;

	// �A�j���[�V�������x
	float speedAnim_;

	// ���f���̒��ɂ���A�j���[�V�����̔ԍ�
	int animNo_;

	// ���f������̊�{���
	Transform transform_;

	// �v���C���[�̏��
	STATE state_;

	// �ړ�����
	void Move(void);

	// �J�����̈ړ�����
	
	// ��ԑJ��
	void ChangeState(STATE state);

	// �A�j���[�V����
	void Animation(void);

	// �A�j���[�V�����̏����ݒ�
	void SetWalkAnimation(void);
	void SetRunAnimation(void);

};

