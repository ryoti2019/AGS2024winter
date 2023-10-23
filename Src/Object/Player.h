#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"

class Player : public UnitBase
{

public:

	// プレイヤーのカプセルの下の相対座標
	static constexpr VECTOR LOCAL_BODY_C_DOWN_POS = { 0.0f,-60.0f,0.0f };

	// プレイヤーのカプセルの上の相対座標
	static constexpr VECTOR LOCAL_BODY_C_UP_POS = { 0.0f,50.0f,0.0f };

	// プレイヤーの衝突判定の球体半径
	static constexpr float COLLISION_BODY_RADIUS = 30.0f;

	// 攻撃時間
	static constexpr float ATTACK_TIME = 0.5f;

	// プレイヤーの状態
	enum class STATE
	{
		IDLE,
		WALK,
		RUN,
		ATTACK
	};

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void);

	void Init(void) override;

	void Draw(void);

	void Release(void);

	// プレイヤーのTransformの取得
	const Transform& GetTransform(void) const;

	// プレイヤーの状態を取得
	Player::STATE GetState(void);

	// 衝突判定の下の座標の取得
	VECTOR GetCPosDown(void);

	// 衝突判定の上の座標の取得
	VECTOR GetCPosUP(void);

	// 攻撃フラグの取得
	bool GetAttack(void);

	// 攻撃フラグの設定
	void SetAttack(bool attack);

	// 敵のHP取得
	int GetHP(void);

	// 敵のHPの設定
	void SetHP(int hp);

protected:

	// プレイヤーの状態
	STATE state_;
	STATE preState_;

	// 攻撃フラグ
	bool attack_;

	// アニメーションごとに変数に代入
	int idleAnim_;
	int walkAnim_;
	int runAnim_;
	int attackAnim_;

	// カプセルをアタッチするフレームの番号
	int playerAttachFrameNum_;

	// プレイヤーの衝突判定の座標
	VECTOR cBodyPosUp_;
	VECTOR cBodyPosDown_;

	// 移動処理
	void Move(void) override;

	// 衝突判定
	void Collision(void);

	// 敵自身の衝突判定
	void PlayerBodyCollision(void);

	// 状態遷移
	void ChangeState(STATE state);

	// アニメーション
	void Animation(void)override;

	// アニメーションの初期設定
	void SetIdleAnimation(void);
	void SetWalkAnimation(void);
	void SetRunAnimation(void);
	void SetAttackAnimation(void);

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

	// キーボードの操作
	void KeybordContoroller(void);

	// ゲームパッドの操作
	void GamePadController(void);

};

