#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::InitAnimation(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_IDLE));

	// 待機アニメーション
	idleAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_IDLE);

	// 歩くアニメーション
	walkAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_WALK);

	// 走るアニメーション
	runAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_RUN);

	// 攻撃アニメーション
	attackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_ATTACK);

	// ダメージヒットアニメーション
	hitAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_HIT);

	// transformの初期化
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(180), AsoUtility::AXIS_Y));
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	// アニメーションする番号
	animNo_ = 1;

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// 再生中のアニメーション時間
	stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 0.0f;

}

void Player::Init(void)
{

	// カプセルをアタッチするフレームの番号を検索
	playerAttachFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:Hips");

	// アニメーションの初期設定
	InitAnimation();

	// プレイヤーのパラメーター
	SetParam();

	// 待機アニメーション
	SetIdleAnimation();

	// 攻撃が当たったかどうか
	hit_ = false;

}

void Player::Update(void)
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

	// アニメーション処理
	Animation();

	// 衝突判定
	Collision();

	transform_.Update();

}

void Player::Draw(void)
{

	// モデルの描画
	UnitBase::Draw();

	// デバッグ描画
	DrawDebug();

}

void Player::Release(void)
{

	// モデルの開放
	MV1DeleteModel(transform_.modelId);

}

const Transform& Player::GetTransform(void) const
{
	return transform_;
}

Player::STATE Player::GetState(void)
{
	return state_;
}

void Player::SetState(Player::STATE state)
{
	ChangeState(state);
}


VECTOR Player::GetCPosDown(void)
{
	return cBodyPosDown_;
}

VECTOR Player::GetCPosUP(void)
{
	return cBodyPosUp_;
}

bool Player::GetAttack(void)
{
	return attack_;
}

void Player::SetAttack(bool attack)
{
	attack_ = attack;
}

int Player::GetHP(void)
{
	return hp_;
}

void Player::SetHP(int hp)
{
	hp_ += hp;
}

bool Player::GetHit(void)
{
	return hit_;
}

void Player::SetHit(bool hit)
{
	hit_ = hit;
}

void Player::Collision(void)
{

	// 敵自身の当たり判定
	PlayerBodyCollision();

}

void Player::PlayerBodyCollision(void)
{

	// 敵の中心にカプセルを追従させる
	// カプセルをアタッチするフレームのローカル→ワールド変換行列を取得する
	MATRIX PlayerFrameMatrix = MV1GetFrameLocalWorldMatrix(transform_.modelId, 0);

	// 行列からラジアン
	auto rot = MGetRotElem(PlayerFrameMatrix);
	auto pos = MGetTranslateElem(PlayerFrameMatrix);

	// 行列からクォータニオン
	Quaternion qua = Quaternion::GetRotation(rot);

	// 追従対象の向き
	Quaternion followRot = qua;

	// 追従対象から自機までの相対座標
	VECTOR cPosDOWN = followRot.PosAxis(LOCAL_BODY_C_DOWN_POS);
	VECTOR cPosUP = followRot.PosAxis(LOCAL_BODY_C_UP_POS);

	// プレイヤーの位置の更新
	cBodyPosDown_ = VAdd(pos, cPosDOWN);
	cBodyPosUp_ = VAdd(pos, cPosUP);

}

void Player::KeybordContoroller(void)
{

	// カメラの角度を取得
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	auto& ins = InputManager::GetInstance();

	// 移動量
	float movePow = 10.0f;

	// 方向(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// WASDでプレイヤーの位置を変える
	if (ins.IsNew(KEY_INPUT_W)) { dir = VAdd(dir, { 0.0f, 0.0f, 1.0f }); }
	if (ins.IsNew(KEY_INPUT_A)) { dir = VAdd(dir, { -1.0f, 0.0f, 0.0f }); }
	if (ins.IsNew(KEY_INPUT_S)) { dir = VAdd(dir, { 0.0f, 0.0f, -1.0f }); }
	if (ins.IsNew(KEY_INPUT_D)) { dir = VAdd(dir, { 1.0f, 0.0f, 0.0f }); }

	// スペースキーで走る
	if (state_ != STATE::ATTACK && state_ != STATE::HIT)
	{

		// 待機状態
		if (AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::IDLE);
		}
		// 走る
		else if (ins.IsNew(KEY_INPUT_SPACE) && !AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::RUN);
			movePow = 20.0f;
		}
		// 歩く
		else if (!AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::WALK);
		}
	}

	// 攻撃処理
	if (ins.IsTrgDown(KEY_INPUT_J) && !hit_)
	{
		ChangeState(STATE::ATTACK);
		hit_ = true;
	}

	// プレイヤーが向いている方向にカメラを向ける
	auto rad = transform_.quaRot.ToEuler();
	if (ins.IsTrgDown(KEY_INPUT_Q))
	{
		// カメラの角度を基準とし、方向分の角度を加える
		SceneManager::GetInstance().GetCamera()->SetLazyAngles(rad);
	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK && state_ != STATE::HIT)
	{

		// 方向を正規化
		dir = VNorm(dir);

		// Y軸の行列
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// 回転行列を使用して、ベクトルを回転させる
		VECTOR moveDir = VTransform(dir, mat);

		// 方向×スピードで移動量を作って、座標に足して移動
		transform_.pos = VAdd(transform_.pos, VScale(moveDir, movePow));

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(dir.x, dir.z);

		// カメラの角度を基準とし、方向分の角度を加える
		LazyRotation(cameraAngles.y + angle);

	}

}

void Player::GamePadController(void)
{

	// カメラの角度を取得
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	auto& ins = InputManager::GetInstance();

	// 移動量
	float movePow = 10.0f;

	// 方向(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// ゲームパッドの番号を取得
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// 左のスティックの情報を取得する
	auto isTrgLStick = ins.IsPadLStickTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER);

	// パッドの方向をdirに直す
	dir.x = pad.AKeyLX;
	dir.z = -pad.AKeyLZ;

	// 攻撃処理
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) && !attack_)
	{
		ChangeState(STATE::ATTACK);
	}

	// Bボタンで走る
	if (state_ != STATE::ATTACK && state_ != STATE::HIT)
	{

		if (AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::IDLE);
		}
		else if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) && !AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::RUN);
			movePow = 20.0f;
		}
		else if (!AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::WALK);
		}

	}

	// プレイヤーが向いている方向にカメラを向ける
	auto rad = transform_.quaRot.ToEuler();
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_BOTTON))
	{
		// カメラの角度を基準とし、方向分の角度を加える
		SceneManager::GetInstance().GetCamera()->SetLazyAngles(rad);
	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK)
	{

		// 方向を正規化
		dir = VNorm(dir);

		// Y軸の行列
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// 回転行列を使用して、ベクトルを回転させる
		VECTOR moveDir = VTransform(dir, mat);

		// 方向×スピードで移動量を作って、座標に足して移動
		transform_.pos = VAdd(transform_.pos, VScale(moveDir, movePow));

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(pad.AKeyLX, -pad.AKeyLZ);

		// カメラの角度を基準とし、方向分の角度を加える
		LazyRotation(cameraAngles.y + angle);

	}

}

void Player::ChangeState(STATE state)
{

	// 状態の更新
	state_ = state;

	// 状態遷移時の初期化処理
	switch (state_)
	{
	case Player::STATE::IDLE:
		SetIdleAnimation();
		break;
	case Player::STATE::WALK:
		SetWalkAnimation();
		break;
	case Player::STATE::RUN:
		SetRunAnimation();
		break;
	case Player::STATE::ATTACK:
		attack_ = true;
		SetAttackAnimation();
		break;
	case Player::STATE::HIT:
		SetHitAnimation();
		break;
	}

}

void Player::SetIdleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, idleAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 20.0f;

}

void Player::SetWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, walkAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 30.0f;

}

void Player::SetRunAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, runAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 40.0f;

}

void Player::SetAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 60.0f;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Player::SetHitAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, hitAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 30.0f;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Player::LazyRotation(float goalRot)
{

	//// 回転処理
	//// クォータニオンからラジアン
	//float radNowAngleY = transform_.quaRot.ToEuler().y;
	//float degNowAngleY = AsoUtility::Rad2DegF(radNowAngleY);
	//float degGoalAngleY = AsoUtility::Rad2DegF(goalRot);

	//// 0度～360度以内に角度をおさめる
	////degGoalAngleY = static_cast<float>(AsoUtility::DegIn360(degGoalAngleY));

	//// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	//int aroundDir = AsoUtility::DirNearAroundDeg(degNowAngleY, degGoalAngleY);

	//// 回転量の作成
	//Quaternion rotPow = Quaternion::Identity();
	//rotPow = Quaternion::Mult(
	//	rotPow,
	//	Quaternion::AngleAxis(AsoUtility::Deg2RadF(3.0f * aroundDir), AsoUtility::AXIS_Y));

	////auto localRotRad = transform_.quaRot.ToEuler();
	////auto localRotDeg = AsoUtility::Rad2DegF(localRotRad.y);
	////auto goalRad = goalRot;

	//if (fabs(degGoalAngleY - degNowAngleY) >= 5)
	//{
	//	transform_.quaRot = Quaternion::Mult(transform_.quaRot, rotPow);
	//}
	//else
	//{
	//	// ラジアンからクォータニオン
	//	Quaternion qua = Quaternion::Euler(0, goalRot, 0);
	//	transform_.quaRot.y = qua.y;
	//}


	//auto* camera = SceneManager::GetInstance().GetCamera();

	//auto cameraRotY = camera->GetRotY();
	//transform_.quaRot = Quaternion::Slerp(transform_.quaRot, cameraRotY, 0.1f);


	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.1f);

}

void Player::DrawDebug(void)
{

	// HPバー
	int hpLength = 300;
	int H;
	int hpGauge;
	H = hp_ * (512.0f / hpMax_) - 100;
	int R = min(max((384 - H), 0), 0xff);
	int G = min(max((H + 64), 0), 0xff);
	int B = max((H - 384), 0);
	hpGauge = hpLength * hp_ / hpMax_;

	if (hp_ >= 0)
	{
		DrawBox(0, Application::SCREEN_SIZE_Y - 30, 500 + hpGauge, Application::SCREEN_SIZE_Y, GetColor(R, G, B), true);
	}

	auto rad = transform_.quaRot.ToEuler();

	VECTOR pDeg = { AsoUtility::Rad2DegF(rad.x),
					AsoUtility::Rad2DegF(rad.y),
					AsoUtility::Rad2DegF(rad.z) };
	DrawFormatString(0, 70, 0xffffff, "プレイヤー座標 : (%.1f, %.1f, %.1f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);
	DrawFormatString(0, 90, 0xffffff, "プレイヤー角度deg : (%.1f, %.1f, %.1f)", pDeg.x, pDeg.y, pDeg.z);
	DrawFormatString(0, 110, 0xffffff, "プレイヤー角度rad : (%.5f, %.5f, %.5f)", rad.x, rad.y, rad.z);

	// エネミー自身の衝突判定のカプセルの描画
	DrawCapsule3D(cBodyPosDown_, cBodyPosUp_, COLLISION_BODY_RADIUS, 10, 0xff0000, 0xff0000, false);

}

void Player::SetParam(void)
{

	// HPの最大値
	hpMax_ = 100;

	// HP
	hp_ = hpMax_;

}

void Player::Animation(void)
{

	// アニメーション再生
	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// アニメーション時間の進行
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		// ループ再生
		stepAnim_ = 0.0f;

		if (state_ == STATE::ATTACK || state_ == STATE::HIT)
		{
			ChangeState(STATE::IDLE);
		}
	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}