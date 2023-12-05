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

	// グリッド線の生成
	grid_ = new Grid();
	grid_->Init();

	// ステージの生成
	stage_ = new Stage();
	stage_->Init();

	// プレイヤーの生成
	player_ = new Player();
	player_->Init();

	// 敵の生成
	enemy_ = new Enemy();
	enemy_->SetFollow(&player_->GetTransform());
	enemy_->Init();

	// 剣の生成
	sword_ = new Sword();
	sword_->SetFollow(&player_->GetTransform());
	sword_->Init();

	//// カメラモード：追従
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetPlayer(&player_->GetTransform());
	camera->SetEnemy(&enemy_->GetTransform());
	camera->ChangeMode(Camera::MODE::FREE);

}

void SpecialMoveScene::Update(void)
{

	// グリッド線の更新
	grid_->Update();

	// ステージの更新
	stage_->Update();

	// プレイヤーの更新
	player_->Update();

	// 敵の更新
	enemy_->Update();

	// 剣の更新
	sword_->Update();

}

void SpecialMoveScene::Draw(void)
{
	// グリッド線の描画
	grid_->Draw();

	// ステージの描画
	stage_->Draw();

	// プレイヤーの描画
	player_->Draw();

	// 敵の描画
	enemy_->Draw();

	// 剣の描画
	sword_->Draw();

}

void SpecialMoveScene::Release(void)
{

	// グリッド線の解放
	grid_->Release();
	delete grid_;

	// ステージの開放
	stage_->Release();
	delete stage_;

	// プレイヤーの解放
	player_->Release();
	delete player_;

	// 敵の開放
	enemy_->Release();
	delete enemy_;

	// 剣の開放
	sword_->Release();
	delete sword_;

}
