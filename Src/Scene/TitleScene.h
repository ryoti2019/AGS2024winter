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

	// �^�C�g�����S�̓_�ŗp�̃J�E���^
	int logoBlinkCnt_;
	
	// ���S�`��
	void DrawLogo(void);

};
