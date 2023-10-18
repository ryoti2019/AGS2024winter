#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"
class Player;

class Enemy : public UnitBase
{

public:

	// 剣から持ち手の当たり判定までの相対座標
	static constexpr VECTOR LOCAL_C_DOWN_POS = { 0.0f,250.0f,40.0f };

	// 剣から剣先の当たり判定までの相対座標
	static constexpr VECTOR LOCAL_C_UP_POS = { 0.0f,50.0f,40.0f };

	// 衝突判定の球体半径
	static constexpr float COLLISION_RADIUS = 100.0f;

	// プレイヤーの状態
	enum class STATE
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		DASH_ATTACK,
	};

	// コンストラクタ
	Enemy(void);

	// デストラクタ
	~Enemy(void);

	void Init(void)override;

	void Draw(void)override;

	void Release(void);

	// 敵のTransformの取得
	const Transform& GetTransform(void) const;

	// 衝突判定の下の座標の取得
	VECTOR GetCPosDown(void);

	// 衝突判定の上の座標の取得
	VECTOR GetCPosUP(void);

	// 敵のHP取得
	int GetHP(void);

	// 敵のHPの設定
	void SetHP(int hp);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

protected:

	// プレイヤーの取得
	Player* player_;

	// プレイヤーの状態
	STATE state_;
	STATE preState_;

	// アニメーションごとに変数に代入
	int idleAnim_;
	int walkAnim_;
	int runAnim_;
	int attackAnim_;
	int dashAttackAnim_;

	// ダッシュ攻撃のフラグ
	bool dashAttack_;

	// 追従対象
	const Transform* followTransform_;

	// 移動処理
	void Move(void) override;

	// 衝突判定
	void Collision(void);

	// 状態遷移
	void ChangeState(STATE state);

	// アニメーションの初期設定
	void SetIdleAnimation(void);
	void SetWalkAnimation(void);
	void SetRunAnimation(void);
	void SetAttackAnimation(void);
	void SetDashAttackAnimation(void);

	// アニメーションの変更
	void ChangeAnimation(void);

	// 遅延回転
	void LazyRotation(float goalRot);

	// デバッグ描画
	void DrawDebug(void);

	// パラメータ設定
	void SetParam(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override;

	// アニメーション
	void Animation(void) override;

	// 衝突判定の座標
	VECTOR cPosUp_;
	VECTOR cPosDown_;

	// 衝突判定の回転
	Quaternion cQuaUP_;
	Quaternion cQuaDOWN_;

};

