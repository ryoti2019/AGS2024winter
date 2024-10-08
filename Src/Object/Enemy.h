#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"
class ShotEnemy;
class AnimationController;

class Enemy : public UnitBase
{

public:

	// エネミー自身のカプセルの下の相対座標
	static constexpr VECTOR LOCAL_BODY_C_DOWN_POS = { 0.0f,0.0f,0.0f };

	// エネミー自身のカプセルの上の相対座標
	static constexpr VECTOR LOCAL_BODY_C_UP_POS = { 0.0f,-100.0f,-50.0f };

	// エネミー武器のカプセルの下の相対座標
	static constexpr VECTOR LOCAL_WEPON_C_DOWN_POS = { 0.0f,0.0f,0.0f };

	// エネミー武器のカプセルの上の相対座標
	static constexpr VECTOR LOCAL_WEPON_C_UP_POS = { 0.0f,150.0f,0.0f };

	// 弾の相対座標
	static constexpr VECTOR LOCAL_SHOT_POS = { 0.0f,200.0f,0.0f };

	// エネミー自身の衝突判定の球体半径
	static constexpr float COLLISION_BODY_RADIUS = 100.0f;

	// エネミー武器の衝突判定の球体半径
	static constexpr float COLLISION_WEPON_RADIUS = 70.0f;

	// 突進の秒数
	static constexpr float TACKLE_TIME = 4.0f;

	// 最初の歩き続ける秒数
	static constexpr float FIRST_WALK_TIME = 2.0f;

	// アタック時の攻撃範囲
	static constexpr float ATTACK_RANGE = 200.0f;

	// ジャンプアタックの攻撃範囲(最低)
	static constexpr float JUMP_ATTACK_RANGE_MIN = 200.0f;

	// ジャンプアタックの攻撃範囲(最高)
	static constexpr float JUMP_ATTACK_RANGE_MAX = 3000.0f;

	//// タックルの攻撃範囲(最低)
	//static constexpr float TACKLE_RANGE_MIN = 1000.0f;

	// 目的の角度までの差
	static constexpr float GOAL_DEG = 5.0f;

	// 歩くスピード
	static constexpr float WALK_SPEED = 2.0f;

	// ジャンプアタックのスピード
	static constexpr float JUMP_ATTACK_SPEED = 40.0f;

	// ジャンプアタックのスピード
	static constexpr float TACKLE_SPEED = 30.0f;

	// ダメージヒットのスピード
	static constexpr float HIT_SPEED = -5.0f;

	// 攻撃の当たり判定が始まる時間
	static constexpr float ATTACK_COLLISION_START_TIME = 30.0f;

	// 攻撃の当たり判定が終わる時間
	static constexpr float ATTACK_COLLISION_END_TIME = 50.0f;

	// ジャンプアタックの当たり判定が始まる時間
	static constexpr float JUMP_ATTACK_COLLISION_START_TIME = 40.0f;

	// ジャンプアタックの当たり判定が終わる時間
	static constexpr float JUMP_ATTACK_COLLISION_END_TIME = 70.0f;

	// ジャンプアタックが終わる時間
	static constexpr float JUMP_ATTACK_END_TIME = 200.0f;

	// 弾の生成が始まる時間
	static constexpr float SHOT_CREATE_START_TIME = 10.0f;

	// 弾の生成が終わる時間
	static constexpr float SHOT_CREATE_END_TIME = 30.0f;

	// ショットアタックが始まる時間
	static constexpr float SHOT_START_TIME = 20.0f;

	// ショットアタックが終わる時間
	static constexpr float SHOT_END_TIME = 40.0f;

	// ダメージヒットが終わる時間
	static constexpr float HIT_END_TIME = 300.0f;

	// 待機アニメーションの再生速度
	static constexpr float IDLE_ANIM_SPEED = 20.0f;

	// 歩くアニメーションの再生速度
	static constexpr float WALK_ANIM_SPEED = 20.0f;

	// 走るアニメーションの再生速度
	static constexpr float RUN_ANIM_SPEED = 40.0f;

	// 攻撃アニメーションの再生速度
	static constexpr float ATTACK_ANIM_SPEED = 20.0f;

	// ジャンプアタックアニメーションの再生速度
	static constexpr float JUMP_ATTACK_ANIM_SPEED = 20.0f;

	// タックルアニメーションの再生速度
	static constexpr float TACKLE_ANIM_SPEED = 40.0f;

	// 弾生成アニメーションの再生速度
	static constexpr float SHOT_CREATE_SPEED = 10.0f;

	// ショットアニメーションの再生速度
	static constexpr float SHOT_ANIM_SPEED = 30.0f;

	// ダメージヒットアニメーションの再生速度
	static constexpr float HIT_ANIM_SPEED = 20.0f;

	// HPバーの長さ
	static constexpr int HP_LENGTH = 780;

	// HPの最大値
	static constexpr int HP_MAX = 100;

	// 弾の生成間隔
	static constexpr float TIME_DELAY_SHOT_CREATE = 0.1f;

	// 弾の発射間隔
	static constexpr float TIME_DELAY_SHOT = 2.0f;

	// 敵のクールタイム
	static constexpr float COOL_TIME = 3.0f;

	// ロックオンカーソルの総数
	static constexpr int LOCKON_CURSOR_NUM = 59;

	// ショット攻撃の全体の時間
	static constexpr float SHOT_ATTACK_TIME = 8.0f;

	// タックル攻撃に入るまでの時間
	static constexpr float BEOFRE_TACKLE_TIME = 1.0f;

	// プレイヤーの状態
	enum class STATE
	{
		THINK,
		IDLE,
		WALK,
		ATTACK,
		JUMP_ATTACK,
		BEFORE_TACKLE,
		TACKLE,
		CREATE,
		SHOT,
		HIT,
		DEATH,
		TURN_LEFT,
		TURN_RIGHT,
		MAX
	};

	std::string ANIM_DATA_KEY[(int)STATE::MAX] =
	{
		"THINK",
		"IDLE",
		"WALK",
		"ATTACK",
		"JUMP_ATTACK",
		"BEFORE_TACKLE",
		"TACKLE",
		"CREATE",
		"SHOT",
		"HIT",
		"DEATH",
		"TURN_LEFT",
		"TURN_RIGHT",
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

	// 弾が死んだ数の取得
	void SetDeathCnt(int cnt);

	// 弾
	std::vector<ShotEnemy*>& GetShots(void);

	// ステージのモデルIDを設定
	void SetStageID(const int modelId);

	// ジャンプアタックするときに保存するプレイヤーの位置
	VECTOR GetAttackPlayerPos(void);

	// 再生中のアニメーション時間
	float GetStepAnim(void);

protected:

	// アニメーション
	AnimationController* animationController_;

	// 弾
	std::vector<ShotEnemy*> shots_;

	// 状態
	STATE state_;
	STATE preState_;

	// アニメーションデータ
	std::string key_;
	std::string preKey_;

	std::vector <std::string> stateHiss_;

	// HPバーの画像
	int imgHPBar_;

	// ステージモデルID
	int stageId_;

	// 待機アニメーション
	int idleAnim_;

	// 歩くアニメーション
	int walkAnim_;

	// タックルアニメーション
	int tackleAnim_;

	// 攻撃アニメーション
	int attackAnim_;

	// ジャンプアタックのアニメーション
	int jumpAttackAnim_;

	// 弾を作る
	int createAnim_;

	// ショットアニメーション
	int shotAnim_;

	// ダメージヒットアニメーション
	int hitAnim_;

	// 死亡アニメーション
	int deathAnim_;

	// 左旋回アニメーション
	int turnLeftAnim_;

	// 右旋回アニメーション
	int turnRightAnim_;

	// 最初の歩きのアニメーションのカウンタ
	float walkCnt_;

	// 回転の開始フラグ
	bool startRotation_;

	// 回転の終了のフラグ
	bool isRotation_;

	// 行動の終了フラグ
	bool isAction_;

	// 攻撃の番号
	int attackNumber_;

	// 攻撃のフラグ
	bool attack_;

	// 弾を発射したかどうか
	bool isShot_;

	// 攻撃が当たったかどうか
	bool hit_;

	// ダッシュ攻撃のフラグ
	bool jumpAttack_;

	// 突進するまでの時間
	float beforeTackleCnt_;

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

	// 弾を生成する間隔
	float delayCreate_;

	// 弾を生成する角度
	int shotCreateDeg_;
	float shotCreateRad_;

	// 弾の発射間隔
	float delayShot_;

	// 弾を発射する全体のカウンタ
	float shotCnt_;

	// 待機している玉の数
	int shotNum_;

	// 敵が動かない時間
	float idleCoolTime_;

	// 敵を動かないようにするフラグ
	bool isNoPlay_;

	// ロックオンカーソル
	int* lockOnCursorImg_;

	// ロックオンカーソルの添え字を増やす
	int lockOnCursorCnt_;

	// ロックオンカーソルの添え字を増やすカウンタ
	float lockOnCursorTime_;

	// ロックオンカーソルのY座標を制御する座標
	VECTOR pos_;

	// 移動後座標
	VECTOR movedPos_;

	// 移動量
	VECTOR movePow_;

	// エフェクト

	// 弾を作るエフェクト
	int effectCreateResId_;
	int effectCreatePlayId_;
	VECTOR effectCreatePos_;

	// タックルのエフェクト
	int effectTackleResId_;
	int effectTacklePlayId_;
	VECTOR effectTacklePos_;

	// タックルの攻撃範囲のエフェクト
	int effectTackleRangeResId_;
	int effectTackleRangePlayId_;
	VECTOR effectTackleRangePos_;

	// ジャンプアタックのエフェクト
	int effectJumpAttackResId_;
	int effectJumpAttackPlayId_;
	VECTOR effectJumpAttackPos_;
	bool isEffectJumpAttack_;

	// ジャンプアタックの攻撃範囲のエフェクト
	int effectJumpAttackRangeResId_;
	int effectJumpAttackRangePlayId_;
	VECTOR effectJumpAttackRangePos_;

	// サウンド
	// 弾を作る音
	int musicCreateId_;

	// タックルの音
	int musicTackleId_;

	// 足音
	int musicFootStepsId_;

	// 足音のカウンタ
	float musicFootStepsCnt_;

	// ジャンプアタックの音
	int musicJumpAttackId_;

	// 地響きの音
	int musicEarthQuakeId_;

	// 攻撃のボイス１
	int musicAttackVoice1Id_;

	// 攻撃のボイス２
	int musicAttackVoice2Id_;

	// 攻撃のボイスのフラグ
	bool isMusicAttackVoice_;

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

	// タックルの前
	void UpdateBeforeTackle(void);

	// タックル攻撃
	void UpdateTackle(void);

	// 弾を作る
	void UpdateCreate(void);

	// ショット攻撃
	void UpdateShot(void);

	// 攻撃ヒット
	void UpdateHit(void);

	// 衝突判定
	void Collision(void);

	// ステージとの衝突判定
	void CollisionStage(void);

	// 敵自身の衝突判定
	void EnemyBodyCollision(void);

	// 敵の武器の衝突判定
	void WeponCollision(void);

	// 状態遷移
	void ChangeState(STATE state);

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

	// 操作：弾発射
	void ProcessShot(void);

	// 自機の弾を発射
	void CreateShot(void);

	// 弾が死んでいるかどうか
	ShotEnemy* GetAvailableShot(void);

	// 弾を打つ
	void Shot(void);

	// エフェクトの初期化
	void InitEffect(void);

	// エフェクト再生
	void CreatePlayEffect(void);
	void TacklePlayEffect(void);
	void TackleRangePlayEffect(void);
	void JumpAttackPlayEffect(void);
	void JumpAttackRangePlayEffect(void);

	// エフェクト位置
	void CreateSyncEffect(void);
	void TackleSyncEffect(void);
	void TackleRangeSyncEffect(void);
	void JumpAttackSyncEffect(void);
	void JumpAttackRangeSyncEffect(void);

	// 音の初期化
	void InitMusic(void);

	// 足音
	void FootStepsMusic(void);

	// 攻撃するボイス
	void AttackMusic(void);

};