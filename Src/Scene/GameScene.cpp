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

	//// �J�������[�h�F�t���[�J����
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

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

	// �G�t�F�N�g�̏����ݒ�
	InitEffect();

	// ���̏����ݒ�
	InitMusic();

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

	// �����蔻��
	CollisionEnemyAndPlayer();

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

	// �f�o�b�O�`��
	DrawDebug();

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
			//enemy_->SetState(Enemy::STATE::HIT);
			player_->SetAttack(false);
			player_->SetHit(true);
			hitStopCnt_ = 5;
			// �v���C���[�̍U���������������̃G�t�F�N�g
			ImpactPlayEffect();

			// �v���C���[�̍U���������������̉�
			ImpactMusic();
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
			enemy_->SetHP(-10);
			//enemy_->SetState(Enemy::STATE::HIT);
			player_->SetAttack(false);
			player_->SetHit(true);
			hitStopCnt_ = 5;
			// �v���C���[�̍U���������������̃G�t�F�N�g
			ImpactPlayEffect();

			// �v���C���[�̍U���������������̉�
			ImpactMusic();
		}
	}

	// �v���C���[�ƓG�̕��퓯�m�̓����蔻��
	if (HitCheck_Capsule_Capsule(player_->GetCPosDown(), player_->GetCPosUP(), player_->COLLISION_BODY_RADIUS,
		enemy_->GetCWeponPosDown(), enemy_->GetCWeponPosUP(), enemy_->COLLISION_WEPON_RADIUS)
		&& (enemy_->GetState() == Enemy::STATE::ATTACK
			|| enemy_->GetState() == Enemy::STATE::JUMP_ATTACK
			|| enemy_->GetState() == Enemy::STATE::TACKLE) && player_->GetState() != Player::STATE::ROLL
		&& player_->GetHP() > 0)
	{
		// �G�̍U�������łɓ������Ă��������Ȃ�
		if (enemy_->GetAttack())
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-10);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);
		}
	}
	// �v���C���[�ƓG���m�̓����蔻��
	else if (HitCheck_Capsule_Capsule(player_->GetCPosDown(), player_->GetCPosUP(), player_->COLLISION_BODY_RADIUS,
		enemy_->GetCBodyPosDown(), enemy_->GetCBodyPosUP(), enemy_->COLLISION_BODY_RADIUS)
		&& (enemy_->GetState() == Enemy::STATE::JUMP_ATTACK
			|| enemy_->GetState() == Enemy::STATE::TACKLE) && player_->GetState() != Player::STATE::ROLL
		&& player_->GetHP() > 0)
	{
		// �G�̍U�������łɓ������Ă��������Ȃ�
		if (enemy_->GetAttack())
		{
			player_->SetState(Player::STATE::HIT);
			player_->SetHP(-10);
			enemy_->SetAttack(false);
			enemy_->SetHit(true);
		}
	}

	// �v���C���[�ƓG�̒e�̓����蔻��
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
	effectImpactResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_EFFECT).handleId_;

}

void GameScene::ImpactPlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectImpactPlayId_ = PlayEffekseer3DEffect(effectImpactResId_);

	float SCALE = 300.0f;
	// �傫��
	SetScalePlayingEffekseer3DEffect(effectImpactPlayId_, SCALE, SCALE, SCALE);

	// �ʒu
	ImpactSyncEffect();

}

void GameScene::ImpactSyncEffect(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = enemy_->GetTransform().pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = enemy_->GetTransform().quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectLPos = followRot.PosAxis(LOCAL_CHRAGE_POS);

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectImpactPos_ = VAdd(followPos, effectLPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectImpactPlayId_, effectImpactPos_.x, effectImpactPos_.y, effectImpactPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectImpactPlayId_, rot.x, rot.y, rot.z);

	enemy_->Update();

}

void GameScene::InitMusic(void)
{

	// �v���C���[�̍U���������������̉��P
	musicImpactId1_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_MUSIC1).handleId_;

	// �v���C���[�̍U���������������̉��Q
	musicImpactId2_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMPACT_MUSIC2).handleId_;

	// �v���C���[�̍U���������������̉��R
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
