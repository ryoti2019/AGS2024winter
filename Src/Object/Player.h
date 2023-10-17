#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"

class Player : public UnitBase
{

public:

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

protected:

	// プレイヤーの状態
	STATE state_;
	STATE preState_;

	// アニメーション
	int idleAnim_;
	int walkAnim_;
	int runAnim_;
	int attackAnim_;

	// 移動処理
	void Move(void) override;

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

