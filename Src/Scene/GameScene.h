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

	// �v���C���[�ƓG���m�̓����蔻��
	void CollisionEnemyAndPlayer();

	// �f�o�b�O�`��
	void DrawDebug(void);

};
