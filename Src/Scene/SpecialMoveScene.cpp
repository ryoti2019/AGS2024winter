#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Grid.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Sword.h"
#include "../Object/UnitBase.h"
#include "SpecialMoveScene.h"

SpecialMoveScene::SpecialMoveScene(void)
{
}

SpecialMoveScene::~SpecialMoveScene(void)
{
}

void SpecialMoveScene::Init(void)
{

	// �O���b�h���̐���
	grid_ = new Grid();
	grid_->Init();

	// �X�e�[�W�̐���
	stage_ = new Stage();
	stage_->Init();

	// �v���C���[�̐���
	player_ = new Player();
	player_->Init();

	// �G�̐���
	enemy_ = new Enemy();
	enemy_->SetFollow(&player_->GetTransform());
	enemy_->Init();

	// ���̐���
	sword_ = new Sword();
	sword_->SetFollow(&player_->GetTransform());
	sword_->Init();

	//// �J�������[�h�F�Ǐ]
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetPlayer(&player_->GetTransform());
	camera->SetEnemy(&enemy_->GetTransform());
	camera->ChangeMode(Camera::MODE::FREE);

}

void SpecialMoveScene::Update(void)
{

	// �O���b�h���̍X�V
	grid_->Update();

	// �X�e�[�W�̍X�V
	stage_->Update();

	// �v���C���[�̍X�V
	player_->Update();

	// �G�̍X�V
	enemy_->Update();

	// ���̍X�V
	sword_->Update();

}

void SpecialMoveScene::Draw(void)
{
	// �O���b�h���̕`��
	grid_->Draw();

	// �X�e�[�W�̕`��
	stage_->Draw();

	// �v���C���[�̕`��
	player_->Draw();

	// �G�̕`��
	enemy_->Draw();

	// ���̕`��
	sword_->Draw();

}

void SpecialMoveScene::Release(void)
{

	// �O���b�h���̉��
	grid_->Release();
	delete grid_;

	// �X�e�[�W�̊J��
	stage_->Release();
	delete stage_;

	// �v���C���[�̉��
	player_->Release();
	delete player_;

	// �G�̊J��
	enemy_->Release();
	delete enemy_;

	// ���̊J��
	sword_->Release();
	delete sword_;

}
