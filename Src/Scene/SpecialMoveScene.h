#pragma once
#include "SceneBase.h"
class Grid;
class Stage;
class Player;
class Enemy;
class Sword;

class SpecialMoveScene : public SceneBase
{
public:

	// �R���X�g���N�^
	SpecialMoveScene(void);

	// �f�X�g���N�^
	~SpecialMoveScene(void);

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





};

