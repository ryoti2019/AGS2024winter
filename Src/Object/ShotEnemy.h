#pragma once
#include "../Object/ShotBase.h"
class Player;

class ShotEnemy : public ShotBase
{

public:

	// コンストラクタ
	ShotEnemy(void);

	// デストラクタ
	~ShotEnemy(void);

	ShotEnemy::STATE GetState(void);

	void SetState(ShotEnemy::STATE state);

	void SetDir(VECTOR dir);

	bool IsAlive(void);

protected:

	// パラメータ設定
	void SetParam(void) override;

	void Move(void)override;

	void UpdateBlast(void)override;

	// 生存チェック
	virtual void CheckAlive(void);

	// 生存フラグ
	bool isAlive_;

};

