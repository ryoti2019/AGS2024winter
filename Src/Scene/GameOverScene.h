#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class Stage;

class GameOverScene : public SceneBase
{

public:

	// �R���X�g���N�^
	GameOverScene(void);

	// �f�X�g���N�^
	~GameOverScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// ���f������̊�{���
	Transform transform_;

	// �X�e�[�W
	Stage* stage_;

	// �Q�[���I�[�o�[�V�[���̉��y
	int musicGameOverId_;

	// ���艹
	int musicDecisionId_;

	// �A�j���[�V�����̃A�^�b�`�ԍ�
	int animAttachNo_;

	// �A�j���[�V�����̑��Đ�����
	float animTotalTime_;

	// �Đ����̃A�j���[�V��������
	float stepAnim_;

	// �A�j���[�V�������x
	float speedAnim_;

	// ���f���̒��ɂ���A�j���[�V�����̔ԍ�
	int animNo_;

	// �Q�[���I�[�o�[�̃A�j���[�V����
	int gameOverAnim_;

	// �Q�[���I�[�o�[�̉摜
	int imgGameOver_;

	// �_�ŗp�̃J�E���^
	int BlinkCnt_;

	// ���S�`��
	void DrawLogo(void);

	// �A�j���[�V�����̏�����
	void InitAnimation(void);

	// �A�j���[�V����
	void Animation(void);

	// �A�j���[�V�����̐ݒ�
	void SetAnimation(void);

	// ���̏�����
	void InitMusic(void);

};

