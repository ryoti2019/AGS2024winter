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

	// �v���C���[�ƓG���m�̓����蔻��
	void CollisionEnemyAndPlayer();

	// �f�o�b�O�`��
	void DrawDebug(void);

};
