#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"

class Sword
{
public:

	Sword(void);

	// デストラクタ
	~Sword(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

	// 剣のTransformの取得
	const Transform& GetTransform(void) const;

private:

	// モデル制御の基本情報
	Transform transform_;

	// アニメーションの初期化
	void InitAnimation(void);

	//武器をアタッチするフレームの番号
	int WeponAttachFrameNum;

	// 追従対象
	const Transform* followTransform_;

	// 敵との当たり判定
	void EnemyCheckHit(void);

	// 衝突判定の座標
	VECTOR collisionPos_;

};