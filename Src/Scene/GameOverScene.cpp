#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Stage.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
	imgGameOver_ = -1;
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::InitAnimation(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_MODEL));

	// 待機アニメーション
	gameOverAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_GAMEOVER);

	// transformの初期化
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	transform_.Update();

	// アニメーションする番号
	animNo_ = 1;

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// 再生中のアニメーション時間
	stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 0.0f;

}

void GameOverScene::Animation(void)
{

	// アニメーション再生
	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// アニメーション時間の進行
	stepAnim_ += (speedAnim_ * deltaTime);

	if (stepAnim_ > animTotalTime_)
	{
		// ループ再生
		stepAnim_ = 0.0f;
	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void GameOverScene::SetAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, gameOverAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 20.0f;

}

void GameOverScene::InitMusic(void)
{

	// ゲームシーンの音楽
	musicGameOverId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_OVER_MUSIC).handleId_;

	ChangeVolumeSoundMem(255 * 80 / 100, musicGameOverId_);

	// ゲームシーンの音楽の再生
	PlaySoundMem(musicGameOverId_, DX_PLAYTYPE_LOOP);

	// 決定音
	musicDecisionId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DECISION_MUSIC).handleId_;

}

void GameOverScene::Init(void)
{

	// 音の初期設定
	InitMusic();

	// アニメーションの初期設定
	InitAnimation();

	// アニメーションの設定
	SetAnimation();

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// タイトルロゴ
	imgGameOver_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMEOVER).handleId_;

	// スペースキーかBボタン
	imgSpaceOrB_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SPACE_OR_B).handleId_;

	// ステージの生成
	stage_ = new Stage();
	stage_->Init();

}

void GameOverScene::Update(void)
{

	// アニメーション処理
	Animation();

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		PlaySoundMem(musicDecisionId_, DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SceneManager::GetInstance().SetIsOperation(true);
	}

	// ゲームパッドの番号を取得
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		PlaySoundMem(musicDecisionId_, DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SceneManager::GetInstance().SetGamePad(true);
		SceneManager::GetInstance().SetIsOperation(true);
	}

	// 点滅カウンタを加算する
	BlinkCnt_++;

	// ステージの更新
	stage_->Update();

	transform_.Update();

}

void GameOverScene::Draw(void)
{

	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(transform_.modelId);

	// ステージの描画
	stage_->Draw();

	// ロゴ描画
	DrawLogo();

}

void GameOverScene::Release(void)
{

	DeleteGraph(imgGameOver_);
	DeleteGraph(imgSpaceOrB_);

	// モデルの開放
	MV1DeleteModel(transform_.modelId);
	MV1DeleteModel(gameOverAnim_);

	DeleteSoundMem(musicGameOverId_);
	DeleteSoundMem(musicDecisionId_);

	// ステージの解放
	stage_->Release();

}

void GameOverScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// ゲームオーバー
	DrawRotaGraph(cx, cy, 1.0, 0.0, imgGameOver_, true);

	// スペースキーかBボタン
	// 点滅させる
	if ((BlinkCnt_ / 30) % 2)
	{
		DrawRotaGraph(cx, cy + 200, 0.5f, 0.0f, imgSpaceOrB_, true);
	}

}
