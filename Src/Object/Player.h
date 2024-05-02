#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"
class Stage;
class AnimationController;
class InputContoroller;
;

class Player : public UnitBase
{

public:

	// プレイヤーのカプセルの下の相対座標
	static constexpr VECTOR LOCAL_BODY_C_DOWN_POS = { 0.0f,-60.0f,0.0f };

	// プレイヤーのカプセルの上の相対座標
	static constexpr VECTOR LOCAL_BODY_C_UP_POS = { 0.0f,50.0f,0.0f };

	// プレイヤーの衝突判定の球体半径
	static constexpr float COLLISION_BODY_RADIUS = 40.0f;

	// 攻撃の1段階目が終わる時間
	static constexpr float ATTACK_END_TIME1 = 40.0f;

	// 攻撃の1段階目の当たり判定が始まる時間
	static constexpr float ATTACK_COLLISION_START_TIME1 = 30.0f;

	// 攻撃の1段階目の当たり判定が終わる時間
	static constexpr float ATTACK_COLLISION_END_TIME1 = 37.0f;

	// 攻撃の2段階目が始まる時間
	static constexpr float ATTACK_START_TIME2 = 40.0f;

	// 攻撃の2段階目が終わる時間
	static constexpr float ATTACK_END_TIME2 = 60.0f;

	// 攻撃の2段階目の当たり判定が始まる時間
	static constexpr float ATTACK_COLLISION_START_TIME2 = 51.0f;

	// 攻撃の2段階目の当たり判定が終わる時間
	static constexpr float ATTACK_COLLISION_END_TIME2 = 58.0f;

	// 攻撃の3段階目が始まる時間
	static constexpr float ATTACK_START_TIME3 = 60.0f;

	// 攻撃の3段階目が終わる時間
	static constexpr float ATTACK_END_TIME3 = 40.0f;

	// 攻撃の3段階目の当たり判定が始まる時間
	static constexpr float ATTACK_COLLISION_START_TIME3 = 100.0f;

	// 攻撃の3段階目の当たり判定が終わる時間
	static constexpr float ATTACK_COLLISION_END_TIME3 = 110.0f;

	// 溜め攻撃の当たり判定が始まる時間
	static constexpr float CHARGE_ATTACK_COLLISION_START_TIME = 20.0f;

	// 溜めの攻撃の当たり判定が終わる時間
	static constexpr float CHARGE_ATTACK_COLLISION_END_TIME = 30.0f;

	// 移動量(歩く)
	static constexpr float MOVE_POW_WALK = 10.0f;

	// 移動量(溜めながら歩く)
	static constexpr float MOVE_POW_CHRAGE_WALK = 5.0f;

	// 移動量(走る)
	static constexpr float MOVE_POW_RUN = 20.0f;

	// 移動量(回避)
	static constexpr float MOVE_POW_ROLL = 20.0f;

	// 待機アニメーションの再生速度
	static constexpr float IDLE_ANIM_SPEED = 20.0f;

	// 歩くアニメーションの再生速度
	static constexpr float WALK_ANIM_SPEED = 30.0f;

	// 溜めながら歩くアニメーションの再生速度
	static constexpr float CHARGE_WALK_ANIM_SPEED = 20.0f;

	// 走るアニメーションの再生速度
	static constexpr float RUN_ANIM_SPEED = 40.0f;

	// 攻撃アニメーションの再生速度
	static constexpr float ATTACK_ANIM_SPEED = 30.0f;

	// 溜め攻撃アニメーションの再生速度
	static constexpr float CHARGE_ATTACK_ANIM_SPEED = 30.0f;

	// 攻撃を受けた時のアニメーションの再生速度
	static constexpr float HIT_ANIM_SPEED = 30.0f;

	// 回避のアニメーションの再生速度
	static constexpr float ROLL_ANIM_SPEED = 50.0f;

	// 回転する強さ
	static constexpr float ROTATION_POW = 0.1f;

	// HPバーの長さ
	static constexpr int HP_LENGTH = 360;

	// HPの最大値
	static constexpr int HP_MAX = 100;

	// ため斬りのボタンを押す秒数
	static constexpr float CHARGE_TIME = 1.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.2f;

	// 溜めのエフェクトの相対座標
	static constexpr VECTOR LOCAL_CHRAGE_POS = { 0.0f,0.0f,0.0f };

	// 回避の無敵が始まる時間
	static constexpr float ROLL_INVINCIBLE_START_TIME = 20.0f;

	// 回避の無敵が終わる時間
	static constexpr float ROLL_INVINCIBLE_END_TIME = 50.0f;

	// スタミナの最大値
	static constexpr float STAMINA_MAX = 100.0f;

	// プレイヤーの状態
	enum class STATE
	{
		IDLE,
		WALK,
		CHARGE_WALK,
		RUN,
		ATTACK,
		ATTACK2,
		ATTACK3,
		CHARGE_ATTACK,
		HIT,
		DEATH,
		ROLL,
		TIRED,
		MAX
	};

	std::string ANIM_DATA_KEY[(int)STATE::MAX] =
	{
		"IDLE",
		"WALK",
		"CHARGE_WALK",
		"RUN",
		"ATTACK",
		"ATTACK2",
		"ATTACK3",
		"CHARGE_ATTACK",
		"HIT",
		"DEATH",
		"ROLL",
		"TIRED"
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

	// プレイヤーの座標を設定
	void SetPos(VECTOR pos);

	// 攻撃フラグの取得
	bool GetAttack(void);

	// 攻撃フラグの設定
	void SetAttack(bool attack);

	// プレイヤーのHP取得
	int GetHP(void);

	// プレイヤーのHPの設定
	void SetHP(int hp);

	// プレイヤーのスタミナ取得
	float GetStamina(void);

	// 攻撃が当たったかどうかを取得
	bool GetHit(void);

	// 攻撃が当たったかどうかを設定
	void SetHit(bool hit);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

	// ステージのモデルIDを設定
	void SetStageID(const int modelId);

	// 無敵かどうか取得
	bool GetInvincible(void);

protected:

	// アニメーション
	AnimationController* animationController_;

	// コントローラ
	InputContoroller* inputController_;

	// プレイヤーの状態
	STATE state_;

	// 一個前の状態
	STATE preState_;

	// アニメーションデータ
	std::string key_;
	std::string preKey_;

	// 追従対象
	const Transform* followTransform_;

	// ステージのID
	int stageId_;

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

	// 溜め斬り
	bool chargeAttack_;

	// ロックオンの前の情報
	bool preLockOn_;

	// 疲れたときのフラグ
	bool isTired_;

	// 待機アニメーション
	int idleAnim_;

	// 歩くアニメーション
	int walkAnim_;

	// 溜めながら歩くアニメーション
	int chargeWalkAnim_;

	// 走るアニメーション
	int runAnim_;

	// 攻撃アニメーション
	int attackAnim_;

	// 溜め攻撃アニメーション
	int chargeAttackAnim_;

	// ダメージヒットアニメーション
	int hitAnim_;

	// 死亡アニメーション
	int deathAnim_;

	// 回避アニメーション
	int rollAnim_;

	// 疲れたアニメーション
	int tiredAnim_;

	// ため斬りのカウンタ
	float chargeCnt_;

	// カプセルをアタッチするフレームの番号
	int playerAttachFrameNum_;

	// プレイヤーの衝突判定の座標
	VECTOR cBodyPosUp_;
	VECTOR cBodyPosDown_;

	// 移動方向
	VECTOR moveDir_;

	// 移動差
	VECTOR moveDiff_;

	// 移動後座標
	VECTOR movedPos_;

	// スピード
	float speed_;

	// 移動量
	VECTOR movePow_;

	// 目的の角度
	Quaternion goalQuaRot_;

	// 回転する時間
	float stepRotTime_;

	// 注視点と移動後座標の距離
	float target2PlayerDis_;

	// 敵との最小限の距離
	float enemyMinDis_;

	// 必殺技のムービーカウンタ
	float specialCnt_;

	// 回避中の無敵時間
	bool isInvincible_;

	// スタミナの最大値
	float staminaMax_;

	// スタミナ
	float stamina_;

	// スタミナのカウンタ
	float staminaCnt_;

	// 最初の行動できないようにするカウンタ
	float limitCnt_;

	// エフェクト
	// 溜めるエフェクト
	int effectChargeResId_;
	int effectChargePlayId_;
	VECTOR effectChargePos_;

	// サウンド
	// 溜める音
	int musicChargeId_;

	// 風を切る音１
	int musicSlash1Id_;

	// 風を切る音２
	int musicSlash2Id_;

	// 風を切る音のフラグ
	bool isMusicSlash_;

	// 足音
	int musicFootStepsId_;

	// 足音のカウンタ
	float musicFootStepsCnt_;

	// 攻撃ボイス１
	int musicSlashVoice1Id_;

	// 攻撃ボイス２
	int musicSlashVoice2Id_;

	// 溜め攻撃ボイス
	int musicChargeSlashVoiceId_;

	// 回避音
	int musicRollId_;

	// 回避ボイス１
	int musicRollVoice1Id_;

	// 回避ボイス２
	int musicRollVoice2Id_;

	// 回避ボイスのフラグ
	bool isMusicRoll_;

	float stepBlend_;
	float blendTime_;

	int list_;

	int slowCnt_;

	std::vector<std::string> stateHiss_;

	// 移動処理
	void KeyboardMove(void);

	// 攻撃処理
	void KeyboardAttack(void);

	// プレイヤー方向にカメラを向ける処理
	void KeyBoardCamera(void);

	// 敵をロックオンする処理
	void KeyBoardLockOn(void);
	void LockOn(void);
	void SetGoalRotate(Quaternion rot);
	void Rotate(void);

	// 衝突判定
	void Collision(void);

	// 自身の衝突判定
	void PlayerBodyCollision(void);

	// ステージとの当たり判定
	void CollisionStage(void);

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

	// キーボードの操作
	void KeyboardContoroller(void);

	// アニメーション
	void Animation(void);

	// アニメーションのフレームの固定
	void AnimationFrame(void);

	// エフェクトの初期化
	void InitEffect(void);

	// エフェクト再生
	// 溜めるエフェクト
	void ChargePlayEffect(void);

	// エフェクト位置
	// 溜めるエフェクト
	void ChargeSyncEffect(void);

	// 音の初期化
	void InitMusic(void);

	// 風を切る音
	void SlashMusic(void);

	// 足音
	void FootStepsMusic(void);

	// 回避音
	void RollMusic(void);

};