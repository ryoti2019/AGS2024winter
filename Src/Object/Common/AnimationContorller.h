#pragma once
#include <string>
#include <map>
class SceneManager;

class AnimationController
{

public:

	//// アニメーションデータ
	//struct Animation
	//{

	//	int attachNo = -1;
	//	int animIndex = 0;
	//	float speed = 0.0f;
	//	float totalTime = 0.0f;
	//	float step = 0.0f;
	//};

	// アニメーションデータ
	struct AnimationData
	{

		// モデル
		int model = -1;

		// アニメーションハンドル
		int animHandle = -1;

		// アニメーションタイプ
		 state = 

		// アタッチNo
		int attachNo = -1;

		// 優先されるアニメーション
		bool isPriority = false;

		// アニメーションの総再生時間
		float animTotalTime = 0.0f;

		// アニメーション速度
		float speedAnim = 0.0f;

		// 再生中のアニメーション時間
		float stepAnim = 0.0f;

		// アニメーションが始まる時間
		float startAnim = 0.0f;

		// アニメーションが終わる時間
		float endAnim = 0.0f;

		// ブレンドレート
		float blendRate = 0.0f;

		// ブレンドタイム
		float blendTime = 0.0f;

	};

	// コンストラクタ
	AnimationController(int modelId);

	// デストラクタ
	~AnimationController(void);

	// アニメーション追加
	//void Add(STATE state, const std::string& path, float speed);
	void Add(const std::string state, const std::string& path, float startStep,
		float endStep, float speed, bool isPriority);

	// アニメーション再生
	void Play(STATE state, bool isLoop = true,
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isPriority = false);

	void Update(void);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	AnimationController::STATE GetPlayType(void) const;

	// 再生終了
	bool IsEnd(void) const;

	// アタッチされている数を取得
	int GetAttachNum(void) const;


private:

	// 状態
	//STATE state_;
	//STATE preState_;

	// モデルのハンドルID
	int modelId_;

	// モデルを読み込んだかのフラグ
	bool LoadModel_;

	// 種類別のアニメーションデータ
	std::map < std::string , AnimationData > animData_;

	std::string state_ = "";

	// アニメーションをループするかしないか
	bool isLoop_;

	// アニメーションを止めたままにする
	bool isStop_;

	// アニメーション終了後に繰り返すループステップ
	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;

	// 逆再生
	float switchLoopReverse_;

	// アタッチしている数
	int AttachNum_;

	// アタッチ
	void Attatch(STATE state);

	// デタッチ
	void Dettach(int attachNo, STATE state);

	// 状態遷移
	void ChangeAnimation(STATE state);

};

