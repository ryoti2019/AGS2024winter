#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"

class Sword
{
public:

	// コンストラクタ
	//Sword(const Transform& follow);

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

	// 追従対象
	//const Transform& follow_;

	// 追従対象
	const Transform* followTransform_;
};


