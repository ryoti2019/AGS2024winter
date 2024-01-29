#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	Resource res;

	// �^�C�g�����S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Scene/Vanquish A Monster.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, res);

	// ��������L�[�{�[�h
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/keyBoad.png");
	resourcesMap_.emplace(SRC::KEYBOAD, res);

	// ��������Q�[���p�b�h
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/gamePad.png");
	resourcesMap_.emplace(SRC::GAMEPAD, res);

	// �X�y�[�X�L�[���a�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Space or B.png");
	resourcesMap_.emplace(SRC::SPACE_OR_B, res);

	// �Q�[���N���A
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Scene/GameClearImage.png");
	resourcesMap_.emplace(SRC::GAMECLEAR, res);

	// �Q�[���I�[�o�[
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Scene/GameOverImage.png");
	resourcesMap_.emplace(SRC::GAMEOVER, res);

	// �v���C���[��HP�o�[�̃C���[�W
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/playerHPBar.png");
	resourcesMap_.emplace(SRC::PLAYER_HP_BAR, res);

	// �G��HP�o�[�̃C���[�W
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/enemyHPBar.png");
	resourcesMap_.emplace(SRC::ENEMY_HP_BAR, res);

	// �e�X�g�p�̃X�e�[�W
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/TestStage.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE, res);

	// �e�X�g1�p�̃X�e�[�W
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/TestStage1.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE1, res);

	// �e�X�g2�p�̃X�e�[�W
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/GameObject.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE2, res);

	// �e�X�g3�p�̃X�e�[�W
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Map.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE3, res);

	// �e�X�g4�p�̃X�e�[�W
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Gladiator Low Poly Arena.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE4, res);

	// �v���C���[�̃��f��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Sword And Shield.mv1");
	resourcesMap_.emplace(SRC::PLAYER_MODEL, res);

	// �v���C���[�̑ҋ@�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER_IDLE, res);

	// �v���C���[�̍U���A�j���[�V����4
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Sword And Shield Slash.mv1");
	resourcesMap_.emplace(SRC::PLAYER_ATTACK, res);

	// �v���C���[�̗��ߍU���̃A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Charge Slash.mv1");
	resourcesMap_.emplace(SRC::PLAYER_CHARGEATTACK, res);

	// �v���C���[�̕����A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Running.mv1");
	resourcesMap_.emplace(SRC::PLAYER_WALK, res);

	// �v���C���[�̕����A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Charge Walk.mv1");
	resourcesMap_.emplace(SRC::PLAYER_CHARGE_WALK, res);

	// �v���C���[�̑���A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Standard Run.mv1");
	resourcesMap_.emplace(SRC::PLAYER_RUN, res);

	// �v���C���[�̃_���[�W�q�b�g�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Sword And Shield Impact.mv1");
	resourcesMap_.emplace(SRC::PLAYER_HIT, res);

	// �v���C���[�̉���A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Stand To Roll.mv1");
	resourcesMap_.emplace(SRC::PLAYER_ROLL, res);

	// �v���C���[�̔�ꂽ�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/tired.mv1");
	resourcesMap_.emplace(SRC::PLAYER_TIRED, res);

	// �v���C���[�̎��S�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Sword And Shield Death.mv1");
	resourcesMap_.emplace(SRC::PLAYER_DEATH, res);

	// �v���C���[�̃Q�[���N���A�V�[���̃A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/win.mv1");
	resourcesMap_.emplace(SRC::PLAYER_GAMECLEAR, res);

	// �v���C���[�̃Q�[���I�[�o�[�V�[���̃A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Sad Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER_GAMEOVER, res);

	// �G�̑ҋ@�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Breathing Idle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_IDLE, res);

	// �G�̕����A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Walking.mv1");
	resourcesMap_.emplace(SRC::ENEMY_WALK, res);

	// �G�̍U���A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Swiping.mv1");
	resourcesMap_.emplace(SRC::ENEMY_ATTACK, res);

	// �G�̃^�b�N���A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Run.mv1");
	resourcesMap_.emplace(SRC::ENEMY_TACKLE, res);

	// �G�̃W�����v�U���A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Jump Attack.mv1");
	resourcesMap_.emplace(SRC::ENEMY_DASH_ATTACK, res);

	// �G�̒e�̐����A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Shot Create.mv1");
	resourcesMap_.emplace(SRC::ENEMY_SHOT_CREATE, res);

	// �G�̉������U���A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Shot Attack.mv1");
	resourcesMap_.emplace(SRC::ENEMY_SHOT_ATTACK, res);

	// �G�̃_���[�W�q�b�g�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Zombie Reaction Hit.mv1");
	resourcesMap_.emplace(SRC::ENEMY_HIT, res);

	// �G�̎��S�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Dying.mv1");
	resourcesMap_.emplace(SRC::ENEMY_DEATH, res);

	// �G�̍�����A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Left Turn 45.mv1");
	resourcesMap_.emplace(SRC::ENEMY_TURN_LEFT, res);

	// �G�̉E����A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Right Turn 45.mv1");
	resourcesMap_.emplace(SRC::ENEMY_TURN_RIGHT, res);

	// �e�̃��f��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Shot/shot.mv1");
	resourcesMap_.emplace(SRC::SHOT_MODEL, res);

	// ���̃��f��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Sword/uploads_files_4143503_sword(1).mv1");
	resourcesMap_.emplace(SRC::SWORD_MODEL, res);

	// ���̃��f��1
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Sword/sword.mv1");
	resourcesMap_.emplace(SRC::SWORD_MODEL1, res);

	// ���b�N�I���J�[�\��
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "UI/enemyCursor.png",20,3,100,100);
	resourcesMap_.emplace(SRC::CURSOR, res);

	// �G�t�F�N�g------------------------------------------------

	// �ԉ΂̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Scene/Fireworks.efkefc");
	resourcesMap_.emplace(SRC::FIREWORKS_EFFECT, res);

	// �v���C���[�̗��߂̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Player/Charge.efkefc");
	resourcesMap_.emplace(SRC::CHARGE_EFFECT, res);

	// �v���C���[�̍U���������������̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Player/Impact.efkefc");
	resourcesMap_.emplace(SRC::PLAYER_IMPACT_EFFECT, res);

	// �G�̍U���������������̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Impact.efkefc");
	resourcesMap_.emplace(SRC::ENEMY_IMPACT_EFFECT, res);

	// �e�̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Shot.efkefc");
	resourcesMap_.emplace(SRC::SHOT_EFFECT, res);

	// �G�̒e�����Ƃ��̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Charge.efkefc");
	resourcesMap_.emplace(SRC::CREATE_EFFECT, res);

	// �^�b�N���̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Tackle.efkefc");
	resourcesMap_.emplace(SRC::TACKLE_EFFECT, res);

	// �^�b�N���̍U���͈͂̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Box.efkefc");
	resourcesMap_.emplace(SRC::TACKLE_RANGE_EFFECT, res);

	// �W�����v�A�^�b�N�̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/JumpAttack.efkefc");
	resourcesMap_.emplace(SRC::JUMPATTACK_EFFECT, res);

	// �W�����v�A�^�b�N�̍U���͈͂̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Ring.efkefc");
	resourcesMap_.emplace(SRC::JUMPATTACK_RANGE_EFFECT, res);

	// �W�����v�A�^�b�N�̍U���͈͓��ɋz�����ރG�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Tornade.efkefc");
	resourcesMap_.emplace(SRC::JUMPATTACK_TORNADE_EFFECT, res);

	// �T�E���h--------------------------------------------------
	
	// ���߂鉹
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/Charge.mp3");
	resourcesMap_.emplace(SRC::PLAYER_CHARGE_MUSIC, res);

	// ����؂鉹�P
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/slash1.mp3");
	resourcesMap_.emplace(SRC::SLASH_MUSIC1, res);

	// ����؂鉹�Q
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/slash2.mp3");
	resourcesMap_.emplace(SRC::SLASH_MUSIC2, res);

	// �v���C���[�̍U���������������̉��P
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/impact1.mp3");
	resourcesMap_.emplace(SRC::PLAYER_IMPACT_MUSIC1, res);

	// �v���C���[�̍U���������������̉��Q
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/impact2.mp3");
	resourcesMap_.emplace(SRC::PLAYER_IMPACT_MUSIC2, res);

	// �v���C���[�̑���
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/footSteps.mp3");
	resourcesMap_.emplace(SRC::PLAYER_FOOTSTEPS_MUSIC, res);

	// �e�����Ƃ��̉�
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/Create.mp3");
	resourcesMap_.emplace(SRC::CREATE_MUSIC, res);

	// �e�𔭎˂���Ƃ��̉�
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/Shot.mp3");
	resourcesMap_.emplace(SRC::SHOT_MUSIC, res);

	// �^�b�N���̉�
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/Tackle.mp3");
	resourcesMap_.emplace(SRC::TACKLE_MUSIC, res);

	// �G�̑���
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/footSteps.mp3");
	resourcesMap_.emplace(SRC::ENEMY_FOOTSTEPS_MUSIC, res);

	// �W�����v�A�^�b�N�̉�
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/JumpAttack.mp3");
	resourcesMap_.emplace(SRC::JUMPATTACK_MUSIC, res);

	// �n�����̉�
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/earthQuake.mp3");
	resourcesMap_.emplace(SRC::EARTHQUAKE_MUSIC, res);

	// �v���C���[�̍U���{�C�X�P
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/slashVoice1.mp3");
	resourcesMap_.emplace(SRC::SLASH_VOICE_MUSIC1, res);

	// �v���C���[�̍U���{�C�X�Q
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/slashVoice2.mp3");
	resourcesMap_.emplace(SRC::SLASH_VOICE_MUSIC2, res);

	// �v���C���[�̗��ߍU���{�C�X
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/chargeSlash.mp3");
	resourcesMap_.emplace(SRC::CHARGEATTACK_VOICE_MUSIC, res);

	// �����
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/roll.mp3");
	resourcesMap_.emplace(SRC::ROLL_MUSIC, res);

	// ����{�C�X�P
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/rollVoice1.mp3");
	resourcesMap_.emplace(SRC::ROLL_VOICE_MUSIC1, res);

	// ����{�C�X�Q
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/rollVoice2.mp3");
	resourcesMap_.emplace(SRC::ROLL_VOICE_MUSIC2, res);

	// �v���C���[�̃_���[�W�q�b�g�{�C�X�P
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/hitVoice1.mp3");
	resourcesMap_.emplace(SRC::PLAYER_HIT_VOICE_MUSIC1, res);

	// �v���C���[�̃_���[�W�q�b�g�{�C�X�Q
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/hitVoice2.mp3");
	resourcesMap_.emplace(SRC::PLAYER_HIT_VOICE_MUSIC2, res);

	// �G�̍U���{�C�X�P
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/attackVoice1.mp3");
	resourcesMap_.emplace(SRC::ENEMY_ATTACK_VOICE_MUSIC1, res);

	// �G�̍U���{�C�X�Q
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/attackVoice2.mp3");
	resourcesMap_.emplace(SRC::ENEMY_ATTACK_VOICE_MUSIC2, res);

	// �G�̃_���[�W�q�b�g�{�C�X�P
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/hitVoice1.mp3");
	resourcesMap_.emplace(SRC::ENEMY_HIT_VOICE_MUSIC1, res);

	// �G�̃_���[�W�q�b�g�{�C�X�Q
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/hitVoice2.mp3");
	resourcesMap_.emplace(SRC::ENEMY_HIT_VOICE_MUSIC2, res);

	// �v���C���[�̎��S�{�C�X
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/death.mp3");
	resourcesMap_.emplace(SRC::PLAYER_DEATH, res);

	// �G�̎��S�{�C�X
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/death.mp3");
	resourcesMap_.emplace(SRC::ENEMY_DEATH, res);

	// �^�C�g���V�[���̉��y
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Scene/Title.mp3");
	resourcesMap_.emplace(SRC::TITLE_MUSIC, res);

	// �Q�[���V�[���̉��y
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Scene/Game.mp3");
	resourcesMap_.emplace(SRC::GAME_MUSIC, res);

	// �Q�[���N���A�V�[���̉��y
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Scene/GameClear.mp3");
	resourcesMap_.emplace(SRC::GAME_CLEAR_MUSIC, res);

	// �Q�[���I�[�o�[�V�[���̉��y
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Scene/GameOver.mp3");
	resourcesMap_.emplace(SRC::GAME_OVER_MUSIC, res);

	// ���艹�̉�
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "UI/decision.mp3");
	resourcesMap_.emplace(SRC::DECISION_MUSIC, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}