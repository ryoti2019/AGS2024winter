#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Sword.h"
#include "../Object/UnitBase.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	//// カメラモード：フリーカメラ
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	grid_ = new Grid();
	grid_->Init();

	stage_ = new Stage();
	stage_->Init();

	player_ = new Player();
	player_->Init();

	enemy_ = new Enemy();
	enemy_->Init();

	//sword_ = new Sword(player_->GetTransform());
	sword_ = new Sword();
	sword_->SetFollow(&player_->GetTransform());
	sword_->Init();

	
	//// カメラモード：追従
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(&player_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW);

}

void GameScene::Update(void)
{
	
	grid_->Update();

	stage_->Update();

	player_->Update();

	enemy_->Update();

	sword_->Update();
}

void GameScene::Draw(void)
{
	
	grid_->Draw();

	stage_->Draw();

	player_->Draw();

	enemy_->Draw();

	sword_->Draw();

	// デバッグ描画
	DrawDebug();

}

void GameScene::Release(void)
{

	grid_->Release();
	delete grid_;

	stage_->Release();
	delete stage_;

	player_->Release();
	delete player_;

	enemy_->Release();
	delete enemy_;

	sword_->Release();
	delete sword_;

}

void GameScene::DrawDebug(void)
{
	// カメラ情報を取得
	Camera* camera = SceneManager::GetInstance().GetCamera();
	VECTOR pos = camera->GetPos();
	VECTOR angles = camera->GetAngles();

	DrawFormatString(0, 10, 0xffffff, "カメラ座標 : (%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 30, 0xffffff, "カメラ角度deg : (%.1f, %.1f, %.1f)", 180 / DX_PI * angles.x, 180 / DX_PI * angles.y, 180 / DX_PI * angles.z);
	DrawFormatString(0, 50, 0xffffff, "カメラ角度rad : (%.5f, %.5f, %.5f)", angles.x, angles.y, angles.z);

	// 注視点
	DrawSphere3D(camera->GetTargetPos(), 20, 1, 0xffffff, 0xffffff, true);

}
