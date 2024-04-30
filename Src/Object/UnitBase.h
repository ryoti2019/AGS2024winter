#pragma once
#include <string>
#include <DxLib.h>
#include "Common/Transform.h"

class AnimationManager;

class UnitBase
{

public:

	// �A�j���[�V�����̍Đ����x
	static constexpr float SPEED_ANIM = 20.0f;

	// ��]�ɂ����鎞��
	static constexpr float TIME_ROT = 1.0f;

	// ��]��(deg)
	static constexpr float SPEED_ROT = 8.0f;

	// ��]��(rad)
	static constexpr float SPEED_ROT_RAD = SPEED_ROT * (DX_PI_F / 180.0f);

	UnitBase(void);
	virtual ~UnitBase(void);
	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	// �v���C���[��Transform�̎擾
	const Transform& GetTransform(void) const;

	// �Đ����̃A�j���[�V��������
	float GetStepAnim(void);

protected:

	//�A�j���[�V����
	AnimationManager* animater_;

	// ���f���t�@�C����
	std::string modelFileName_;

	// ���f������̊�{���
	Transform transform_;

	// ���f��ID
	int modelId_;

	//�A�j���[�V����
	int modelWalk_;
	int modelRun_;

	// �\�����W
	VECTOR pos_;

	// �p�x
	VECTOR rot_;

	// ���[�J���p�x
	//(�����I�ɒ������邽�߂̊p�x)
	VECTOR rotLocal_;

	// HP
	int hp_;

	// HP�̍ő�l
	int hpMax_;

	// �p�����[�^�ݒ�
	virtual void SetParam(void) = 0;

	// �ړ�
	virtual void Move(void);

	// �A�j���[�V����
	virtual void Animation(void);

	// �A�j���[�V�����̏�����
	virtual void InitAnimation(void) = 0;

};
