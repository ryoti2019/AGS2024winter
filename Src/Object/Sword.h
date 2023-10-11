#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"

class Sword
{
public:

	Sword(void);

	// �f�X�g���N�^
	~Sword(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

	// ����Transform�̎擾
	const Transform& GetTransform(void) const;

private:

	// ���f������̊�{���
	Transform transform_;

	// �A�j���[�V�����̏�����
	void InitAnimation(void);

	//������A�^�b�`����t���[���̔ԍ�
	int WeponAttachFrameNum;

	// �Ǐ]�Ώ�
	const Transform* followTransform_;

	// �G�Ƃ̓����蔻��
	void EnemyCheckHit(void);

	// �Փ˔���̍��W
	VECTOR collisionPos_;

};