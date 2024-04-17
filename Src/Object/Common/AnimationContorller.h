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
	struct Animation
	{
		// モデル
		int model = -1;

		// アニメーションハンドル
		int animHandle = -1;

		// アニメーションタイプ
		int animType = 0;

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
	void Add(int type, const std::string& path, float speed);

	// アニメーション再生
	void Play(int type, bool isLoop = true,
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	void Update(void);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	int GetPlayType(void) const;

	// 再生終了
	bool IsEnd(void) const;

private:

	// モデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map<int, Animation> animations_;

	int playType_;
	Animation playAnim_;

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

};

