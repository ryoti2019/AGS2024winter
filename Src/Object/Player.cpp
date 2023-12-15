#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Enemy.h"
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

	// 溜めながら歩くアニメーション
	chargeWalkAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_CHARGE_WALK);
	
	// 走るアニメーション
	runAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_RUN);

	// 攻撃アニメーション
	attackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_ATTACK);

	// 溜め攻撃アニメーション
	chargeAttackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_CHARGE_ATTACK);

	// ダメージヒットアニメーション
	hitAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_HIT);

	// 回避アニメーション
	rollAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_ROLL);
	
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

	// アニメーションの初期設定
	InitAnimation();

	// プレイヤーのパラメーター
	SetParam();

	// 待機アニメーション
	SetIdleAnimation();

	// カプセルをアタッチするフレームの番号を検索
	playerAttachFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:Hips");

	// 攻撃が当たったかどうか
	hit_ = false;

	// 攻撃したかどうか
	attack_ = false;

	// 攻撃１段階目
	attack1_ = false;

	// 攻撃２段階目
	attack2_ = false;

	// 攻撃３段階目
	attack3_ = false;

	// 攻撃４
	chargeAttack_ = false;

	moveDir_ = { 0.0f,0.0f,0.0f };

}

void Player::Update(void)
{

	// 必殺技になったら必殺技以外の処理を止める
	if (SceneManager::GetInstance().GetSceneID() == SceneManager::SCENE_ID::SPECIALMOVE)
	{
		// 必殺技の更新
  		SpecialMoveUpdate();
		return;
	}

	// 状態の遷移
	switch (state_)
	{
	case Player::STATE::IDLE:
		break;
	case Player::STATE::WALK:
		break;
	case Player::STATE::CHARGE_WALK:
		break;
	case Player::STATE::RUN:
		break;
	case Player::STATE::ATTACK:
		if (stepAnim_ >= ATTACK_COLLISION_START_TIME1
			&& stepAnim_ <= ATTACK_COLLISION_END_TIME1
			&& !hit_)
		{
			attack_ = true;
		}
		if (stepAnim_ >= ATTACK_END_TIME1 - 1.0f)
		{
			attack1_ = false;
		}
		break;
	case Player::STATE::ATTACK2:
		if (stepAnim_ >= ATTACK_COLLISION_START_TIME2
			&& stepAnim_ <= ATTACK_COLLISION_END_TIME2
			&& !hit_)
		{
			attack_ = true;
		}
		if (stepAnim_ >= ATTACK_END_TIME2 - 1.0f)
		{
			attack2_ = false;
		}
		break;
	case Player::STATE::ATTACK3:
		if (stepAnim_ >= ATTACK_COLLISION_START_TIME3
			&& stepAnim_ <= ATTACK_COLLISION_END_TIME3
			&& !hit_)
		{
			attack_ = true;
		}
		break;
	case Player::STATE::CHARGE_ATTACK:
		if (stepAnim_ >= CHARGE_ATTACK_COLLISION_START_TIME
			&& stepAnim_ <= CHARGE_ATTACK_COLLISION_END_TIME
			&& !hit_)
		{
			attack_ = true;
		}
		break;
	case Player::STATE::HIT:
		break;
	case Player::STATE::ROLL:
		if (stepAnim_ >= 45.0f)
		{
			speed_ = 0.0f;
		}
		break;
	}

	// キーボードでの操作
	if (!SceneManager::GetInstance().GetGamePad())
	{
		KeyboardContoroller();
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

	// HPバー
	DrawHPBar();

}

void Player::DrawHPBar(void)
{
	// HPバー
	int hpLength = HP_LENGTH;
	int H;
	int hpGauge;
	H = hp_ * (512.0f / hpMax_) - 100;
	int R = min(max((384 - H), 0), 0xff);
	int G = min(max((H + 64), 0), 0xff);
	int B = max((H - 384), 0);
	hpGauge = hpLength * hp_ / hpMax_;

	if (hp_ >= 0)
	{
		DrawBox(0, Application::SCREEN_SIZE_Y - 30, hpGauge, Application::SCREEN_SIZE_Y, GetColor(R, G, B), true);
	}
}

void Player::SpecialMoveUpdate(void)
{

	//　必殺技中の状態の遷移
	switch (specialState_)
	{
	case Player::SPECIAL_STATE::IDLE:
		break;
	}

	// 待機状態にする
	if (specialCnt_ <= 0.0f)
	{
		SpecialChangeState(SPECIAL_STATE::IDLE);
	}

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

void Player::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
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

void Player::KeyboardContoroller(void)
{

	// 移動処理
	KeyboardMove();

	//攻撃処理
	KeyboardAttack();

	// プレイヤー方向にカメラを向ける処理
	KeyBoardCamera();

	// 敵をロックオンする処理
	KeyBoardLockOn();

}

void Player::KeyboardMove(void)
{

	// カメラの取得
	Camera* camera = SceneManager::GetInstance().GetCamera();

	// カメラの角度を取得
	VECTOR cameraAngles = camera->GetAngles();

	auto& ins = InputManager::GetInstance();

	// 方向(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// WASDでプレイヤーの位置を変える
	//if (camera->GetMode() == Camera::MODE::FOLLOW)
	//{
	if (ins.IsNew(KEY_INPUT_W))
	{
		dir = VAdd(dir, { 0.0f, 0.0f, 1.0f });
	}
	//}
	//else if (camera->GetMode() == Camera::MODE::LOCKON)
	//{
	//	if (ins.IsNew(KEY_INPUT_W) && target2PlayerDis_ > enemyMinDis_)
	//	{
	//		dir = VAdd(dir, { 0.0f, 0.0f, 1.0f });
	//	}
	//}
	if (ins.IsNew(KEY_INPUT_A)) { dir = VAdd(dir, { -1.0f, 0.0f, 0.0f }); }
	if (ins.IsNew(KEY_INPUT_S)) { dir = VAdd(dir, { 0.0f, 0.0f, -1.0f }); }
	if (ins.IsNew(KEY_INPUT_D)) { dir = VAdd(dir, { 1.0f, 0.0f, 0.0f }); }

	// 移動
	if (state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL)
	{
		// 走る
		if (ins.IsNew(KEY_INPUT_LSHIFT) && !AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::RUN);
			speed_ = MOVE_POW_RUN;
		}
		// 歩く
		else if (!AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::WALK);
			// 移動量
			speed_ = MOVE_POW_WALK;
		}
		// 待機状態
		else if (AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::IDLE);
			speed_ = 0.0f;
		}
	}

	//溜めながら歩く
	if (ins.IsClickMouseLeft() && state_ != STATE::HIT)
	{
		speed_ = MOVE_POW_CHRAGE_WALK;
	}

	// 回避
	if (ins.IsTrgDown(KEY_INPUT_SPACE) && !AsoUtility::EqualsVZero(dir) &&
		state_ != STATE::HIT && state_ != STATE::ROLL)
	{

		ChangeState(STATE::ROLL);

		// 方向を正規化
		dir = VNorm(dir);

		// Y軸の行列
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(dir, mat);

		// 移動量
		speed_ = MOVE_POW_RUN;

	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL)
	{

		// 方向を正規化
		dir = VNorm(dir);

		// Y軸の行列
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngles.y));

		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(dir, mat);

		// ロックオン時は相手に近づくのに制限をつける
		if (camera->GetMode() == Camera::MODE::LOCKON)
		{
			LockOn();
		}

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(dir.x, dir.z);

		// カメラの角度を基準とし、方向分の角度を加える
		LazyRotation(cameraAngles.y + angle);

	}

	// 移動量
	movePow_ = VScale(moveDir_, speed_);

	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	// カメラの注視点
	auto cameraTargetPos = followTransform_->pos;

	float y = movedPos_.y;

	// XZ平面の移動後座標
	auto movedPosXZ = movedPos_;

	// XZ平面のカメラの注視点
	auto cameraTargetPosXZ = cameraTargetPos;

	// XZ平面のカメラ座標
	auto cameraPosXZ = camera->GetPos();

	// 移動後座標とカメラの注視点とカメラ座標を0にすることでXZ平面座標にしている
	movedPosXZ.y = cameraTargetPosXZ.y = cameraPosXZ.y = 0.0f;

	// 注視点からのプレイヤーのベクトル
	auto target2Player = VNorm(VSub(movedPosXZ, cameraTargetPosXZ));

	// 移動後座標と移動前座標が0以上の時
	if (!AsoUtility::EqualsVZero(moveDiff_))
	{

		// 注視点と移動後座標の距離
		target2PlayerDis_ = AsoUtility::Distance(cameraTargetPos, movedPos_);

		// 敵との最小限の距離
		enemyMinDis_ = 100.0f;

		// 注視点と敵との最小限の距離が100未満の時
		if (target2PlayerDis_ < enemyMinDis_)
		{

			// 注視点から移動後座標のベクトルをクォータニオンに
			auto rot = Quaternion::LookRotation(target2Player);

			// 移動後座標を更新
			movedPos_ = VAdd(cameraTargetPos, VScale(rot.GetForward(), enemyMinDis_ + 0.5f));
			movedPos_.y = y;

		}

	}

	//  移動
	moveDiff_ = VSub(movedPos_, transform_.pos);
	transform_.pos = movedPos_;

}

void Player::KeyboardAttack(void)
{

	auto& insInput = InputManager::GetInstance();
	auto& insScene = SceneManager::GetInstance();

	// 攻撃処理
	// ボタンがクリックされたかどうかを確認
	if (insInput.IsClickMouseLeft() && chargeCnt <= CHARGE_TIME && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::ATTACK && state_ != STATE::ATTACK2 && state_ != STATE::ATTACK3 && state_ != STATE::HIT)
	{
		chargeCnt += insScene.GetDeltaTime();
		ChangeState(STATE::CHARGE_WALK);
	}

	if (insInput.IsTrgUpMouseLeft() && chargeCnt <= CHARGE_TIME && state_ != STATE::HIT)
	{

		//ボタンが押されたらアニメーションを切り替える
		//１段階目
		if (state_ == STATE::IDLE || state_ == STATE::RUN || state_ == STATE::WALK)
		{
			attack_ = true;
			attack1_ = true;
			ChangeState(STATE::ATTACK);
		}
		// ２段階目
		else if (state_ == STATE::ATTACK && !attack2_)
		{
			attack2_ = true;
		}
		// 3段階目
		else if (state_ == STATE::ATTACK && attack2_)
		{
			attack3_ = true;
		}
		else if (state_ == STATE::ATTACK2)
		{
			attack3_ = true;
		}
	}

	// １段階目が終わったら遷移する
	if (attack2_ && !attack1_ && state_ == STATE::ATTACK)
	{
		chargeCnt = 0.0f;
		ChangeState(STATE::ATTACK2);
	}

	// ２段階目が終わったら遷移する
	if (attack3_ && !attack2_ && state_ == STATE::ATTACK2)
	{
		chargeCnt = 0.0f;
		ChangeState(STATE::ATTACK3);
	}

	// 溜め斬り
	if (chargeCnt >= CHARGE_TIME)
	{
		chargeAttack_ = true;
		chargeCnt = 0.0f;
		ChangeState(STATE::CHARGE_ATTACK);
	}

}

void Player::KeyBoardCamera(void)
{

	auto& ins = InputManager::GetInstance();

	// プレイヤーが向いている方向にカメラを向ける
	auto rad = transform_.quaRot.ToEuler();
	if (ins.IsTrgDown(KEY_INPUT_Q))
	{
		// カメラの角度を基準とし、方向分の角度を加える
		SceneManager::GetInstance().GetCamera()->SetLazyAngles(rad);
	}

}

void Player::KeyBoardLockOn(void)
{

	auto& ins = InputManager::GetInstance();

	auto camera = SceneManager::GetInstance().GetCamera();

	// プレイヤーの方向を求める
	auto length = AsoUtility::Distance(followTransform_->pos, transform_.pos);
	// キーを押したらロックオンする
	if (ins.IsTrgDown(KEY_INPUT_V) && length <= 3000)
	{
		camera->ChangeLockOnFlag();
	}

	if (length >= 3000 && camera->GetLockOn())
	{
		camera->ChangeMode(Camera::MODE::FOLLOW);
	}


}

void Player::GamePadController(void)
{

	// 移動処理
	GamePadMove();

	// 攻撃処理
	GamePadAttack();

	// プレイヤー方向にカメラを向ける処理
	GamePadCamera();

	// 敵をロックオンする処理
	GamePadLockOn();

}

void Player::GamePadMove(void)
{

	// カメラの角度を取得
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	auto& ins = InputManager::GetInstance();

	// 方向(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// 移動量
	float movePow = MOVE_POW_WALK;

	// ゲームパッドの番号を取得
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// 左のスティックの情報を取得する
	auto isTrgLStick = ins.IsPadLStickTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER);

	// パッドの方向をdirに直す
	dir.x = pad.AKeyLX;
	dir.z = -pad.AKeyLZ;

	// 移動
	if (state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL)
	{

		// 走る
		if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN) && !AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::RUN);
			movePow = MOVE_POW_RUN;
		}
		// 歩く
		else if (!AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::WALK);
		}
		// 待機状態
		else if (AsoUtility::EqualsVZero(dir))
		{
			ChangeState(STATE::IDLE);
		}

	}

	//溜めながら歩く
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)
		&& state_ != STATE::HIT)
	{
		movePow = MOVE_POW_CHRAGE_WALK;
	}

	// 回避
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN)
		&& state_ != STATE::HIT && state_ != STATE::ROLL)
	{
		ChangeState(STATE::ROLL);
	}

	if (!AsoUtility::EqualsVZero(dir) && state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::HIT && state_ != STATE::ROLL)
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

void Player::GamePadAttack(void)
{

	auto& insInput = InputManager::GetInstance();
	auto& insScene = SceneManager::GetInstance();

	// 攻撃処理
	// ボタンがクリックされたかどうかを確認
	if (insInput.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)
		&& chargeCnt <= CHARGE_TIME && state_ != STATE::CHARGE_ATTACK
		&& state_ != STATE::ATTACK && state_ != STATE::ATTACK2
		&& state_ != STATE::ATTACK3 && state_ != STATE::HIT)
	{
		chargeCnt += insScene.GetDeltaTime();
		ChangeState(STATE::CHARGE_WALK);
	}

	if (insInput.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)
		&& chargeCnt <= CHARGE_TIME && state_ != STATE::HIT)
	{

		//ボタンが押されたらアニメーションを切り替える
		//１段階目
		if (state_ == STATE::IDLE || state_ == STATE::RUN || state_ == STATE::WALK)
		{
			attack_ = true;
			attack1_ = true;
			ChangeState(STATE::ATTACK);
		}
		// ２段階目
		else if (state_ == STATE::ATTACK && !attack2_)
		{
			attack2_ = true;
		}
		// 3段階目
		else if (state_ == STATE::ATTACK && attack2_)
		{
			attack3_ = true;
		}
		else if (state_ == STATE::ATTACK2)
		{
			attack3_ = true;
		}
	}

	// １段階目が終わったら遷移する
	if (attack2_ && !attack1_ && state_ == STATE::ATTACK)
	{
		chargeCnt = 0.0f;
		ChangeState(STATE::ATTACK2);
	}

	// ２段階目が終わったら遷移する
	if (attack3_ && !attack2_ && state_ == STATE::ATTACK2)
	{
		chargeCnt = 0.0f;
		ChangeState(STATE::ATTACK3);
	}

	// 溜め斬り
	if (chargeCnt >= CHARGE_TIME)
	{
		chargeAttack_ = true;
		chargeCnt = 0.0f;
		ChangeState(STATE::CHARGE_ATTACK);
	}

}

void Player::GamePadCamera(void)
{

	auto& ins = InputManager::GetInstance();

	// プレイヤーが向いている方向にカメラを向ける
	auto rad = transform_.quaRot.ToEuler();
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_BOTTON))
	{
		// カメラの角度を基準とし、方向分の角度を加える
		SceneManager::GetInstance().GetCamera()->SetLazyAngles(rad);
	}

}

void Player::GamePadLockOn(void)
{

	auto& ins = InputManager::GetInstance();

	// キーを押したらロックオンする
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		
	}

	// キーを押したらロックオンを解除する
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		
	}

}

void Player::LockOn(void)
{

	// 敵に近づき敵の周りを走り続けるとカメラがプレイヤーを追わなくなる

	Camera* camera = SceneManager::GetInstance().GetCamera();

	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	// ①カメラがロックオンの時は、自動回転を行わない
	if (camera->GetMode() != Camera::MODE::LOCKON)
	{
		// 移動方向に応じた回転
		Rotate();
	}

	//// ②移動処理前に、ターゲットとの特別衝突判定
	//moveDiff_ = VSub(movedPos_, transform_.pos);

	//// ロックオンモードの時
	//if (camera->GetMode() == Camera::MODE::LOCKON)
	//{

	//	// カメラの注視点
	//	auto cameraTargetPos = camera->GetTargetPos();

	//	float y = movedPos_.y;

	//	// XZ平面の移動後座標
	//	auto movedPosXZ = movedPos_;

	//	// XZ平面のカメラの注視点
	//	auto cameraTargetPosXZ = cameraTargetPos;

	//	// XZ平面のカメラ座標
	//	auto cameraPosXZ = camera->GetPos();

	//	// 移動後座標とカメラの注視点とカメラ座標を0にすることでXZ平面座標にしている
	//	movedPosXZ.y = cameraTargetPosXZ.y = cameraPosXZ.y = 0.0f;

	//	// 注視点からのプレイヤーのベクトル
	//	auto target2Player = VNorm(VSub(movedPosXZ, cameraTargetPosXZ));

	//	//// 目的の角度までの差を測る
	//	//SetGoalRotate(Quaternion::LookRotation(VScale(target2Player, -1.0)));

	//	//// 少しずつ回転させる
	//	//Rotate();

	//	// 移動後座標と移動前座標が0以上の時
	//	if (!AsoUtility::EqualsVZero(moveDiff_))
	//	{

	//		// 注視点と移動後座標の距離
	//		target2PlayerDis_ = AsoUtility::Distance(cameraTargetPos, movedPos_);

	//		// 敵との最小限の距離
	//		enemyMinDis_ = 100.0f;

	//		// 注視点と敵との最小限の距離が100未満の時
	//		if (target2PlayerDis_ < enemyMinDis_)
	//		{

	//			// 注視点から移動後座標のベクトルをクォータニオンに
	//			auto rot = Quaternion::LookRotation(target2Player);

	////			// 右方向
	////			auto r = rot.GetRight();

	////			// 左方向
	////			auto l = rot.GetLeft();

	////			// 右方向の内積
	////			auto dotR = VDot(r, target2Player);

	////			// 左方向の内積
	////			auto dotL = VDot(l, target2Player);

	////			float deg = 2.0f;


	////			// 右か左かを選ぶ処理

	////			if (dotR + 0.01f > dotL)

	////			{

	////				deg *= -1.0f;

	////				// キャラが右に回る

	////				rot = rot.Mult(

	////					Quaternion::AngleAxis(-deg * DX_PI_F / 180.0f, AsoUtility::AXIS_Y));

	////				// 内積の大きいほうに角度を足す
	////				camera->AddLockOnAnglesY(deg * DX_PI_F / 180.0f);


	////			}

	////			else

	////			{

	////				// キャラが左に回る

	////				rot = rot.Mult(

	////					Quaternion::AngleAxis(deg * DX_PI_F / 180.0f, AsoUtility::AXIS_Y));

	////				camera->AddLockOnAnglesY(deg * DX_PI_F / 180.0f);

	////			}


	//			movedPos_ = VAdd(cameraTargetPos, VScale(rot.GetForward(), enemyMinDis_ + 0.5f));
	//			movedPos_.y = y;

	//		}

	//	}

	//}


	//// 移動

	//moveDiff_ = VSub(movedPos_, transform_.pos);

	//transform_.pos = movedPos_;


	// ③回転完了までの時間短縮


}

// 補足関数
void Player::SetGoalRotate(Quaternion rot)
{

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(rot, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		// 回転完了の時間
		stepRotTime_ = TIME_ROT;
	}

	// 目的の角度
	goalQuaRot_ = rot;

}

void Player::Rotate(void)
{

	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

	// 回転の球面補間
	transform_.quaRot = Quaternion::Slerp(
		transform_.quaRot, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

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
	case Player::STATE::CHARGE_WALK:
		SetChargeWalkAnimation();
		break;
	case Player::STATE::RUN:
		SetRunAnimation();
		break;
	case Player::STATE::ATTACK:
		hit_ = false;
		SetAttackAnimation();
		break;
	case Player::STATE::ATTACK2:
		hit_ = false;
		SetAttackAnimation2();
		break;
	case Player::STATE::ATTACK3:
		hit_ = false;
		SetAttackAnimation3();
		break;
	case Player::STATE::CHARGE_ATTACK:
		hit_ = false;
		SetChargeAttackAnimation();
		break;
	case Player::STATE::HIT:
		SetHitAnimation();
		break;
	case Player::STATE::ROLL:
		SetRollAnimation();
		break;
	}

}

void Player::SpecialChangeState(SPECIAL_STATE state)
{
	// 状態の更新
	specialState_ = state;

	// 状態遷移時の初期化処理
	switch (specialState_)
	{
	case Player::SPECIAL_STATE::IDLE:
		SetIdleAnimation();
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
	speedAnim_ = IDLE_ANIM_SPEED;

}

void Player::SetWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, walkAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = WALK_ANIM_SPEED;

}

void Player::SetChargeWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, chargeWalkAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = CHARGE_WALK_ANIM_SPEED;

}

void Player::SetRunAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, runAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = RUN_ANIM_SPEED;

}

void Player::SetAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = ATTACK_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Player::SetAttackAnimation2(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = ATTACK_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = ATTACK_START_TIME2;

}

void Player::SetAttackAnimation3(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = ATTACK_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = ATTACK_START_TIME3;

}

void Player::SetChargeAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, chargeAttackAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = CHARGE_ATTACK_ANIM;

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
	speedAnim_ = HIT_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Player::SetRollAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, rollAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = ROLL_ANIM_SPEED;

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
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, ROTATION_POW);

}

void Player::DrawDebug(void)
{

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
	hpMax_ = HP_MAX;

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

		if (state_ == STATE::ATTACK || state_ == STATE::ATTACK2
			|| state_ == STATE::ATTACK3 || state_ == STATE::CHARGE_ATTACK 
			|| state_ == STATE::HIT || state_ == STATE::ROLL)
		{
			stepAnim_ = 0.0f;
			attack1_ = false;
			attack2_ = false;
			attack3_ = false;
			chargeAttack_ = false;

			hit_ = false;
			ChangeState(STATE::IDLE);
			chargeCnt = 0.0f;
		}
	}

	// 2段階目に進まないときはリセット
	if (state_ == STATE::ATTACK && stepAnim_ >= ATTACK_END_TIME1 && !attack2_)
	{
		stepAnim_ = 0.0f;
		hit_ = false;
		ChangeState(STATE::IDLE);
		chargeCnt = 0.0f;
	}

	// 3段階目に進まないときはリセット
	if (state_ == STATE::ATTACK2 && stepAnim_ >= ATTACK_END_TIME2 && !attack3_)
	{
		stepAnim_ = 0.0f;
		hit_ = false;
		ChangeState(STATE::IDLE);
		chargeCnt = 0.0f;
	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

	// アニメーションの固定
	AnimationFrame();

}

void Player::AnimationFrame(void)
{

	// 対象フレームのローカル行列を初期値にリセットする
	MV1ResetFrameUserLocalMatrix(transform_.modelId, playerAttachFrameNum_);

	// ジャンプ攻撃時に座標を固定する
	if (state_ == STATE::ATTACK || state_ == STATE::ATTACK2 || state_ == STATE::ATTACK3 || state_ == STATE::CHARGE_ATTACK || state_ == STATE::ROLL)
	{

		// 対象フレームのローカル行列(大きさ、回転、位置)を取得する
		auto mat = MV1GetFrameLocalMatrix(transform_.modelId, playerAttachFrameNum_);

		auto scl = MGetSize(mat); // 行列から大きさを取り出す
		auto rot = MGetRotElem(mat); // 行列から回転を取り出す
		auto pos = MGetTranslateElem(mat); // 行列から移動値を取り出す

		// 大きさ、回転、位置をローカル行列に戻す
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // 大きさ
		mix = MMult(mix, rot); // 回転

		// ここでローカル座標を行列に、そのまま戻さず、
		// 調整したローカル座標を設定する
		mix = MMult(mix, MGetTranslate({ pos.x, pos.y, 0.0f }));

		// 合成した行列を対象フレームにセットし直して、
		// アニメーションの移動値を無効化
		MV1SetFrameUserLocalMatrix(transform_.modelId, playerAttachFrameNum_, mix);

	}

}