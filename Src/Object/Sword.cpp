#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "Sword.h"

Sword::Sword(const Transform& follow) : follow_(follow)
{
}

Sword::~Sword(void)
{
}

void Sword::InitAnimation(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SWORD));
	float scale = 0.1f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 400.0f, 300.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(-90), AsoUtility::AXIS_X));
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	//animNo_ = 1;

	//// �Đ�����A�j���[�V�����̐ݒ�
	//animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	//// �A�j���[�V���������Ԃ̎擾
	//animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	//stepAnim_ = 0.0f;

	//// �A�j���[�V�������x
	//speedAnim_ = 20.0f;

	//// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	//MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void Sword::Init(void)
{

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

}

const Transform& Sword::GetTransform(void) const
{
	return transform_;
}

void Sword::Update(void)
{

	// �v���C���[�̉E��ɒǏ]������
	// �Ǐ]�Ώ�(�v���C���[)�̈ʒu
	VECTOR followPos = MV1GetFramePosition(follow_->modelId, 34);

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = follow_->quaRot;

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR swordPos = followRot.PosAxis(transform_.pos);

	// ���̈ʒu�̍X�V
	transform_.pos = VAdd(followPos, swordPos);

}

void Sword::Draw(void)
{

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);

}

void Sword::Release(void)
{

	// ���f���̊J��
	MV1DeleteModel(transform_.modelId);

}

void Sword::SetFollow(const Transform* follow)
{
	follow_ = follow;
}