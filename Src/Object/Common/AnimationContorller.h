#pragma once
#include <string>
#include <map>
class SceneManager;

class AnimationController
{

public:

	//// �A�j���[�V�����f�[�^
	//struct Animation
	//{

	//	int attachNo = -1;
	//	int animIndex = 0;
	//	float speed = 0.0f;
	//	float totalTime = 0.0f;
	//	float step = 0.0f;
	//};

	// �A�j���[�V�����f�[�^
	struct Animation
	{
		// ���f��
		int model = -1;

		// �A�j���[�V�����n���h��
		int animHandle = -1;

		// �A�j���[�V�����^�C�v
		int animType = 0;

		// �A�^�b�`No
		int attachNo = -1;

		// �D�悳���A�j���[�V����
		bool isPriority = false;

		// �A�j���[�V�����̑��Đ�����
		float animTotalTime = 0.0f;

		// �A�j���[�V�������x
		float speedAnim = 0.0f;

		// �Đ����̃A�j���[�V��������
		float stepAnim = 0.0f;

		// �u�����h���[�g
		float blendRate = 0.0f;

		// �u�����h�^�C��
		float blendTime = 0.0f;

	};

	// �R���X�g���N�^
	AnimationController(int modelId);

	// �f�X�g���N�^
	~AnimationController(void);

	// �A�j���[�V�����ǉ�
	void Add(int type, const std::string& path, float speed);

	// �A�j���[�V�����Đ�
	void Play(int type, bool isLoop = true,
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	void Update(void);

	// �A�j���[�V�����I����ɌJ��Ԃ����[�v�X�e�b�v
	void SetEndLoop(float startStep, float endStep, float speed);

	// �Đ����̃A�j���[�V����
	int GetPlayType(void) const;

	// �Đ��I��
	bool IsEnd(void) const;

private:

	// ���f���̃n���h��ID
	int modelId_;

	// ��ޕʂ̃A�j���[�V�����f�[�^
	std::map<int, Animation> animations_;

	int playType_;
	Animation playAnim_;

	// �A�j���[�V���������[�v���邩���Ȃ���
	bool isLoop_;

	// �A�j���[�V�������~�߂��܂܂ɂ���
	bool isStop_;

	// �A�j���[�V�����I����ɌJ��Ԃ����[�v�X�e�b�v
	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;

	// �t�Đ�
	float switchLoopReverse_;

};

