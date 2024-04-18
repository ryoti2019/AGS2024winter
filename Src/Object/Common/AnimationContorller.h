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
	struct AnimationData
	{

		// ���f��
		int model = -1;

		// �A�j���[�V�����n���h��
		int animHandle = -1;

		// �A�j���[�V�����^�C�v
		 state = 

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

		// �A�j���[�V�������n�܂鎞��
		float startAnim = 0.0f;

		// �A�j���[�V�������I��鎞��
		float endAnim = 0.0f;

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
	//void Add(STATE state, const std::string& path, float speed);
	void Add(const std::string state, const std::string& path, float startStep,
		float endStep, float speed, bool isPriority);

	// �A�j���[�V�����Đ�
	void Play(STATE state, bool isLoop = true,
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isPriority = false);

	void Update(void);

	// �A�j���[�V�����I����ɌJ��Ԃ����[�v�X�e�b�v
	void SetEndLoop(float startStep, float endStep, float speed);

	// �Đ����̃A�j���[�V����
	AnimationController::STATE GetPlayType(void) const;

	// �Đ��I��
	bool IsEnd(void) const;

	// �A�^�b�`����Ă��鐔���擾
	int GetAttachNum(void) const;


private:

	// ���
	//STATE state_;
	//STATE preState_;

	// ���f���̃n���h��ID
	int modelId_;

	// ���f����ǂݍ��񂾂��̃t���O
	bool LoadModel_;

	// ��ޕʂ̃A�j���[�V�����f�[�^
	std::map < std::string , AnimationData > animData_;

	std::string state_ = "";

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

	// �A�^�b�`���Ă��鐔
	int AttachNum_;

	// �A�^�b�`
	void Attatch(STATE state);

	// �f�^�b�`
	void Dettach(int attachNo, STATE state);

	// ��ԑJ��
	void ChangeAnimation(STATE state);

};

