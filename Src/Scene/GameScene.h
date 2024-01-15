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

	// プレイヤーの攻撃が当たった時のエフェクトの相対座標
	static constexpr VECTOR LOCAL_CHRAGE_POS = { 0.0f,200.0f,0.0f };

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
	// 攻撃が当たった時のエフェクト
	int effectImpactResId_;
	int effectImpactPlayId_;
	VECTOR effectImpactPos_;

	// 音
	// プレイヤーの攻撃が当たった時の音１
	int musicImpactId1_;

	// プレイヤーの攻撃が当たった時の音２
	int musicImpactId2_;

	// プレイヤーの攻撃が当たった時の音３
	int musicImpactId3_;

	// プレイヤーと敵同士の当たり判定
	void CollisionEnemyAndPlayer();

	// デバッグ描画
	void DrawDebug(void);

	// エフェクトの初期化
	void InitEffect(void);

	// 攻撃が当たった時のエフェクト
	void ImpactPlayEffect(void);

	// 攻撃が当たった時のエフェクト
	void ImpactSyncEffect(void);

	// 音の初期化
	void InitMusic(void);

	// プレイヤーの攻撃が当たった時の音
	void ImpactMusic(void);

};
