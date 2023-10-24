#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "Player.h"
#include "Enemy.h"

Enemy::Enemy(void)
{
}

Enemy::~Enemy(void)
{
}

void Enemy::InitAnimation(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::ENEMY_IDLE));

	// 待機アニメーション
	idleAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_IDLE);

	// 歩くアニメーション
	walkAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_WALK);

	// タックルアニメーション
	tackleAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_TACKLE);

	// 攻撃アニメーション
	attackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_ATTACK);

	// ダッシュ攻撃アニメーション
	jumpAttackAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_DASH_ATTACK);

	float scale = 2.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(180), AsoUtility::AXIS_Y));
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	// 再生するアニメーションの番号
	animNo_ = 0;

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// 再生中のアニメーション時間
	stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 0.0f;

	// ダッシュアタックのフラグ
	jumpAttack_ = false;

	// タックルアタックのフラグ
	tackleAttack_ = false;

}

void Enemy::Init(void)
{

	// アニメーションの初期設定
	InitAnimation();

	// プレイヤーのパラメーター
	SetParam();

	// 待機アニメーション
	SetIdleAnimation();

	// カプセルをアタッチするフレームの番号を検索
	enemyAttachFrameNum_ = MV1SearchFrame(followTransform_->modelId, "mixamorig:Spine2");

	// 武器をアタッチするフレームの番号を検索
	weponAttachFrameNum_ = MV1SearchFrame(followTransform_->modelId, "mixamorig:LeftHand");

	// フレーム番号をフレーム名で取得する
	enemyPosFrameNum_ = MV1SearchFrame(transform_.modelId, "mixamorig:Hips");

}

void Enemy::Draw(void)
{

	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(transform_.modelId);

	// デバッグ描画
	DrawDebug();

}

void Enemy::Release(void)
{

	// モデルの開放
	MV1DeleteModel(transform_.modelId);

}

const Transform& Enemy::GetTransform(void) const
{
	return transform_;
}

Enemy::STATE Enemy::GetState(void)
{
	return state_;
}

VECTOR Enemy::GetCBodyPosDown(void)
{
	return cBodyPosDown_;
}

VECTOR Enemy::GetCBodyPosUP(void)
{
	return cBodyPosUp_;
}

VECTOR Enemy::GetCWeponPosDown(void)
{
	return cWeponPosDown_;
}

VECTOR Enemy::GetCWeponPosUP(void)
{
	return cWeponPosUp_;
}

int Enemy::GetHP(void)
{
	return hp_;
}

void Enemy::SetHP(int hp)
{
	hp_ += hp;
}

void Enemy::SetFollow(const Transform* follow)
{

	followTransform_ = follow;

}

bool Enemy::GetAttack(void)
{
	return attack_;
}

void Enemy::SetAttack(bool attack)
{
	attack_ = attack;
}

void Enemy::Move(void)
{

	// 一個前の状態を保存
	preState_ = state_;

	// 回転処理
	Rotation();

	// 待機
	Idle();

	// 移動
	Walk();

	// 通常攻撃
	Attack();

	// ダッシュ攻撃
	//DashAttack();

	// タックル攻撃
	Tackle();

	// 衝突判定用
	Collision();

	// アニメーションの変更
	ChangeAnimation();

}

void Enemy::Rotation(void)
{

	// プレイヤーの方向を求める
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);

	// 正規化
	VECTOR Vdirection = VNorm(vec);

	// 方向を角度に変換する(XZ平面 Y軸)
	float angle = atan2f(Vdirection.x, Vdirection.z);

	// 回転
	if (state_ == STATE::IDLE || state_ == STATE::WALK)
	{
		LazyRotation(angle);
	}

}

void Enemy::Idle(void)
{

	// 方向(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// 待機状態
	if (!AsoUtility::EqualsVZero(dir))
	{
		ChangeState(STATE::IDLE);
	}

}

void Enemy::Walk(void)
{

	// プレイヤーの方向を求める
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);

	// 正規化
	VECTOR Vdirection = VNorm(vec);

	// 移動
	if (state_ != STATE::ATTACK && state_ != STATE::JUMP_ATTACK || state_ != STATE::TACKLE && stepAnim_ == 0.0f)
	{
		transform_.pos = VAdd(transform_.pos, VScale(Vdirection, 2.0f));
		ChangeState(STATE::WALK);
	}

}

void Enemy::Attack(void)
{

	// 方向(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// プレイヤーの方向を求める
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);
	float length = AsoUtility::Magnitude(vec);

	// 敵とプレイヤーの距離が一定距離になったら攻撃する
	if (length < 300.0f && AsoUtility::EqualsVZero(dir) && state_ != STATE::JUMP_ATTACK && stepAnim_ == 0.0f)
	{
		ChangeState(STATE::ATTACK);
	}

}

void Enemy::DashAttack(void)
{

	// プレイヤーの方向を求める
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);
	float length = AsoUtility::Magnitude(vec);

	// ジャンプ攻撃
	if (length > 2000.0f && stepAnim_ == 0.0f)
	{
		jumpAttack_ = true;
		attackPlayerPos_ = followTransform_->pos;
	}

	// 元居たプレイヤーの方向を求める
	VECTOR vec2 = VSub(attackPlayerPos_, transform_.pos);

	// 正規化
	VECTOR Vdirection = VNorm(vec2);

	// ジャンプ攻撃時にプレイヤーの方向に
	if (jumpAttack_)
	{
		ChangeState(STATE::JUMP_ATTACK);
		transform_.pos = VAdd(transform_.pos, VScale(Vdirection, 20.0f));
	}

}

void Enemy::Tackle(void)
{

	// プレイヤーの方向を求める
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);
	float length = AsoUtility::Magnitude(vec);

	// タックル攻撃
	if (length > 2000.0f && stepAnim_ == 0.0f)
	{
		tackleAttack_ = true;
		attackPlayerPos_ = followTransform_->pos;
		tackleCnt_ = TACKLE_TIME;
	}

	// タックル攻撃時にプレイヤー方向に突進
	if (tackleAttack_)
	{
		ChangeState(STATE::TACKLE);
		transform_.pos = VAdd(transform_.pos, VScale(pDirection_, 20.0f));
	}

}

void Enemy::Collision(void)
{

	// 敵自身の当たり判定
	EnemyBodyCollision();

	// 敵の武器の当たり判定
	WeponCollision();

	transform_.Update();

}

void Enemy::EnemyBodyCollision(void)
{

	// 敵の中心にカプセルを追従させる
	// カプセルをアタッチするフレームのローカル→ワールド変換行列を取得する
	MATRIX EnemyFrameMatrix = MV1GetFrameLocalWorldMatrix(transform_.modelId, enemyAttachFrameNum_);

	// 行列からラジアン
	auto rot = MGetRotElem(EnemyFrameMatrix);
	auto pos = MGetTranslateElem(EnemyFrameMatrix);

	// 行列からクォータニオン
	Quaternion qua = Quaternion::GetRotation(rot);

	// 追従対象の向き
	Quaternion followRot = qua;

	// 追従対象から自機までの相対座標
	VECTOR cPosDOWN = followRot.PosAxis(LOCAL_BODY_C_DOWN_POS);
	VECTOR cPosUP = followRot.PosAxis(LOCAL_BODY_C_UP_POS);

	// 敵の位置の更新
	cBodyPosDown_ = VAdd(pos, cPosDOWN);
	cBodyPosUp_ = VAdd(pos, cPosUP);

}

void Enemy::WeponCollision(void)
{

	// エネミー武器にカプセルを追従させる
	// カプセルをアタッチするフレームのローカル→ワールド変換行列を取得する
	MATRIX WeponFrameMatrix = MV1GetFrameLocalWorldMatrix(transform_.modelId, weponAttachFrameNum_);

	// 行列からラジアン
	auto rot = MGetRotElem(WeponFrameMatrix);
	auto pos = MGetTranslateElem(WeponFrameMatrix);

	// 行列からクォータニオン
	Quaternion qua = Quaternion::GetRotation(rot);

	// 追従対象の向き
	Quaternion followRot = qua;

	// 追従対象から自機までの相対座標
	VECTOR cPosDOWN = followRot.PosAxis(LOCAL_WEPON_C_DOWN_POS);
	VECTOR cPosUP = followRot.PosAxis(LOCAL_WEPON_C_UP_POS);

	// 敵の位置の更新
	//cWeponPosDown_ = VAdd(transform_.pos, cPosDOWN);
	//cWeponPosUp_ = VAdd(transform_.pos, cPosUP);
	cWeponPosDown_ = VAdd(pos, cPosDOWN);
	cWeponPosUp_ = VAdd(pos, cPosUP);

}

void Enemy::ChangeState(STATE state)
{

	// 状態の更新
	state_ = state;

}

void Enemy::SetIdleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, idleAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 20.0f;

}

void Enemy::SetWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, walkAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 20.0f;

}

void Enemy::SetTackleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, tackleAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 20.0f;

	// プレイヤーの方向を求める
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);
	float length = AsoUtility::Magnitude(vec);

	// プレイヤーの方向を求める
	VECTOR vec2 = VSub(attackPlayerPos_, transform_.pos);

	// 正規化
	pDirection_ = VNorm(vec2);

}

void Enemy::SetAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, attackAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 20.0f;

}

void Enemy::SetJumpAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, jumpAttackAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 20.0f;

}

void Enemy::ChangeAnimation(void)
{

	if (state_ == preState_) return;

	//stepAnim_ = 0.0f;
	switch (state_)
	{
	case Enemy::STATE::IDLE:
		SetIdleAnimation();
		break;
	case Enemy::STATE::WALK:
		SetWalkAnimation();
		break;
	case Enemy::STATE::TACKLE:
		SetTackleAnimation();
		attack_ = true;
		break;
	case Enemy::STATE::ATTACK:
		SetAttackAnimation();
		attack_ = true;
		break;
	case Enemy::STATE::JUMP_ATTACK:
		SetJumpAttackAnimation();
		attack_ = true;
		break;
	}

	// 攻撃のフラグをtrueに直す
	if (Enemy::STATE::IDLE == state_ ||
		Enemy::STATE::WALK == state_)
	{
		attack_ = true;
	}

}

void Enemy::LazyRotation(float goalRot)
{

	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.02f);

}

void Enemy::DrawDebug(void)
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
		DrawBox(700, 0, 700 + hpGauge, 30, GetColor(R, G, B), true);
	}

	// エネミー自身の衝突判定のカプセルの描画
	DrawCapsule3D(cBodyPosDown_, cBodyPosUp_, COLLISION_BODY_RADIUS, 10, 0xff0000, 0xff0000, false);

	// エネミー武器の衝突判定のカプセルの描画
	DrawCapsule3D(cWeponPosDown_, cWeponPosUp_, COLLISION_WEPON_RADIUS, 10, 0xff0000, 0xff0000, false);

}

void Enemy::SetParam(void)
{

	// HPの最大値
	hpMax_ = 100;

	// HP
	hp_ = hpMax_;

}

void Enemy::Animation(void)
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

		if (state_ == STATE::ATTACK || state_ == STATE::JUMP_ATTACK)
		{
			ChangeState(STATE::IDLE);
			SetIdleAnimation();
		}

	}

	// ジャンプ攻撃のフラグをfalseに
	if (state_ == STATE::JUMP_ATTACK && stepAnim_ >= 40.0f)
	{
		jumpAttack_ = false;
	}

	// タックル攻撃のフラグをfalseに
	VECTOR vec = VSub(attackPlayerPos_, transform_.pos);
	float length = AsoUtility::Magnitude(vec);

	// 突進する秒数のカウント
	tackleCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	if (tackleCnt_ <= 0.0f)
	{
		tackleCnt_ = 0.0f;
	}

	// 突進のカウントが終了したら突進を止める
	if (tackleCnt_ <= 0.0f && tackleAttack_)
	{
		tackleAttack_ = false;
		ChangeState(STATE::IDLE);
	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

	// アニメーションの固定
	AnimationFrame();

}

void Enemy::AnimationFrame(void)
{

	// 対象フレームのローカル行列を初期値にリセットする
	MV1ResetFrameUserLocalMatrix(transform_.modelId, enemyPosFrameNum_);

	// ジャンプ攻撃時に座標を固定する
	if (state_ == STATE::JUMP_ATTACK)
	{

		// 対象フレームのローカル行列(大きさ、回転、位置)を取得する
		auto mat = MV1GetFrameLocalMatrix(transform_.modelId, enemyPosFrameNum_);

		auto scl = MGetSize(mat); // 行列から大きさを取り出す
		auto rot = MGetRotElem(mat); // 行列から回転を取り出す
		auto pos = MGetTranslateElem(mat); // 行列から移動値を取り出す

		// 大きさ、回転、位置をローカル行列に戻す
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // 大きさ
		mix = MMult(mix, rot); // 回転

		// ここでローカル座標を行列に、そのまま戻さず、
		// 調整したローカル座標を設定する
		mix = MMult(mix, MGetTranslate({ 0.0f, pos.y, 0.0f }));

		// 合成した行列を対象フレームにセットし直して、
		// アニメーションの移動値を無効化
		MV1SetFrameUserLocalMatrix(transform_.modelId, enemyPosFrameNum_, mix);

	}

}
