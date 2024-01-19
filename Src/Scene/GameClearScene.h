#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

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

	// モデル制御の基本情報
	Transform transform_;

	// ゲームシーンの音楽
	int musicGameClearId_;

	// 決定音
	int musicDecisionId_;

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
	int gameClearAnim_;

	// ゲームクリアの画像
	int imgGameClear_;

	// 点滅用のカウンタ
	int BlinkCnt_;

	// エフェクト
	int effectFireWorksResId_;
	int effectFireWorksPlayId_;
	VECTOR effectPos_;
	float stepEffect_;

	// ロゴ描画
	void DrawLogo(void);

	// アニメーションの初期化
	void InitAnimation(void);

	// エフェクトの初期化
	void InitEffect(void);

	// アニメーション
	void Animation(void);

	// アニメーションの設定
	void SetAnimation(void);

	// エフェクト再生
	void PlayEffect(void);

	// エフェクト位置
	void SyncEffect(void);

	// 音の初期化
	void InitMusic(void);

};