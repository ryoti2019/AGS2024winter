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
	static constexpr float COLLISION_BODY_RADIUS = 40.0f;

	// 攻撃の2段階目が始まる時間
	static constexpr float ATTACK_START_TIME2 = 40.0f;

	// 攻撃の2段階目が始まる時間
	static constexpr float ATTACK_START_TIME3 = 60.0f;

	// 攻撃の1段階目が終わる時間
	static constexpr float ATTACK_END_TIME1 = 40.0f;

	// 攻撃の2段階目が終わる時間
	static constexpr float ATTACK_END_TIME2 = 60.0f;

	// 攻撃の3段階目が終わる時間
	static constexpr float ATTACK_END_TIME3 = 40.0f;

	// 移動量(歩く)
	static constexpr float MOVE_POW_WALK = 10.0f;

	// 移動量(走る)
	static constexpr float MOVE_POW_RUN = 20.0f;

	// 待機アニメーションの再生速度
	static constexpr float IDLE_ANIM_SPEED = 20.0f;

	// 歩くアニメーションの再生速度
	static constexpr float WALK_ANIM_SPEED = 30.0f;

	// 走るアニメーションの再生速度
	static constexpr float RUN_ANIM_SPEED = 40.0f;

	// 攻撃アニメーションの再生速度
	static constexpr float ATTACK_ANIM_SPEED = 60.0f;

	// 攻撃を受けた時のアニメーションの再生速度
	static constexpr float HIT_ANIM_SPEED = 30.0f;

	// 回転する強さ
	static constexpr float ROTATION_POW = 0.1f;

	// HPバーの長さ
	static constexpr int HP_LENGTH = 300;

	// ため斬りのボタンを押す秒数
	static constexpr float CHARGE_TIME = 1.0f;

	// プレイヤーの状態
	enum class STATE
	{
		IDLE,
		WALK,
		ATTACK_WALK,
		RUN,
		ATTACK,
		ATTACK2,
		ATTACK3,
		ATTACK4,
		HIT
	};

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void);

	void Init(void) override;

	void Update(void) override;

	void Draw(void);

	void Release(void);

	// プレイヤーのTransformの取得
	const Transform& GetTransform(void) const;

	// プレイヤーの状態を取得
	Player::STATE GetState(void);

	// プレイヤーの状態を設定
	void SetState(Player::STATE state);

	// 衝突判定の下の座標の取得
	VECTOR GetCPosDown(void);

	// 衝突判定の上の座標の取得
	VECTOR GetCPosUP(void);

	// 攻撃フラグの取得
	bool GetAttack(void);

	// 攻撃フラグの設定
	void SetAttack(bool attack);

	// プレイヤーのHP取得
	int GetHP(void);

	// プレイヤーのHPの設定
	void SetHP(int hp);

	// 攻撃が当たったかを取得
	bool GetHit(void);

	// 攻撃が当たったかを設定
	void SetHit(bool hit);

protected:

	// プレイヤーの状態
	STATE state_;

	// 攻撃フラグ
	bool attack_;

	// 攻撃がヒットしたか
	bool hit_;

	// 攻撃１段階目
	bool attack1_;

	// 攻撃２段階目
	bool attack2_;

	// 攻撃３段階目
	bool attack3_;

	// アニメーションごとに変数に代入
	int idleAnim_;
	int walkAnim_;
	int attackWalkAnim_;
	int runAnim_;
	//int attackAnim_;
	//int attackAnim2_;
	//int attackAnim3_;
	int attackAnim4_;
	int attackAnim5_;
	int hitAnim_;

	// ため斬りのカウンタ
	float chargeCnt;

	// カプセルをアタッチするフレームの番号
	int playerAttachFrameNum_;

	// プレイヤーの衝突判定の座標
	VECTOR cBodyPosUp_;
	VECTOR cBodyPosDown_;

	// 移動処理
	void KeyboardMove(void);
	void GamePadMove(void);

	// 攻撃処理
	void KeyboardAttack(void);
	void GamePadAttack(void);

	// プレイヤー方向にカメラを向ける処理
	void KeyBoardCamera(void);
	void GamePadCamera(void);

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
	void SetAttackAnimation2(void);
	void SetAttackAnimation3(void);
	void SetAttackAnimation4(void);
	void SetHitAnimation(void);

	// 遅延回転
	void LazyRotation(float goalRot);

	// デバッグ描画
	void DrawDebug(void);

	// パラメータ設定
	void SetParam(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override;

	// キーボードの操作
	void KeyboardContoroller(void);

	// ゲームパッドの操作
	void GamePadController(void);

	// アニメーションのフレームの固定
	void AnimationFrame(void);

	// HPバーの描画
	void DrawHPBar(void);

};

