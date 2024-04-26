#include "../../Manager/InputManager.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/SceneManager.h"
#include "../../Object/Common/AnimationContorller.h"
#include "../Player.h"
#include "InputContoroller.h"

InputContoroller::InputContoroller(Player* player)
{
	player_ = player;
}

InputContoroller::~InputContoroller(void)
{
}

VECTOR InputContoroller::Dir()
{

	dir_ = AsoUtility::VECTOR_ZERO;

	auto& ins = InputManager::GetInstance();

	//// ����(direction)
	//VECTOR dir = AsoUtility::VECTOR_ZERO;

	// �R���g���[���[
	// WASD�Ńv���C���[�̈ʒu��ς���
	if (!SceneManager::GetInstance().GetGamePad())
	{
		if (ins.IsNew(KEY_INPUT_W))
		{
			dir_ = { 0.0f, 0.0f, 1.0f };
		}
		if (ins.IsNew(KEY_INPUT_A))
		{
			dir_ = { -1.0f, 0.0f, 0.0f };
		}
		if (ins.IsNew(KEY_INPUT_S))
		{
			dir_ = { 0.0f, 0.0f, -1.0f };
		}
		if (ins.IsNew(KEY_INPUT_D))
		{
			dir_ = { 1.0f, 0.0f, 0.0f };
		}
	}
	else
	{
		// �Q�[���p�b�h�̔ԍ����擾
		auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		// �p�b�h�̕�����dir�ɒ���
		dir_.x = pad.AKeyLX;
		dir_.z = -pad.AKeyLZ;
	}

	return dir_;

	dir_ = AsoUtility::VECTOR_ZERO;

}

bool InputContoroller::ChargeWalk()
{

	auto& ins = InputManager::GetInstance();

	//���߂Ȃ������
	if (!SceneManager::GetInstance().GetGamePad())
	{

		if (ins.IsClickMouseLeft() && (ins.IsNew(KEY_INPUT_W) || ins.IsNew(KEY_INPUT_A) ||
			ins.IsNew(KEY_INPUT_S) || ins.IsNew(KEY_INPUT_D)) && !AsoUtility::EqualsVZero(dir_) &&
			player_->GetState() != Player::STATE::HIT && player_->GetState() != Player::STATE::ROLL &&
			player_->GetState() != Player::STATE::TIRED)
		{
			return true;
		}
	}
	if (SceneManager::GetInstance().GetGamePad())
	{
		if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) &&
			!AsoUtility::EqualsVZero(dir_) && player_->GetState() != Player::STATE::HIT &&
			player_->GetState() != Player::STATE::ROLL && player_->GetState() != Player::STATE::TIRED)
		{
			return true;
		}
	}

	return false;

}

bool InputContoroller::Run()
{

	auto& ins = InputManager::GetInstance();

	if (!SceneManager::GetInstance().GetGamePad() && ins.IsNew(KEY_INPUT_LSHIFT))
	{
		return true;
	}
	else if (SceneManager::GetInstance().GetGamePad() &&
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		return true;
	}

	return false;

}

bool InputContoroller::Roll()
{

	auto& ins = InputManager::GetInstance();

	if (!SceneManager::GetInstance().GetGamePad() && ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		return true;
	}
	else if (SceneManager::GetInstance().GetGamePad() &&
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_BOTTON))
	{
		return true;
	}

	return false;

}

bool InputContoroller::Attack()
{

	auto& ins = InputManager::GetInstance();

	if (!SceneManager::GetInstance().GetGamePad() && ins.IsTrgUpMouseLeft())
	{
		return true;
	}
	else if (SceneManager::GetInstance().GetGamePad() &&
		ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		return true;
	}

	return false;

}

bool InputContoroller::ChargeAttack()
{

	auto& ins = InputManager::GetInstance();

	if (!SceneManager::GetInstance().GetGamePad() && ins.IsClickMouseLeft())
	{
		return true;
	}
	else if (SceneManager::GetInstance().GetGamePad() &&
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		return true;
	}
	return false;

}
