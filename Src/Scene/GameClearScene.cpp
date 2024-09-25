#include <string>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Sword.h"
#include "../Object/Stage.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene(void)
{
	imgGameClear_ = -1;
}

GameClearScene::~GameClearScene(void)
{
}

void GameClearScene::InitAnimation(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_MODEL));

	// �ҋ@�A�j���[�V����
	gameClearAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_GAMECLEAR);

	// transform�̏�����
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	Quaternion rotPow = Quaternion::Identity();
	transform_.Update();

	// �A�j���[�V��������ԍ�
	animNo_ = 1;

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_);

	// �Đ����̃A�j���[�V��������
	stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 0.0f;

}

void GameClearScene::InitEffect(void)
{

	// �ԉ΂̃G�t�F�N�g
	effectFireWorksResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::FIREWORKS_EFFECT).handleId_;

}

void GameClearScene::InitMusic(void)
{

	// �Q�[���V�[���̉��y
	musicGameClearId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_CLEAR_MUSIC).handleId_;

	ChangeVolumeSoundMem(255 * 80 / 100, musicGameClearId_);

	// �Q�[���V�[���̉��y�̍Đ�
	PlaySoundMem(musicGameClearId_, DX_PLAYTYPE_LOOP);

	// ���艹
	musicDecisionId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DECISION_MUSIC).handleId_;

}

void GameClearScene::Init(void)
{

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

	// �G�t�F�N�g�̏����ݒ�
	InitEffect();

	// ���̏����ݒ�
	InitMusic();

	// �A�j���[�V�����̐ݒ�
	SetAnimation();

	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// �Q�[���N���A
	imgGameClear_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMECLEAR).handleId_;

	// �X�y�[�X�L�[��B�{�^��
	imgSpaceOrB_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SPACE_OR_B).handleId_;

	// �G�t�F�N�g�Đ�
	PlayEffect();

	// ���̐���
	sword_ = new Sword();
	sword_->SetFollow(&transform_);
	sword_->Init();

	// �X�e�[�W�̐���
	stage_ = new Stage();
	stage_->Init();

}

void GameClearScene::Update(void)
{

	// �A�j���[�V��������
	Animation();

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		PlaySoundMem(musicDecisionId_, DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SceneManager::GetInstance().SetIsOperation(true);
	}

	// �Q�[���p�b�h�̔ԍ����擾
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		PlaySoundMem(musicDecisionId_, DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SceneManager::GetInstance().SetGamePad(true);
		SceneManager::GetInstance().SetIsOperation(true);
	}

	// �_�ŃJ�E���^�����Z����
	BlinkCnt_++;

	//// �G�t�F�N�g�Đ�
	//PlayEffect();
	
	// ���̍X�V
	sword_->Update();

	// �X�e�[�W�̍X�V
	stage_->Update();

	transform_.Update();

}

void GameClearScene::Draw(void)
{

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);

	// ���̕`��
	sword_->Draw();

	// �X�e�[�W�̕`��
	stage_->Draw();

	// ���S�`��
	DrawLogo();

}

void GameClearScene::Release(void)
{

	DeleteGraph(imgGameClear_);
	DeleteGraph(imgSpaceOrB_);

	// ���f���̊J��
	MV1DeleteModel(transform_.modelId);
	MV1DeleteModel(gameClearAnim_);

	StopEffekseer3DEffect(effectFireWorksPlayId_);
	DeleteEffekseerEffect(effectFireWorksResId_);
	DeleteEffekseerEffect(effectFireWorksPlayId_);

	DeleteSoundMem(musicGameClearId_);
	DeleteSoundMem(musicDecisionId_);

	// ���̉��
	sword_->Release();

	// �X�e�[�W�̉��
	stage_->Release();

}

void GameClearScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// �^�C�g�����S
	DrawRotaGraph(cx, cy, 1.0, 0.0, imgGameClear_, true);

	// �X�y�[�X�L�[��B�{�^��
	// �_�ł�����
	if ((BlinkCnt_ / 30) % 2)
	{
		DrawRotaGraph(cx, cy + 200, 0.5f, 0.0f, imgSpaceOrB_, true);
	}
}

void GameClearScene::Animation(void)
{

	// �A�j���[�V�����Đ�
	// �o�ߎ��Ԃ̎擾
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// �A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (speedAnim_ * deltaTime);

	if (stepAnim_ > animTotalTime_)
	{
		// ���[�v�Đ�
		stepAnim_ = 0.0f;
	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(transform_.modelId, animAttachNo_, stepAnim_);

}

void GameClearScene::SetAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, gameClearAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = 30.0f;

}

void GameClearScene::PlayEffect(void)
{

	// �G�t�F�N�g�Đ�
	effectFireWorksPlayId_ = PlayEffekseer3DEffect(effectFireWorksResId_);

	float SCALE = 20.0f;
	// �傫��
	SetScalePlayingEffekseer3DEffect(effectFireWorksPlayId_, SCALE, SCALE, SCALE);

	// �ʒu
	SyncEffect();

}

void GameClearScene::SyncEffect(void)
{

	// �ʒu
	effectPos_ = {0.0f,-400.0f,0.0f};

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectFireWorksPlayId_, effectPos_.x, effectPos_.y, effectPos_.z);

}