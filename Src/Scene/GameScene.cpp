#include <EffekseerForDXLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Object/Grid.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/ShotEnemy.h"
#include "../Object/Sword.h"
#include "../Object/UnitBase.h"
#include "../Object/Common/AnimationContorller.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	// カメラモード：フリーカメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

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

	// プレイヤーの回避用の座標
	rollPos_ = Utility::VECTOR_ZERO;

	// HPバーの画像
	imgPlayerHPBar_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_HP_BAR).handleId_;

	// キーボードの操作説明
	imgKeyBoad_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::KEYBOAD).handleId_;

	// ゲームパッドの操作説明
	imgGamePad_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMEPAD).handleId_;

	// エフェクトの初期設定
	InitEffect();

	// 音の初期設定
	InitMusic();

	// 操作説明をスキップさせないようにするカウンタ
	operationCnt_ = 0.0f;

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

	operationCnt_ += SceneManager::GetInstance().GetDeltaTime();

	if (operationCnt_ <= 1.0f)
	{
		return;
	}

	auto& sce = SceneManager::GetInstance();

	// キーボードでの操作
	if (!SceneManager::GetInstance().GetGamePad() && ins.IsTrgDown(KEY_INPUT_SPACE)
		&& SceneManager::GetInstance().GetIsOperation())
	{
		PlaySoundMem(musicDecisionId_, DX_PLAYTYPE_BACK);
		sce.SetIsOperation(false);
		DeleteGraph(imgKeyBoad_);
	}

	// ゲームパッドでの操作
	if (SceneManager::GetInstance().GetGamePad() && ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)
		&& SceneManager::GetInstance().GetIsOperation())
	{
		PlaySoundMem(musicDecisionId_, DX_PLAYTYPE_BACK);
		sce.SetIsOperation(false);
		DeleteGraph(imgGamePad_);
	}

	// タイトルシーンに戻る
	if (!SceneManager::GetInstance().GetGamePad() && ins.IsTrgDown(KEY_INPUT_B) && SceneManager::GetInstance().GetIsOperation())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SceneManager::GetInstance().SetIsOperation(true);
	}

	if (SceneManager::GetInstance().GetGamePad() && ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN)
		&& SceneManager::GetInstance().GetIsOperation())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SceneManager::GetInstance().SetGamePad(true);
		SceneManager::GetInstance().SetIsOperation(true);
	}

	// 操作説明を飛ばしたら動く
	if (SceneManager::GetInstance().GetIsOperation())
	{
		return;
	}

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

	// プレイヤーを攻撃範囲に引き寄せる
	if (enemy_->GetStepAnim() <= 50.0f && enemy_->GetState() == Enemy::STATE::JUMP_ATTACK)
	{

		VECTOR vec = VSub(enemy_->GetAttackPlayerPos(), player_->GetTransform().pos);
		vec = VNorm(vec);
		rollPos_ = VScale(vec, 15.0f);
		player_->SetPos(rollPos_);
		EnemyTornadeSyncEffect();

	}

	// 回避用の座標を初期化
	if (enemy_->GetState() != Enemy::STATE::JUMP_ATTACK)
	{
		rollPos_ = Utility::VECTOR_ZERO;
		isEffectTornade_ = true;
	}

	// ジャンプアタックの吸い込むエフェクトの再生
	if (isEffectTornade_ && enemy_->GetState() == Enemy::STATE::JUMP_ATTACK)
	{
		EnemyTornadePlayEffect();
		isEffectTornade_ = false;
	}


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

	// ステージの描画
	stage_->Draw();

	// プレイヤーの描画
	player_->Draw();

	// 剣の描画
	sword_->Draw();

	// 敵の描画
	enemy_->Draw();

	DrawHPBar();

	// キーボードの操作説明
	if (SceneManager::GetInstance().GetIsOperation() && !SceneManager::GetInstance().GetGamePad())
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.5f, 0.0f, imgKeyBoad_, true);
	}

	// ゲームパッドの操作説明
	if (SceneManager::GetInstance().GetIsOperation() && SceneManager::GetInstance().GetGamePad())
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.5f, 0.0f, imgGamePad_, true);
	}

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

	DeleteGraph(imgPlayerHPBar_);

	DeleteSoundMem(musicGameId_);
	DeleteSoundMem(musicEnemyDeath_);
	DeleteSoundMem(musicEnemyHitVoice1_);
	DeleteSoundMem(musicEnemyHitVoice2_);
	DeleteSoundMem(musicDecisionId_);
	DeleteSoundMem(musicImpactId1_);
	DeleteSoundMem(musicImpactId2_);
	DeleteSoundMem(musicPlayerDeath_);

	DeleteEffekseerEffect(effectEnemyImpactPlayId_);
	DeleteEffekseerEffect(effectEnemyImpactResId_);
	DeleteEffekseerEffect(effectEnemyTornadePlayId_);
	DeleteEffekseerEffect(effectEnemyTornadeResId_);
	DeleteEffekseerEffect(effectPlayerImpactPlayId_);
	DeleteEffekseerEffect(effectPlayerImpactResId_);

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
			player_->SetAttack(false);
			player_->SetHit(true);
			hitStopCnt_ = 5;
			// プレイヤーの攻撃が当たった時のエフェクト
			PlayerImpactPlayEffect();

			// プレイヤーの攻撃が当たった時の音
			ImpactMusic();

			// 敵のダメージヒット音
			EnemyHitMusic();

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
			enemy_->SetHP(-15);
			player_->SetAttack(false);
			player_->SetHit(true);
			hitStopCnt_ = 5;
			// プレイヤーの攻撃が当たった時のエフェクト
			PlayerImpactPlayEffect();

			// プレイヤーの攻撃が当たった時の音
			ImpactMusic();

			// 敵のダメージヒット音
			EnemyHitMusic();

		}
	}

	// プレイヤーと敵の武器同士の当たり判定(通常攻撃）
	if (HitCheck_Capsule_Capsule(player_->GetCPosDown(), player_->GetCPosUP(), player_->COLLISION_BODY_RADIUS,
		enemy_->GetCWeponPosDown(), enemy_->GetCWeponPosUP(), enemy_->COLLISION_WEPON_RADIUS)
		&& enemy_->GetState() == Enemy::STATE::ATTACK && player_->GetState() != Player::STATE::ROLL &&
		((player_->GetStepAnim() >= 0.0f && player_->GetStepAnim() <= 5.0f) ||
			(player_->GetStepAnim() >= 45.0f && player_->GetStepAnim() <= 71.0f)) && player_->GetHP() > 0)
	{
		// 敵の攻撃がすでに当たっていたら入らない
		if (enemy_->GetAttack())
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-15);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);

			// 敵の攻撃が当たった時のエフェクト
			EnemyImpactPlayEffect();

			// ダメージヒット音の再生
			PlayerHitMusic();
		}
	}

	// プレイヤーと敵同士の当たり判定
	else if (HitCheck_Capsule_Capsule(player_->GetCPosDown(), player_->GetCPosUP(), player_->COLLISION_BODY_RADIUS,
		enemy_->GetCBodyPosDown(), enemy_->GetCBodyPosUP(), enemy_->COLLISION_BODY_RADIUS)
		&& enemy_->GetState() == Enemy::STATE::TACKLE
		&& player_->GetState() != Player::STATE::ROLL && !player_->GetInvincible()
		&& player_->GetHP() > 0)
	{
		// 敵の攻撃がすでに当たっていたら入らない
		if (enemy_->GetAttack())
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-10);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);

			// 敵の攻撃が当たった時のエフェクト
			EnemyImpactPlayEffect();

			// ダメージヒット音の再生
			PlayerHitMusic();
		}
	}

	// プレイヤーと敵の弾の当たり判定
	for (auto s : enemy_->GetShots())
	{
		if (Utility::IsHitSphereCapsule(s->GetPos(), s->GetCollisionRadius(),
			player_->GetCPosDown(), player_->GetCPosUP(), player_->COLLISION_BODY_RADIUS)
			&& (s->GetState() == ShotEnemy::STATE::SHOT) && player_->GetState() != Player::STATE::ROLL
			&& player_->GetHP() > 0)
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-3);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);

			// 敵の攻撃が当たった時のエフェクト
			EnemyImpactPlayEffect();

			// ダメージヒット音の再生
			PlayerHitMusic();
		}
	}

	// エネミーからプレイヤーまでのベクトル
	VECTOR diff = VSub(player_->GetTransform().pos, enemy_->GetTransform().pos);

	// XZ距離
	float distance = diff.x * diff.x + diff.z * diff.z;

	// ジャンプアタック
	if (distance <= 1000 * 1000 && enemy_->GetState() == Enemy::STATE::JUMP_ATTACK
		&& enemy_->GetStepAnim() >= 45.0f && enemy_->GetStepAnim() <= 60.0f
		&& player_->GetHP() > 0 && player_->GetState() != Player::STATE::ROLL)
	{
		// 敵の攻撃がすでに当たっていたら入らない
		if (enemy_->GetAttack())
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-20);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);

			// 敵の攻撃が当たった時のエフェクト
			EnemyImpactPlayEffect();

			// ダメージヒット音の再生
			PlayerHitMusic();
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
	effectPlayerImpactResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_EFFECT).handleId_;

	// 敵の攻撃が当たった時のエフェクト
	effectEnemyImpactResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_IMPACT_EFFECT).handleId_;

	// ジャンプアタックの吸い込むエフェクト
	effectEnemyTornadeResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::JUMPATTACK_TORNADE_EFFECT).handleId_;

	// ジャンプアタックの吸い込むエフェクトのフラグ
	isEffectTornade_ = true;

}

void GameScene::PlayerImpactPlayEffect(void)
{

	// エフェクト再生
	effectPlayerImpactPlayId_ = PlayEffekseer3DEffect(effectPlayerImpactResId_);

	float SCALE = 300.0f;
	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectPlayerImpactPlayId_, SCALE, SCALE, SCALE);

	// 位置
	PlayerImpactSyncEffect();

}

void GameScene::EnemyImpactPlayEffect(void)
{

	// エフェクト再生
	effectEnemyImpactPlayId_ = PlayEffekseer3DEffect(effectEnemyImpactResId_);

	float SCALE = 100.0f;
	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectEnemyImpactPlayId_, SCALE, SCALE, SCALE);

	// 位置
	EnemyImpactSyncEffect();

}

void GameScene::EnemyTornadePlayEffect(void)
{

	// エフェクト再生
	effectEnemyTornadePlayId_ = PlayEffekseer3DEffect(effectEnemyTornadeResId_);

	float SCALE = 100.0f;
	// 大きさ
	SetScalePlayingEffekseer3DEffect(effectEnemyTornadePlayId_, SCALE, SCALE, SCALE);

	// 位置
	EnemyTornadeSyncEffect();

}

void GameScene::PlayerImpactSyncEffect(void)
{

	// 追従対象の位置
	VECTOR followPos = enemy_->GetTransform().pos;

	// 追従対象の向き
	Quaternion followRot = enemy_->GetTransform().quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectPos = followRot.PosAxis({0.0f,200.0f,0.0f});

	// エフェクトの位置の更新
	effectPlayerImpactPos_ = VAdd(followPos, effectPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectPlayerImpactPlayId_, effectPlayerImpactPos_.x, effectPlayerImpactPos_.y, effectPlayerImpactPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectPlayerImpactPlayId_, rot.x, rot.y, rot.z);

	enemy_->Update();

}

void GameScene::EnemyImpactSyncEffect(void)
{

	// 追従対象の位置
	VECTOR followPos = player_->GetTransform().pos;

	// 追従対象の向き
	Quaternion followRot = player_->GetTransform().quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectPos = followRot.PosAxis({0.0f,100.0f,0.0f});

	// エフェクトの位置の更新
	effectEnemyImpactPos_ = VAdd(followPos, effectPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectEnemyImpactPlayId_, effectEnemyImpactPos_.x, effectEnemyImpactPos_.y, effectEnemyImpactPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectEnemyImpactPlayId_, rot.x, rot.y, rot.z);

	player_->Update();

}

void GameScene::EnemyTornadeSyncEffect(void)
{

	// 追従対象の位置
	VECTOR followPos = enemy_->GetAttackPlayerPos();

	// 追従対象の向き
	Quaternion followRot = enemy_->GetTransform().quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectPos = followRot.PosAxis({ 0.0f,100.0f,0.0f });

	// エフェクトの位置の更新
	effectEnemyTornadePos_ = VAdd(followPos, effectPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectEnemyTornadePlayId_, effectEnemyTornadePos_.x, effectEnemyTornadePos_.y, effectEnemyTornadePos_.z);
	SetRotationPlayingEffekseer3DEffect(effectEnemyTornadePlayId_, rot.x, rot.y, rot.z);

}

void GameScene::InitMusic(void)
{

	// プレイヤーの死亡ボイス
	musicPlayerDeath_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_DEATH).handleId_;

	ChangeVolumeSoundMem(255 * 100 / 100, musicPlayerDeath_);

	// 敵の死亡ボイス
	musicEnemyDeath_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_DEATH).handleId_;

	ChangeVolumeSoundMem(255 * 100 / 100, musicEnemyDeath_);

	// プレイヤーの攻撃が当たった時の音１
	musicImpactId1_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_MUSIC1).handleId_;

	// プレイヤーの攻撃が当たった時の音２
	musicImpactId2_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_MUSIC2).handleId_;

	// プレイヤーのダメージヒットボイス１
	musicPlayerHitVoice1_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_HIT_VOICE_MUSIC1).handleId_;

	// プレイヤーのダメージヒットボイス２
	musicPlayerHitVoice2_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_HIT_VOICE_MUSIC2).handleId_;

	// 敵のダメージヒットボイス１
	musicEnemyHitVoice1_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_HIT_VOICE_MUSIC1).handleId_;

	// 敵のダメージヒットボイス２
	musicEnemyHitVoice2_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_HIT_VOICE_MUSIC2).handleId_;

	// ゲームシーンの音楽
	musicGameId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_MUSIC).handleId_;

	ChangeVolumeSoundMem(255 * 80 / 100, musicGameId_);

	// ゲームシーンの音楽の再生
	PlaySoundMem(musicGameId_, DX_PLAYTYPE_LOOP);

	// 決定音
	musicDecisionId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DECISION_MUSIC).handleId_;

}

void GameScene::ImpactMusic(void)
{

	int number = GetRand(1);
	if (number == 0)
	{
		PlaySoundMem(musicImpactId1_, DX_PLAYTYPE_BACK);
	}
	else if (number == 1)
	{
		PlaySoundMem(musicImpactId2_, DX_PLAYTYPE_BACK);
	}

}

void GameScene::PlayerHitMusic(void)
{

	int number = GetRand(1);
	if (player_->GetState() == Player::STATE::HIT)
	{
		if (number == 0)
		{
			PlaySoundMem(musicPlayerHitVoice1_, DX_PLAYTYPE_BACK);
		}
		else if (number == 1)
		{
			PlaySoundMem(musicPlayerHitVoice2_, DX_PLAYTYPE_BACK);
		}
	}

}

void GameScene::EnemyHitMusic(void)
{

	int number = GetRand(1);
	if (number == 0)
	{
		PlaySoundMem(musicEnemyHitVoice1_, DX_PLAYTYPE_BACK);
	}
	else if (number == 1)
	{
		PlaySoundMem(musicEnemyHitVoice2_, DX_PLAYTYPE_BACK);
	}

}

void GameScene::DrawHPBar(void)
{

	// HPバー
	int hpLength = player_->HP_LENGTH;
	int H;
	int hpGauge;
	H = player_->GetHP() * (512.0f / player_->HP_MAX) - 100;
	int R = min(max((384 - H), 0), 0xff);
	int G = min(max((H + 64), 0), 0xff);
	int B = max((H - 384), 0);
	hpGauge = hpLength * player_->GetHP() / player_->HP_MAX;

	if (player_->GetHP() >= 0)
	{
		DrawBox(40, 20, 40 + hpGauge, 40, GetColor(R, G, B), true);
	}

	// HPバーの描画
	DrawExtendGraph(40, 20, 400, 40, imgPlayerHPBar_, true);

	// スタミナバー
	int staminaLength = player_->HP_LENGTH;
	int S;
	int staminaGauge;
	staminaGauge = staminaLength * player_->GetStamina() / player_->STAMINA_MAX;

	if (player_->GetStamina() >= 0)
	{
		DrawBox(40, 50, 40 + staminaGauge, 70, 0xffff00, true);
	}

	// スタミナバーの描画
	DrawExtendGraph(40, 50, 400, 70, imgPlayerHPBar_, true);

}
