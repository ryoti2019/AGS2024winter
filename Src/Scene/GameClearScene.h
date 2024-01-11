#pragma once
#include "SceneBase.h"

class GameClearScene : public SceneBase
{

public:

	// コンストラクタ
	GameClearScene(void);

	// デストラクタ
	~GameClearScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// タイトルロゴ
	int imgTitleLogo_;

	// タイトルロゴの点滅用のカウンタ
	int logoBlinkCnt_;

	// ロゴ描画
	void DrawLogo(void);

};