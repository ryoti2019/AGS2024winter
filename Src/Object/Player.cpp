#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{

	// アニメーションの初期設定
	InitAnimation();

}

void Player::InitAnimation(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_WALK));
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 200.0f, 0.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	animNo_ = 1;

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 20.0f;

	// モデルに指定時間のアニメーションを設定する
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Player::Update(void)
{

	// 移動処理
	Move();

	// アニメーション処理
	Animation();

	//// モデルの移動設定
	//MV1SetPosition(transform_.modelId, transform_.pos);

	transform_.Update();
}

void Player::Draw(void)
{

	MV1DrawModel(transform_.modelId);

}

void Player::Release(void)
{

	MV1DeleteModel(transform_.modelId);

}

const Transform& Player::GetTransform(void) const
{
	return transform_;
}

void Player::Move(void)
{

	// カメラの角度を取得
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	auto& ins = InputManager::GetInstance();
	float movePow = 10.0f;

	// 左のシフトキーで走る
	if (ins.IsNew(KEY_INPUT_SPACE))
	{
		ChangeState(STATE::RUN);
		movePow = 20.0f;
	}
	else if (!ins.IsNew(KEY_INPUT_SPACE))
	{
		ChangeState(STATE::WALK);
	}

	switch (state_)
	{
	case Player::STATE::WALK:
		SetWalkAnimation();
		break;
	case Player::STATE::RUN:
		SetRunAnimation();
		break;
	}

	// 方向(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// WASDでカメラの位置を変える
	if (ins.IsNew(KEY_INPUT_W)) { dir = { 0.0f, 0.0f, 1.0f }; }
	if (ins.IsNew(KEY_INPUT_A)) { dir = { -1.0f, 0.0f, 0.0f }; }
	if (ins.IsNew(KEY_INPUT_S)) { dir = { 0.0f, 0.0f, -1.0f }; }
	if (ins.IsNew(KEY_INPUT_D)) { dir = { 1.0f, 0.0f, 0.0f }; }

	if (!AsoUtility::EqualsVZero(dir))
	{

		// XYZの回転行列
		// XZ平面移動にする場合は、XZの回転を考慮しないようにする
		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotX(angles_.x));
		mat = MMult(mat, MGetRotY(cameraAngles.y));
		//mat = MMult(mat, MGetRotZ(angles_.z));

		// 回転行列を使用して、ベクトルを回転させる
		VECTOR moveDir = VTransform(dir, mat);

		// 方向×スピードで移動量を作って、座標に足して移動
		transform_.pos = VAdd(transform_.pos, VScale(moveDir, movePow));

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(dir.x, dir.z);

		// カメラの角度を基準とし、方向分の角度を加える
		//rot_.y = cameraAngles.y + angle;

		// カメラの角度を基準とし、方向分の角度を加える
		//LazyRotation(cameraAngles.y + angle);

	}

}

void Player::ChangeState(STATE state)
{

	// 状態の更新
	state_ = state;

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
	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_WALK), animAttachNo_, stepAnim_);

}

void Player::SetWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
	//stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 20.0f;

	// モデルに指定時間のアニメーションを設定する
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Player::SetRunAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
	//stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 20.0f;

	// モデルに指定時間のアニメーションを設定する
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}
