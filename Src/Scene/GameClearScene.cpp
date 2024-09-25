#include <string>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Sword.h"
#include "../Object/Stage.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene(void)
{
	imgGameClear_ = -1;
}

GameClearScene::~GameClearScene(void)
{
}

void GameClearScene::InitAnimation(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_MODEL));

	// 待機アニメーション
	gameClearAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_GAMECLEAR);

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

void GameClearScene::InitEffect(void)
{

	// 花火のエフェクト
	effectFireWorksResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::FIREWORKS_EFFECT).handleId_;

}

void GameClearScene::InitMusic(void)
{

	// ゲームシーンの音楽
	musicGameClearId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_CLEAR_MUSIC).handleId_;

	ChangeVolumeSoundMem(255 * 80 / 100, musicGameClearId_);

	// ゲームシーンの音楽の再生
	PlaySoundMem(musicGameClearId_, DX_PLAYTYPE_LOOP);

	// 決定音
	musicDecisionId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DECISION_MUSIC).handleId_;

}

void GameClearScene::Init(void)
{

	// アニメーションの初期設定
	InitAnimation();

	// エフェクトの初期設定
	InitEffect();

	// 音の初期設定
	InitMusic();

	// アニメーションの設定
	SetAnimation();

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// ゲームクリア
	imgGameClear_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMECLEAR).handleId_;

	// スペースキーかBボタン
	imgSpaceOrB_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SPACE_OR_B).handleId_;

	// エフェクト再生
	PlayEffect();

	// 剣の生成
	sword_ = new Sword();
	sword_->SetFollow(&transform_);
	sword_->Init();

	// ステージの生成
	stage_ = new Stage();
	stage_->Init();

}

void GameClearScene::Update(void)
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

	//// エフェクト再生
	//PlayEffect();
	
	// 剣の更新
	sword_->Update();

	// ステージの更新
	stage_->Update();

	transform_.Update();

}

void GameClearScene::Draw(void)
{

	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(transform_.modelId);

	// 剣の描画
	sword_->Draw();

	// ステージの描画
	stage_->Draw();

	// ロゴ描画
	DrawLogo();

}

void GameClearScene::Release(void)
{

	DeleteGraph(imgGameClear_);
	DeleteGraph(imgSpaceOrB_);

	// モデルの開放
	MV1DeleteModel(transform_.modelId);
	MV1DeleteModel(gameClearAnim_);

	StopEffekseer3DEffect(effectFireWorksPlayId_);
	DeleteEffekseerEffect(effectFireWorksResId_);
	DeleteEffekseerEffect(effectFireWorksPlayId_);

	DeleteSoundMem(musicGameClearId_);
	DeleteSoundMem(musicDecisionId_);

	// 剣の解放
	sword_->Release();

	// ステージの解放
	stage_->Release();

}

void GameClearScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// タイトルロゴ
	DrawRotaGraph(cx, cy, 1.0, 0.0, imgGameClear_, true);

	// スペースキーかBボタン
	// 点滅させる
	if ((BlinkCnt_ / 30) % 2)
	{
		DrawRotaGraph(cx, cy + 200, 0.5f, 0.0f, imgSpaceOrB_, true);
	}
}

void GameClearScene::Animation(void)
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

void GameClearScene::SetAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, gameClearAnim_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// アニメーション速度
	speedAnim_ = 30.0f;

}

void GameClearScene::PlayEffect(void)
{

	// エフェクト再生
	effectFireWorksPlayId_ = PlayEffekseer3DEffect(effectFireWorksResId_);

	float SCALE = 20.0f;
	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectFireWorksPlayId_, SCALE, SCALE, SCALE);

	// 位置
	SyncEffect();

}

void GameClearScene::SyncEffect(void)
{

	// 位置
	effectPos_ = {0.0f,-400.0f,0.0f};

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectFireWorksPlayId_, effectPos_.x, effectPos_.y, effectPos_.z);

}