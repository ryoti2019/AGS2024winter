
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
		float endStep = 0.0f;

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
	AnimationController(int modelId, int animIndex);

	// デストラクタ
	~AnimationController(void);

	// アニメーション追加
	//void Add(STATE state, const std::string& path, float speed);
	void Add(const std::string state, const std::string& path, float startStep,
		float animTotalTime, float speed, int animHandle, bool isLoop, bool isStop);

	void Update(void);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	std::string GetPlayType(void) const;

	// アタッチされている数を取得
	int GetAttachNum(void) const;

	// 状態遷移
	void ChangeAnimation(std::string state, bool isForce = false);

	// アニメーションデータの取得
	AnimationData GetAnimData(const std::string& state);

	const std::map <std::string, AnimationData>& GetAnimDatas(void) const;

	// 今のアニメーションデータの取得
	std::string GetAnimDataNow(void);

	// アニメーションが始まる時間を設定
	void SetStartStepAnim(std::string state, float stepAnim);

	// 優先されているアニメーションタイプを取得
	bool GetIsPriority(void);

	bool IsEndPlayAnimation(void);

	bool IsEndBlendPlayingAnimation(std::string state);

	bool IsBlendPlay(std::string state);

private:

	// 状態
	//STATE state_;
	//STATE preState_;

	// モデルのハンドルID
	int modelId_;

	// モデルを読み込んだかのフラグ
	bool LoadModel_;

	// 種類別のアニメーションデータ
	std::map <std::string, AnimationData> animData_;

	std::string state_ = "";

	std::string preState_ = "";

	// アニメーション終了後に繰り返すループステップ
	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;

	// 逆再生
	float switchLoopReverse_;

	// アタッチしている数
	int AttachNum_;

	// アニメーション番号
	int animIndex_;

	// アタッチ
	void Attatch(std::string state);

	// デタッチ
	void Dettach(int attachNo);

};
