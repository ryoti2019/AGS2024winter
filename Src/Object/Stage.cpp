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

	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::TEST_STAGE4));

	float scale = 8.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, -160.0f, 0.0f };

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

int Stage::GetModelId(void)
{
	return transform_.modelId;
}
