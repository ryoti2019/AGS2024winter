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
	case Camera::MODE::LOCKON:
		SetBeforeDrawLockOn();
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
		VECTOR localPos = rotY_.PosAxis(LOCAL_P2T_POS);
		targetPos_ = VAdd(playerTransform_->pos, localPos);

		// カメラ位置
		localPos = rotXY_.PosAxis(LOCAL_P2C_POS);
		pos_ = VAdd(playerTransform_->pos, localPos);

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

void Camera::SetBeforeDrawLockOn(void)
{

	// 同期先の位置
	VECTOR pos = playerTransform_->pos;

	VECTOR localPos;

	// +注視点をロックオン対象とする
	auto goalPos = enemyTransform_->pos;

	float min = 300.0f;
	float dis = AsoUtility::Distance(goalPos, pos);
	//if (dis < min)
	//{
	//	// 注視点の距離が近すぎる場合、一定の距離を保つ
	//	auto dir = VNorm(VSub(goalPos, pos));
	//	goalPos = VAdd(pos, VScale(dir, min));
	//}
	targetPos_ = AsoUtility::Lerp(targetPos_, goalPos, 0.1f);

	// +キャラクターから見た注視点の方向
	auto followToTargetDir = VSub(targetPos_, pos);
	followToTargetDir.y = 0.0f;
	lockOnLook_ = Quaternion::LookRotation(VNorm(followToTargetDir));

	// カメラ位置
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

		// 注視点の距離が近すぎる場合、一定の距離を保つ
		//auto dir = VNorm(VSub(goalCameraPos, targetPos_));
		auto dir = lockOnLook_.GetBack();
		goalCameraPos = VAdd(targetPos_, VScale(dir, min));

		// どうするか
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

	// カメラの上方向
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

	// Qキーを押したらtrueになる
	if (isLazy_)
	{
		LazyRotation();
	}

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

	//// Qキーを押したらtrueになる
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

	// ロックオンする
	if (lockOn_)
	{
		lockOn_ = false;
	}
	// ロックオンを解除する
	else if(!lockOn_)
	{
		lockOn_ = true;
	}

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

bool Camera::GetLockOn(void)
{
	return lockOn_;
}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = VAdd(pos_, LOCAL_P2T_POS);

	// カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// カメラはX軸に傾いているが、
	// この傾いた状態を角度ゼロ、傾き無しとする
	rotXY_ = Quaternion::Identity();

	// ロックオン
	lockOn_ = false;

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

		// プレイヤーがカメラを動かしたときのフラグ
		isOp_ = true;

		// カメラを回転させる
		// X軸のカメラの移動制御
		angle_.x += AsoUtility::Deg2RadF(axisDeg.x);
		angle_.y += AsoUtility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, AsoUtility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));

	}

	// プレイヤーが動かしていないとき
	if (AsoUtility::EqualsVZero(axisDeg))
	{
		isOp_ = false;
	}

	VECTOR playerPos = playerTransform_->pos;
	VECTOR enemyPos = enemyTransform_->pos;

	VECTOR vec = VSub(enemyPos, playerPos);
	float length = AsoUtility::Magnitude(vec);

	// 正規化
	VECTOR vNorm = VNorm(vec);

	// ロックオンしていないとき
	if (!lockOn_)
	{

		// 追従対象の位置
		VECTOR followPos = playerTransform_->pos;

		// 追従対象から注視点までの相対座標を回転
		VECTOR relativeTPos = rotY_.PosAxis(LOCAL_P2T_POS);

		// 注視点の更新
		targetPos_ = VAdd(followPos, relativeTPos);

		// 追従対象からカメラまでの相対座標
		VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

		// カメラ位置の更新
		pos_ = VAdd(followPos, relativeCPos);

		// カメラの上方向
		cameraUp_ = AsoUtility::DIR_U;

	}
	// ロックオンしているとき
	else if (lockOn_ && length <= 2000.0f)
	{

		// 追従対象の位置
		VECTOR enemyPos = enemyTransform_->pos;
		VECTOR playerPos = playerTransform_->pos;

		// 追従対象から注視点までの相対座標を回転
		VECTOR relativeTPos = rotY_.PosAxis(LOCAL_E2T_POS);

		// 注視点の更新
		targetPos_ = VAdd(enemyPos, relativeTPos);

		// 追従対象からカメラまでの相対座標
		VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_E2C_POS);

		// カメラ位置の更新
		vNorm = { -vNorm.x, -vNorm.y ,-vNorm.z };
		relativeCPos = VAdd(relativeCPos, VScale(vNorm, 100.0f));
		pos_ = VAdd(relativeCPos, playerPos);

		// カメラの上方向
		cameraUp_ = AsoUtility::DIR_U;

	}

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
	// 右方向
	if (pad.AKeyRX > 0)
	{
		axisDeg.y = pad.AKeyRX;
	}
	// 左方向
	if (pad.AKeyRX < 0)
	{
		axisDeg.y = pad.AKeyRX;
	}
	// 上方向
	if (pad.AKeyRZ < 0 && AsoUtility::Rad2DegF(angle_.x) <= 30.0f)
	{
		axisDeg.x = -pad.AKeyRZ;
	}
	// 下方向
	if (pad.AKeyRZ > 0 && AsoUtility::Rad2DegF(angle_.x) >= -30.0f)
	{
		axisDeg.x = -pad.AKeyRZ;
	}

	if (!AsoUtility::EqualsVZero(axisDeg))
	{

		// プレイヤーがカメラを動かしたときのフラグ
		isOp_ = true;

		// 方向を正規化
		axisDeg = VNorm(axisDeg);

		VECTOR moveAxisDeg = VScale(axisDeg, 1.0f);

		// カメラを回転させる
		// X軸のカメラの移動制御
		angle_.x += AsoUtility::Deg2RadF(moveAxisDeg.x);
		angle_.y += AsoUtility::Deg2RadF(moveAxisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, AsoUtility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));

	}

	// プレイヤーが動かしていないとき
	if (AsoUtility::EqualsVZero(axisDeg))
	{
		isOp_ = false;
	}

	// 追従対象の位置
	VECTOR followPos = playerTransform_->pos;

	// 追従対象から注視点までの相対座標を回転
	VECTOR relativeTPos = rotY_.PosAxis(LOCAL_P2T_POS);

	// 注視点の更新
	targetPos_ = VAdd(followPos, relativeTPos);

	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

	// カメラ位置の更新
	pos_ = VAdd(followPos, relativeCPos);

	// カメラの上方向
	//cameraUp_ = followRot.PosAxis(rotXY_.GetUp());
	cameraUp_ = AsoUtility::DIR_U;

}

void Camera::LazyRotation(void)
{

	if (isOp_)
	{
		isLazy_ = false;
	}

	// プレイヤーが向いている方向にカメラを回転させる
	// 二つのクォータニオンの角度差
	if (Quaternion::Angle(rotY_, lazyGoalRotY_) > abs(2.0f))
	{
		rotY_ = Quaternion::Slerp(rotY_, lazyGoalRotY_, 0.1f);
		angle_.y = rotY_.ToEuler().y;
		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));
	}
	// 回転しない場合
	else
	{
		rotY_ = lazyGoalRotY_;
		isLazy_ = false;
		angle_.y = rotY_.ToEuler().y;
		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, AsoUtility::AXIS_X));
	}


}



