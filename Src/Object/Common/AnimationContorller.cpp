#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "AnimationContorller.h"

AnimationController::AnimationController(int modelId)
{

	modelId_ = modelId;
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

void AnimationController::Add(const std::string state, const std::string& path,float startStep,
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
	anim.blendRate = 1.0f;
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
		animData.second.stepAnim += (deltaTime * animData.second.speedAnim);

		if (animData.second.stepAnim > animData.second.animTotalTime && animData.second.isLoop)
		{
			// ���[�v�Đ�
			animData.second.stepAnim = 0.0f;
		}
		else if (animData.second.stepAnim > animData.second.animTotalTime && !animData.second.isLoop)
		{
			animData.second.stepAnim = animData.second.animTotalTime;
		}

		// �A�j���[�V�����ݒ�
		MV1SetAttachAnimTime(modelId_, animData.second.attachNo, animData.second.stepAnim);

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


bool AnimationController::IsEnd(void) const
{

	bool ret = false;

	for (auto& animData : animData_)
	{
		if (animData.second.isLoop)
		{
			// ���[�v�ݒ肳��Ă���Ȃ�A
			// �������ŏI�����Ȃ���Ԃ�
			return ret;
		}
	}

	for (auto& animData : animData_)
	{
		if (animData.second.stepAnim >= animData.second.animTotalTime)
		{
			// �Đ����Ԃ��߂�����true
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

	// �A�j���[�V���������Ԃ̎擾
	animData_[state].animTotalTime = MV1GetAttachAnimTotalTime(modelId_, animData_[state].attachNo);

}

void AnimationController::Dettach(int attachNo)
{
	AttachNum_--;
	MV1DetachAnim(modelId_, attachNo);
}

void AnimationController::ChangeAnimation(std::string state)
{

	// ������Ԃ����������Ȃ�
	if (state == preState_) return;

	state_ = state;

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

	//state_ = state;

	//Play(state,)

	//// ��ԑJ�ڎ��̏���������
	//switch(animData_[state].animHandle)
	//{
	//case animData.animHandle:
	//	Play((AnimationController::STATE)STATE::IDLE, false, 0.0f, 300.0f, false, false);
	//	// �������~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	break;
	//case Player::STATE::WALK:
	//	animationController_->Play((AnimationController::STATE)STATE::WALK, false, 0.0f, 21.0f, false, false);
	//	break;
	//case Player::STATE::CHARGE_WALK:
	//	animationController_->Play((AnimationController::STATE)STATE::CHARGE_WALK, false, 0.0f, 33.0f, false, false);
	//	// �G�t�F�N�g�̍Đ�
	//	ChargePlayEffect();

	//	// ���߂鉹�̍Đ�
	//	PlaySoundMem(musicChargeId_, DX_PLAYTYPE_BACK);

	//	// �������~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	break;
	//case Player::STATE::RUN:
	//	animationController_->Play((AnimationController::STATE)STATE::RUN, false, 0.0f, 22.0f, false, false);
	//	break;
	//case Player::STATE::ATTACK:
	//	animationController_->Play((AnimationController::STATE)STATE::ATTACK, false, 0.0f, 45.0f, false, false);
	//	hit_ = false;
	//	// �G�t�F�N�g���~�߂�
	//	StopEffekseer3DEffect(effectChargePlayId_);
	//	// �������~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	break;
	//case Player::STATE::ATTACK2:
	//	animationController_->Play((AnimationController::STATE)STATE::ATTACK2, false, 0.0f, 50.0f, false, false);
	//	hit_ = false;
	//	// �������~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	break;
	//case Player::STATE::ATTACK3:
	//	animationController_->Play((AnimationController::STATE)STATE::ATTACK3, false, 0.0f, 73.0f, false, false);
	//	hit_ = false;
	//	// �������~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	break;
	//case Player::STATE::CHARGE_ATTACK:
	//	animationController_->Play((AnimationController::STATE)STATE::CHARGE_ATTACK, false, 0.0f, 52.0f, false, false);
	//	hit_ = false;
	//	// �������~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	break;
	//case Player::STATE::HIT:
	//	animationController_->Play((AnimationController::STATE)STATE::HIT, false, 0.0f, 21.0f, false, false);
	//	// �G�t�F�N�g���~�߂�
	//	StopEffekseer3DEffect(effectChargePlayId_);
	//	// �������~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	break;
	//case Player::STATE::DEATH:
	//	animationController_->Play((AnimationController::STATE)STATE::DEATH, false, 0.0f, 117.0f, false, false);
	//	// �������~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	break;
	//case Player::STATE::ROLL:
	//	animationController_->Play((AnimationController::STATE)STATE::ROLL, false, 0.0f, 71.0f, false, false);
	//	// �������~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	// �X�^�~�i�����炷
	//	stamina_ -= 10.0f;
	//	break;
	//case Player::STATE::TIRED:
	//	animationController_->Play((AnimationController::STATE)STATE::TIRED, false, 0.0f, 80.0f, false, false);
	//	// �������~�߂�
	//	StopSoundMem(musicFootStepsId_);
	//	musicFootStepsCnt_ = 0.0f;
	//	break;
	//}

	//if (state_ != STATE::CHARGE_WALK)
	//{
	//	StopSoundMem(musicChargeId_);
	//}

}

AnimationController::AnimationData AnimationController::GetAnimData(const std::string& state)
{
	return animData_[state];
}

const std::map<std::string, AnimationController::AnimationData>& AnimationController::GetAnimDatas(void) const
{
	return animData_;
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