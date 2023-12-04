#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "ShotEnemy.h"
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

	// 弾を生成するアニメーション
	createAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_SHOT_CREATE);

	// ショットアニメーション
	shotAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_SHOT_ATTACK);

	// 攻撃を食らったときのアニメーション
	hitAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_HIT);

	// transformの初期化
	float scale = 2.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 1000.0f };
	transform_.quaRot = Quaternion();
	transform_.Update();

	// 再生するアニメーションの番号
	animNo_ = 0;

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// 再生中のアニメーション時間
	stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 0.0f;

	// 待機状態の弾の数
	shotNum_ = 0;

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

	// 攻撃中かどうか
	attack_ = false;

	// ダッシュアタックのフラグ
	jumpAttack_ = false;

	// タックルアタックのフラグ
	tackleAttack_ = false;

	// 回転の終了のフラグ
	rotationEnd_ = false;

	// 弾を発射したかどうか
	isShot_ = false;

	// 当たったかのフラグ
	hit_ = false;

	// 最初の歩きのアニメーションのカウンタ
	walkCnt_ = 0.0f;

	// 初期状態
	ChangeState(STATE::THINK);

}

void Enemy::Update(void)
{

	// 回転しきっていなかったら処理しない
	if (rotationEnd_)
	{
		return;
	}

	switch (state_)
	{
	case Enemy::STATE::THINK:
		break;
	case Enemy::STATE::IDLE:
		UpdateIdle();
		break;
	case Enemy::STATE::WALK:
		UpdateWalk();
		break;
	case Enemy::STATE::ATTACK:
		if (stepAnim_ >= ATTACK_COLLISION_START_TIME
			&& stepAnim_ <= ATTACK_COLLISION_END_TIME
			&& !hit_)
		{
			attack_ = true;
		}
		UpdateAttack();
		break;
	case Enemy::STATE::JUMP_ATTACK:
		if (stepAnim_ >= JUMP_ATTACK_COLLISION_START_TIME
			&& stepAnim_ <= JUMP_ATTACK_COLLISION_END_TIME
			&& !hit_)
		{
			attack_ = true;
		}
		UpdateJumpAttack();
		break;
	case Enemy::STATE::TACKLE:
		UpdateTackle();
		break;
	case Enemy::STATE::CREATE:
		UpdateCreate();
		break;
	case Enemy::STATE::SHOT:
		UpdateShot();
		break;
	case Enemy::STATE::HIT:
		//UpdateHit();
		break;
	}

	// アニメーション処理
	Animation();

	// 回転処理
	Rotation();

	// 衝突判定用
	Collision();

	transform_.Update();

	walkCnt_ += SceneManager::GetInstance().GetDeltaTime();

	for (auto v : shots_)
	{
		v->Update();
	}

}

void Enemy::Draw(void)
{

	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(transform_.modelId);

	// デバッグ描画
	DrawDebug();

	for (auto v : shots_)
	{
		v->Draw();
	}

}

void Enemy::Release(void)
{

	// モデルの開放
	MV1DeleteModel(transform_.modelId);

	for (auto& s : shots_)
	{
		s->Release();
		delete s;
	}
	shots_.clear();

}

const Transform& Enemy::GetTransform(void) const
{
	return transform_;
}

Enemy::STATE Enemy::GetState(void)
{
	return state_;
}

void Enemy::SetState(Enemy::STATE state)
{
	ChangeState(state);
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

bool Enemy::GetHit(void)
{
	return hit_;
}

void Enemy::SetHit(bool hit)
{
	hit_ = hit;
}

void Enemy::SetDeathCnt(int cnt)
{
	shotNum_ += cnt;
}

std::vector<ShotEnemy*>& Enemy::GetShots(void)
{
	return shots_;
}

void Enemy::Think(void)
{

	// ベクトル方向
	VECTOR vec = { 0.0f,0.0f,0.0f };

	// 距離
	float length = 0.0f;

	// 攻撃の選択
	attackNumber_ = GetRand(2);
	
	// 攻撃が当たったかどうか
	hit_ = false;

	// 移動 --------------------------------------------------

	// 移動
	if (state_ != STATE::ATTACK && state_ != STATE::JUMP_ATTACK || state_ != STATE::TACKLE && stepAnim_ == 0.0f)
	{
		ChangeState(STATE::WALK);
	}

	if (walkCnt_ <= FIRST_WALK_TIME)
	{
		return;
	}

	// 通常攻撃 ----------------------------------------------

	// プレイヤーの方向を求める
	vec = VSub(followTransform_->pos, transform_.pos);
	length = AsoUtility::Magnitude(vec);

	// 敵とプレイヤーの距離が一定距離になったら攻撃する
	if (length < ATTACK_RANGE)
	{
		ChangeState(STATE::ATTACK);
	}

	// ジャンプ攻撃 ------------------------------------------

	if (attackNumber_ == 0)
	{

		// プレイヤーがいた座標を代入
		attackPlayerPos_ = followTransform_->pos;

		// プレイヤーの方向を求める
		vec = VSub(attackPlayerPos_, transform_.pos);
		length = AsoUtility::Magnitude(vec);

		// 正規化
		pDirection_ = VNorm(vec);

		// ジャンプ攻撃
		if ((length > JUMP_ATTACK_RANGE_MIN && length < JUMP_ATTACK_RANGE_MAX) || length < ATTACK_RANGE)
		{
			ChangeState(STATE::JUMP_ATTACK);
		}

	}

	// タックル攻撃 ------------------------------------------

	if (attackNumber_ == 1)
	{

		// プレイヤーがいた座標を代入
		attackPlayerPos_ = followTransform_->pos;

		// プレイヤーの方向を求める
		vec = VSub(attackPlayerPos_, transform_.pos);
		length = AsoUtility::Magnitude(vec);

		// 正規化
		pDirection_ = VNorm(vec);

		// タックルし続ける時間
		tackleCnt_ = TACKLE_TIME;

		// タックル攻撃
		if (length > ATTACK_RANGE)
		{
			ChangeState(STATE::TACKLE);
		}

	}

	// ショット攻撃-------------------------------------------

	if (attackNumber_ == 2)
	{
		ChangeState(STATE::CREATE);
	}

}

void Enemy::Rotation(void)
{

	// プレイヤーの方向を求める
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);

	vec = { -vec.x,-vec.y,-vec.z };

	// 正規化
	VECTOR Vdirection = VNorm(vec);

	// 方向を角度に変換する(XZ平面 Y軸)
	float angle = atan2f(Vdirection.x, Vdirection.z);

	// 回転
	if (state_ == STATE::IDLE || state_ == STATE::WALK || state_ == STATE::SHOT)
	{
		LazyRotation(angle );
	}

}

void Enemy::AfterRotation(void)
{

	// プレイヤーの方向を求める
	VECTOR vec = VSub(followTransform_->pos, transform_.pos);

	vec = { -vec.x,-vec.y,-vec.z };

	// 正規化
	VECTOR Vdirection = VNorm(vec);

	// 方向を角度に変換する(XZ平面 Y軸)
	float angle = atan2f(Vdirection.x, Vdirection.z);

	auto goal = Quaternion::Euler(0.0f, angle, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.02f);

	// クォータニオンからラジアン
	VECTOR rad = transform_.quaRot.ToEuler();

	// ラジアンからデグリー
	float deg = AsoUtility::Rad2DegF(rad.y);
	deg = AsoUtility::DegIn360(deg);

	// ラジアンからデグリー
	float goalDeg = AsoUtility::Rad2DegF(angle);
	goalDeg = AsoUtility::DegIn360(goalDeg);

	// 目的の角度と自分の角度の差を測る
	float sub = goalDeg - deg;

	// 差が10度未満だったらtrueにする
	if (sub <= GOAL_DEG && sub >= -GOAL_DEG)
	{
		rotationEnd_ = true;
	}

}

void Enemy::UpdateIdle(void)
{
}

void Enemy::UpdateWalk(void)
{

	// プレイヤーの方向を求める
	auto vec = VSub(followTransform_->pos, transform_.pos);

	// 正規化
	pDirection_ = VNorm(vec);

	// 移動処理
	transform_.pos = VAdd(transform_.pos, VScale(pDirection_, WALK_SPEED));

}

void Enemy::UpdateAttack(void)
{

	attack_ = false;

	// 攻撃判定が入るアニメーションの秒数
	if (stepAnim_ >= ATTACK_COLLISION_START_TIME && stepAnim_ <= ATTACK_COLLISION_END_TIME && !hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateJumpAttack(void)
{

	// プレイヤーとの距離を求める
	VECTOR vec = VSub(attackPlayerPos_, transform_.pos);
	float length = AsoUtility::Magnitude(vec);
	attack_ = false;

	// プレイヤーとの距離が10.0f未満になるまで移動
	if (stepAnim_ <= JUMP_ATTACK_END_TIME)
	{
		if (length >= JUMP_ATTACK_RANGE_MIN)
		{
			transform_.pos = VAdd(transform_.pos, VScale(pDirection_, JUMP_ATTACK_SPEED));
		}
	}

	// 攻撃判定が入るアニメーションの秒数
	if (stepAnim_ >= JUMP_ATTACK_COLLISION_START_TIME && stepAnim_ <= JUMP_ATTACK_COLLISION_END_TIME && !hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateTackle(void)
{

	// タックルする時間を計算
	tackleCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	// タックルし続ける間は座標を動かす
	if (tackleCnt_ > 0.0f)
	{
		// 移動処理
		transform_.pos = VAdd(transform_.pos, VScale(pDirection_, TACKLE_SPEED));
	}

	// 終わったらIDLEに戻す
	if (tackleCnt_ < 0.0f)
	{
		ChangeState(STATE::IDLE);
	}
	
	// 攻撃フラグ
	if (!hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateCreate(void)
{

	// 弾の生成
	CreateShot();

}

void Enemy::UpdateShot(void)
{

	// 弾の発射処理
	ProcessShot();

	// 攻撃フラグ
	if (!hit_)
	{
		attack_ = true;
	}

}

void Enemy::UpdateHit(void)
{

	// プレイヤーとの距離を求める
	VECTOR vec = VSub(attackPlayerPos_, transform_.pos);
	pDirection_ = VNorm(vec);

	if (stepAnim_ <= HIT_END_TIME)
	{
		transform_.pos = VAdd(transform_.pos, VScale(pDirection_, -HIT_SPEED));
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

	// 状態遷移時の初期化処理
	switch (state_)
	{
	case Enemy::STATE::THINK:
		// 回転のフラグを戻す
		rotationEnd_ = false;
		// これからの行動を考える
		//Think();
		break;
	case Enemy::STATE::IDLE:
		SetIdleAnimation();
		break;
	case Enemy::STATE::WALK:
		SetWalkAnimation();
		break;
	case Enemy::STATE::ATTACK:
		SetAttackAnimation();
		break;
	case Enemy::STATE::JUMP_ATTACK:
		SetJumpAttackAnimation();
		break;
	case Enemy::STATE::TACKLE:
		SetTackleAnimation();
		break;
	case Enemy::STATE::CREATE:
		SetCreateAnimation();
		break;
	case Enemy::STATE::SHOT:
		delayShot_ = TIME_DELAY_SHOT;
		SetShotAnimation();
		break;
	case Enemy::STATE::HIT:
		attackPlayerPos_ = followTransform_->pos;
		SetHitAnimation();
		break;
	}

}

void Enemy::SetIdleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, idleAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = IDLE_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Enemy::SetWalkAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, walkAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = WALK_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Enemy::SetAttackAnimation(void)
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

void Enemy::SetJumpAttackAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, jumpAttackAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = JUMP_ATTACK_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Enemy::SetTackleAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, tackleAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = TACKLE_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 0.0f;

}

void Enemy::SetCreateAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, createAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = SHOT_CREATE_END_TIME;

	// アニメーション速度
	speedAnim_ = SHOT_CREATE_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = SHOT_CREATE_START_TIME;

}

void Enemy::SetShotAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, shotAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = SHOT_END_TIME;

	// アニメーション速度
	speedAnim_ = SHOT_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = SHOT_START_TIME;

}

void Enemy::SetHitAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, hitAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = HIT_ANIM_SPEED;

	// アニメーション時間の初期化
	stepAnim_ = 10.0f;

}

void Enemy::LazyRotation(float goalRot)
{

	// 目的の角度まで回転させる
	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, 0.02f);

}

void Enemy::DrawDebug(void)
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

	// HPの描画
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
	hpMax_ = HP_MAX;

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
	if (state_ != STATE::SHOT || shotNum_ == 0)
	{
		isShot_ = false;
		if (stepAnim_ > animTotalTime_)
		{

			// ループ再生
			stepAnim_ = 0.0f;

			if (state_ != STATE::TACKLE)
			{
				// 待機状態にする
				ChangeState(STATE::IDLE);
			}

		}

	}

	if (state_ == STATE::SHOT && stepAnim_ >= SHOT_END_TIME)
	{
		stepAnim_ = SHOT_START_TIME;
		isShot_ = false;
	}

	// 行動後プレイヤー方向に角度を変える
	if (state_ == STATE::IDLE)
	{
		AfterRotation();
	}

	// 行動を選択
	if (rotationEnd_)
	{
		ChangeState(STATE::THINK);
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
	if (state_ == STATE::JUMP_ATTACK || state_ == STATE::HIT)
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

void Enemy::ProcessShot(void)
{

	auto& ins = InputManager::GetInstance();

	delayShot_ -= SceneManager::GetInstance().GetDeltaTime();

	// 弾を時間をずらして飛ばす
	if (stepAnim_ >= 35.0f && !isShot_)
	{
		Shot();
		isShot_ = true;
	}

}

void Enemy::Shot(void)
{

	if (shotNum_ < 0) return;
	// 背中にある弾から撃つ弾(待機状態の弾)選んで発射
	for (auto& shot : shots_)
	{
		if (shot->IsIdle())
		{

			// 弾の方向
			auto vec = VSub(VAdd(followTransform_->pos,{0.0f,100.0f,0.0f}), shot->GetPos());
			auto dir = VNorm(vec);

			// 発射処理
			shot->Shot(dir);
			shotNum_--;
			break;

		}
	}

}

void Enemy::CreateShot(void)
{
	
	// 8個の弾を敵の背後に作る(8回ループ)
	if (shotNum_ < 8 && delayCreate_ <= 0.0f)
	{

		// 利用可能なものを探す
		ShotEnemy* shot = GetAvailableShot();

		// デグリーをラジアンに変換
		shotCreateRad_ = shotCreateDeg_ * DX_PI_F / 180.0f;

		// 45度ずつ弾の位置を変える
		shotCreateDeg_ += (360 / 8);

		// 角度から方向(ベクトル)を求める
		auto qua = Quaternion::Euler({ 0.0f,0.0f,shotCreateRad_ });

		// 相対座標
		auto rPos = qua.PosAxis(LOCAL_SHOT_POS);

		// 弾の作成
		shot->Create(rPos, &transform_);
		
		// 弾を生成したら増やす
		shotNum_++;

		// 生成したらまた時間を増やす
		delayCreate_ = TIME_DELAY_SHOT_CREATE;

	}

	delayCreate_ -= SceneManager::GetInstance().GetDeltaTime();

	// アニメーションが終わったらショットに移行する
	if (stepAnim_ >= SHOT_CREATE_END_TIME - 1.0f)
	{
		ChangeState(STATE::SHOT);
	}

}

ShotEnemy* Enemy::GetAvailableShot(void)
{

	// 再利用探索
	for (auto& v : shots_)
	{
		if (!v->IsAlive())
		{
			return v;
		}
	}

	// なかった場合作成
	ShotEnemy* newShot = new ShotEnemy();
	//newShot->Create({ transform_.pos.x,
	//	transform_.pos.y + 150 ,
	//	transform_.pos.z },
	//	transform_.GetForward());
		
	// 弾の管理配列に追加
	shots_.push_back(newShot);
	return newShot;

}
