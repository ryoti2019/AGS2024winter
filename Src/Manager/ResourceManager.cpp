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
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Vanquish A Monster.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, res);

	// �e�X�g�p�̃X�e�[�W
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/TestStage.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE, res);

	// �e�X�g1�p�̃X�e�[�W
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/TestStage1.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE1, res);

	// �v���C���[�̑ҋ@�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER_IDLE, res);

	// �v���C���[�̍U���A�j���[�V����4
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Sword And Shield Slash.mv1");
	resourcesMap_.emplace(SRC::PLAYER_ATTACK, res);

	// �v���C���[�̗��ߍU���̃A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Charge Slash.mv1");
	resourcesMap_.emplace(SRC::PLAYER_CHARGE_ATTACK, res);

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

	// �G�̉������U���A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Shot.mv1");
	resourcesMap_.emplace(SRC::ENEMY_SHOT_ATTACK, res);

	// �G�̃_���[�W�q�b�g�A�j���[�V����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Zombie Reaction Hit.mv1");
	resourcesMap_.emplace(SRC::ENEMY_HIT, res);

	// �e�̃��f��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Shot/shot.mv1");
	resourcesMap_.emplace(SRC::SHOT_MODEL, res);

	// ���̃��f��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Sword/uploads_files_4143503_sword(1).mv1");
	resourcesMap_.emplace(SRC::SWORD_MODEL, res);

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
