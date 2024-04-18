#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "AnimationContorller.h"

AnimationController::AnimationController(int modelId)
{

	modelId_ = modelId;

	state_ = STATE::IDLE;
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
//		// ����ւ�
//		animData_.emplace((int)state, anim);
//	}
//	else
//	{
//		// �ǉ�
//		animData_[(int)state].model = anim.model;
//		animData_[(int)state].animType = anim.animType;
//		animData_[(int)state].attachNo = anim.attachNo;
//		animData_[(int)state].animTotalTime = anim.animTotalTime;
//	}
//
//}

void AnimationController::Add(const std::string state, const std::string& path,float startStep,
	float endStep, float speed, bool isPriority)
{

	AnimationData anim;

	// �ǉ�
	if (LoadModel_)
	{
		modelId_ = MV1LoadModel(path.c_str());
		LoadModel_ = false;
	}

	anim.model = modelId_;
	anim.speedAnim = speed;
	anim.animType = anim.animType;
	anim.attachNo = anim.attachNo;
	anim.animTotalTime = anim.animTotalTime;

	// ����ւ�
	animData_.emplace(state, anim);

	state_ = state;

}

void AnimationController::Play(STATE state, bool isLoop, float startStep, float endStep, bool isStop, bool isPriority)
{

	//// ������Ԃ����������Ȃ�
	//if (state == preState_) return;

	//// �O�̏�Ԃ̗D��x���Ȃ���
	//animData_[(int)preState_].isPriority = false;

	// �A�^�b�`
	Attatch(state);

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
			Dettach(animData.second.attachNo, preState_);

			// �l�̏�����
			animData.second.blendRate = 0.0f;
			animData.second.isPriority = false;
			animData.second.stepAnim = 0.0f;
			animData.second.attachNo = -1;
		}

		rate -= animData.second.blendRate;

	}

	// �A�j���[�V�����Đ�
	for (auto& animData : animData_)
	{
		if (animData.second.attachNo == -1)
		{
			continue;
		}

		if (!isStop_)
		{
			// �Đ�
			animData.second.stepAnim += (deltaTime * animData.second.speedAnim * switchLoopReverse_);

			// �A�j���[�V�����I������
			bool isEnd = false;
			if (switchLoopReverse_ > 0.0f)
			{
				// �ʏ�Đ��̏ꍇ
				if (animData.second.stepAnim > animData.second.animTotalTime)
				{
					isEnd = true;
				}
			}
			else
			{
				// �t�Đ��̏ꍇ
				if (animData.second.stepAnim < animData.second.animTotalTime)
				{
					isEnd = true;
				}
			}

			if (isEnd)
			{
				// �A�j���[�V�������I��������
				if (isLoop_)
				{
					// ���[�v�Đ�
					if (stepEndLoopStart_ > 0.0f)
					{
						// �A�j���[�V�����I����̎w��t���[���Đ�
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
						// �ʏ�̃��[�v�Đ�
						animData.second.stepAnim = 0.0f;
					}
				}
				else
				{
					// ���[�v���Ȃ�
					animData.second.stepAnim = animData.second.animTotalTime;
				}

			}

		}

		// �A�j���[�V�����ݒ�
		MV1SetAttachAnimTime(modelId_, animData.second.attachNo, animData.second.stepAnim);

	}

}

void AnimationController::SetEndLoop(float startStep, float endStep, float speed)
{
	stepEndLoopStart_ = startStep;
	stepEndLoopEnd_ = endStep;
	endLoopSpeed_ = speed;
}

AnimationController::STATE AnimationController::GetPlayType(void) const
{
	return state_;
}


bool AnimationController::IsEnd(void) const
{

	bool ret = false;

	if (isLoop_)
	{
		// ���[�v�ݒ肳��Ă���Ȃ�A
		// �������ŏI�����Ȃ���Ԃ�
		return ret;
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

void AnimationController::Attatch(STATE state)
{

	if (animData_[(int)state].attachNo != -1)
	{
		animData_[(int)state].isPriority = true;
		return;
	}
	AttachNum_++;
	animData_[(int)state].attachNo = MV1AttachAnim(modelId_, 1, animData_[(int)state].animHandle);
	animData_[(int)state].isPriority = true;

	// �A�j���[�V���������Ԃ̎擾
	animData_[(int)state].animTotalTime = MV1GetAttachAnimTotalTime(modelId_, animData_[(int)state].attachNo);

}

void AnimationController::Dettach(int attachNo, STATE state)
{

	if (animData_[(int)state].attachNo == attachNo)return;
	AttachNum_--;
	MV1DetachAnim(modelId_, attachNo);

}

void AnimationController::ChangeAnimation(STATE state)
{

	// ������Ԃ����������Ȃ�
	if (state == preState_) return;

	// �O�̏�Ԃ̗D��x���Ȃ���
	animData_[(int)preState_].isPriority = false;

	// �Đ�����A�j���[�V�����̐ݒ�
	Attatch(state);

	// ��ԑJ�ڎ��̏���������
	switch (state)
	{
	case Player::STATE::IDLE:
		animationController_->Play((AnimationController::STATE)STATE::IDLE, false, 0.0f, 300.0f, false, false);
		//// �������~�߂�
		//StopSoundMem(musicFootStepsId_);
		//musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::WALK:
		animationController_->Play((AnimationController::STATE)STATE::WALK, false, 0.0f, 21.0f, false, false);
		break;
	case Player::STATE::CHARGE_WALK:
		animationController_->Play((AnimationController::STATE)STATE::CHARGE_WALK, false, 0.0f, 33.0f, false, false);
		// �G�t�F�N�g�̍Đ�
		ChargePlayEffect();

		// ���߂鉹�̍Đ�
		PlaySoundMem(musicChargeId_, DX_PLAYTYPE_BACK);

		// �������~�߂�
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::RUN:
		animationController_->Play((AnimationController::STATE)STATE::RUN, false, 0.0f, 22.0f, false, false);
		break;
	case Player::STATE::ATTACK:
		animationController_->Play((AnimationController::STATE)STATE::ATTACK, false, 0.0f, 45.0f, false, false);
		hit_ = false;
		// �G�t�F�N�g���~�߂�
		StopEffekseer3DEffect(effectChargePlayId_);
		// �������~�߂�
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::ATTACK2:
		animationController_->Play((AnimationController::STATE)STATE::ATTACK2, false, 0.0f, 50.0f, false, false);
		hit_ = false;
		// �������~�߂�
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::ATTACK3:
		animationController_->Play((AnimationController::STATE)STATE::ATTACK3, false, 0.0f, 73.0f, false, false);
		hit_ = false;
		// �������~�߂�
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::CHARGE_ATTACK:
		animationController_->Play((AnimationController::STATE)STATE::CHARGE_ATTACK, false, 0.0f, 52.0f, false, false);
		hit_ = false;
		// �������~�߂�
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::HIT:
		animationController_->Play((AnimationController::STATE)STATE::HIT, false, 0.0f, 21.0f, false, false);
		// �G�t�F�N�g���~�߂�
		StopEffekseer3DEffect(effectChargePlayId_);
		// �������~�߂�
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::DEATH:
		animationController_->Play((AnimationController::STATE)STATE::DEATH, false, 0.0f, 117.0f, false, false);
		// �������~�߂�
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	case Player::STATE::ROLL:
		animationController_->Play((AnimationController::STATE)STATE::ROLL, false, 0.0f, 71.0f, false, false);
		// �������~�߂�
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		// �X�^�~�i�����炷
		stamina_ -= 10.0f;
		break;
	case Player::STATE::TIRED:
		animationController_->Play((AnimationController::STATE)STATE::TIRED, false, 0.0f, 80.0f, false, false);
		// �������~�߂�
		StopSoundMem(musicFootStepsId_);
		musicFootStepsCnt_ = 0.0f;
		break;
	}

	if (state_ != STATE::CHARGE_WALK)
	{
		StopSoundMem(musicChargeId_);
	}

	// ��O�̏�Ԃ�ۑ�
	preState_ = state_;

}
