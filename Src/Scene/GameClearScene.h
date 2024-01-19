#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

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

	// ���f������̊�{���
	Transform transform_;

	// �Q�[���V�[���̉��y
	int musicGameClearId_;

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
	int gameClearAnim_;

	// �Q�[���N���A�̉摜
	int imgGameClear_;

	// �_�ŗp�̃J�E���^
	int BlinkCnt_;

	// �G�t�F�N�g
	int effectFireWorksResId_;
	int effectFireWorksPlayId_;
	VECTOR effectPos_;
	float stepEffect_;

	// ���S�`��
	void DrawLogo(void);

	// �A�j���[�V�����̏�����
	void InitAnimation(void);

	// �G�t�F�N�g�̏�����
	void InitEffect(void);

	// �A�j���[�V����
	void Animation(void);

	// �A�j���[�V�����̐ݒ�
	void SetAnimation(void);

	// �G�t�F�N�g�Đ�
	void PlayEffect(void);

	// �G�t�F�N�g�ʒu
	void SyncEffect(void);

	// ���̏�����
	void InitMusic(void);

};