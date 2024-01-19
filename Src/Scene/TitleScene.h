#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �^�C�g�����S
	int imgTitleLogo_;

	// �^�C�g���V�[���̉��y
	int musicTitleId_;

	// ���艹
	int musicDecisionId_;

	// �_�ŗp�̃J�E���^
	int BlinkCnt_;
	
	// ���S�`��
	void DrawLogo(void);

	// ���̏�����
	void InitMusic(void);

};
