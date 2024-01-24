#pragma once
#include "Common/Transform.h"
#include "UnitBase.h"
class ShotEnemy;

class Enemy : public UnitBase
{

public:

	// �G�l�~�[���g�̃J�v�Z���̉��̑��΍��W
	static constexpr VECTOR LOCAL_BODY_C_DOWN_POS = { 0.0f,0.0f,0.0f };

	// �G�l�~�[���g�̃J�v�Z���̏�̑��΍��W
	static constexpr VECTOR LOCAL_BODY_C_UP_POS = { 0.0f,-100.0f,-50.0f };

	// �G�l�~�[����̃J�v�Z���̉��̑��΍��W
	static constexpr VECTOR LOCAL_WEPON_C_DOWN_POS = { 0.0f,0.0f,0.0f };

	// �G�l�~�[����̃J�v�Z���̏�̑��΍��W
	static constexpr VECTOR LOCAL_WEPON_C_UP_POS = { 0.0f,150.0f,0.0f };

	// �e�̑��΍��W
	static constexpr VECTOR LOCAL_SHOT_POS = { 0.0f,200.0f,0.0f };

	// �G�l�~�[���g�̏Փ˔���̋��̔��a
	static constexpr float COLLISION_BODY_RADIUS = 100.0f;

	// �G�l�~�[����̏Փ˔���̋��̔��a
	static constexpr float COLLISION_WEPON_RADIUS = 70.0f;

	// �ːi�̕b��
	static constexpr float TACKLE_TIME = 4.0f;

	// �ŏ��̕���������b��
	static constexpr float FIRST_WALK_TIME = 2.0f;

	// �A�^�b�N���̍U���͈�
	static constexpr float ATTACK_RANGE = 200.0f;

	// �W�����v�A�^�b�N�̍U���͈�(�Œ�)
	static constexpr float JUMP_ATTACK_RANGE_MIN = 200.0f;

	// �W�����v�A�^�b�N�̍U���͈�(�ō�)
	static constexpr float JUMP_ATTACK_RANGE_MAX = 3000.0f;

	//// �^�b�N���̍U���͈�(�Œ�)
	//static constexpr float TACKLE_RANGE_MIN = 1000.0f;

	// �ړI�̊p�x�܂ł̍�
	static constexpr float GOAL_DEG = 5.0f;

	// �����X�s�[�h
	static constexpr float WALK_SPEED = 2.0f;

	// �W�����v�A�^�b�N�̃X�s�[�h
	static constexpr float JUMP_ATTACK_SPEED = 40.0f;

	// �W�����v�A�^�b�N�̃X�s�[�h
	static constexpr float TACKLE_SPEED = 30.0f;

	// �_���[�W�q�b�g�̃X�s�[�h
	static constexpr float HIT_SPEED = -5.0f;

	// �U���̓����蔻�肪�n�܂鎞��
	static constexpr float ATTACK_COLLISION_START_TIME = 30.0f;

	// �U���̓����蔻�肪�I��鎞��
	static constexpr float ATTACK_COLLISION_END_TIME = 50.0f;

	// �W�����v�A�^�b�N�̓����蔻�肪�n�܂鎞��
	static constexpr float JUMP_ATTACK_COLLISION_START_TIME = 40.0f;

	// �W�����v�A�^�b�N�̓����蔻�肪�I��鎞��
	static constexpr float JUMP_ATTACK_COLLISION_END_TIME = 70.0f;

	// �W�����v�A�^�b�N���I��鎞��
	static constexpr float JUMP_ATTACK_END_TIME = 200.0f;

	// �e�̐������n�܂鎞��
	static constexpr float SHOT_CREATE_START_TIME = 10.0f;

	// �e�̐������I��鎞��
	static constexpr float SHOT_CREATE_END_TIME = 30.0f;

	// �V���b�g�A�^�b�N���n�܂鎞��
	static constexpr float SHOT_START_TIME = 20.0f;

	// �V���b�g�A�^�b�N���I��鎞��
	static constexpr float SHOT_END_TIME = 40.0f;

	// �_���[�W�q�b�g���I��鎞��
	static constexpr float HIT_END_TIME = 300.0f;

	// �ҋ@�A�j���[�V�����̍Đ����x
	static constexpr float IDLE_ANIM_SPEED = 20.0f;

	// �����A�j���[�V�����̍Đ����x
	static constexpr float WALK_ANIM_SPEED = 20.0f;

	// ����A�j���[�V�����̍Đ����x
	static constexpr float RUN_ANIM_SPEED = 40.0f;

	// �U���A�j���[�V�����̍Đ����x
	static constexpr float ATTACK_ANIM_SPEED = 20.0f;

	// �W�����v�A�^�b�N�A�j���[�V�����̍Đ����x
	static constexpr float JUMP_ATTACK_ANIM_SPEED = 20.0f;

	// �^�b�N���A�j���[�V�����̍Đ����x
	static constexpr float TACKLE_ANIM_SPEED = 40.0f;

	// �e�����A�j���[�V�����̍Đ����x
	static constexpr float SHOT_CREATE_SPEED = 10.0f;

	// �V���b�g�A�j���[�V�����̍Đ����x
	static constexpr float SHOT_ANIM_SPEED = 30.0f;

	// �_���[�W�q�b�g�A�j���[�V�����̍Đ����x
	static constexpr float HIT_ANIM_SPEED = 20.0f;

	// HP�o�[�̒���
	static constexpr int HP_LENGTH = 780;

	// HP�̍ő�l
	static constexpr int HP_MAX = 100;

	// �e�̐����Ԋu
	static constexpr float TIME_DELAY_SHOT_CREATE = 0.1f;

	// �e�̔��ˊԊu
	static constexpr float TIME_DELAY_SHOT = 2.0f;

	// �G�̃N�[���^�C��
	static constexpr float COOL_TIME = 3.0f;

	// ���b�N�I���J�[�\���̑���
	static constexpr int LOCKON_CURSOR_NUM = 59;

	// �V���b�g�U���̑S�̂̎���
	static constexpr float SHOT_ATTACK_TIME = 7.0f;

	// �v���C���[�̏��
	enum class STATE
	{
		THINK,
		IDLE,
		WALK,
		ATTACK,
		JUMP_ATTACK,
		TACKLE,
		CREATE,
		SHOT,
		HIT,
		DEATH,
		TURN_LEFT,
		TURN_RIGHT,
	};

	// �R���X�g���N�^
	Enemy(void);

	// �f�X�g���N�^
	~Enemy(void);

	void Init(void)override;

	void Update(void)override;

	void Draw(void)override;

	void Release(void);

	// �G��Transform�̎擾
	const Transform& GetTransform(void) const;

	// �v���C���[�̏�Ԃ��擾
	Enemy::STATE GetState(void);

	// �v���C���[�̏�Ԃ��擾
	void SetState(Enemy::STATE state);

	// �G���g�̏Փ˔���̉��̍��W�̎擾
	VECTOR GetCBodyPosDown(void);

	// �G���g�̏Փ˔���̏�̍��W�̎擾
	VECTOR GetCBodyPosUP(void);

	// �G����̏Փ˔���̉��̍��W�̎擾
	VECTOR GetCWeponPosDown(void);

	// �G����̏�̍��W�̎擾
	VECTOR GetCWeponPosUP(void);

	// �G��HP�擾
	int GetHP(void);

	// �G��HP�̐ݒ�
	void SetHP(int hp);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

	// �U���t���O�̎擾
	bool GetAttack(void);

	// �U���t���O�̐ݒ�
	void SetAttack(bool attack);

	// �U���������������ǂ����̎擾
	bool GetHit(void);

	// �U���������������ǂ����̐ݒ�
	void SetHit(bool hit);

	// �e�����񂾐��̎擾
	void SetDeathCnt(int cnt);

	// �e
	std::vector<ShotEnemy*>& GetShots(void);

	// �X�e�[�W�̃��f��ID��ݒ�
	void SetStageID(const int modelId);

	// �W�����v�A�^�b�N����Ƃ��ɕۑ�����v���C���[�̈ʒu
	VECTOR GetAttackPlayerPos(void);

	// �^�b�N�������ǂ���
	bool GetIsTackle(void);

protected:

	// �e
	std::vector<ShotEnemy*> shots_;

	// ���
	STATE state_;
	STATE preState_;

	// HP�o�[�̉摜
	int imgHPBar_;

	// �X�e�[�W���f��ID
	int stageId_;

	// �ҋ@�A�j���[�V����
	int idleAnim_;

	// �����A�j���[�V����
	int walkAnim_;

	// �^�b�N���A�j���[�V����
	int tackleAnim_;

	// �U���A�j���[�V����
	int attackAnim_;

	// �W�����v�A�^�b�N�̃A�j���[�V����
	int jumpAttackAnim_;

	// �e�����
	int createAnim_;

	// �V���b�g�A�j���[�V����
	int shotAnim_;

	// �_���[�W�q�b�g�A�j���[�V����
	int hitAnim_;

	// ���S�A�j���[�V����
	int deathAnim_;

	// ������A�j���[�V����
	int turnLeftAnim_;

	// �E����A�j���[�V����
	int turnRightAnim_;

	// �ŏ��̕����̃A�j���[�V�����̃J�E���^
	float walkCnt_;

	// ��]�̊J�n�t���O
	bool startRotation_;

	// ��]�̏I���̃t���O
	bool isRotation_;

	// �s���̏I���t���O
	bool isAction_;

	// �U���̔ԍ�
	int attackNumber_;

	// �U���̃t���O
	bool attack_;

	// �e�𔭎˂������ǂ���
	bool isShot_;

	// �U���������������ǂ���
	bool hit_;

	// �_�b�V���U���̃t���O
	bool jumpAttack_;

	// �^�b�N���̍U���t���O
	bool isTackle_;

	// �ːi����܂ł̎���
	float beforeTackleCnt_;

	// �ːi��������b��
	float tackleCnt_;

	// �ړ�����������t���[���ԍ�
	int enemyPosFrameNum_;

	// �J�v�Z�����A�^�b�`����t���[���̔ԍ�
	int enemyAttachFrameNum_;

	// ������A�^�b�`����t���[���̔ԍ�
	int weponAttachFrameNum_;

	// �v���C���[�̕���
	VECTOR pDirection_;

	// �G�l�~�[���g�̏Փ˔���̍��W
	VECTOR cBodyPosUp_;
	VECTOR cBodyPosDown_;

	// �G�l�~�[����̏Փ˔���̍��W
	VECTOR cWeponPosUp_;
	VECTOR cWeponPosDown_;

	// �_�b�V���U�����鎞�̃v���C���[���W��ۊǂ�����W
	VECTOR attackPlayerPos_;

	// �Ǐ]�Ώ�
	const Transform* followTransform_;

	// �e�𐶐�����Ԋu
	float delayCreate_;

	// �e�𐶐�����p�x
	int shotCreateDeg_;
	float shotCreateRad_;

	// �e�̔��ˊԊu
	float delayShot_;

	// �e�𔭎˂���S�̂̃J�E���^
	float shotCnt_;

	// �ҋ@���Ă���ʂ̐�
	int shotNum_;

	// �G�������Ȃ�����
	float noPlayTime_;

	// �G�𓮂��Ȃ��悤�ɂ���t���O
	bool isNoPlay_;

	// ���b�N�I���J�[�\��
	int* lockOnCursorImg_;

	// ���b�N�I���J�[�\���̓Y�����𑝂₷
	int lockOnCursorCnt_;

	// ���b�N�I���J�[�\���̓Y�����𑝂₷�J�E���^
	float lockOnCursorTime_;

	// ���b�N�I���J�[�\����Y���W�𐧌䂷����W
	VECTOR pos_;

	// �ړ�����W
	VECTOR movedPos_;

	// �ړ���
	VECTOR movePow_;

	// �G�t�F�N�g

	// �e�����G�t�F�N�g
	int effectCreateResId_;
	int effectCreatePlayId_;
	VECTOR effectCreatePos_;

	// �^�b�N���̃G�t�F�N�g
	int effectTackleResId_;
	int effectTacklePlayId_;
	VECTOR effectTacklePos_;

	// �^�b�N���̍U���͈͂̃G�t�F�N�g
	int effectTackleRangeResId_;
	int effectTackleRangePlayId_;
	VECTOR effectTackleRangePos_;

	// �W�����v�A�^�b�N�̃G�t�F�N�g
	int effectJumpAttackResId_;
	int effectJumpAttackPlayId_;
	VECTOR effectJumpAttackPos_;
	bool isEffectJumpAttack_;

	// �W�����v�A�^�b�N�̍U���͈͂̃G�t�F�N�g
	int effectJumpAttackRangeResId_;
	int effectJumpAttackRangePlayId_;
	VECTOR effectJumpAttackRangePos_;

	// �T�E���h
	// �e����鉹
	int musicCreateId_;

	// �^�b�N���̉�
	int musicTackleId_;

	// ����
	int musicFootStepsId_;

	// �����̃J�E���^
	float musicFootStepsCnt_;

	// �W�����v�A�^�b�N�̉�
	int musicJumpAttackId_;

	// �n�����̉�
	int musicEarthQuakeId_;

	// �U���̃{�C�X�P
	int musicAttackVoice1Id_;

	// �U���̃{�C�X�Q
	int musicAttackVoice2Id_;

	// �U���̃{�C�X�R
	int musicAttackVoice3Id_;

	// �U���̃{�C�X�̃t���O
	bool isMusicAttackVoice_;

	// �s���̑I��
	void Think(void);

	// ��]����
	void Rotation(void);

	// �s����̉�]����
	void AfterRotation(void);

	// �ҋ@
	void UpdateIdle(void);

	// �ړ�
	void UpdateWalk(void);

	// �ʏ�U��
	void UpdateAttack(void);

	// �_�b�V���U��
	void UpdateJumpAttack(void);

	// �^�b�N���U��
	void UpdateTackle(void);

	// �e�����
	void UpdateCreate(void);

	// �V���b�g�U��
	void UpdateShot(void);

	// �U���q�b�g
	void UpdateHit(void);

	// �Փ˔���
	void Collision(void);

	// �X�e�[�W�Ƃ̏Փ˔���
	void CollisionStage(void);

	// �G���g�̏Փ˔���
	void EnemyBodyCollision(void);

	// �G�̕���̏Փ˔���
	void WeponCollision(void);

	// ��ԑJ��
	void ChangeState(STATE state);

	// �ҋ@�A�j���[�V�����̐ݒ�
	void SetIdleAnimation(void);

	// �����A�j���[�V�����̐ݒ�
	void SetWalkAnimation(void);

	// �^�b�N���̃A�j���[�V�����̐ݒ�
	void SetTackleAnimation(void);

	// �U���A�j���[�V�����̐ݒ�
	void SetAttackAnimation(void);

	// �W�����v�A�^�b�N�A�j���[�V�����̐ݒ�
	void SetJumpAttackAnimation(void);

	// �e�����A�j���[�V�����̐ݒ�
	void SetCreateAnimation(void);

	// �V���b�g�̐ݒ�
	void SetShotAnimation(void);

	// �_���[�W�q�b�g�q�b�g�A�j���[�V�����̐ݒ�
	void SetHitAnimation(void);

	// ���S�A�j���[�V�����̐ݒ�
	void SetDeathAnimation(void);

	// ������A�j���[�V�����̐ݒ�
	void SetTurnLeftAnimation(void);

	// �E����A�j���[�V�����̐ݒ�
	void SetTurnRightAnimation(void);

	// �x����]
	void LazyRotation(float goalRot);

	// �f�o�b�O�`��
	void DrawDebug(void);

	// �p�����[�^�ݒ�
	void SetParam(void) override;

	// �A�j���[�V�����̏�����
	void InitAnimation(void) override;

	// �A�j���[�V����
	void Animation(void) override;

	// �A�j���[�V�����̃t���[���̌Œ�
	void AnimationFrame(void);

	// ����F�e����
	void ProcessShot(void);

	// ���@�̒e�𔭎�
	void CreateShot(void);

	// �e������ł��邩�ǂ���
	ShotEnemy* GetAvailableShot(void);

	// �e��ł�
	void Shot(void);

	// �G�t�F�N�g�̏�����
	void InitEffect(void);

	// �G�t�F�N�g�Đ�
	void CreatePlayEffect(void);
	void TacklePlayEffect(void);
	void TackleRangePlayEffect(void);
	void JumpAttackPlayEffect(void);
	void JumpAttackRangePlayEffect(void);

	// �G�t�F�N�g�ʒu
	void CreateSyncEffect(void);
	void TackleSyncEffect(void);
	void TackleRangeSyncEffect(void);
	void JumpAttackSyncEffect(void);
	void JumpAttackRangeSyncEffect(void);

	// ���̏�����
	void InitMusic(void);

	// ����
	void FootStepsMusic(void);

	// �U������{�C�X
	void AttackMusic(void);

};