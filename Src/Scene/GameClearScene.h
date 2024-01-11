#pragma once
#include "SceneBase.h"

class GameClearScene : public SceneBase
{

public:

	// �R���X�g���N�^
	GameClearScene(void);

	// �f�X�g���N�^
	~GameClearScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �^�C�g�����S
	int imgTitleLogo_;

	// �^�C�g�����S�̓_�ŗp�̃J�E���^
	int logoBlinkCnt_;

	// ���S�`��
	void DrawLogo(void);

};