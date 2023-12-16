#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{

	//// モデル制御の基本情報
	//transform_.SetModel(
	//	ResourceManager::GetInstance().LoadModelDuplicate(
	//		ResourceManager::SRC::TEST_STAGE));

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::TEST_STAGE3));

	float scale = 3.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { -1500.0f, 0.0f, 11000.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

}

void Stage::Update(void)
{

	transform_.Update();

}

void Stage::Draw(void)
{

	MV1DrawModel(transform_.modelId);

}

void Stage::Release(void)
{
}
