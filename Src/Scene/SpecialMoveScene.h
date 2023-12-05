#pragma once
#include "SceneBase.h"
class Grid;
class Stage;
class Player;
class Enemy;
class Sword;

class SpecialMoveScene : public SceneBase
{
public:

	// コンストラクタ
	SpecialMoveScene(void);

	// デストラクタ
	~SpecialMoveScene(void);

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





};

