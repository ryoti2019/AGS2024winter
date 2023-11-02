#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
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

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// タイトルロゴ
	imgTitleLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

}

void TitleScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	// ゲームパッドの番号を取得
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		SceneManager::GetInstance().SetGamePad(true);
	}

	// 点滅カウンタを加算する
	logoBlinkCnt_++;

}

void TitleScene::Draw(void)
{

	// ロゴ描画
	DrawLogo();

}

void TitleScene::Release(void)
{
	DeleteGraph(imgTitleLogo_);
}

void TitleScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// タイトルロゴ
	DrawRotaGraph(cx, cy - 100, 0.4, 0.0, imgTitleLogo_, true);

	// Pushメッセージ
	std::string msg = "Push Space";
	SetFontSize(40);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);

	// 点滅させる
	if ((logoBlinkCnt_ / 30) % 2)
	{
		DrawFormatString(cx - (width / 2), 500, 0x000000, msg.c_str());
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

}
