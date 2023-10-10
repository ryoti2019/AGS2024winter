#include "../Utility/AsoUtility.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "../Object/Common/Transform.h"
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
		VECTOR localPos = rotY_.PosAxis(LOCAL_F2T_POS);
		targetPos_ = VAdd(followTransform_->pos, localPos);

		// �J�����ʒu
		localPos = rotXY_.PosAxis(LOCAL_F2C_POS);
		pos_ = VAdd(followTransform_->pos, localPos);

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

void Camera::SetAngles(const VECTOR angles)
{

	angle_ = angles;

	LazyRotation(angle_.y);

	rotY_ = Quaternion::AngleAxis(angle_.y, AsoUtility::AXIS_Y);
	rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));
	
}

void Camera::SetLazyAngles(const VECTOR angles)
{
	lazyGoalRotY_ = Quaternion::Euler(0.0f, angles.y, 0.0f);
	isLazy = true;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

void Camera::SetBeforeDrawFollow(void)
{

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

	if (isLazy)
	{
		LazyRotation2();
	}

}

void Camera::SetFollow(const Transform* follow)
{

	followTransform_ = follow;

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

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	// �����_
	targetPos_ = VAdd(pos_, LOCAL_F2T_POS);

	// �J�����̏����
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// �J������X���ɌX���Ă��邪�A
	// ���̌X������Ԃ��p�x�[���A�X�������Ƃ���
	rotXY_ = Quaternion::Identity();

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

	if (ins.IsNew(KEY_INPUT_UP) && AsoUtility::Rad2DegF(angle_.x) >= -30.0f)
	{
		axisDeg.x += -1.0f;
	}
	if (ins.IsNew(KEY_INPUT_DOWN) && AsoUtility::Rad2DegF(angle_.x) <= 30.0f)
	{
		axisDeg.x += 1.0f;
	}

	if (!AsoUtility::EqualsVZero(axisDeg))
	{

		// �J��������]������
		// X���̃J�����̈ړ�����

		angle_.x += AsoUtility::Deg2RadF(axisDeg.x);
		angle_.y += AsoUtility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, AsoUtility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));

	}

	//// �f�o�b�O�`��
	//DebugDraw();

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = followTransform_->pos;

	// �Ǐ]�Ώۂ��璍���_�܂ł̑��΍��W����]
	VECTOR relativeTPos = rotY_.PosAxis(LOCAL_F2T_POS);

	// �����_�̍X�V
	targetPos_ = VAdd(followPos, relativeTPos);

	// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_F2C_POS);

	// �J�����ʒu�̍X�V
	pos_ = VAdd(followPos, relativeCPos);

	// �J�����̏����
	//cameraUp_ = followRot.PosAxis(rotXY_.GetUp());
	cameraUp_ = AsoUtility::DIR_U;

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
	axisDeg.x = pad.AKeyRZ;
	axisDeg.y = pad.AKeyRX;

	if (isTrgRStick)
	{

		// �J��������]������
		// X���̃J�����̈ړ�����

		// �����𐳋K��
		axisDeg = VNorm(axisDeg);

		VECTOR moveAxisDeg = VScale(axisDeg, 3.0f);

		angle_.x += AsoUtility::Deg2RadF(moveAxisDeg.x);
		angle_.y += AsoUtility::Deg2RadF(moveAxisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, AsoUtility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));

	}

	//// �f�o�b�O�`��
	//DebugDraw();

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = followTransform_->pos;

	// �Ǐ]�Ώۂ��璍���_�܂ł̑��΍��W����]
	VECTOR relativeTPos = rotY_.PosAxis(LOCAL_F2T_POS);

	// �����_�̍X�V
	targetPos_ = VAdd(followPos, relativeTPos);

	// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_F2C_POS);

	// �J�����ʒu�̍X�V
	pos_ = VAdd(followPos, relativeCPos);

	// �J�����̏����
	//cameraUp_ = followRot.PosAxis(rotXY_.GetUp());
	cameraUp_ = AsoUtility::DIR_U;

}

void Camera::LazyRotation(float goalRot)
{
	//float time = 0.0f;

	//time += SceneManager::GetInstance().GetDeltaTime();

	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	rotY_ = Quaternion::Slerp(rotY_, goal, 0.1f);
}

void Camera::LazyRotation2(void)
{
	if (Quaternion::Angle(rotY_, lazyGoalRotY_) > 2.0f)
	{
		rotY_ = Quaternion::Slerp(rotY_, lazyGoalRotY_, 0.1f);
		angle_.y = rotY_.ToEuler().y;
		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));
	}
	else
	{
		rotY_ = lazyGoalRotY_;
		isLazy = false;
		angle_.y = rotY_.ToEuler().y;
		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));
	}
}

