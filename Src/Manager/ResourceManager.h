#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		TITLE_LOGO,
		GAMECLEAR,
		GAMEOVER,
		TEST_STAGE,
		TEST_STAGE1,
		TEST_STAGE2,
		TEST_STAGE3,
		PLAYER_MODEL,
		PLAYER_IDLE,
		PLAYER_ATTACK,
		PLAYER_CHARGEATTACK,
		PLAYER_WALK,
		PLAYER_RUN,
		PLAYER_HIT,
		PLAYER_CHARGE_WALK,
		PLAYER_ROLL,
		PLAYER_DEATH,
		PLAYER_GAMEOVER,
		PLAYER_GAMECLEAR,
		ENEMY_IDLE,
		ENEMY_WALK,
		ENEMY_TACKLE,
		ENEMY_ATTACK,
		ENEMY_DASH_ATTACK,
		ENEMY_SHOT_CREATE,
		ENEMY_SHOT_ATTACK,
		ENEMY_HIT,
		ENEMY_DEATH,
		ENEMY_TURN_LEFT,
		ENEMY_TURN_RIGHT,
		CURSOR,
		SHOT_MODEL,
		SWORD_MODEL,
		FIREWORKS_EFFECT,
		CHARGE_EFFECT,
		PLAYER_IMPACT_EFFECT,
		SHOT_EFFECT,
		CREATE_EFFECT,
		TACKLE_EFFECT,
		JUMPATTACK_EFFECT,
		PLAYER_CHARGE_MUSIC,
		SLASH_MUSIC1,
		SLASH_MUSIC2,
		SLASH_MUSIC3,
		PLAYER_IMPACT_MUSIC1,
		PLAYER_IMPACT_MUSIC2,
		PLAYER_IMPACT_MUSIC3,
		PLAYER_FOOTSTEPS_MUSIC,
		CREATE_MUSIC,
		SHOT_MUSIC,
		TACKLE_MUSIC,
		ENEMY_FOOTSTEPS_MUSIC,
		JUMPATTACK_MUSIC,
		EARTHQUAKE_MUSIC,
		SLASH_VOICE_MUSIC1,
		SLASH_VOICE_MUSIC2,
		SLASH_VOICE_MUSIC3,
		CHARGEATTACK_VOICE_MUSIC,
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

};

