#pragma once
#include "../Object/ShotBase.h"

class ShotEnemy : public ShotBase
{

public:

	// �R���X�g���N�^
	ShotEnemy(void);

	// �f�X�g���N�^
	~ShotEnemy(void);

	ShotEnemy::STATE GetState(void);

	void SetState(ShotEnemy::STATE state);

protected:

	// �p�����[�^�ݒ�
	void SetParam(void) override;

};

