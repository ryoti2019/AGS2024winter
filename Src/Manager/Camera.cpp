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
	// カメラの初期設定
	SetDefault();
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
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

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
}

void Camera::SetBeforeDrawFree(void)
{

	auto& ins = InputManager::GetInstance();

	// 回転
	//-------------------------------------
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x += 1.0f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x += -1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y += 1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += -1.0f; }

	// 移動
	//--------------------------------------
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }
	//---------------------------------------

	if (!AsoUtility::EqualsVZero(axisDeg))
	{

		// カメラを回転させる
		angle_.x += AsoUtility::Deg2RadF(axisDeg.x);
		angle_.y += AsoUtility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, AsoUtility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));

		// 注視点(通常重力でいうところのY値を追従対象と同じにする)
		VECTOR localPos = rotY_.PosAxis(LOCAL_F2T_POS);
		targetPos_ = VAdd(followTransform_->pos, localPos);

		// カメラ位置
		localPos = rotXY_.PosAxis(LOCAL_F2C_POS);
		pos_ = VAdd(followTransform_->pos, localPos);

		// カメラの上方向
		cameraUp_ = rotXY_.GetUp();

	}

	if (!AsoUtility::EqualsVZero(moveDir))
	{

		// 押下された移動ボタンの方向に移動
		VECTOR direction = VNorm(rotY_.PosAxis(moveDir)); // 回転させる必要がある

		// 移動量(方向 * スピード)
		VECTOR movePow = VScale(direction, SPEED);

		// 移動(座標 + 移動量)
		// カメラ位置とカメラ注視点
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

	// キーボードでの操作
	if (!SceneManager::GetInstance().GetGamePad())
	{
		KeybordContoroller();
	}

	// ゲームパッドでの操作
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

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
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

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = VAdd(pos_, LOCAL_F2T_POS);

	// カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// カメラはX軸に傾いているが、
	// この傾いた状態を角度ゼロ、傾き無しとする
	rotXY_ = Quaternion::Identity();

}

void Camera::SetTargetPosFollowForward(void)
{

	// カメラ位置(プレイヤーからの相対座標で制御)
	VECTOR localCameraPos = { 0.0f, HEIGHT, -DIS_FOLLOW2CAMERA };
	VECTOR localTargetPos = { 0.0f, 0.0f, DIS_FOLLOW2TARGET };

	// 注視点の移動
	VECTOR followPos = { 0.0f,0.0f,0.0f };

	// カメラの回転行列を作成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(rotXY_.x));
	mat = MMult(mat, MGetRotY(rotXY_.y));

	VECTOR localRotPos;

	// 注視点の移動
	localRotPos = VTransform(localTargetPos, mat);
	targetPos_ = VAdd(followPos, localRotPos);

	// カメラの移動
	localRotPos = VTransform(localCameraPos, mat);
	pos_ = VAdd(followPos, localRotPos);

}

void Camera::KeybordContoroller(void)
{

	auto& ins = InputManager::GetInstance();

	// 回転
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

		// カメラを回転させる
		// X軸のカメラの移動制御

		angle_.x += AsoUtility::Deg2RadF(axisDeg.x);
		angle_.y += AsoUtility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, AsoUtility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));

	}

	//// デバッグ描画
	//DebugDraw();

	// 追従対象の位置
	VECTOR followPos = followTransform_->pos;

	// 追従対象から注視点までの相対座標を回転
	VECTOR relativeTPos = rotY_.PosAxis(LOCAL_F2T_POS);

	// 注視点の更新
	targetPos_ = VAdd(followPos, relativeTPos);

	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_F2C_POS);

	// カメラ位置の更新
	pos_ = VAdd(followPos, relativeCPos);

	// カメラの上方向
	//cameraUp_ = followRot.PosAxis(rotXY_.GetUp());
	cameraUp_ = AsoUtility::DIR_U;

}

void Camera::GamePadController(void)
{

	auto& ins = InputManager::GetInstance();

	// 回転
	//-------------------------------------
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;

	// ゲームパッドの番号を取得
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// 右のスティックの情報を取得する
	auto isTrgRStick = ins.IsPadRStickTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER);

	// パッドの方向をdirに直す
	axisDeg.x = pad.AKeyRZ;
	axisDeg.y = pad.AKeyRX;

	if (isTrgRStick)
	{

		// カメラを回転させる
		// X軸のカメラの移動制御

		// 方向を正規化
		axisDeg = VNorm(axisDeg);

		VECTOR moveAxisDeg = VScale(axisDeg, 3.0f);

		angle_.x += AsoUtility::Deg2RadF(moveAxisDeg.x);
		angle_.y += AsoUtility::Deg2RadF(moveAxisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, AsoUtility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));

	}

	//// デバッグ描画
	//DebugDraw();

	// 追従対象の位置
	VECTOR followPos = followTransform_->pos;

	// 追従対象から注視点までの相対座標を回転
	VECTOR relativeTPos = rotY_.PosAxis(LOCAL_F2T_POS);

	// 注視点の更新
	targetPos_ = VAdd(followPos, relativeTPos);

	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_F2C_POS);

	// カメラ位置の更新
	pos_ = VAdd(followPos, relativeCPos);

	// カメラの上方向
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

