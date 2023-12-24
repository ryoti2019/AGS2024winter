#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"

class Stage
{

public:

	// �R���X�g���N�^
	Stage(void);

	// �f�X�g���N�^
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// ���f��ID�̃Q�b�^�[
	int GetModelId(void);



private:

	// ���f������̊�{���
	Transform transform_;

};


