#pragma once
#include "../Object/ShotBase.h"
class Player;

class ShotEnemy : public ShotBase
{

public:

	// �R���X�g���N�^
	ShotEnemy();

	// �f�X�g���N�^
	~ShotEnemy(void);

	ShotEnemy::STATE GetState(void);

	void SetState(ShotEnemy::STATE state);

	void SetDir(VECTOR dir);

	bool IsAlive(void);

	bool IsIdle(void);

	void Shot(VECTOR dir);

	/// <summary>
	/// �e�̐���
	/// </summary>
	/// <param name="birthPos">�������W</param>
	/// <param name="dir">�i�s����</param>
	void Create(VECTOR relPos, Transform* follow) override;

protected:

	// �p�����[�^�ݒ�
	void SetParam(void) override;

	void Move(void)override;

	void UpdateIdle(void)override;
	void UpdateShot(void)override;
	void UpdateBlast(void)override;

	// �����`�F�b�N
	virtual void CheckAlive(void);

	// �����t���O
	bool isAlive_;

	// �G��Transform
	Transform* enemyTransform_;

	// ���΍��W
	VECTOR rPos_;

	// �G�t�F�N�g
	// �e�̃G�t�F�N�g
	int effectShotResId_;
	int effectShotPlayId_;
	VECTOR effectShotPos_;

	// �T�E���h
	// �e����鉹
	int musicCreateId_;

	// �e�𔭎˂���Ƃ��̉�
	int musicShotId_;

	// �G�t�F�N�g�̏�����
	void InitEffect(void);

	// �G�t�F�N�g�Đ�
	// �e�̃G�t�F�N�g
	void ShotPlayEffect(void);

	// �G�t�F�N�g�ʒu
	// �e�̃G�t�F�N�g
	void ShotSyncEffect(void);

	// ���̏�����
	void InitMusic(void);

};

