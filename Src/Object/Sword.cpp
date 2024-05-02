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
			ResourceManager::SRC::SWORD_MODEL1));
	float scale = 0.15f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	rotPow = Quaternion::Mult(
		rotPow,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(-270), AsoUtility::AXIS_Z));
	rotPow = rotPow.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(0), AsoUtility::AXIS_X));
	//VECTOR localRot = { AsoUtility::Deg2RadF(90),AsoUtility::Deg2RadF(0), AsoUtility::Deg2RadF(60) };
	//transform_.quaRotLocal = Quaternion::Euler(localRot);
	transform_.quaRotLocal = Quaternion::Mult(transform_.quaRotLocal, rotPow);
	transform_.Update();

	// �G�ƌ��̓����蔻��
	EnemyCheckHit();

	cPosUp_ = { 0.0f,0.0f,0.0f };
	cPosDown_ = { 0.0f,0.0f,0.0f };

	Update();

}

void Sword::EnemyCheckHit(void)
{

}

void Sword::Init(void)
{

	//������A�^�b�`����t���[���̔ԍ�������
	WeponAttachFrameNum_ = MV1SearchFrame(followTransform_->modelId, "mixamorig:RightHand");

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

}

const Transform& Sword::GetTransform(void) const
{
	return transform_;
}

VECTOR Sword::GetCPosDown(void)
{
	return cPosDown_;
}

VECTOR Sword::GetCPosUP(void)
{
	return cPosUp_;
}

void Sword::Update(void)
{

	// �v���C���[�̉E��ɒǏ]������
	// �Ǐ]�Ώ�(�v���C���[)�̈ʒu

	//������A�^�b�`����t���[���̃��[�J�������[���h�ϊ��s����擾����
	MATRIX WeponFrameMatrix = MV1GetFrameLocalWorldMatrix(followTransform_->modelId, WeponAttachFrameNum_);

	// �s�񂩂烉�W�A��
	auto rot = MGetRotElem(WeponFrameMatrix);
	auto pos = MGetTranslateElem(WeponFrameMatrix);

	// �s�񂩂�N�H�[�^�j�I��
	Quaternion qua = Quaternion::GetRotation(rot);

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = qua;

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR swordPos = followRot.PosAxis(LOCAL_SWORD_POS);

	// ���̈ʒu�̍X�V
	transform_.pos = VAdd(pos, swordPos);

	transform_.quaRot = qua;

	// �����瓖���蔻��̉��܂ł̑��΍��W
	VECTOR cPosDOWN = transform_.quaRot.PosAxis(LOCAL_C_DOWN_POS);
	// �����瓖���蔻��̏�܂ł̑��΍��W
	VECTOR cPosUP = transform_.quaRot.PosAxis(LOCAL_C_UP_POS);

	// ���̈ʒu�̍X�V
	cPosDown_ = VAdd(transform_.pos, cPosDOWN);
	cPosUp_ = VAdd(transform_.pos, cPosUP);

	transform_.Update();

}

void Sword::Draw(void)
{

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);

	//collisionUPPos_ = VAdd(collisionUPPos_, { 0.0f,5.0f,0.0f });
	//collisionDOWNPos_ = VAdd(collisionDOWNPos_, { 0.0f,-5.0f,0.0f });

	// �����蔻��̕`��
	//DrawCapsule3D(cPosDown_, cPosUp_, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

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