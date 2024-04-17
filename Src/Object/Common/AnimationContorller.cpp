#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "AnimationContorller.h"

AnimationController::AnimationController(int modelId)
{

	modelId_ = modelId;

	playType_ = -1;
	isLoop_ = false;

	isStop_ = false;
	switchLoopReverse_ = 0.0f;
	endLoopSpeed_ = 0.0f;
	stepEndLoopStart_ = 0.0f;
	stepEndLoopEnd_ = 0.0f;

}

AnimationController::~AnimationController(void)
{
	for (const auto& anim : animations_)
	{
		MV1DeleteModel(anim.second.model);
	}
}

void AnimationController::Add(int type, const std::string& path, float speed)
{

	Animation anim;

	anim.model = MV1LoadModel(path.c_str());
	anim.animType = type;
	anim.speedAnim = speed;

	if (animations_.count(type) == 0)
	{
		// 入れ替え
		animations_.emplace(type, anim);
	}
	else
	{
		// 追加
		animations_[type].model = anim.model;
		animations_[type].animType = anim.animType;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].animTotalTime = anim.animTotalTime;
	}

}

void AnimationController::Play(int type, bool isLoop, float startStep, float endStep, bool isStop, bool isForce)
{

	if (playType_ != type || isForce) {

		if (playType_ != -1)
		{
			// モデルからアニメーションを外す
			playAnim_.attachNo = MV1DetachAnim(modelId_, playAnim_.attachNo);
		}

		// アニメーション種別を変更
		playType_ = type;
		playAnim_ = animations_[type];

		// 初期化
		playAnim_.stepAnim = startStep;

		// モデルにアニメーションを付ける
		int animIdx = 0;
		if (MV1GetAnimNum(playAnim_.model) > 1)
		{
			// アニメーションが複数保存されていたら、番号1を指定
			animIdx = 1;
		}
		playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);

		// アニメーション総時間の取得
		if (endStep > 0.0f)
		{
			playAnim_.animTotalTime = endStep;
		}
		else
		{
			playAnim_.animTotalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);
		}

		// アニメーションループ
		isLoop_ = isLoop;

		// アニメーションしない
		isStop_ = isStop;

		stepEndLoopStart_ = -1.0f;
		stepEndLoopEnd_ = -1.0f;
		switchLoopReverse_ = 1.0f;
	}

}

void AnimationController::Update(void)
{

	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// レートの計算
	float rate = 1.0f;
	for (auto& animData : animations_)
	{
		if (animData.attachNo_ == -1 || animData.isPriority_)
		{
			continue;
		}

		//animData.blendRate_ = animData.stepAnim_ / animData.blendTime_;
		animData.blendRate_ -= deltaTime / animData.blendTime_;

		if (animData.blendRate_ <= 0.0f)
		{
			Dettach(animData.attachNo_, (int)preState_);

			// 値の初期化
			animData.blendRate_ = 0.0f;
			animData.isPriority_ = false;
			animData.stepAnim_ = 0.0f;
			animData.attachNo_ = -1;
		}

		rate -= animData.blendRate_;

	}
	if (!isStop_)
	{
		// 再生
		playAnim_.stepAnim += (deltaTime * playAnim_.speedAnim * switchLoopReverse_);

		// アニメーション終了判定
		bool isEnd = false;
		if (switchLoopReverse_ > 0.0f)
		{
			// 通常再生の場合
			if (playAnim_.stepAnim > playAnim_.animTotalTime)
			{
				isEnd = true;
			}
		}
		else
		{
			// 逆再生の場合
			if (playAnim_.stepAnim < playAnim_.animTotalTime)
			{
				isEnd = true;
			}
		}

		if (isEnd)
		{
			// アニメーションが終了したら
			if (isLoop_)
			{
				// ループ再生
				if (stepEndLoopStart_ > 0.0f)
				{
					// アニメーション終了後の指定フレーム再生
					switchLoopReverse_ *= -1.0f;
					if (switchLoopReverse_ > 0.0f)
					{
						playAnim_.stepAnim = stepEndLoopStart_;
						playAnim_.animTotalTime = stepEndLoopEnd_;
					}
					else
					{
						playAnim_.stepAnim = stepEndLoopEnd_;
						playAnim_.animTotalTime = stepEndLoopStart_;
					}
					playAnim_.speedAnim = endLoopSpeed_;

				}
				else
				{
					// 通常のループ再生
					playAnim_.stepAnim = 0.0f;
				}
			}
			else
			{
				// ループしない
				playAnim_.stepAnim = playAnim_.animTotalTime;
			}

		}

	}

	// アニメーション設定
	MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.stepAnim);

}

void AnimationController::SetEndLoop(float startStep, float endStep, float speed)
{
	stepEndLoopStart_ = startStep;
	stepEndLoopEnd_ = endStep;
	endLoopSpeed_ = speed;
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

bool AnimationController::IsEnd(void) const
{

	bool ret = false;

	if (isLoop_)
	{
		// ループ設定されているなら、
		// 無条件で終了しないを返す
		return ret;
	}

	if (playAnim_.stepAnim >= playAnim_.animTotalTime)
	{
		// 再生時間を過ぎたらtrue
		return true;
	}

	return ret;

}
