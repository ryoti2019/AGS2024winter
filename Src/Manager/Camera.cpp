#include "../Utility/AsoUtility.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Player.h"
#include "Camera.h"

Camera::Camera(void)
{
	mode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	rotXY_ = Quaternion::Identity();
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// �J�����̏����ݒ�
	SetDefault();
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::LOCKON:
		SetBeforeDrawLockOn();
		break;
	}

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// �������Ȃ�
}

void Camera::SetBeforeDrawFree(void)
{

	auto& ins = InputManager::GetInstance();

	// ��]
	//-------------------------------------
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x += 1.0f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x += -1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y += 1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += -1.0f; }

	// �ړ�
	//--------------------------------------
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }
	//---------------------------------------

	if (!AsoUtility::EqualsVZero(axisDeg))
	{

		// �J��������]������
		angle_.x += AsoUtility::Deg2RadF(axisDeg.x);
		angle_.y += AsoUtility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, AsoUtility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));

		// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
		VECTOR localPos = rotY_.PosAxis(LOCAL_P2T_POS);
		targetPos_ = VAdd(playerTransform_->pos, localPos);

		// �J�����ʒu
		localPos = rotXY_.PosAxis(LOCAL_P2C_POS);
		pos_ = VAdd(playerTransform_->pos, localPos);

		// �J�����̏����
		cameraUp_ = rotXY_.GetUp();

	}

	if (!AsoUtility::EqualsVZero(moveDir))
	{

		// �������ꂽ�ړ��{�^���̕����Ɉړ�
		VECTOR direction = VNorm(rotY_.PosAxis(moveDir)); // ��]������K�v������

		// �ړ���(���� * �X�s�[�h)
		VECTOR movePow = VScale(direction, SPEED);

		// �ړ�(���W + �ړ���)
		// �J�����ʒu�ƃJ���������_
		pos_ = VAdd(pos_, movePow);
		targetPos_ = VAdd(targetPos_, movePow);

	}

}

void Camera::SetBeforeDrawLockOn(void)
{

	// ������̈ʒu
	VECTOR pos = playerTransform_->pos;

	VECTOR localPos;

	// +�����_�����b�N�I���ΏۂƂ���
	auto goalPos = enemyTransform_->pos;

	float min = 300.0f;
	float dis = AsoUtility::Distance(goalPos, pos);
	//if (dis < min)
	//{
	//	// �����_�̋������߂�����ꍇ�A���̋�����ۂ�
	//	auto dir = VNorm(VSub(goalPos, pos));
	//	goalPos = VAdd(pos, VScale(dir, min));
	//}
	targetPos_ = AsoUtility::Lerp(targetPos_, goalPos, 0.1f);

	// +�L�����N�^�[���猩�������_�̕���
	auto followToTargetDir = VSub(targetPos_, pos);
	followToTargetDir.y = 0.0f;
	lockOnLook_ = Quaternion::LookRotation(VNorm(followToTargetDir));

	// �J�����ʒu
	lockOnLook_ = lockOnLook_.Mult(Quaternion::AngleAxis(lockOnAngles_.y, AsoUtility::AXIS_Y));
	lockOnLook_ = lockOnLook_.Mult(Quaternion::AngleAxis(-angles_.x + lockOnAngles_.x, AsoUtility::AXIS_X));
	localPos = lockOnLook_.PosAxis(LOCAL_LOCK_ON_F2C_POS);
	auto goalCameraPos = VAdd(pos, localPos);
	dis = AsoUtility::Distance(goalCameraPos, targetPos_);
	min = 500.0f;
	isNearLockOnTarget_ = false;
	if (dis < min)
	{
		float minmin = 400.0f;
		if (dis < minmin)
		{
			isNearLockOnTarget_ = true;
			//followTransform_->pos = VAdd(targetPos_,
			//	VScale(VNorm(VSub(followTransform_->pos, targetPos_)), minmin));
		}

		// �����_�̋������߂�����ꍇ�A���̋�����ۂ�
		//auto dir = VNorm(VSub(goalCameraPos, targetPos_));
		auto dir = lockOnLook_.GetBack();
		goalCameraPos = VAdd(targetPos_, VScale(dir, min));

		// �ǂ����邩
		goalCameraPos_ = goalCameraPos;
		pos_ = AsoUtility::Lerp(pos_, goalCameraPos, 0.05f);

	}
	else
	{
		goalCameraPos_ = goalCameraPos;
		pos_ = AsoUtility::Lerp(pos_, goalCameraPos, 0.1f);
	}

	rot_ = Quaternion::LookRotation(VSub(goalPos, goalCameraPos_));

	auto targetPosXZ = goalPos;
	targetPosXZ.y = 0.0f;
	auto posXZ = goalCameraPos_;
	posXZ.y = 0.0f;
	auto cameraDir = VSub(targetPosXZ, posXZ);
	rotOutX_Move_ = Quaternion::LookRotation(cameraDir);

	angles_ = rotOutX_Move_.ToEuler();

	// �J�����̏����
	cameraUp_ = gRot.GetUp();

}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angle_;
}

void Camera::SetLazyAngles(const VECTOR angles)
{

	lazyGoalRotY_ = Quaternion::Euler(0.0f, angles.y, 0.0f);
	isLazy_ = true;

}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

void Camera::SetBeforeDrawFollow(void)
{

	VECTOR playerPos = playerTransform_->pos;
	VECTOR enemyPos = enemyTransform_->pos;

	VECTOR vec = VSub(enemyPos, playerPos);
	float length = AsoUtility::Magnitude(vec);
	if (length >= 2000.0f)
	{
		lockOn_ = false;
	}

	// Q�L�[����������true�ɂȂ�
	if (isLazy_)
	{
		LazyRotation();
	}

	// �L�[�{�[�h�ł̑���
	if (!SceneManager::GetInstance().GetGamePad())
	{
		KeybordContoroller();
	}

	// �Q�[���p�b�h�ł̑���
	if (SceneManager::GetInstance().GetGamePad())
	{
		GamePadController();
	}

	//// Q�L�[����������true�ɂȂ�
	//if (isLazy_ && !isOp_)
	//{
	//	LazyRotation2();
	//}

}

//void Camera::SetFollow(const Transform* follow)
//{
//
//	followTransform_ = follow;
//
//}

void Camera::SetPlayer(const Transform* follow)
{
	playerTransform_ = follow;
}

void Camera::SetEnemy(const Transform* follow)
{
	enemyTransform_ = follow;
}

void Camera::ChangeLockOnFlag(void)
{

	// ���b�N�I������
	if (lockOn_)
	{
		lockOn_ = false;
	}
	// ���b�N�I������������
	else if(!lockOn_)
	{
		lockOn_ = true;
	}

}

void Camera::ChangeMode(MODE mode)
{

	// �J�����̏����ݒ�
	SetDefault();

	// �J�������[�h�̕ύX
	mode_ = mode;

	// �ύX���̏���������
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	}

}

Quaternion Camera::GetRotY(void) const
{
	return rotY_;
}

bool Camera::GetLockOn(void)
{
	return lockOn_;
}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	// �����_
	targetPos_ = VAdd(pos_, LOCAL_P2T_POS);

	// �J�����̏����
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// �J������X���ɌX���Ă��邪�A
	// ���̌X������Ԃ��p�x�[���A�X�������Ƃ���
	rotXY_ = Quaternion::Identity();

	// ���b�N�I��
	lockOn_ = false;

}

void Camera::SetTargetPosFollowForward(void)
{

	// �J�����ʒu(�v���C���[����̑��΍��W�Ő���)
	VECTOR localCameraPos = { 0.0f, HEIGHT, -DIS_FOLLOW2CAMERA };
	VECTOR localTargetPos = { 0.0f, 0.0f, DIS_FOLLOW2TARGET };

	// �����_�̈ړ�
	VECTOR followPos = { 0.0f,0.0f,0.0f };

	// �J�����̉�]�s����쐬
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(rotXY_.x));
	mat = MMult(mat, MGetRotY(rotXY_.y));

	VECTOR localRotPos;

	// �����_�̈ړ�
	localRotPos = VTransform(localTargetPos, mat);
	targetPos_ = VAdd(followPos, localRotPos);

	// �J�����̈ړ�
	localRotPos = VTransform(localCameraPos, mat);
	pos_ = VAdd(followPos, localRotPos);

}

void Camera::KeybordContoroller(void)
{

	auto& ins = InputManager::GetInstance();

	// ��]
	//-------------------------------------
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;


	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y += -1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += 1.0f; }

	if (ins.IsNew(KEY_INPUT_DOWN) && AsoUtility::Rad2DegF(angle_.x) >= -30.0f)
	{
		axisDeg.x += -1.0f;
	}
	if (ins.IsNew(KEY_INPUT_UP) && AsoUtility::Rad2DegF(angle_.x) <= 30.0f)
	{
		axisDeg.x += 1.0f;
	}

	if (!AsoUtility::EqualsVZero(axisDeg))
	{

		// �v���C���[���J�����𓮂������Ƃ��̃t���O
		isOp_ = true;

		// �J��������]������
		// X���̃J�����̈ړ�����
		angle_.x += AsoUtility::Deg2RadF(axisDeg.x);
		angle_.y += AsoUtility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, AsoUtility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));

	}

	// �v���C���[���������Ă��Ȃ��Ƃ�
	if (AsoUtility::EqualsVZero(axisDeg))
	{
		isOp_ = false;
	}

	VECTOR playerPos = playerTransform_->pos;
	VECTOR enemyPos = enemyTransform_->pos;

	VECTOR vec = VSub(enemyPos, playerPos);
	float length = AsoUtility::Magnitude(vec);

	// ���K��
	VECTOR vNorm = VNorm(vec);

	// ���b�N�I�����Ă��Ȃ��Ƃ�
	if (!lockOn_)
	{

		// �Ǐ]�Ώۂ̈ʒu
		VECTOR followPos = playerTransform_->pos;

		// �Ǐ]�Ώۂ��璍���_�܂ł̑��΍��W����]
		VECTOR relativeTPos = rotY_.PosAxis(LOCAL_P2T_POS);

		// �����_�̍X�V
		targetPos_ = VAdd(followPos, relativeTPos);

		// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
		VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

		// �J�����ʒu�̍X�V
		pos_ = VAdd(followPos, relativeCPos);

		// �J�����̏����
		cameraUp_ = AsoUtility::DIR_U;

	}
	// ���b�N�I�����Ă���Ƃ�
	else if (lockOn_ && length <= 2000.0f)
	{

		// �Ǐ]�Ώۂ̈ʒu
		VECTOR enemyPos = enemyTransform_->pos;
		VECTOR playerPos = playerTransform_->pos;

		// �Ǐ]�Ώۂ��璍���_�܂ł̑��΍��W����]
		VECTOR relativeTPos = rotY_.PosAxis(LOCAL_E2T_POS);

		// �����_�̍X�V
		targetPos_ = VAdd(enemyPos, relativeTPos);

		// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
		VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_E2C_POS);

		// �J�����ʒu�̍X�V
		vNorm = { -vNorm.x, -vNorm.y ,-vNorm.z };
		relativeCPos = VAdd(relativeCPos, VScale(vNorm, 100.0f));
		pos_ = VAdd(relativeCPos, playerPos);

		// �J�����̏����
		cameraUp_ = AsoUtility::DIR_U;

	}

}

void Camera::GamePadController(void)
{

	auto& ins = InputManager::GetInstance();

	// ��]
	//-------------------------------------
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;

	// �Q�[���p�b�h�̔ԍ����擾
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// �E�̃X�e�B�b�N�̏����擾����
	auto isTrgRStick = ins.IsPadRStickTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER);

	// �p�b�h�̕�����dir�ɒ���
	// �E����
	if (pad.AKeyRX > 0)
	{
		axisDeg.y = pad.AKeyRX;
	}
	// ������
	if (pad.AKeyRX < 0)
	{
		axisDeg.y = pad.AKeyRX;
	}
	// �����
	if (pad.AKeyRZ < 0 && AsoUtility::Rad2DegF(angle_.x) <= 30.0f)
	{
		axisDeg.x = -pad.AKeyRZ;
	}
	// ������
	if (pad.AKeyRZ > 0 && AsoUtility::Rad2DegF(angle_.x) >= -30.0f)
	{
		axisDeg.x = -pad.AKeyRZ;
	}

	if (!AsoUtility::EqualsVZero(axisDeg))
	{

		// �v���C���[���J�����𓮂������Ƃ��̃t���O
		isOp_ = true;

		// �����𐳋K��
		axisDeg = VNorm(axisDeg);

		VECTOR moveAxisDeg = VScale(axisDeg, 1.0f);

		// �J��������]������
		// X���̃J�����̈ړ�����
		angle_.x += AsoUtility::Deg2RadF(moveAxisDeg.x);
		angle_.y += AsoUtility::Deg2RadF(moveAxisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, AsoUtility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));

	}

	// �v���C���[���������Ă��Ȃ��Ƃ�
	if (AsoUtility::EqualsVZero(axisDeg))
	{
		isOp_ = false;
	}

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = playerTransform_->pos;

	// �Ǐ]�Ώۂ��璍���_�܂ł̑��΍��W����]
	VECTOR relativeTPos = rotY_.PosAxis(LOCAL_P2T_POS);

	// �����_�̍X�V
	targetPos_ = VAdd(followPos, relativeTPos);

	// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

	// �J�����ʒu�̍X�V
	pos_ = VAdd(followPos, relativeCPos);

	// �J�����̏����
	//cameraUp_ = followRot.PosAxis(rotXY_.GetUp());
	cameraUp_ = AsoUtility::DIR_U;

}

void Camera::LazyRotation(void)
{

	if (isOp_)
	{
		isLazy_ = false;
	}

	// �v���C���[�������Ă�������ɃJ��������]������
	// ��̃N�H�[�^�j�I���̊p�x��
	if (Quaternion::Angle(rotY_, lazyGoalRotY_) > abs(2.0f))
	{
		rotY_ = Quaternion::Slerp(rotY_, lazyGoalRotY_, 0.1f);
		angle_.y = rotY_.ToEuler().y;
		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));
	}
	// ��]���Ȃ��ꍇ
	else
	{
		rotY_ = lazyGoalRotY_;
		isLazy_ = false;
		angle_.y = rotY_.ToEuler().y;
		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));
	}


}



