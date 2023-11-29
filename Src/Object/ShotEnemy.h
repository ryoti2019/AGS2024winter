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
	void UpdateBlast(void)override;

	// �����`�F�b�N
	virtual void CheckAlive(void);

	// �����t���O
	bool isAlive_;

	// �G��Transform
	Transform* enemyTransform_;

	// ���΍��W
	VECTOR rPos_;

};

