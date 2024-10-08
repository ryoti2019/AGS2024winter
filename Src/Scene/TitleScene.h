#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// タイトルロゴ
	int imgTitleLogo_;

	// スペースキーかBボタン
	int imgSpaceOrB_;

	// タイトルシーンの音楽
	int musicTitleId_;

	// 決定音
	int musicDecisionId_;

	// 点滅用のカウンタ
	int BlinkCnt_;
	
	// ロゴ描画
	void DrawLogo(void);

	// 音の初期化
	void InitMusic(void);

};
