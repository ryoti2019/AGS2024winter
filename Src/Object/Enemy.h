#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"
class Player;

class Enemy : public UnitBase
{

public:

	// エネミー自身のカプセルの下の相対座標
	static constexpr VECTOR LOCAL_BODY_C_DOWN_POS = { 0.0f,50.0f,0.0f };

	// エネミー自身のカプセルの上の相対座標
	static constexpr VECTOR LOCAL_BODY_C_UP_POS = { 0.0f,-100.0f,-50.0f };

	// エネミー武器のカプセルの下の相対座標
	static constexpr VECTOR LOCAL_WEPON_C_DOWN_POS = { 0.0f,0.0f,0.0f };

	// エネミー武器のカプセルの上の相対座標
	static constexpr VECTOR LOCAL_WEPON_C_UP_POS = { 0.0f,150.0f,0.0f };

	// エネミー自身の衝突判定の球体半径
	static constexpr float COLLISION_BODY_RADIUS = 100.0f;

	// エネミー武器の衝突判定の球体半径
	static constexpr float COLLISION_WEPON_RADIUS = 50.0f;

	// 突進の秒数
	static constexpr float TACKLE_TIME = 4.0f;

	// プレイヤーの状態
	enum class STATE
	{
		IDLE,
		WALK,
		TACKLE,
		ATTACK,
		JUMP_ATTACK,
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

	// プレイヤーの状態を取得
	Enemy::STATE GetState(void);

	// 敵自身の衝突判定の下の座標の取得
	VECTOR GetCBodyPosDown(void);

	// 敵自身の衝突判定の上の座標の取得
	VECTOR GetCBodyPosUP(void);

	// 敵武器の衝突判定の下の座標の取得
	VECTOR GetCWeponPosDown(void);

	// 敵武器の上の座標の取得
	VECTOR GetCWeponPosUP(void);

	// 敵のHP取得
	int GetHP(void);

	// 敵のHPの設定
	void SetHP(int hp);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

	// 攻撃フラグの取得
	bool GetAttack(void);

	// 攻撃フラグの設定
	void SetAttack(bool attack);

protected:

	// プレイヤーの取得
	Player* player_;

	// プレイヤーの状態
	STATE state_;
	STATE preState_;

	// アニメーションごとに変数に代入
	int idleAnim_;
	int walkAnim_;
	int tackleAnim_;
	int attackAnim_;
	int jumpAttackAnim_;

	// 攻撃のフラグ
	bool attack_;

	// ダッシュ攻撃のフラグ
	bool jumpAttack_;

	// タックルの攻撃フラグ
	bool tackleAttack_;

	// 突進し続ける秒数
	float tackleCnt_;

	// 移動制限をつけるフレーム番号
	int enemyPosFrameNum_;
	
	// カプセルをアタッチするフレームの番号
	int enemyAttachFrameNum_;

	// 武器をアタッチするフレームの番号
	int weponAttachFrameNum_;

	// プレイヤーの方向
	VECTOR pDirection_;

	// エネミー自身の衝突判定の座標
	VECTOR cBodyPosUp_;
	VECTOR cBodyPosDown_;

	// エネミー武器の衝突判定の座標
	VECTOR cWeponPosUp_;
	VECTOR cWeponPosDown_;

	// ダッシュ攻撃する時のプレイヤー座標を保管する座標
	VECTOR attackPlayerPos_;

	// 追従対象
	const Transform* followTransform_;

	// 移動処理
	void Move(void) override;

	// 回転処理
	void Rotation(void);

	// 待機
	void Idle(void);

	// 移動
	void Walk(void);

	// 通常攻撃
	void Attack(void);

	// ダッシュ攻撃
	void DashAttack(void);

	// タックル攻撃
	void Tackle(void);

	// 衝突判定
	void Collision(void);

	// 敵自身の衝突判定
	void EnemyBodyCollision(void);

	// 敵の武器の衝突判定
	void WeponCollision(void);

	// 状態遷移
	void ChangeState(STATE state);

	// アニメーションの初期設定
	void SetIdleAnimation(void);
	void SetWalkAnimation(void);
	void SetTackleAnimation(void);
	void SetAttackAnimation(void);
	void SetJumpAttackAnimation(void);

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

	// アニメーションのフレームの固定
	void AnimationFrame(void);

};

