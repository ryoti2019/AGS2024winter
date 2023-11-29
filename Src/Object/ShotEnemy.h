#pragma once
#include "../Object/ShotBase.h"
class Player;

class ShotEnemy : public ShotBase
{

public:

	// コンストラクタ
	ShotEnemy();

	// デストラクタ
	~ShotEnemy(void);

	ShotEnemy::STATE GetState(void);

	void SetState(ShotEnemy::STATE state);

	void SetDir(VECTOR dir);

	bool IsAlive(void);

	bool IsIdle(void);

	void Shot(VECTOR dir);

	/// <summary>
	/// 弾の生成
	/// </summary>
	/// <param name="birthPos">初期座標</param>
	/// <param name="dir">進行方向</param>
	void Create(VECTOR relPos, Transform* follow) override;

protected:

	// パラメータ設定
	void SetParam(void) override;

	void Move(void)override;

	void UpdateIdle(void)override;
	void UpdateBlast(void)override;

	// 生存チェック
	virtual void CheckAlive(void);

	// 生存フラグ
	bool isAlive_;

	// 敵のTransform
	Transform* enemyTransform_;

	// 相対座標
	VECTOR rPos_;

};

