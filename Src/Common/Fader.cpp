#include <DxLib.h>
#include "../Application.h"
#include "Fader.h"

Fader::Fader()
	:
	state_(STATE::NONE),
	alpha_(0),
	isPreEnd_(true),
	isEnd_(true)
{
}

void Fader::Update()
{

	if (isEnd_)
	{
		return;
	}

	switch (state_)
	{
	case STATE::NONE:
		return;

	case STATE::FADE_OUT:
		alpha_ += SPEED_ALPHA;
		if (alpha_ > 255)
		{
			// �t�F�[�h�I��
			alpha_ = 255;
			if (isPreEnd_)
			{
				// 1�t���[����(Draw��)�ɏI���Ƃ���
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}

		break;

	case STATE::FADE_IN:
		alpha_ -= SPEED_ALPHA;
		if (alpha_ < 0)
		{
			// �t�F�[�h�I��
			alpha_ = 0;
			if (isPreEnd_)
			{
				// 1�t���[����(Draw��)�ɏI���Ƃ���
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
		break;

	default:
		return;
	}

}

void Fader::Draw()
{

	switch (state_)
	{
	case STATE::NONE:
		return;
	case STATE::FADE_OUT:
	case STATE::FADE_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
		DrawBox(
			0, 0,
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y,
			0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}

}
