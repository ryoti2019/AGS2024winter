#pragma once
#include <string>
#include <DxLib.h>
#include "Common/Transform.h"

class AnimationManager;

class UnitBase
{

public:

	// アニメーションの再生速度
	static constexpr float SPEED_ANIM = 20.0f;

	// 回転にかかる時間
	static constexpr float TIME_ROT = 1.0f;

	// 回転量(deg)
	static constexpr float SPEED_ROT = 8.0f;

	// 回転量(rad)
	static constexpr float SPEED_ROT_RAD = SPEED_ROT * (DX_PI_F / 180.0f);

	UnitBase(void);
	virtual ~UnitBase(void);
	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	// プレイヤーのTransformの取得
	const Transform& GetTransform(void) const;

	// 再生中のアニメーション時間
	float GetStepAnim(void);

protected:

	//アニメーション
	AnimationManager* animater_;

	// モデルファイル名
	std::string modelFileName_;

	// モデル制御の基本情報
	Transform transform_;

	// モデルID
	int modelId_;

	//アニメーション
	int modelWalk_;
	int modelRun_;

	// 表示座標
	VECTOR pos_;

	// 角度
	VECTOR rot_;

	// ローカル角度
	//(内部的に調整するための角度)
	VECTOR rotLocal_;

	// HP
	int hp_;

	// HPの最大値
	int hpMax_;

	// パラメータ設定
	virtual void SetParam(void) = 0;

	// 移動
	virtual void Move(void);

	// アニメーション
	virtual void Animation(void);

	// アニメーションの初期化
	virtual void InitAnimation(void) = 0;

};
