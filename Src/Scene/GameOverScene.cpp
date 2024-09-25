#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Stage.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
	imgGameOver_ = -1;
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::InitAnimation(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_MODEL));

	// �ҋ@�A�j���[�V����
	gameOverAnim_ = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_GAMEOVER);

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

void GameOverScene::Animation(void)
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

void GameOverScene::SetAnimation(void)
{

	MV1DetachAnim(transform_.modelId, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(transform_.modelId, animNo_, gameOverAnim_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

}

void GameOverScene::InitMusic(void)
{

	// �Q�[���V�[���̉��y
	musicGameOverId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_OVER_MUSIC).handleId_;

	ChangeVolumeSoundMem(255 * 80 / 100, musicGameOverId_);

	// �Q�[���V�[���̉��y�̍Đ�
	PlaySoundMem(musicGameOverId_, DX_PLAYTYPE_LOOP);

	// ���艹
	musicDecisionId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DECISION_MUSIC).handleId_;

}

void GameOverScene::Init(void)
{

	// ���̏����ݒ�
	InitMusic();

	// �A�j���[�V�����̏����ݒ�
	InitAnimation();

	// �A�j���[�V�����̐ݒ�
	SetAnimation();

	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// �^�C�g�����S
	imgGameOver_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMEOVER).handleId_;

	// �X�y�[�X�L�[��B�{�^��
	imgSpaceOrB_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SPACE_OR_B).handleId_;

	// �X�e�[�W�̐���
	stage_ = new Stage();
	stage_->Init();

}

void GameOverScene::Update(void)
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

	// �X�e�[�W�̍X�V
	stage_->Update();

	transform_.Update();

}

void GameOverScene::Draw(void)
{

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);

	// �X�e�[�W�̕`��
	stage_->Draw();

	// ���S�`��
	DrawLogo();

}

void GameOverScene::Release(void)
{

	DeleteGraph(imgGameOver_);
	DeleteGraph(imgSpaceOrB_);

	// ���f���̊J��
	MV1DeleteModel(transform_.modelId);
	MV1DeleteModel(gameOverAnim_);

	DeleteSoundMem(musicGameOverId_);
	DeleteSoundMem(musicDecisionId_);

	// �X�e�[�W�̉��
	stage_->Release();

}

void GameOverScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// �Q�[���I�[�o�[
	DrawRotaGraph(cx, cy, 1.0, 0.0, imgGameOver_, true);

	// �X�y�[�X�L�[��B�{�^��
	// �_�ł�����
	if ((BlinkCnt_ / 30) % 2)
	{
		DrawRotaGraph(cx, cy + 200, 0.5f, 0.0f, imgSpaceOrB_, true);
	}

}
