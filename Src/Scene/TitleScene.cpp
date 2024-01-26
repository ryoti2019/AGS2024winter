#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Stage.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgTitleLogo_ = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{

	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// �^�C�g�����S
	imgTitleLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

	// ���̏�����
	InitMusic();

}

void TitleScene::InitMusic(void)
{

	// �^�C�g���V�[���̉��y
	musicTitleId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_MUSIC).handleId_;

	ChangeVolumeSoundMem(255 * 80 / 100, musicTitleId_);

	// �^�C�g���V�[���̉��y�̍Đ�
	PlaySoundMem(musicTitleId_, DX_PLAYTYPE_LOOP);

	// ���艹
	musicDecisionId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DECISION_MUSIC).handleId_;


}

void TitleScene::Update(void)
{

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		PlaySoundMem(musicDecisionId_,DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	// �Q�[���p�b�h�̔ԍ����擾
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		PlaySoundMem(musicDecisionId_, DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		SceneManager::GetInstance().SetGamePad(true);
	}

	// �_�ŃJ�E���^�����Z����
	BlinkCnt_++;

}

void TitleScene::Draw(void)
{

	// ���S�`��
	DrawLogo();

}

void TitleScene::Release(void)
{

	DeleteGraph(imgTitleLogo_);

	StopSoundMem(musicTitleId_);

}

void TitleScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// �^�C�g�����S
	DrawRotaGraph(cx, cy - 100, 0.4, 0.0, imgTitleLogo_, true);

	// Push���b�Z�[�W
	ChangeFont("BIZ UD���� medium");
	std::string msg = "Push Space";
	SetFontSize(40);

	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);

	// �_�ł�����
	if ((BlinkCnt_ / 30) % 2)
	{
		DrawFormatString(cx - (width / 2), 500, 0x000000, msg.c_str());
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

}
