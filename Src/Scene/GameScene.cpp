#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
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

	grid_ = new Grid;
	grid_->Init();

	stage_ = new Stage;
	stage_->Init();

	player_ = new Player;
	player_->Init();

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

}

void GameScene::Draw(void)
{
	
	grid_->Draw();

	stage_->Draw();

	player_->Draw();

	Camera* camera = SceneManager::GetInstance().GetCamera();

	DrawSphere3D(camera->GetTargetPos(), 20, 1, 0xffffff, 0xffffff, true);

}

void GameScene::Release(void)
{

	grid_->Release();

	stage_->Release();

	player_->Release();

}
