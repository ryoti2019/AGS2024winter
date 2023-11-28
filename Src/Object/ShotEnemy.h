#pragma once
#include "../Object/ShotBase.h"
class Player;

class ShotEnemy : public ShotBase
{

public:

	// �R���X�g���N�^
	ShotEnemy(void);

	// �f�X�g���N�^
	~ShotEnemy(void);

	ShotEnemy::STATE GetState(void);

	void SetState(ShotEnemy::STATE state);

	void SetDir(VECTOR dir);

	bool IsAlive(void);

protected:

	// �p�����[�^�ݒ�
	void SetParam(void) override;

	void Move(void)override;

	void UpdateBlast(void)override;

	// �����`�F�b�N
	virtual void CheckAlive(void);

	// �����t���O
	bool isAlive_;

};

