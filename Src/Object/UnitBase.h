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

	VECTOR GetPos(void) const;
	VECTOR GetRot(void) const;

	// プレイヤーのTransformの取得
	const Transform& GetTransform(void) const;

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

	// 表示座標
	VECTOR pos_;

	// 角度
	VECTOR rot_;

	// ローカル角度
	//(内部的に調整するための角度)
	VECTOR rotLocal_;

	// パラメータ設定
	virtual void SetParam(void) = 0;

	// 移動 実装して貰うように。
	virtual void Move(void);

	// アニメーション
	void Animation(void);

	// 回転設定
	void SetRotation(void);

	// 遅延回転
	void LazyRotation(float goalRot);
	
	// 
	virtual void InitAnimation(void);

};
