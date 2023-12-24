#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"

class Stage
{

public:

	// コンストラクタ
	Stage(void);

	// デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// モデルIDのゲッター
	int GetModelId(void);



private:

	// モデル制御の基本情報
	Transform transform_;

};


