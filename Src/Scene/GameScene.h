#pragma once
#include "SceneBase.h"

class Grid;
class Stage;
class Player;
class Enemy;
class Sword;

class GameScene : public SceneBase
{

public:

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �O���b�h��
	Grid* grid_;

	// �X�e�[�W
	Stage* stage_;

	// �v���C���[
	Player* player_;

	// �G
	Enemy* enemy_;

	// ��
	Sword* sword_;

	// �v���C���[�̉��p�̍��W
	VECTOR rollPos_;

	// HP�o�[�̉摜
	int imgPlayerHPBar_;

	// �q�b�g�X�g�b�v
	int hitStopCnt_;

	// �X���[
	int slowCnt_;

	// ���̃V�[���Ɉڂ�܂ł̎���
	float sceneCnt_;

	// �G�����񂾂Ƃ��̃t���O
	bool enemyDeath_;

	// �v���C���[�����񂾂Ƃ��̃t���O
	bool playerDeath_;

	// �G�t�F�N�g
	// �v���C���[�̍U���������������̃G�t�F�N�g
	int effectPlayerImpactResId_;
	int effectPlayerImpactPlayId_;
	VECTOR effectPlayerImpactPos_;

	// �G�̍U���������������̃G�t�F�N�g
	int effectEnemyImpactResId_;
	int effectEnemyImpactPlayId_;
	VECTOR effectEnemyImpactPos_;

	// �W�����v�A�^�b�N�̋z�����ރG�t�F�N�g
	int effectEnemyTornadeResId_;
	int effectEnemyTornadePlayId_;
	VECTOR effectEnemyTornadePos_;
	bool isEffectTornade_;

	// ��
	
	// �Q�[���V�[���̉��y
	int musicGameId_;

	// �v���C���[�̍U���������������̉��P
	int musicImpactId1_;

	// �v���C���[�̍U���������������̉��Q
	int musicImpactId2_;

	// �v���C���[�̍U���������������̉��R
	int musicImpactId3_;

	// �v���C���[�̃_���[�W�q�b�g�{�C�X�P
	int musicPlayerHitVoice1_;

	// �v���C���[�̃_���[�W�q�b�g�{�C�X�Q
	int musicPlayerHitVoice2_;

	// �G�̃_���[�W�q�b�g�̃{�C�X�P
	int musicEnemyHitVoice1_;

	// �G�̃_���[�W�q�b�g�̃{�C�X�Q
	int musicEnemyHitVoice2_;

	// �G�̃_���[�W�q�b�g�̃{�C�X�R
	int musicEnemyHitVoice3_;

	// �G�̃v���C���[�ƓG���m�̓����蔻��
	void CollisionEnemyAndPlayer();

	// �f�o�b�O�`��
	void DrawDebug(void);

	// �G�t�F�N�g�̏�����
	void InitEffect(void);

	// �v���C���[�̍U���������������̃G�t�F�N�g
	void PlayerImpactPlayEffect(void);

	// �G�̍U���������������̃G�t�F�N�g
	void EnemyImpactPlayEffect(void);

	// �W�����v�A�^�b�N�̋z�����ރG�t�F�N�g
	void EnemyTornadePlayEffect(void);

	// �v���C���[�̍U���������������̃G�t�F�N�g
	void PlayerImpactSyncEffect(void);

	// �G�̍U���������������̃G�t�F�N�g
	void EnemyImpactSyncEffect(void);

	// �W�����v�A�^�b�N�̋z�����ރG�t�F�N�g
	void EnemyTornadeSyncEffect(void);

	// ���̏�����
	void InitMusic(void);

	// �v���C���[�̍U���������������̉�
	void ImpactMusic(void);

	// �v���C���[�_���[�W�q�b�g��
	void PlayerHitMusic(void);

	// �G�̃_���[�W�q�b�g��
	void EnemyHitMusic(void);

	// HP�o�[�̕`��
	void DrawHPBar(void);

};
