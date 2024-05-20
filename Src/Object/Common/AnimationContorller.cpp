
#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "AnimationContorller.h"

AnimationController::AnimationController(int modelId, int animIndex)
{

	modelId_ = modelId;
	animIndex_ = animIndex;
	switchLoopReverse_ = 0.0f;
	endLoopSpeed_ = 0.0f;
	stepEndLoopStart_ = 0.0f;
	stepEndLoopEnd_ = 0.0f;
	AttachNum_ = 0;

}

AnimationController::~AnimationController(void)
{
	MV1DeleteModel(modelId_);
}

void AnimationController::Add(const std::string state, const std::string& path, float startStep,
	float animTotalTime, float speed, int animHandle, bool isLoop, bool isStop)
{

	AnimationData anim;

	// �ǉ�
	if (LoadModel_)
	{
		modelId_ = MV1LoadModel(path.c_str());
		Attatch(state);
		LoadModel_ = false;
	}

	anim.speedAnim = speed;
	anim.startTime = startStep;
	anim.animTotalTime = animTotalTime;
	anim.animHandle = animHandle;
	anim.blendTime = 0.5f;
	anim.blendRate = 0.0f;
	anim.isLoop = isLoop;
	anim.isStop = isStop;
	anim.state = state;
	animData_.emplace(state, anim);

}

void AnimationController::Update(void)
{

	// �o�ߎ��Ԃ̎擾
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// ���[�g�̌v�Z
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
   			Dettach(animData.second.attachNo);

			// �l�̏�����
			animData.second.blendRate = 0.0f;
			animData.second.isPriority = false;
			animData.second.stepAnim = 0.0f;
			animData.second.attachNo = -1;
		}

		rate -= animData.second.blendRate;

	}

	animData_.at(state_).blendRate = rate;

	// �A�j���[�V�����Đ�
	for (auto& animData : animData_)
	{
		if (animData.second.attachNo == -1)
		{
			continue;
		}

		// �Đ�
		if (animData.second.stepAnim <= animData.second.animTotalTime)
		{
			animData.second.stepAnim += (deltaTime * animData.second.speedAnim);
		}
		if (animData.second.stepAnim > animData.second.animTotalTime && animData.second.isLoop)
		{
			// ���[�v�Đ�
			animData.second.stepAnim = 0.0f;
		}
		else if (animData.second.stepAnim > animData.second.animTotalTime && !animData.second.isLoop)
		{
			animData.second.stepAnim = animData.second.animTotalTime;
		}

		// �Đ�����A�j���[�V�������Ԃ̐ݒ�
		MV1SetAttachAnimTime(modelId_, animData.second.attachNo, animData.second.stepAnim);

		if (animData.second.isPriority)
		{
			animData.second.blendRate = rate;
		}

		MV1SetAttachAnimBlendRate(
			modelId_, animData.second.attachNo, animData.second.blendRate);

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

int AnimationController::GetAttachNum(void) const
{
	return AttachNum_;
}

void AnimationController::Attatch(std::string state)
{

	animData_[state].stepAnim = 0.0f;


	if (animData_[state].attachNo != -1)
	{
		animData_[state].isPriority = true;
		return;
	}
	AttachNum_++;
	animData_[state].attachNo = MV1AttachAnim(modelId_, animIndex_, animData_[state].animHandle);
	animData_[state].isPriority = true;

	// �A�j���[�V���������Ԃ̎擾
	animData_[state].animTotalTime = MV1GetAttachAnimTotalTime(modelId_, animData_[state].attachNo);

}

void AnimationController::Dettach(int attachNo)
{
	AttachNum_--;
	MV1DetachAnim(modelId_, attachNo);
}

void AnimationController::ChangeAnimation(std::string state, bool isForce)
{

	// ������Ԃ����������Ȃ�
	if (state == preState_ && !isForce) return;

	state_ = state;

	if (state_ == "IDLE")
	{
		int a = 1;
	}
	// �O�̏�Ԃ�
	if (preState_ != "")
	{
		animData_[preState_].isPriority = false;
	}

	animData_[state_].blendRate = 0.0f;

	// �Đ�����A�j���[�V�����̐ݒ�
	Attatch(state);

	// ��O�̏�Ԃ�ۑ�
	preState_ = state;

}

AnimationController::AnimationData AnimationController::GetAnimData(const std::string& state)
{
	return animData_[state];
}

const std::map<std::string, AnimationController::AnimationData>& AnimationController::GetAnimDatas(void) const
{
	return animData_;
}

std::string AnimationController::GetAnimDataNow(void)
{
	return state_;
}

void AnimationController::SetStartStepAnim(std::string state, float stepAnim)
{
	animData_[state].stepAnim = stepAnim;
}

bool AnimationController::GetIsPriority(void)
{
	return animData_[preState_].isPriority;
}

bool AnimationController::IsEndPlayAnimation(void)
{
	const auto& data = animData_[state_];
	if (!data.isLoop && data.stepAnim >= data.animTotalTime)
	{
		return true;
	}
	return false;
}

bool AnimationController::IsEndBlendPlayingAnimation(std::string state)
{
	// �w�肵���A�j���[�V�������Đ��I������Ă��邩
	const auto& data = animData_[state];
	if (data.blendRate > 0.0 && data.state == state)
	{

		if (!data.isLoop && data.stepAnim >= data.animTotalTime)
		{
			return true;
		}
	}
	return false;
}

bool AnimationController::IsBlendPlay(std::string state)
{

	for (auto& data : animData_)
	{
		if (data.second.blendRate > 0.0 && data.second.state == state)
		{
			return true;
		}
	}

	return false;
}
