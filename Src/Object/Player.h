#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"

class Player : public UnitBase
{

public:

	// 移動スピード
	static constexpr float SPEED_MOVE = 0.0f;

	// 回転量(deg)
	static constexpr float SPEED_ROT = 300.0f;

	// 回転量(rad)
	static constexpr float SPEED_ROT_RAD = SPEED_ROT * (DX_PI_F / 180.0f);

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

protected:

	// プレイヤーの状態
	STATE state_;

	// 回転
	Quaternion quaRot_;

	// 移動処理
	void Move(void) override;

	// カメラの移動処理
	
	// 状態遷移
	void ChangeState(STATE state);

	// アニメーションの初期設定
	void SetIdleAnimation(void);
	void SetWalkAnimation(void);
	void SetRunAnimation(void);

	// 遅延回転
	void LazyRotation(float goalRot);

	// デバッグ描画
	void DrawDebug(void);

	// パラメータ設定
	void SetParam(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override;

};

