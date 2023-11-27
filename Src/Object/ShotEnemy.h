#pragma once
#include "../Object/ShotBase.h"

class ShotEnemy : public ShotBase
{

public:

	// コンストラクタ
	ShotEnemy(void);

	// デストラクタ
	~ShotEnemy(void);

	ShotEnemy::STATE GetState(void);

	void SetState(ShotEnemy::STATE state);

protected:

	// パラメータ設定
	void SetParam(void) override;

};

