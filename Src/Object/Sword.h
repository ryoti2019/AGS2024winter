#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"

class Sword
{
public:

	// �R���X�g���N�^
	//Sword(const Transform& follow);

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

	// �Ǐ]�Ώ�
	//const Transform& follow_;

	// �Ǐ]�Ώ�
	const Transform* followTransform_;
};


