#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
	imgTitleLogo_ = -1;
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::InitAnimation(void)
{
	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_MODEL));

	// �ҋ@�A�j���[�V����
	gameOverAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_GAMEOVER);

	// transform�̏�����
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	transform_.Update();

	// �A�j���[�V��������ԍ�
	animNo_ = 1;

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// �Đ����̃A�j���[�V��������
	stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 0.0f;

}

void GameOverScene::Animation(void)
{

	// �A�j���[�V�����Đ�
	// �o�ߎ��Ԃ̎擾
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// �A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (speedAnim_ * deltaTime);

	if (stepAnim_ > animTotalTime_)
	{
		// ���[�v�Đ�
		stepAnim_ = 0.0f;
	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void GameOverScene::SetAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, gameOverAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

}

void GameOverScene::Init(void)
{

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

	// �A�j���[�V�����̐ݒ�
	SetAnimation();

	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// �^�C�g�����S
	imgTitleLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

}

void GameOverScene::Update(void)
{

	// �A�j���[�V��������
	Animation();

	transform_.pos = { 0.0f,0.0f,0.0f };

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	// �Q�[���p�b�h�̔ԍ����擾
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SceneManager::GetInstance().SetGamePad(true);
	}

	// �_�ŃJ�E���^�����Z����
	logoBlinkCnt_++;

	transform_.Update();

}

void GameOverScene::Draw(void)
{

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);

	// ���S�`��
	DrawLogo();

}

void GameOverScene::Release(void)
{
	DeleteGraph(imgTitleLogo_);
}

void GameOverScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// Push���b�Z�[�W
	std::string msg = "Space";
	SetFontSize(40);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);

	// �_�ł�����
	if ((logoBlinkCnt_ / 30) % 2)
	{
		DrawFormatString(cx - (width / 2), 500, 0x000000, msg.c_str());
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

}
