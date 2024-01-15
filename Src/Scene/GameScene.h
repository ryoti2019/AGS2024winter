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

	// �v���C���[�̍U���������������̃G�t�F�N�g�̑��΍��W
	static constexpr VECTOR LOCAL_CHRAGE_POS = { 0.0f,200.0f,0.0f };

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
	// �U���������������̃G�t�F�N�g
	int effectImpactResId_;
	int effectImpactPlayId_;
	VECTOR effectImpactPos_;

	// ��
	// �v���C���[�̍U���������������̉��P
	int musicImpactId1_;

	// �v���C���[�̍U���������������̉��Q
	int musicImpactId2_;

	// �v���C���[�̍U���������������̉��R
	int musicImpactId3_;

	// �v���C���[�ƓG���m�̓����蔻��
	void CollisionEnemyAndPlayer();

	// �f�o�b�O�`��
	void DrawDebug(void);

	// �G�t�F�N�g�̏�����
	void InitEffect(void);

	// �U���������������̃G�t�F�N�g
	void ImpactPlayEffect(void);

	// �U���������������̃G�t�F�N�g
	void ImpactSyncEffect(void);

	// ���̏�����
	void InitMusic(void);

	// �v���C���[�̍U���������������̉�
	void ImpactMusic(void);

};
