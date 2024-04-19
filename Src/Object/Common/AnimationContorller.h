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
		std::string state = "";

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
		float startTime = 0.0f;

		// アニメーションが終わる時間
		float endTime = 0.0f;

		// ブレンドレート
		float blendRate = 0.0f;

		// ブレンドタイム
		float blendTime = 0.0f;

		// ループするかしないか
		bool isLoop = false;

		// ストップするかしないか
		bool isStop = false;


	};

	// コンストラクタ
	AnimationController(int modelId);

	// デストラクタ
	~AnimationController(void);

	// アニメーション追加
	//void Add(STATE state, const std::string& path, float speed);
	void Add(const std::string state, const std::string& path, float startStep,
		float endStep, float speed, int animHandle);

	// アニメーション再生
	void Play(std::string state, bool isLoop = true, bool isStop = false, bool isPriority = false);

	void Update(void);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	std::string GetPlayType(void) const;

	// 再生終了
	bool IsEnd(void) const;

	// アタッチされている数を取得
	int GetAttachNum(void) const;

	// 状態遷移
	void ChangeAnimation(std::string state);

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

	std::string preState_ = "";

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
	void Attatch(std::string state);

	// デタッチ
	void Dettach(int attachNo);

};

