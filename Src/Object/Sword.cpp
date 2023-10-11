#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "Sword.h"

Sword::Sword(void)
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
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(-90), AsoUtility::AXIS_Z));
	rotPow = rotPow.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(90), AsoUtility::AXIS_X));
	//VECTOR localRot = { AsoUtility::Deg2RadF(90),AsoUtility::Deg2RadF(0), AsoUtility::Deg2RadF(60) };
	//transform_.quaRotLocal = Quaternion::Euler(localRot);
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	// �G�ƌ��̓����蔻��
	EnemyCheckHit();

}

void Sword::EnemyCheckHit(void)
{

}

void Sword::Init(void)
{
	//������A�^�b�`����t���[���̔ԍ�������
	WeponAttachFrameNum = MV1SearchFrame(followTransform_->modelId, "mixamorig:RightHand");

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

	//������A�^�b�`����t���[���̃��[�J�������[���h�ϊ��s����擾����
	MATRIX WeponFrameMatrix = MV1GetFrameLocalWorldMatrix(followTransform_->modelId, WeponAttachFrameNum);

	// �s�񂩂烉�W�A��
	auto rot = MGetRotElem(WeponFrameMatrix);
	auto pos = MGetTranslateElem(WeponFrameMatrix);

	// �s�񂩂�N�H�[�^�j�I��
	Quaternion qua = Quaternion::GetRotation(rot);

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = qua;

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR swordPos = followRot.PosAxis({60.0f,0.0f,0.0f});

	// ���̈ʒu�̍X�V
	transform_.pos = VAdd(pos, swordPos);

	transform_.quaRot = qua;

	//// ���W�A������s��
	//MATRIX matPos = MGetTranslate(transform_.pos);

	//MV1SetMatrix(transform_.modelId, matPos);

	//MV1SetPosition(followTransform_->modelId,transform_.pos);

	//transform_.pos = followPos;

	transform_.Update();

}

void Sword::Draw(void)
{

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);

	// �����蔻��̕`��
	DrawCapsule3D(transform_.pos,transform_.pos, 10, 10, 0xff0000, 0xff0000, false);

}

void Sword::Release(void)
{

	// ���f���̊J��
	MV1DeleteModel(transform_.modelId);

}

void Sword::SetFollow(const Transform* follow)
{

	followTransform_ = follow;

}
