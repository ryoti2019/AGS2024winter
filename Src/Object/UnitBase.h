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

	VECTOR GetPos(void) const;
	VECTOR GetRot(void) const;

	// �v���C���[��Transform�̎擾
	const Transform& GetTransform(void) const;

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

	// �\�����W
	VECTOR pos_;

	// �p�x
	VECTOR rot_;

	// ���[�J���p�x
	//(�����I�ɒ������邽�߂̊p�x)
	VECTOR rotLocal_;

	// �p�����[�^�ݒ�
	virtual void SetParam(void) = 0;

	// �ړ� �������ĖႤ�悤�ɁB
	virtual void Move(void);

	// �A�j���[�V����
	void Animation(void);

	// ��]�ݒ�
	void SetRotation(void);

	// �x����]
	void LazyRotation(float goalRot);
	
	// 
	virtual void InitAnimation(void);

};
