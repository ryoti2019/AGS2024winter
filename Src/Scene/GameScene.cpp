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

	// �J�������[�h�F�t���[�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// �O���b�h���̐���
	grid_ = new Grid();
	grid_->Init();

	// �X�e�[�W�̐���
	stage_ = new Stage();
	stage_->Init();

	// �v���C���[�̐���
	player_ = new Player();
	player_->SetStageID(stage_->GetModelId());
	player_->Init();

	// �G�̐���
	enemy_ = new Enemy();
	player_->SetFollow(&enemy_->GetTransform());
	enemy_->SetFollow(&player_->GetTransform());
	enemy_->SetStageID(stage_->GetModelId());
	enemy_->Init();

	// ���̐���
	sword_ = new Sword();
	sword_->SetFollow(&player_->GetTransform());
	sword_->Init();

	//// �J�������[�h�F�Ǐ]
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetPlayer(&player_->GetTransform());
	camera->SetEnemy(&enemy_->GetTransform());
	camera->SetStageID(stage_->GetModelId());
	camera->ChangeMode(Camera::MODE::FOLLOW);

	// �G�����񂾎��̃t���O
	enemyDeath_ = false;

	// �v���C���[�̉��p�̍��W
	rollPos_ = Utility::VECTOR_ZERO;

	// HP�o�[�̉摜
	imgPlayerHPBar_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_HP_BAR).handleId_;

	// �L�[�{�[�h�̑������
	imgKeyBoad_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::KEYBOAD).handleId_;

	// �Q�[���p�b�h�̑������
	imgGamePad_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMEPAD).handleId_;

	// �G�t�F�N�g�̏����ݒ�
	InitEffect();

	// ���̏����ݒ�
	InitMusic();

	// ����������X�L�b�v�����Ȃ��悤�ɂ���J�E���^
	operationCnt_ = 0.0f;

}

void GameScene::Update(void)
{

	auto& ins = InputManager::GetInstance();

	// �q�b�g�X�g�b�v�ŏ������f
	if (hitStopCnt_ > 0)
	{
		hitStopCnt_--;
		return;
	}

	// �X���[
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

	// �L�[�{�[�h�ł̑���
	if (!SceneManager::GetInstance().GetGamePad() && ins.IsTrgDown(KEY_INPUT_SPACE)
		&& SceneManager::GetInstance().GetIsOperation())
	{
		PlaySoundMem(musicDecisionId_, DX_PLAYTYPE_BACK);
		sce.SetIsOperation(false);
		DeleteGraph(imgKeyBoad_);
	}

	// �Q�[���p�b�h�ł̑���
	if (SceneManager::GetInstance().GetGamePad() && ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)
		&& SceneManager::GetInstance().GetIsOperation())
	{
		PlaySoundMem(musicDecisionId_, DX_PLAYTYPE_BACK);
		sce.SetIsOperation(false);
		DeleteGraph(imgGamePad_);
	}

	// �^�C�g���V�[���ɖ߂�
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

	// ����������΂����瓮��
	if (SceneManager::GetInstance().GetIsOperation())
	{
		return;
	}

	// �X�e�[�W�̍X�V
	stage_->Update();

	// �v���C���[�̍X�V
	player_->Update();

	// �G�̍X�V
 	enemy_->Update();

	// ���̍X�V
	sword_->Update();

	// �����蔻��
	CollisionEnemyAndPlayer();

	// �v���C���[���U���͈͂Ɉ����񂹂�
	if (enemy_->GetStepAnim() <= 50.0f && enemy_->GetState() == Enemy::STATE::JUMP_ATTACK)
	{

		VECTOR vec = VSub(enemy_->GetAttackPlayerPos(), player_->GetTransform().pos);
		vec = VNorm(vec);
		rollPos_ = VScale(vec, 15.0f);
		player_->SetPos(rollPos_);
		EnemyTornadeSyncEffect();

	}

	// ���p�̍��W��������
	if (enemy_->GetState() != Enemy::STATE::JUMP_ATTACK)
	{
		rollPos_ = Utility::VECTOR_ZERO;
		isEffectTornade_ = true;
	}

	// �W�����v�A�^�b�N�̋z�����ރG�t�F�N�g�̍Đ�
	if (isEffectTornade_ && enemy_->GetState() == Enemy::STATE::JUMP_ATTACK)
	{
		EnemyTornadePlayEffect();
		isEffectTornade_ = false;
	}


	// �q�b�g�X�g�b�v������
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

	// ���񂾂�V�[���ڍs���邽�߂ɃJ�E���^�𑝂₷
	if (enemy_->GetHP() <= 0)
	{
		sceneCnt_ += SceneManager::GetInstance().GetDeltaTime();
	}

	if (player_->GetHP() <= 0)
	{
		sceneCnt_ += SceneManager::GetInstance().GetDeltaTime();
	}

	// �V�[���J��
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

	// �X�e�[�W�̕`��
	stage_->Draw();

	// �v���C���[�̕`��
	player_->Draw();

	// ���̕`��
	sword_->Draw();

	// �G�̕`��
	enemy_->Draw();

	DrawHPBar();

	// �L�[�{�[�h�̑������
	if (SceneManager::GetInstance().GetIsOperation() && !SceneManager::GetInstance().GetGamePad())
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.5f, 0.0f, imgKeyBoad_, true);
	}

	// �Q�[���p�b�h�̑������
	if (SceneManager::GetInstance().GetIsOperation() && SceneManager::GetInstance().GetGamePad())
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.5f, 0.0f, imgGamePad_, true);
	}

}

void GameScene::Release(void)
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

	// �v���C���[�̌��ƓG�̃J�v�Z�����m�̓����蔻��
	// ��U��
	if (HitCheck_Capsule_Capsule(sword_->GetCPosDown(), sword_->GetCPosUP(), sword_->COLLISION_RADIUS,
		enemy_->GetCBodyPosDown(), enemy_->GetCBodyPosUP(), enemy_->COLLISION_BODY_RADIUS)
		&& (player_->GetState() == Player::STATE::ATTACK
			|| player_->GetState() == Player::STATE::ATTACK2
			|| player_->GetState() == Player::STATE::ATTACK3)
		&& enemy_->GetHP() > 0)
	{
		// �v���C���[�̍U�������łɓ������Ă��������Ȃ�
		if (player_->GetAttack())
		{
			enemy_->SetHP(-5);
			player_->SetAttack(false);
			player_->SetHit(true);
			hitStopCnt_ = 5;
			// �v���C���[�̍U���������������̃G�t�F�N�g
			PlayerImpactPlayEffect();

			// �v���C���[�̍U���������������̉�
			ImpactMusic();

			// �G�̃_���[�W�q�b�g��
			EnemyHitMusic();

		}
	}

	// ���ߍU��
	if (HitCheck_Capsule_Capsule(sword_->GetCPosDown(), sword_->GetCPosUP(), sword_->COLLISION_RADIUS,
		enemy_->GetCBodyPosDown(), enemy_->GetCBodyPosUP(), enemy_->COLLISION_BODY_RADIUS)
		&& player_->GetState() == Player::STATE::CHARGE_ATTACK
		&& enemy_->GetHP() > 0)
	{
		// �v���C���[�̍U�������łɓ������Ă��������Ȃ�
		if (player_->GetAttack())
		{
			enemy_->SetHP(-15);
			player_->SetAttack(false);
			player_->SetHit(true);
			hitStopCnt_ = 5;
			// �v���C���[�̍U���������������̃G�t�F�N�g
			PlayerImpactPlayEffect();

			// �v���C���[�̍U���������������̉�
			ImpactMusic();

			// �G�̃_���[�W�q�b�g��
			EnemyHitMusic();

		}
	}

	// �v���C���[�ƓG�̕��퓯�m�̓����蔻��(�ʏ�U���j
	if (HitCheck_Capsule_Capsule(player_->GetCPosDown(), player_->GetCPosUP(), player_->COLLISION_BODY_RADIUS,
		enemy_->GetCWeponPosDown(), enemy_->GetCWeponPosUP(), enemy_->COLLISION_WEPON_RADIUS)
		&& enemy_->GetState() == Enemy::STATE::ATTACK && player_->GetState() != Player::STATE::ROLL &&
		((player_->GetStepAnim() >= 0.0f && player_->GetStepAnim() <= 5.0f) ||
			(player_->GetStepAnim() >= 45.0f && player_->GetStepAnim() <= 71.0f)) && player_->GetHP() > 0)
	{
		// �G�̍U�������łɓ������Ă��������Ȃ�
		if (enemy_->GetAttack())
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-15);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);

			// �G�̍U���������������̃G�t�F�N�g
			EnemyImpactPlayEffect();

			// �_���[�W�q�b�g���̍Đ�
			PlayerHitMusic();
		}
	}

	// �v���C���[�ƓG���m�̓����蔻��
	else if (HitCheck_Capsule_Capsule(player_->GetCPosDown(), player_->GetCPosUP(), player_->COLLISION_BODY_RADIUS,
		enemy_->GetCBodyPosDown(), enemy_->GetCBodyPosUP(), enemy_->COLLISION_BODY_RADIUS)
		&& enemy_->GetState() == Enemy::STATE::TACKLE
		&& player_->GetState() != Player::STATE::ROLL && !player_->GetInvincible()
		&& player_->GetHP() > 0)
	{
		// �G�̍U�������łɓ������Ă��������Ȃ�
		if (enemy_->GetAttack())
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-10);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);

			// �G�̍U���������������̃G�t�F�N�g
			EnemyImpactPlayEffect();

			// �_���[�W�q�b�g���̍Đ�
			PlayerHitMusic();
		}
	}

	// �v���C���[�ƓG�̒e�̓����蔻��
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

			// �G�̍U���������������̃G�t�F�N�g
			EnemyImpactPlayEffect();

			// �_���[�W�q�b�g���̍Đ�
			PlayerHitMusic();
		}
	}

	// �G�l�~�[����v���C���[�܂ł̃x�N�g��
	VECTOR diff = VSub(player_->GetTransform().pos, enemy_->GetTransform().pos);

	// XZ����
	float distance = diff.x * diff.x + diff.z * diff.z;

	// �W�����v�A�^�b�N
	if (distance <= 1000 * 1000 && enemy_->GetState() == Enemy::STATE::JUMP_ATTACK
		&& enemy_->GetStepAnim() >= 45.0f && enemy_->GetStepAnim() <= 60.0f
		&& player_->GetHP() > 0 && player_->GetState() != Player::STATE::ROLL)
	{
		// �G�̍U�������łɓ������Ă��������Ȃ�
		if (enemy_->GetAttack())
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-20);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);

			// �G�̍U���������������̃G�t�F�N�g
			EnemyImpactPlayEffect();

			// �_���[�W�q�b�g���̍Đ�
			PlayerHitMusic();
		}
	}

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

	// �����_
	DrawSphere3D(camera->GetTargetPos(), 20, 1, 0xffffff, 0xffffff, true);

}

void GameScene::InitEffect(void)
{

	// �v���C���[�̍U���������������̃G�t�F�N�g
	effectPlayerImpactResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_EFFECT).handleId_;

	// �G�̍U���������������̃G�t�F�N�g
	effectEnemyImpactResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_IMPACT_EFFECT).handleId_;

	// �W�����v�A�^�b�N�̋z�����ރG�t�F�N�g
	effectEnemyTornadeResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::JUMPATTACK_TORNADE_EFFECT).handleId_;

	// �W�����v�A�^�b�N�̋z�����ރG�t�F�N�g�̃t���O
	isEffectTornade_ = true;

}

void GameScene::PlayerImpactPlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectPlayerImpactPlayId_ = PlayEffekseer3DEffect(effectPlayerImpactResId_);

	float SCALE = 300.0f;
	// �傫��
	SetScalePlayingEffekseer3DEffect(effectPlayerImpactPlayId_, SCALE, SCALE, SCALE);

	// �ʒu
	PlayerImpactSyncEffect();

}

void GameScene::EnemyImpactPlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectEnemyImpactPlayId_ = PlayEffekseer3DEffect(effectEnemyImpactResId_);

	float SCALE = 100.0f;
	// �傫��
	SetScalePlayingEffekseer3DEffect(effectEnemyImpactPlayId_, SCALE, SCALE, SCALE);

	// �ʒu
	EnemyImpactSyncEffect();

}

void GameScene::EnemyTornadePlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectEnemyTornadePlayId_ = PlayEffekseer3DEffect(effectEnemyTornadeResId_);

	float SCALE = 100.0f;
	// �傫��
	SetScalePlayingEffekseer3DEffect(effectEnemyTornadePlayId_, SCALE, SCALE, SCALE);

	// �ʒu
	EnemyTornadeSyncEffect();

}

void GameScene::PlayerImpactSyncEffect(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = enemy_->GetTransform().pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = enemy_->GetTransform().quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis({0.0f,200.0f,0.0f});

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectPlayerImpactPos_ = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectPlayerImpactPlayId_, effectPlayerImpactPos_.x, effectPlayerImpactPos_.y, effectPlayerImpactPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectPlayerImpactPlayId_, rot.x, rot.y, rot.z);

	enemy_->Update();

}

void GameScene::EnemyImpactSyncEffect(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = player_->GetTransform().pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = player_->GetTransform().quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis({0.0f,100.0f,0.0f});

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectEnemyImpactPos_ = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectEnemyImpactPlayId_, effectEnemyImpactPos_.x, effectEnemyImpactPos_.y, effectEnemyImpactPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectEnemyImpactPlayId_, rot.x, rot.y, rot.z);

	player_->Update();

}

void GameScene::EnemyTornadeSyncEffect(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = enemy_->GetAttackPlayerPos();

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = enemy_->GetTransform().quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis({ 0.0f,100.0f,0.0f });

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectEnemyTornadePos_ = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectEnemyTornadePlayId_, effectEnemyTornadePos_.x, effectEnemyTornadePos_.y, effectEnemyTornadePos_.z);
	SetRotationPlayingEffekseer3DEffect(effectEnemyTornadePlayId_, rot.x, rot.y, rot.z);

}

void GameScene::InitMusic(void)
{

	// �v���C���[�̎��S�{�C�X
	musicPlayerDeath_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_DEATH).handleId_;

	ChangeVolumeSoundMem(255 * 100 / 100, musicPlayerDeath_);

	// �G�̎��S�{�C�X
	musicEnemyDeath_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_DEATH).handleId_;

	ChangeVolumeSoundMem(255 * 100 / 100, musicEnemyDeath_);

	// �v���C���[�̍U���������������̉��P
	musicImpactId1_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_MUSIC1).handleId_;

	// �v���C���[�̍U���������������̉��Q
	musicImpactId2_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_MUSIC2).handleId_;

	// �v���C���[�̃_���[�W�q�b�g�{�C�X�P
	musicPlayerHitVoice1_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_HIT_VOICE_MUSIC1).handleId_;

	// �v���C���[�̃_���[�W�q�b�g�{�C�X�Q
	musicPlayerHitVoice2_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_HIT_VOICE_MUSIC2).handleId_;

	// �G�̃_���[�W�q�b�g�{�C�X�P
	musicEnemyHitVoice1_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_HIT_VOICE_MUSIC1).handleId_;

	// �G�̃_���[�W�q�b�g�{�C�X�Q
	musicEnemyHitVoice2_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_HIT_VOICE_MUSIC2).handleId_;

	// �Q�[���V�[���̉��y
	musicGameId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_MUSIC).handleId_;

	ChangeVolumeSoundMem(255 * 80 / 100, musicGameId_);

	// �Q�[���V�[���̉��y�̍Đ�
	PlaySoundMem(musicGameId_, DX_PLAYTYPE_LOOP);

	// ���艹
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

	// HP�o�[
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

	// HP�o�[�̕`��
	DrawExtendGraph(40, 20, 400, 40, imgPlayerHPBar_, true);

	// �X�^�~�i�o�[
	int staminaLength = player_->HP_LENGTH;
	int S;
	int staminaGauge;
	staminaGauge = staminaLength * player_->GetStamina() / player_->STAMINA_MAX;

	if (player_->GetStamina() >= 0)
	{
		DrawBox(40, 50, 40 + staminaGauge, 70, 0xffff00, true);
	}

	// �X�^�~�i�o�[�̕`��
	DrawExtendGraph(40, 50, 400, 70, imgPlayerHPBar_, true);

}
