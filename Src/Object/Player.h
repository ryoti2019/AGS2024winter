#pragma once
#include "Common/Transform.h"

class Player
{

public:

	// 移動スピード
	static constexpr float SPEED_MOVE = 0.0f;

	// プレイヤーの状態
	enum class STATE
	{
		WALK,
		RUN,
		ATTACK
	};

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void);

	void Init(void);

	void InitAnimation(void);

	void Update(void);

	void Draw(void);

	void Release(void);

	// プレイヤーのTransformの取得
	const Transform& GetTransform(void) const;

protected:

	// アニメーションのアタッチ番号
	int animAttachNo_;

	// アニメーションの総再生時間
	float animTotalTime_;

	// 再生中のアニメーション時間
	float stepAnim_;

	// アニメーション速度
	float speedAnim_;

	// モデルの中にあるアニメーションの番号
	int animNo_;

	// モデル制御の基本情報
	Transform transform_;

	// プレイヤーの状態
	STATE state_;

	// 移動処理
	void Move(void);

	// カメラの移動処理
	
	// 状態遷移
	void ChangeState(STATE state);

	// アニメーション
	void Animation(void);

	// アニメーションの初期設定
	void SetWalkAnimation(void);
	void SetRunAnimation(void);

};

