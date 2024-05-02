#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"
class Stage;
class AnimationController;
class InputContoroller;
;

class Player : public UnitBase
{

public:

	// �v���C���[�̃J�v�Z���̉��̑��΍��W
	static constexpr VECTOR LOCAL_BODY_C_DOWN_POS = { 0.0f,-60.0f,0.0f };

	// �v���C���[�̃J�v�Z���̏�̑��΍��W
	static constexpr VECTOR LOCAL_BODY_C_UP_POS = { 0.0f,50.0f,0.0f };

	// �v���C���[�̏Փ˔���̋��̔��a
	static constexpr float COLLISION_BODY_RADIUS = 40.0f;

	// �U����1�i�K�ڂ��I��鎞��
	static constexpr float ATTACK_END_TIME1 = 40.0f;

	// �U����1�i�K�ڂ̓����蔻�肪�n�܂鎞��
	static constexpr float ATTACK_COLLISION_START_TIME1 = 30.0f;

	// �U����1�i�K�ڂ̓����蔻�肪�I��鎞��
	static constexpr float ATTACK_COLLISION_END_TIME1 = 37.0f;

	// �U����2�i�K�ڂ��n�܂鎞��
	static constexpr float ATTACK_START_TIME2 = 40.0f;

	// �U����2�i�K�ڂ��I��鎞��
	static constexpr float ATTACK_END_TIME2 = 60.0f;

	// �U����2�i�K�ڂ̓����蔻�肪�n�܂鎞��
	static constexpr float ATTACK_COLLISION_START_TIME2 = 51.0f;

	// �U����2�i�K�ڂ̓����蔻�肪�I��鎞��
	static constexpr float ATTACK_COLLISION_END_TIME2 = 58.0f;

	// �U����3�i�K�ڂ��n�܂鎞��
	static constexpr float ATTACK_START_TIME3 = 60.0f;

	// �U����3�i�K�ڂ��I��鎞��
	static constexpr float ATTACK_END_TIME3 = 40.0f;

	// �U����3�i�K�ڂ̓����蔻�肪�n�܂鎞��
	static constexpr float ATTACK_COLLISION_START_TIME3 = 100.0f;

	// �U����3�i�K�ڂ̓����蔻�肪�I��鎞��
	static constexpr float ATTACK_COLLISION_END_TIME3 = 110.0f;

	// ���ߍU���̓����蔻�肪�n�܂鎞��
	static constexpr float CHARGE_ATTACK_COLLISION_START_TIME = 20.0f;

	// ���߂̍U���̓����蔻�肪�I��鎞��
	static constexpr float CHARGE_ATTACK_COLLISION_END_TIME = 30.0f;

	// �ړ���(����)
	static constexpr float MOVE_POW_WALK = 10.0f;

	// �ړ���(���߂Ȃ������)
	static constexpr float MOVE_POW_CHRAGE_WALK = 5.0f;

	// �ړ���(����)
	static constexpr float MOVE_POW_RUN = 20.0f;

	// �ړ���(���)
	static constexpr float MOVE_POW_ROLL = 20.0f;

	// �ҋ@�A�j���[�V�����̍Đ����x
	static constexpr float IDLE_ANIM_SPEED = 20.0f;

	// �����A�j���[�V�����̍Đ����x
	static constexpr float WALK_ANIM_SPEED = 30.0f;

	// ���߂Ȃ�������A�j���[�V�����̍Đ����x
	static constexpr float CHARGE_WALK_ANIM_SPEED = 20.0f;

	// ����A�j���[�V�����̍Đ����x
	static constexpr float RUN_ANIM_SPEED = 40.0f;

	// �U���A�j���[�V�����̍Đ����x
	static constexpr float ATTACK_ANIM_SPEED = 30.0f;

	// ���ߍU���A�j���[�V�����̍Đ����x
	static constexpr float CHARGE_ATTACK_ANIM_SPEED = 30.0f;

	// �U�����󂯂����̃A�j���[�V�����̍Đ����x
	static constexpr float HIT_ANIM_SPEED = 30.0f;

	// ����̃A�j���[�V�����̍Đ����x
	static constexpr float ROLL_ANIM_SPEED = 50.0f;

	// ��]���鋭��
	static constexpr float ROTATION_POW = 0.1f;

	// HP�o�[�̒���
	static constexpr int HP_LENGTH = 360;

	// HP�̍ő�l
	static constexpr int HP_MAX = 100;

	// ���ߎa��̃{�^���������b��
	static constexpr float CHARGE_TIME = 1.0f;

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 0.2f;

	// ���߂̃G�t�F�N�g�̑��΍��W
	static constexpr VECTOR LOCAL_CHRAGE_POS = { 0.0f,0.0f,0.0f };

	// ����̖��G���n�܂鎞��
	static constexpr float ROLL_INVINCIBLE_START_TIME = 20.0f;

	// ����̖��G���I��鎞��
	static constexpr float ROLL_INVINCIBLE_END_TIME = 50.0f;

	// �X�^�~�i�̍ő�l
	static constexpr float STAMINA_MAX = 100.0f;

	// �v���C���[�̏��
	enum class STATE
	{
		IDLE,
		WALK,
		CHARGE_WALK,
		RUN,
		ATTACK,
		ATTACK2,
		ATTACK3,
		CHARGE_ATTACK,
		HIT,
		DEATH,
		ROLL,
		TIRED,
		MAX
	};

	std::string ANIM_DATA_KEY[(int)STATE::MAX] =
	{
		"IDLE",
		"WALK",
		"CHARGE_WALK",
		"RUN",
		"ATTACK",
		"ATTACK2",
		"ATTACK3",
		"CHARGE_ATTACK",
		"HIT",
		"DEATH",
		"ROLL",
		"TIRED"
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

	// �v���C���[�̍��W��ݒ�
	void SetPos(VECTOR pos);

	// �U���t���O�̎擾
	bool GetAttack(void);

	// �U���t���O�̐ݒ�
	void SetAttack(bool attack);

	// �v���C���[��HP�擾
	int GetHP(void);

	// �v���C���[��HP�̐ݒ�
	void SetHP(int hp);

	// �v���C���[�̃X�^�~�i�擾
	float GetStamina(void);

	// �U���������������ǂ������擾
	bool GetHit(void);

	// �U���������������ǂ�����ݒ�
	void SetHit(bool hit);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

	// �X�e�[�W�̃��f��ID��ݒ�
	void SetStageID(const int modelId);

	// ���G���ǂ����擾
	bool GetInvincible(void);

protected:

	// �A�j���[�V����
	AnimationController* animationController_;

	// �R���g���[��
	InputContoroller* inputController_;

	// �v���C���[�̏��
	STATE state_;

	// ��O�̏��
	STATE preState_;

	// �A�j���[�V�����f�[�^
	std::string key_;
	std::string preKey_;

	// �Ǐ]�Ώ�
	const Transform* followTransform_;

	// �X�e�[�W��ID
	int stageId_;

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

	// ���ߎa��
	bool chargeAttack_;

	// ���b�N�I���̑O�̏��
	bool preLockOn_;

	// ��ꂽ�Ƃ��̃t���O
	bool isTired_;

	// �ҋ@�A�j���[�V����
	int idleAnim_;

	// �����A�j���[�V����
	int walkAnim_;

	// ���߂Ȃ�������A�j���[�V����
	int chargeWalkAnim_;

	// ����A�j���[�V����
	int runAnim_;

	// �U���A�j���[�V����
	int attackAnim_;

	// ���ߍU���A�j���[�V����
	int chargeAttackAnim_;

	// �_���[�W�q�b�g�A�j���[�V����
	int hitAnim_;

	// ���S�A�j���[�V����
	int deathAnim_;

	// ����A�j���[�V����
	int rollAnim_;

	// ��ꂽ�A�j���[�V����
	int tiredAnim_;

	// ���ߎa��̃J�E���^
	float chargeCnt_;

	// �J�v�Z�����A�^�b�`����t���[���̔ԍ�
	int playerAttachFrameNum_;

	// �v���C���[�̏Փ˔���̍��W
	VECTOR cBodyPosUp_;
	VECTOR cBodyPosDown_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ���
	VECTOR moveDiff_;

	// �ړ�����W
	VECTOR movedPos_;

	// �X�s�[�h
	float speed_;

	// �ړ���
	VECTOR movePow_;

	// �ړI�̊p�x
	Quaternion goalQuaRot_;

	// ��]���鎞��
	float stepRotTime_;

	// �����_�ƈړ�����W�̋���
	float target2PlayerDis_;

	// �G�Ƃ̍ŏ����̋���
	float enemyMinDis_;

	// �K�E�Z�̃��[�r�[�J�E���^
	float specialCnt_;

	// ��𒆂̖��G����
	bool isInvincible_;

	// �X�^�~�i�̍ő�l
	float staminaMax_;

	// �X�^�~�i
	float stamina_;

	// �X�^�~�i�̃J�E���^
	float staminaCnt_;

	// �ŏ��̍s���ł��Ȃ��悤�ɂ���J�E���^
	float limitCnt_;

	// �G�t�F�N�g
	// ���߂�G�t�F�N�g
	int effectChargeResId_;
	int effectChargePlayId_;
	VECTOR effectChargePos_;

	// �T�E���h
	// ���߂鉹
	int musicChargeId_;

	// ����؂鉹�P
	int musicSlash1Id_;

	// ����؂鉹�Q
	int musicSlash2Id_;

	// ����؂鉹�̃t���O
	bool isMusicSlash_;

	// ����
	int musicFootStepsId_;

	// �����̃J�E���^
	float musicFootStepsCnt_;

	// �U���{�C�X�P
	int musicSlashVoice1Id_;

	// �U���{�C�X�Q
	int musicSlashVoice2Id_;

	// ���ߍU���{�C�X
	int musicChargeSlashVoiceId_;

	// �����
	int musicRollId_;

	// ����{�C�X�P
	int musicRollVoice1Id_;

	// ����{�C�X�Q
	int musicRollVoice2Id_;

	// ����{�C�X�̃t���O
	bool isMusicRoll_;

	float stepBlend_;
	float blendTime_;

	int list_;

	int slowCnt_;

	std::vector<std::string> stateHiss_;

	// �ړ�����
	void KeyboardMove(void);

	// �U������
	void KeyboardAttack(void);

	// �v���C���[�����ɃJ�����������鏈��
	void KeyBoardCamera(void);

	// �G�����b�N�I�����鏈��
	void KeyBoardLockOn(void);
	void LockOn(void);
	void SetGoalRotate(Quaternion rot);
	void Rotate(void);

	// �Փ˔���
	void Collision(void);

	// ���g�̏Փ˔���
	void PlayerBodyCollision(void);

	// �X�e�[�W�Ƃ̓����蔻��
	void CollisionStage(void);

	// ��ԑJ��
	void ChangeState(STATE state);

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

	// �A�j���[�V����
	void Animation(void);

	// �A�j���[�V�����̃t���[���̌Œ�
	void AnimationFrame(void);

	// �G�t�F�N�g�̏�����
	void InitEffect(void);

	// �G�t�F�N�g�Đ�
	// ���߂�G�t�F�N�g
	void ChargePlayEffect(void);

	// �G�t�F�N�g�ʒu
	// ���߂�G�t�F�N�g
	void ChargeSyncEffect(void);

	// ���̏�����
	void InitMusic(void);

	// ����؂鉹
	void SlashMusic(void);

	// ����
	void FootStepsMusic(void);

	// �����
	void RollMusic(void);

};