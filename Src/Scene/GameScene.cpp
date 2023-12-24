#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Grid.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/ShotEnemy.h"
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

	// グリッド線の生成
	grid_ = new Grid();
	grid_->Init();

	// ステージの生成
	stage_ = new Stage();
	stage_->Init();

	// プレイヤーの生成
	player_ = new Player();
	player_->SetStageID(stage_->GetModelId());
	player_->Init();


	// 敵の生成
	enemy_ = new Enemy();
	player_->SetFollow(&enemy_->GetTransform());
	enemy_->SetFollow(&player_->GetTransform());
	enemy_->SetStageID(stage_->GetModelId());
	enemy_->Init();

	// 剣の生成
	sword_ = new Sword();
	sword_->SetFollow(&player_->GetTransform());
	sword_->Init();

	//// カメラモード：追従
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetPlayer(&player_->GetTransform());
	camera->SetEnemy(&enemy_->GetTransform());
	camera->SetStageID(stage_->GetModelId());
	camera->ChangeMode(Camera::MODE::FOLLOW);

}

void GameScene::Update(void)
{

	auto& ins = InputManager::GetInstance();

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

	CollisionEnemyAndPlayer();



}

void GameScene::Draw(void)
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

	// デバッグ描画
	DrawDebug();

}

void GameScene::Release(void)
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

void GameScene::CollisionEnemyAndPlayer()
{

	// プレイヤーの剣と敵のカプセル同士の当たり判定
	if (HitCheck_Capsule_Capsule(sword_->GetCPosDown(), sword_->GetCPosUP(), sword_->COLLISION_RADIUS,
		enemy_->GetCBodyPosDown(), enemy_->GetCBodyPosUP(), enemy_->COLLISION_BODY_RADIUS)
		&& (player_->GetState() == Player::STATE::ATTACK
			|| player_->GetState() == Player::STATE::ATTACK2
			|| player_->GetState() == Player::STATE::ATTACK3
			|| player_->GetState() == Player::STATE::CHARGE_ATTACK))
	{
		// プレイヤーの攻撃がすでに当たっていたら入らない
		if (player_->GetAttack())
		{
			enemy_->SetHP(-10);
			//enemy_->SetState(Enemy::STATE::HIT);
			player_->SetAttack(false);
			player_->SetHit(true);
		}
	}

	// プレイヤーと敵の武器同士の当たり判定
	if (HitCheck_Capsule_Capsule(player_->GetCPosDown(), player_->GetCPosUP(), player_->COLLISION_BODY_RADIUS,
		enemy_->GetCWeponPosDown(), enemy_->GetCWeponPosUP(), enemy_->COLLISION_WEPON_RADIUS)
		&& (enemy_->GetState() == Enemy::STATE::ATTACK
			|| enemy_->GetState() == Enemy::STATE::JUMP_ATTACK
			|| enemy_->GetState() == Enemy::STATE::TACKLE) && player_->GetState() != Player::STATE::ROLL)
	{
		// 敵の攻撃がすでに当たっていたら入らない
		if (enemy_->GetAttack())
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-10);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);
		}
	}
	// プレイヤーと敵同士の当たり判定
	else if (HitCheck_Capsule_Capsule(player_->GetCPosDown(), player_->GetCPosUP(), player_->COLLISION_BODY_RADIUS,
		enemy_->GetCBodyPosDown(), enemy_->GetCBodyPosUP(), enemy_->COLLISION_BODY_RADIUS)
		&& (enemy_->GetState() == Enemy::STATE::JUMP_ATTACK
			|| enemy_->GetState() == Enemy::STATE::TACKLE) && player_->GetState() != Player::STATE::ROLL)
	{
		// 敵の攻撃がすでに当たっていたら入らない
		if (enemy_->GetAttack())
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-10);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);
		}
	}

	// プレイヤーと敵の弾の当たり判定
	for (auto s : enemy_->GetShots())
	{
		if (AsoUtility::IsHitSphereCapsule(s->GetPos(), s->GetCollisionRadius(),
			player_->GetCPosDown(), player_->GetCPosUP(), player_->COLLISION_BODY_RADIUS)
			&& (s->GetState() == ShotEnemy::STATE::SHOT) && player_->GetState() != Player::STATE::ROLL)
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-10);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);
		}
	}

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