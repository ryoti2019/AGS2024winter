#pragma once
#include "SceneBase.h"

class Grid;
class Stage;
class Player;
class Enemy;
class Sword;

class GameScene : public SceneBase
{

public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// グリッド線
	Grid* grid_;

	// ステージ
	Stage* stage_;

	// プレイヤー
	Player* player_;

	// 敵
	Enemy* enemy_;

	// 剣
	Sword* sword_;

	// プレイヤーの回避用の座標
	VECTOR rollPos_;

	// HPバーの画像
	int imgPlayerHPBar_;

	// ヒットストップ
	int hitStopCnt_;

	// スロー
	int slowCnt_;

	// 次のシーンに移るまでの時間
	float sceneCnt_;

	// 敵が死んだときのフラグ
	bool enemyDeath_;

	// プレイヤーが死んだときのフラグ
	bool playerDeath_;

	// エフェクト
	// プレイヤーの攻撃が当たった時のエフェクト
	int effectPlayerImpactResId_;
	int effectPlayerImpactPlayId_;
	VECTOR effectPlayerImpactPos_;

	// 敵の攻撃が当たった時のエフェクト
	int effectEnemyImpactResId_;
	int effectEnemyImpactPlayId_;
	VECTOR effectEnemyImpactPos_;

	// ジャンプアタックの吸い込むエフェクト
	int effectEnemyTornadeResId_;
	int effectEnemyTornadePlayId_;
	VECTOR effectEnemyTornadePos_;
	bool isEffectTornade_;

	// 音
	
	// ゲームシーンの音楽
	int musicGameId_;

	// プレイヤーの攻撃が当たった時の音１
	int musicImpactId1_;

	// プレイヤーの攻撃が当たった時の音２
	int musicImpactId2_;

	// プレイヤーの攻撃が当たった時の音３
	int musicImpactId3_;

	// プレイヤーのダメージヒットボイス１
	int musicPlayerHitVoice1_;

	// プレイヤーのダメージヒットボイス２
	int musicPlayerHitVoice2_;

	// 敵のダメージヒットのボイス１
	int musicEnemyHitVoice1_;

	// 敵のダメージヒットのボイス２
	int musicEnemyHitVoice2_;

	// 敵のダメージヒットのボイス３
	int musicEnemyHitVoice3_;

	// 敵のプレイヤーと敵同士の当たり判定
	void CollisionEnemyAndPlayer();

	// デバッグ描画
	void DrawDebug(void);

	// エフェクトの初期化
	void InitEffect(void);

	// プレイヤーの攻撃が当たった時のエフェクト
	void PlayerImpactPlayEffect(void);

	// 敵の攻撃が当たった時のエフェクト
	void EnemyImpactPlayEffect(void);

	// ジャンプアタックの吸い込むエフェクト
	void EnemyTornadePlayEffect(void);

	// プレイヤーの攻撃が当たった時のエフェクト
	void PlayerImpactSyncEffect(void);

	// 敵の攻撃が当たった時のエフェクト
	void EnemyImpactSyncEffect(void);

	// ジャンプアタックの吸い込むエフェクト
	void EnemyTornadeSyncEffect(void);

	// 音の初期化
	void InitMusic(void);

	// プレイヤーの攻撃が当たった時の音
	void ImpactMusic(void);

	// プレイヤーダメージヒット音
	void PlayerHitMusic(void);

	// 敵のダメージヒット音
	void EnemyHitMusic(void);

	// HPバーの描画
	void DrawHPBar(void);

};
