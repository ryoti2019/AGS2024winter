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
	static constexpr float COLLISION_WEPON_RADIUS = 70.0f;

	// 突進の秒数
	static constexpr float TACKLE_TIME = 4.0f;

	// プレイヤーの状態
	enum class STATE
	{
		THINK,
		IDLE,
		WALK,
		ATTACK,
		JUMP_ATTACK,
		TACKLE,
		HIT,
	};

	// コンストラクタ
	Enemy(void);

	// デストラクタ
	~Enemy(void);

	void Init(void)override;

	void Update(void)override;

	void Draw(void)override;

	void Release(void);

	// 敵のTransformの取得
	const Transform& GetTransform(void) const;

	// プレイヤーの状態を取得
	Enemy::STATE GetState(void);

	// プレイヤーの状態を取得
	void SetState(Enemy::STATE state);

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

	// 攻撃が当たったかどうかの取得
	bool GetHit(void);

	// 攻撃が当たったかどうかの設定
	void SetHit(bool hit);

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
	int hitAnim_;

	// 最初の歩きのアニメーションのカウンタ
	float walkCnt_;
	
	// 回転の終了のフラグ
	bool rotationEnd_;

	// 攻撃の番号
	int attackNumber_;

	// 攻撃のフラグ
	bool attack_;

	// 攻撃が当たったかどうか
	bool hit_;

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

	// 行動の選択
	void Think(void);

	// 回転処理
	void Rotation(void);
	
	// 行動後の回転処理
	void AfterRotation(void);

	// 待機
	void UpdateIdle(void);

	// 移動
	void UpdateWalk(void);

	// 通常攻撃
	void UpdateAttack(void);

	// ダッシュ攻撃
	void UpdateJumpAttack(void);

	// タックル攻撃
	void UpdateTackle(void);

	// 攻撃ヒット
	void UpdateHit(void);

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
	void SetHitAnimation(void);

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

