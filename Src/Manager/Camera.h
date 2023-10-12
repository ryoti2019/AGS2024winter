#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:

	// カメラの高さ
	static constexpr float HEIGHT = 250.0f;

	// 追従対象からカメラまでの距離
	static constexpr float DIS_FOLLOW2CAMERA = 150.0f;

	// 追従対象から注視点までの距離
	static constexpr float DIS_FOLLOW2TARGET = 350.0f;

	// カメラスピード：NEAR
	static constexpr float SPEED = 20.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 500.0f, -500.0f };

	// カメラ位置から注視点までの相対座標
	//static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	// 追従対象からカメラ位置までの相対座標(完全追従)
	//static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 1000.0f, 300.0f };


	// F - FOLLOW - 追従対象 - Player
	// C - CAMERA - カメラ位置
	// T - TARGET - 注視点

	// 追従対象からカメラの位置までの相対座標
	static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 500.0f,-400.0f };

	// 追従対象から注視点までの相対座標
	static constexpr VECTOR LOCAL_F2T_POS = { 0.0f,0.0f,500.0f };

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// 定点カメラ
		FREE,			// フリーモード
		FOLLOW,			// 追従モード
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;

	VECTOR GetAngles(void) const;

	void SetAngles(const VECTOR angles);

	// プレイヤーが向いている角度
	void SetLazyAngles(const VECTOR angles);

	VECTOR GetTargetPos(void) const;

	void SetBeforeDrawFollow(void);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

	// カメラモードの変更
	void ChangeMode(MODE mode);

	Quaternion GetRotY(void) const;

	// 遅延回転
	void LazyRotation(float goalRot);

	// 遅延回転2
	void LazyRotation2(void);

private:

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラの注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	// カメラの回転
	Quaternion rotXY_;
	Quaternion rotY_;

	// Y軸のゴールの角度
	Quaternion lazyGoalRotY_;

	// SetLazyAngleに値が入ったらtrue
	bool isLazy_ = false;

	// カメラの回転
	VECTOR angle_;

	// プレイヤーがカメラを動かしたときのフラグ
	bool isOp_ = false;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 注視点をキャラクター前方位置に設定
	void SetTargetPosFollowForward(void);

	// 追従対象
	const Transform* followTransform_;

	// キーボードの操作
	void KeybordContoroller(void);

	// ゲームパッドの操作
	void GamePadController(void);

};

