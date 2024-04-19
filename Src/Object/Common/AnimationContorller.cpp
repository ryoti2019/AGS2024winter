#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "AnimationContorller.h"

AnimationController::AnimationController(int modelId)
{

	modelId_ = modelId;

	isLoop_ = false;
	isStop_ = false;
	switchLoopReverse_ = 0.0f;
	endLoopSpeed_ = 0.0f;
	stepEndLoopStart_ = 0.0f;
	stepEndLoopEnd_ = 0.0f;
	AttachNum_ = 0;

}

AnimationController::~AnimationController(void)
{
	for (const auto& anim : animData_)
	{
		MV1DeleteModel(anim.second.model);
	}
}

//void AnimationController::Add(const std::string& state, float speed)
//{
//
//	AnimationData anim;
//
//	anim.model = MV1LoadModel(path.c_str());
//	anim.animType = state;
//	anim.speedAnim = speed;
//
//	if (animData_.count((int)state) == 0)
//	{
//		// 入れ替え
//		animData_.emplace((int)state, anim);
//	}
//	else
//	{
//		// 追加
//		animData_[(int)state].model = anim.model;
//		animData_[(int)state].animType = anim.animType;
//		animData_[(int)state].attachNo = anim.attachNo;
//		animData_[(int)state].animTotalTime = anim.animTotalTime;
//	}
//
//}

void AnimationController::Add(const std::string state, const std::string& path,float startStep,
	float endStep, float speed, int animHandle)
{

	AnimationData anim;

	// 追加
	if (LoadModel_)
	{
		modelId_ = MV1LoadModel(path.c_str());
		Attatch(state);
		LoadModel_ = false;
	}

	anim.model = modelId_;
	anim.speedAnim = speed;
	anim.startTime = startStep;
	anim.animTotalTime = endStep;
	anim.animHandle = animHandle;
	anim.blendTime = 0.5f;
	anim.blendRate = 1.0f;

	// 入れ替え
	animData_.emplace(state, anim);

	state_ = state;

}

void AnimationController::Play(std::string state, bool isLoop, bool isStop, bool isPriority)
{

	//// 同じ状態だったら入らない
	//if (state == preState_) return;

	//// 前の状態の優先度をなくす
	//animData_[(int)preState_].isPriority = false;

	isLoop_ = isLoop;
	isStop_ = isStop;
	AnimationData anim;

	animData_[state].isPriority = isPriority;

	// アタッチ
	Attatch(state);

}

void AnimationController::Update(void)
{

	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// レートの計算
	float rate = 1.0f;
	for (auto& animData : animData_)
	{
		if (animData.second.attachNo == -1 || animData.second.isPriority)
		{
			continue;
		}

		animData.second.blendRate -= deltaTime / animData.second.blendTime;
		if (animData.second.blendRate <= 0.0f)
		{
			animData.second.blendRate = 0.0f;
		}

		MV1SetAttachAnimBlendRate(
			modelId_, animData.second.attachNo, animData.second.blendRate);


		rate -= animData.second.blendRate;

	}

	animData_.at(state_).blendRate = rate;

	// アニメーション再生
	for (auto& animData : animData_)
	{
		if (animData.second.attachNo == -1)
		{
			continue;
		}

		if (!isStop_)
		{
			// 再生
			animData.second.stepAnim += (deltaTime * animData.second.speedAnim * switchLoopReverse_);

			// アニメーション終了判定
			bool isEnd = false;
			if (switchLoopReverse_ > 0.0f)
			{
				// 通常再生の場合
				if (animData.second.stepAnim > animData.second.animTotalTime)
				{
					isEnd = true;
				}
			}
			else
			{
				// 逆再生の場合
				if (animData.second.stepAnim < animData.second.animTotalTime)
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
							animData.second.stepAnim = stepEndLoopStart_;
							animData.second.animTotalTime = stepEndLoopEnd_;
						}
						else
						{
							animData.second.stepAnim = stepEndLoopEnd_;
							animData.second.animTotalTime = stepEndLoopStart_;
						}
						animData.second.speedAnim = endLoopSpeed_;

					}
					else
					{
						// 通常のループ再生
						animData.second.stepAnim = 0.0f;
					}
				}
				else
				{
					// ループしない
					animData.second.stepAnim = animData.second.animTotalTime;
				}

			}

		}

		// アニメーション設定
		MV1SetAttachAnimTime(modelId_, animData.second.attachNo, animData.second.stepAnim);

	}

	for (auto& animData : animData_)
	{
		if (animData.second.attachNo == -1)continue;

		if (animData.second.blendRate <= 0.0f)
		{
			Dettach(animData.second.attachNo);

			// 値の初期化
			animData.second.blendRate = 0.0f;
			animData.second.isPriority = false;
			animData.second.stepAnim = 0.0f;
			animData.second.attachNo = -1;
		}
	}

}

void AnimationController::SetEndLoop(float startStep, float endStep, float speed)
{
	stepEndLoopStart_ = startStep;
	stepEndLoopEnd_ = endStep;
	endLoopSpeed_ = speed;
}

std::string AnimationController::GetPlayType(void) const
{
	return state_;
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

	for (auto& animData : animData_)
	{
		if (animData.second.stepAnim >= animData.second.animTotalTime)
		{
			// 再生時間を過ぎたらtrue
			return true;
		}
	}

	return ret;

}

int AnimationController::GetAttachNum(void) const
{
	return AttachNum_;
}

void AnimationController::Attatch(std::string state)
{

	if (animData_[state].attachNo != -1)
	{
		animData_[state].isPriority = true;
		return;
	}
	AttachNum_++;
	animData_[state].attachNo = MV1AttachAnim(modelId_, 1, animData_[state].animHandle);
	animData_[state].isPriority = true;

	// アニメーション総時間の取得
	animData_[state].animTotalTime = MV1GetAttachAnimTotalTime(modelId_, animData_[state].attachNo);

}

void AnimationController::Dettach(int attachNo)
{
	AttachNum_--;
	MV1DetachAnim(modelId_, attachNo);

}

void AnimationController::ChangeAnimation(std::string state)
{

	// 同じ状態だったら入らない
	if (state == preState_) return;

	// 前の状態の優先度をなくす
	animData_[preState_].isPriority = false;

	// 再生するアニメーションの設定
	Attatch(state);

	// 一個前の状態を保存
	preState_ = state;

	Play(state,)

	// 状態遷移時の初期化処理
	switch(animData_[state].animHa)
	{
	case :
		Play((AnimationController::STATE)STATE::IDLE, false, 0.0f, 300.0f, false, false);
		// 足音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::WALK:
		animationController_->Play((AnimationController::STATE)STATE::WALK, false, 0.0f, 21.0f, false, false);
		break;
	case Player::STATE::CHARGE_WALK:
		animationController_->Play((AnimationController::STATE)STATE::CHARGE_WALK, false, 0.0f, 33.0f, false, false);
		// エフェクトの再生
		ChargePlayEffect();

		// 溜める音の再生
		PlaySoundMem(musicChargeId_, DX_PLAYTYPE_BACK);

		// 足音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::RUN:
		animationController_->Play((AnimationController::STATE)STATE::RUN, false, 0.0f, 22.0f, false, false);
		break;
	case Player::STATE::ATTACK:
		animationController_->Play((AnimationController::STATE)STATE::ATTACK, false, 0.0f, 45.0f, false, false);
		hit_ = false;
		// エフェクトを止める
		StopEffekseer3DEffect(effectChargePlayId_);
		// 足音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::ATTACK2:
		animationController_->Play((AnimationController::STATE)STATE::ATTACK2, false, 0.0f, 50.0f, false, false);
		hit_ = false;
		// 足音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::ATTACK3:
		animationController_->Play((AnimationController::STATE)STATE::ATTACK3, false, 0.0f, 73.0f, false, false);
		hit_ = false;
		// 足音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::CHARGE_ATTACK:
		animationController_->Play((AnimationController::STATE)STATE::CHARGE_ATTACK, false, 0.0f, 52.0f, false, false);
		hit_ = false;
		// 足音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::HIT:
		animationController_->Play((AnimationController::STATE)STATE::HIT, false, 0.0f, 21.0f, false, false);
		// エフェクトを止める
		StopEffekseer3DEffect(effectChargePlayId_);
		// 足音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::DEATH:
		animationController_->Play((AnimationController::STATE)STATE::DEATH, false, 0.0f, 117.0f, false, false);
		// 足音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::ROLL:
		animationController_->Play((AnimationController::STATE)STATE::ROLL, false, 0.0f, 71.0f, false, false);
		// 足音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		 スタミナを減らす
		stamina_ -= 10.0f;
		break;
	case Player::STATE::TIRED:
		animationController_->Play((AnimationController::STATE)STATE::TIRED, false, 0.0f, 80.0f, false, false);
		// 足音を止める
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	}

	//if (state_ != STATE::CHARGE_WALK)
	//{
	//	StopSoundMem(musicChargeId_);
	//}

}
