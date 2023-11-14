#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"

class Player : public UnitBase
{

public:

	// �v���C���[�̃J�v�Z���̉��̑��΍��W
	static constexpr VECTOR LOCAL_BODY_C_DOWN_POS = { 0.0f,-60.0f,0.0f };

	// �v���C���[�̃J�v�Z���̏�̑��΍��W
	static constexpr VECTOR LOCAL_BODY_C_UP_POS = { 0.0f,50.0f,0.0f };

	// �v���C���[�̏Փ˔���̋��̔��a
	static constexpr float COLLISION_BODY_RADIUS = 40.0f;

	// �U����2�i�K�ڂ��n�܂鎞��
	static constexpr float ATTACK_START_TIME2 = 40.0f;

	// �U����2�i�K�ڂ��n�܂鎞��
	static constexpr float ATTACK_START_TIME3 = 60.0f;

	// �U����1�i�K�ڂ��I��鎞��
	static constexpr float ATTACK_END_TIME1 = 40.0f;

	// �U����2�i�K�ڂ��I��鎞��
	static constexpr float ATTACK_END_TIME2 = 60.0f;

	// �U����3�i�K�ڂ��I��鎞��
	static constexpr float ATTACK_END_TIME3 = 40.0f;

	// �ړ���(����)
	static constexpr float MOVE_POW_WALK = 10.0f;

	// �ړ���(����)
	static constexpr float MOVE_POW_RUN = 20.0f;

	// �ҋ@�A�j���[�V�����̍Đ����x
	static constexpr float IDLE_ANIM_SPEED = 20.0f;

	// �����A�j���[�V�����̍Đ����x
	static constexpr float WALK_ANIM_SPEED = 30.0f;

	// ����A�j���[�V�����̍Đ����x
	static constexpr float RUN_ANIM_SPEED = 40.0f;

	// �U���A�j���[�V�����̍Đ����x
	static constexpr float ATTACK_ANIM_SPEED = 60.0f;

	// �U�����󂯂����̃A�j���[�V�����̍Đ����x
	static constexpr float HIT_ANIM_SPEED = 30.0f;

	// ��]���鋭��
	static constexpr float ROTATION_POW = 0.1f;

	// HP�o�[�̒���
	static constexpr int HP_LENGTH = 300;

	// ���ߎa��̃{�^���������b��
	static constexpr float CHARGE_TIME = 1.0f;

	// �v���C���[�̏��
	enum class STATE
	{
		IDLE,
		WALK,
		ATTACK_WALK,
		RUN,
		ATTACK,
		ATTACK2,
		ATTACK3,
		ATTACK4,
		HIT
	};

	// �R���X�g���N�^
	Player(void);

	// �f�X�g���N�^
	~Player(void);

	void Init(void) override;

	void Update(void) override;

	void Draw(void);

	void Release(void);

	// �v���C���[��Transform�̎擾
	const Transform& GetTransform(void) const;

	// �v���C���[�̏�Ԃ��擾
	Player::STATE GetState(void);

	// �v���C���[�̏�Ԃ�ݒ�
	void SetState(Player::STATE state);

	// �Փ˔���̉��̍��W�̎擾
	VECTOR GetCPosDown(void);

	// �Փ˔���̏�̍��W�̎擾
	VECTOR GetCPosUP(void);

	// �U���t���O�̎擾
	bool GetAttack(void);

	// �U���t���O�̐ݒ�
	void SetAttack(bool attack);

	// �v���C���[��HP�擾
	int GetHP(void);

	// �v���C���[��HP�̐ݒ�
	void SetHP(int hp);

	// �U�����������������擾
	bool GetHit(void);

	// �U����������������ݒ�
	void SetHit(bool hit);

protected:

	// �v���C���[�̏��
	STATE state_;

	// �U���t���O
	bool attack_;

	// �U�����q�b�g������
	bool hit_;

	// �U���P�i�K��
	bool attack1_;

	// �U���Q�i�K��
	bool attack2_;

	// �U���R�i�K��
	bool attack3_;

	// �A�j���[�V�������Ƃɕϐ��ɑ��
	int idleAnim_;
	int walkAnim_;
	int attackWalkAnim_;
	int runAnim_;
	//int attackAnim_;
	//int attackAnim2_;
	//int attackAnim3_;
	int attackAnim4_;
	int attackAnim5_;
	int hitAnim_;

	// ���ߎa��̃J�E���^
	float chargeCnt;

	// �J�v�Z�����A�^�b�`����t���[���̔ԍ�
	int playerAttachFrameNum_;

	// �v���C���[�̏Փ˔���̍��W
	VECTOR cBodyPosUp_;
	VECTOR cBodyPosDown_;

	// �ړ�����
	void KeyboardMove(void);
	void GamePadMove(void);

	// �U������
	void KeyboardAttack(void);
	void GamePadAttack(void);

	// �v���C���[�����ɃJ�����������鏈��
	void KeyBoardCamera(void);
	void GamePadCamera(void);

	// �Փ˔���
	void Collision(void);

	// �G���g�̏Փ˔���
	void PlayerBodyCollision(void);

	// ��ԑJ��
	void ChangeState(STATE state);

	// �A�j���[�V����
	void Animation(void)override;

	// �A�j���[�V�����̏����ݒ�
	void SetIdleAnimation(void);
	void SetWalkAnimation(void);
	void SetRunAnimation(void);
	void SetAttackAnimation(void);
	void SetAttackAnimation2(void);
	void SetAttackAnimation3(void);
	void SetAttackAnimation4(void);
	void SetHitAnimation(void);

	// �x����]
	void LazyRotation(float goalRot);

	// �f�o�b�O�`��
	void DrawDebug(void);

	// �p�����[�^�ݒ�
	void SetParam(void) override;

	// �A�j���[�V�����̏�����
	void InitAnimation(void) override;

	// �L�[�{�[�h�̑���
	void KeyboardContoroller(void);

	// �Q�[���p�b�h�̑���
	void GamePadController(void);

	// �A�j���[�V�����̃t���[���̌Œ�
	void AnimationFrame(void);

	// HP�o�[�̕`��
	void DrawHPBar(void);

};

