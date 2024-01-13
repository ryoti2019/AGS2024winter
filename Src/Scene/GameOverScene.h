#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class GameOverScene : public SceneBase
{

public:

	// コンストラクタ
	GameOverScene(void);

	// デストラクタ
	~GameOverScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// モデル制御の基本情報
	Transform transform_;

	// アニメーションのアタッチ番号
	int animAttachNo_;

	// アニメーションの総再生時間
	float animTotalTime_;

	// 再生中のアニメーション時間
	float stepAnim_;

	// アニメーション速度
	float speedAnim_;

	// モデルの中にあるアニメーションの番号
	int animNo_;

	// ゲームオーバーのアニメーション
	int gameOverAnim_;

	// ゲームオーバーの画像
	int imgGameOver_;

	// 点滅用のカウンタ
	int BlinkCnt_;

	// ロゴ描画
	void DrawLogo(void);

	// アニメーションの初期化
	void InitAnimation(void);

	// アニメーション
	void Animation(void);

	// アニメーションの設定
	void SetAnimation(void);

};

