#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		TITLE_LOGO,
		KEYBOAD,
		GAMEPAD,
		SPACE_OR_B,
		GAMECLEAR,
		GAMEOVER,
		PLAYER_HP_BAR,
		ENEMY_HP_BAR,
		TEST_STAGE,
		TEST_STAGE1,
		TEST_STAGE2,
		TEST_STAGE3,
		TEST_STAGE4,
		PLAYER_MODEL,
		PLAYER_IDLE,
		PLAYER_ATTACK,
		PLAYER_ATTACK1,
		PLAYER_ATTACK2,
		PLAYER_ATTACK3,
		PLAYER_CHARGEATTACK,
		PLAYER_WALK,
		PLAYER_RUN,
		PLAYER_HIT,
		PLAYER_CHARGE_WALK,
		PLAYER_ROLL,
		PLAYER_TIRED,
		PLAYER_DEATH,
		PLAYER_GAMEOVER,
		PLAYER_GAMECLEAR,
		ENEMY_IDLE,
		ENEMY_WALK,
		ENEMY_TACKLE,
		ENEMY_ATTACK,
		ENEMY_DASH_ATTACK,
		ENEMY_SHOT_CREATE,
		ENEMY_SHOT_ATTACK,
		ENEMY_HIT,
		ENEMY_DEATH,
		ENEMY_TURN_LEFT,
		ENEMY_TURN_RIGHT,
		CURSOR,
		SHOT_MODEL,
		SWORD_MODEL,
		SWORD_MODEL1,
		FIREWORKS_EFFECT,
		CHARGE_EFFECT,
		PLAYER_IMPACT_EFFECT,
		ENEMY_IMPACT_EFFECT,
		SHOT_EFFECT,
		CREATE_EFFECT,
		TACKLE_EFFECT,
		TACKLE_RANGE_EFFECT,
		JUMPATTACK_EFFECT,
		JUMPATTACK_RANGE_EFFECT,
		JUMPATTACK_TORNADE_EFFECT,
		PLAYER_CHARGE_MUSIC,
		SLASH_MUSIC1,
		SLASH_MUSIC2,
		PLAYER_IMPACT_MUSIC1,
		PLAYER_IMPACT_MUSIC2,
		PLAYER_FOOTSTEPS_MUSIC,
		CREATE_MUSIC,
		SHOT_MUSIC,
		TACKLE_MUSIC,
		ENEMY_FOOTSTEPS_MUSIC,
		JUMPATTACK_MUSIC,
		EARTHQUAKE_MUSIC,
		SLASH_VOICE_MUSIC1,
		SLASH_VOICE_MUSIC2,
		CHARGEATTACK_VOICE_MUSIC,
		ROLL_MUSIC,
		ROLL_VOICE_MUSIC1,
		ROLL_VOICE_MUSIC2,
		PLAYER_HIT_VOICE_MUSIC1,
		PLAYER_HIT_VOICE_MUSIC2,
		ENEMY_ATTACK_VOICE_MUSIC1,
		ENEMY_ATTACK_VOICE_MUSIC2,
		ENEMY_HIT_VOICE_MUSIC1,
		ENEMY_HIT_VOICE_MUSIC2,
		TITLE_MUSIC,
		GAME_MUSIC,
		GAME_CLEAR_MUSIC,
		GAME_OVER_MUSIC,
		DECISION_MUSIC,
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> loadedMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void) = default;

	// �������[�h
	Resource* _Load(SRC src);

};

