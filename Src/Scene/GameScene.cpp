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

	//// �J�������[�h�F�t���[�J����
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	grid_ = new Grid;
	grid_->Init();

	stage_ = new Stage;
	stage_->Init();

	player_ = new Player;
	player_->Init();

	//// �J�������[�h�F�Ǐ]
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

	// �f�o�b�O�`��
	DrawDebug();

}

void GameScene::Release(void)
{

	grid_->Release();

	stage_->Release();

	player_->Release();

}

void GameScene::DrawDebug(void)
{
	// �J���������擾
	Camera* camera = SceneManager::GetInstance().GetCamera();
	VECTOR pos = camera->GetPos();
	VECTOR angles = camera->GetAngles();

	DrawFormatString(0, 10, 0xffffff, "�J�������W : (%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 30, 0xffffff, "�J�����p�xdeg : (%.1f, %.1f, %.1f)", 180 / DX_PI * angles.x, 180 / DX_PI * angles.y, 180 / DX_PI * angles.z);
	DrawFormatString(0, 50, 0xffffff, "�J�����p�xrad : (%.5f, %.5f, %.5f)", angles.x, angles.y, angles.z);

	DrawSphere3D(camera->GetTargetPos(), 20, 1, 0xffffff, 0xffffff, true);

}
