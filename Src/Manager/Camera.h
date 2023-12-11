#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class Transform;
class Player;

class Camera
{

public:

	// �J�����̍���
	static constexpr float HEIGHT = 250.0f;

	// �Ǐ]�Ώۂ���J�����܂ł̋���
	static constexpr float DIS_FOLLOW2CAMERA = 150.0f;

	// �Ǐ]�Ώۂ��璍���_�܂ł̋���
	static constexpr float DIS_FOLLOW2TARGET = 350.0f;

	// �J�����X�s�[�h�FNEAR
	static constexpr float SPEED = 20.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 500.0f, -500.0f };

	// �J�����ʒu���璍���_�܂ł̑��΍��W
	//static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	// �Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(���S�Ǐ])
	//static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 1000.0f, 300.0f };


	// F - FOLLOW - �Ǐ]�Ώ� - Player
	// C - CAMERA - �J�����ʒu
	// T - TARGET - �����_

	// �v���C���[����J�����̈ʒu�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_P2C_POS = { 0.0f, 500.0f,-400.0f };

	// �G����J�����̈ʒu�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_E2C_POS = { 0.0f, 500.0f,-400.0f };

	// �v���C���[���璍���_�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_P2T_POS = { 0.0f,0.0f,500.0f };

	// �G���璍���_�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_E2T_POS = { 0.0f,0.0f,500.0f };

	static constexpr VECTOR LOCAL_LOCK_ON_F2C_POS = { 0.0f,300.0f,-500.0f };


	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// ��_�J����
		FREE,			// �t���[���[�h
		FOLLOW,			// �Ǐ]���[�h
		LOCKON,			// ���b�N�I�����[�h
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawLockOn(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;

	VECTOR GetAngles(void) const;

	// �v���C���[�������Ă���p�x
	void SetLazyAngles(const VECTOR angles);

	VECTOR GetTargetPos(void) const;

	void SetBeforeDrawFollow(void);
	
	// �Ǐ]�Ώۂ̐ݒ�
	void SetPlayer(const Transform* follow);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetEnemy(const Transform* follow);

	// ���b�N�I���̃t���O��ς���
	void ChangeLockOnFlag(void);

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	Quaternion GetRotY(void) const;

	// �x����]
	void LazyRotation(void);

	// ���b�N�I���̎擾
	bool GetLockOn(void);

	Camera::MODE GetMode(void);

	void AddLockOnAnglesY(float rad);

private:

	bool lockOn_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����̒����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

	// �J�����̉�]
	Quaternion rotXY_;
	Quaternion rotY_;

	// Y���̃S�[���̊p�x
	Quaternion lazyGoalRotY_;

	// SetLazyAngle�ɒl����������true
	bool isLazy_ = false;

	// �J�����̉�]
	VECTOR angle_;

	Quaternion lockOnLook_;

	// ���b�N�I����p�R���g���[������p
	VECTOR lockOnAngles_;

	VECTOR angles_;

	bool isNearLockOnTarget_;

	VECTOR goalCameraPos_;

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	// �����_���L�����N�^�[�O���ʒu�ɐݒ�
	void SetTargetPosFollowForward(void);

	// �Ǐ]�Ώ�
	const Transform* playerTransform_;
	const Transform* enemyTransform_;

	// �L�[�{�[�h�̑���
	void KeybordContoroller(void);
	void KeybordLockOnContoroller(void);

	// �Q�[���p�b�h�̑���
	void GamePadController(void);

};

