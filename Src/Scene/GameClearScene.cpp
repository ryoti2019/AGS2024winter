#include <string>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
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
	transform_.pos = { 0.0f, -150.0f, 0.0f };
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

	// �G�t�F�N�g�Đ�
	PlayEffect();

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
	}

	// �Q�[���p�b�h�̔ԍ����擾
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		PlaySoundMem(musicDecisionId_, DX_PLAYTYPE_BACK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SceneManager::GetInstance().SetGamePad(true);
	}

	// �_�ŃJ�E���^�����Z����
	BlinkCnt_++;

	//// �G�t�F�N�g�Đ�
	//PlayEffect();

	transform_.Update();

}

void GameClearScene::Draw(void)
{
	// ���S�`��
	DrawLogo();

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);

}

void GameClearScene::Release(void)
{

	DeleteGraph(imgGameClear_);

	// ���f���̊J��
	MV1DeleteModel(transform_.modelId);

	StopEffekseer3DEffect(effectFireWorksPlayId_);

	StopSoundMem(musicGameClearId_);

}

void GameClearScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// �^�C�g�����S
	DrawRotaGraph(cx, cy, 1.0, 0.0, imgGameClear_, true);

	// Push���b�Z�[�W
	std::string msg = "Push Space";
	SetFontSize(40);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);

	// �_�ł�����
	if ((BlinkCnt_ / 30) % 2)
	{
		DrawFormatString(cx - (width / 2), 500, 0xffffff, msg.c_str());
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

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
	effectPos_ = {0.0f,-500.0f,200.0f};

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectFireWorksPlayId_, effectPos_.x, effectPos_.y, effectPos_.z);

}