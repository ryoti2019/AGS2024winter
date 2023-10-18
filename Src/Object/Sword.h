#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"

class Sword
{
public:

	// プレイヤーの右手から剣までの相対座標
	static constexpr VECTOR LOCAL_SWORD_POS = { 60.0f,0.0f,0.0f };

	// カプセルの上の相対座標
	static constexpr VECTOR LOCAL_C_DOWN_POS = { -50.0f,5.0f,0.0f };

	// カプセルの下の相対座標
	static constexpr VECTOR LOCAL_C_UP_POS = { 100.0f,5.0f,0.0f };

	// 衝突判定の球体半径
	static constexpr float COLLISION_RADIUS = 10.0f;

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

	// 衝突判定の下の座標の取得
	VECTOR GetCPosDown(void);

	// 衝突判定の上の座標の取得
	VECTOR GetCPosUP(void);

private:

	// モデル制御の基本情報
	Transform transform_;

	// アニメーションの初期化
	void InitAnimation(void);

	// 武器をアタッチするフレームの番号
	int WeponAttachFrameNum_;

	// 追従対象
	const Transform* followTransform_;

	// 敵との当たり判定
	void EnemyCheckHit(void);

	// 衝突判定の座標
	VECTOR cPosUp_;
	VECTOR cPosDown_;

	// 衝突判定の回転
	Quaternion cQuaUP_;
	Quaternion cQuaDOWN_;

};