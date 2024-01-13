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

	// プレイヤーと敵同士の当たり判定
	void CollisionEnemyAndPlayer();

	// デバッグ描画
	void DrawDebug(void);

};
