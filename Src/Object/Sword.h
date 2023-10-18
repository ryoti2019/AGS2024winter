#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"

class Sword
{
public:

	// �v���C���[�̉E�肩�猕�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_SWORD_POS = { 60.0f,0.0f,0.0f };

	// �J�v�Z���̏�̑��΍��W
	static constexpr VECTOR LOCAL_C_DOWN_POS = { -50.0f,5.0f,0.0f };

	// �J�v�Z���̉��̑��΍��W
	static constexpr VECTOR LOCAL_C_UP_POS = { 100.0f,5.0f,0.0f };

	// �Փ˔���̋��̔��a
	static constexpr float COLLISION_RADIUS = 10.0f;

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

	// �Փ˔���̉��̍��W�̎擾
	VECTOR GetCPosDown(void);

	// �Փ˔���̏�̍��W�̎擾
	VECTOR GetCPosUP(void);

private:

	// ���f������̊�{���
	Transform transform_;

	// �A�j���[�V�����̏�����
	void InitAnimation(void);

	// ������A�^�b�`����t���[���̔ԍ�
	int WeponAttachFrameNum_;

	// �Ǐ]�Ώ�
	const Transform* followTransform_;

	// �G�Ƃ̓����蔻��
	void EnemyCheckHit(void);

	// �Փ˔���̍��W
	VECTOR cPosUp_;
	VECTOR cPosDown_;

	// �Փ˔���̉�]
	Quaternion cQuaUP_;
	Quaternion cQuaDOWN_;

};