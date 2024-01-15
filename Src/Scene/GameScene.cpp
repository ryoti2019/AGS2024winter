#include <EffekseerForDXLib.h>
#include "../Manager/ResourceManager.h"
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

	// 敵が死んだ時のフラグ
	enemyDeath_ = false;

	// エフェクトの初期設定
	InitEffect();

	// 音の初期設定
	InitMusic();

}

void GameScene::Update(void)
{

	auto& ins = InputManager::GetInstance();

	// ヒットストップで処理中断
	if (hitStopCnt_ > 0)
	{
		hitStopCnt_--;
		return;
	}

	// スロー
	if (slowCnt_ > 0)
	{
		slowCnt_--;
		if (slowCnt_ % 5 != 0)
		{
			return;
		}
	}

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

	// 当たり判定
	CollisionEnemyAndPlayer();

	// ヒットストップを入れる
	if (enemy_->GetHP() <= 0 && !enemyDeath_)
	{
		slowCnt_ = 60;
		enemyDeath_ = true;
		enemy_->SetState(Enemy::STATE::DEATH);
	}
	
	if (player_->GetHP() <= 0 && !playerDeath_)
	{
		slowCnt_ = 60;
		playerDeath_ = true;
		player_->SetState(Player::STATE::DEATH);
	}

	// 死んだらシーン移行するためにカウンタを増やす
	if (enemy_->GetHP() <= 0)
	{
		sceneCnt_ += SceneManager::GetInstance().GetDeltaTime();
	}

	if (player_->GetHP() <= 0)
	{
		sceneCnt_ += SceneManager::GetInstance().GetDeltaTime();
	}

	// シーン遷移
	if (enemy_->GetHP() <= 0 && sceneCnt_ >= 5.0f)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
	}

	if (player_->GetHP() <= 0 && sceneCnt_ >= 5.0f)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

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
	// 弱攻撃
	if (HitCheck_Capsule_Capsule(sword_->GetCPosDown(), sword_->GetCPosUP(), sword_->COLLISION_RADIUS,
		enemy_->GetCBodyPosDown(), enemy_->GetCBodyPosUP(), enemy_->COLLISION_BODY_RADIUS)
		&& (player_->GetState() == Player::STATE::ATTACK
			|| player_->GetState() == Player::STATE::ATTACK2
			|| player_->GetState() == Player::STATE::ATTACK3)
		&& enemy_->GetHP() > 0)
	{
		// プレイヤーの攻撃がすでに当たっていたら入らない
		if (player_->GetAttack())
		{
			enemy_->SetHP(-5);
			//enemy_->SetState(Enemy::STATE::HIT);
			player_->SetAttack(false);
			player_->SetHit(true);
			hitStopCnt_ = 5;
			// プレイヤーの攻撃が当たった時のエフェクト
			ImpactPlayEffect();

			// プレイヤーの攻撃が当たった時の音
			ImpactMusic();
		}
	}

	// 溜め攻撃
	if (HitCheck_Capsule_Capsule(sword_->GetCPosDown(), sword_->GetCPosUP(), sword_->COLLISION_RADIUS,
		enemy_->GetCBodyPosDown(), enemy_->GetCBodyPosUP(), enemy_->COLLISION_BODY_RADIUS)
		&& player_->GetState() == Player::STATE::CHARGE_ATTACK
		&& enemy_->GetHP() > 0)
	{
		// プレイヤーの攻撃がすでに当たっていたら入らない
		if (player_->GetAttack())
		{
			enemy_->SetHP(-10);
			//enemy_->SetState(Enemy::STATE::HIT);
			player_->SetAttack(false);
			player_->SetHit(true);
			hitStopCnt_ = 5;
			// プレイヤーの攻撃が当たった時のエフェクト
			ImpactPlayEffect();

			// プレイヤーの攻撃が当たった時の音
			ImpactMusic();
		}
	}

	// プレイヤーと敵の武器同士の当たり判定
	if (HitCheck_Capsule_Capsule(player_->GetCPosDown(), player_->GetCPosUP(), player_->COLLISION_BODY_RADIUS,
		enemy_->GetCWeponPosDown(), enemy_->GetCWeponPosUP(), enemy_->COLLISION_WEPON_RADIUS)
		&& (enemy_->GetState() == Enemy::STATE::ATTACK
			|| enemy_->GetState() == Enemy::STATE::JUMP_ATTACK
			|| enemy_->GetState() == Enemy::STATE::TACKLE) && player_->GetState() != Player::STATE::ROLL
		&& player_->GetHP() > 0)
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
			|| enemy_->GetState() == Enemy::STATE::TACKLE) && player_->GetState() != Player::STATE::ROLL
		&& player_->GetHP() > 0)
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
			&& (s->GetState() == ShotEnemy::STATE::SHOT) && player_->GetState() != Player::STATE::ROLL
			&& player_->GetHP() > 0)
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

void GameScene::InitEffect(void)
{

	// プレイヤーの攻撃が当たった時のエフェクト
	effectImpactResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_EFFECT).handleId_;

}

void GameScene::ImpactPlayEffect(void)
{

	// エフェクト再生
	effectImpactPlayId_ = PlayEffekseer3DEffect(effectImpactResId_);

	float SCALE = 300.0f;
	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectImpactPlayId_, SCALE, SCALE, SCALE);

	// 位置
	ImpactSyncEffect();

}

void GameScene::ImpactSyncEffect(void)
{

	// 追従対象の位置
	VECTOR followPos = enemy_->GetTransform().pos;

	// 追従対象の向き
	Quaternion followRot = enemy_->GetTransform().quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectLPos = followRot.PosAxis(LOCAL_CHRAGE_POS);

	// エフェクトの位置の更新
	effectImpactPos_ = VAdd(followPos, effectLPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectImpactPlayId_, effectImpactPos_.x, effectImpactPos_.y, effectImpactPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectImpactPlayId_, rot.x, rot.y, rot.z);

	enemy_->Update();

}

void GameScene::InitMusic(void)
{

	// プレイヤーの攻撃が当たった時の音１
	musicImpactId1_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_MUSIC1).handleId_;

	// プレイヤーの攻撃が当たった時の音２
	musicImpactId2_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_MUSIC2).handleId_;

	// プレイヤーの攻撃が当たった時の音３
	musicImpactId3_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_MUSIC3).handleId_;

}

void GameScene::ImpactMusic(void)
{

	int number = GetRand(2);
	if (number == 0)
	{
		PlaySoundMem(musicImpactId1_, DX_PLAYTYPE_BACK);
	}
	else if (number == 1)
	{
		PlaySoundMem(musicImpactId2_, DX_PLAYTYPE_BACK);
	}
	else if (number == 2)
	{
		PlaySoundMem(musicImpactId3_, DX_PLAYTYPE_BACK);
	}

}
