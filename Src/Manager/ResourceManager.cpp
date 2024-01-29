#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	Resource res;

	// タイトルロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Scene/Vanquish A Monster.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, res);

	// 操作説明キーボード
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/keyBoad.png");
	resourcesMap_.emplace(SRC::KEYBOAD, res);

	// 操作説明ゲームパッド
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/gamePad.png");
	resourcesMap_.emplace(SRC::GAMEPAD, res);

	// スペースキーかＢボタン
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/Space or B.png");
	resourcesMap_.emplace(SRC::SPACE_OR_B, res);

	// ゲームクリア
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Scene/GameClearImage.png");
	resourcesMap_.emplace(SRC::GAMECLEAR, res);

	// ゲームオーバー
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Scene/GameOverImage.png");
	resourcesMap_.emplace(SRC::GAMEOVER, res);

	// プレイヤーのHPバーのイメージ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/playerHPBar.png");
	resourcesMap_.emplace(SRC::PLAYER_HP_BAR, res);

	// 敵のHPバーのイメージ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/enemyHPBar.png");
	resourcesMap_.emplace(SRC::ENEMY_HP_BAR, res);

	// テスト用のステージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/TestStage.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE, res);

	// テスト1用のステージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/TestStage1.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE1, res);

	// テスト2用のステージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/GameObject.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE2, res);

	// テスト3用のステージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Map.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE3, res);

	// テスト4用のステージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Gladiator Low Poly Arena.mv1");
	resourcesMap_.emplace(SRC::TEST_STAGE4, res);

	// プレイヤーのモデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Sword And Shield.mv1");
	resourcesMap_.emplace(SRC::PLAYER_MODEL, res);

	// プレイヤーの待機アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER_IDLE, res);

	// プレイヤーの攻撃アニメーション4
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Sword And Shield Slash.mv1");
	resourcesMap_.emplace(SRC::PLAYER_ATTACK, res);

	// プレイヤーの溜め攻撃のアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Charge Slash.mv1");
	resourcesMap_.emplace(SRC::PLAYER_CHARGEATTACK, res);

	// プレイヤーの歩くアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Running.mv1");
	resourcesMap_.emplace(SRC::PLAYER_WALK, res);

	// プレイヤーの歩くアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Charge Walk.mv1");
	resourcesMap_.emplace(SRC::PLAYER_CHARGE_WALK, res);

	// プレイヤーの走るアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Standard Run.mv1");
	resourcesMap_.emplace(SRC::PLAYER_RUN, res);

	// プレイヤーのダメージヒットアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Sword And Shield Impact.mv1");
	resourcesMap_.emplace(SRC::PLAYER_HIT, res);

	// プレイヤーの回避アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Stand To Roll.mv1");
	resourcesMap_.emplace(SRC::PLAYER_ROLL, res);

	// プレイヤーの疲れたアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/tired.mv1");
	resourcesMap_.emplace(SRC::PLAYER_TIRED, res);

	// プレイヤーの死亡アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Sword And Shield Death.mv1");
	resourcesMap_.emplace(SRC::PLAYER_DEATH, res);

	// プレイヤーのゲームクリアシーンのアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/win.mv1");
	resourcesMap_.emplace(SRC::PLAYER_GAMECLEAR, res);

	// プレイヤーのゲームオーバーシーンのアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Sad Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER_GAMEOVER, res);

	// 敵の待機アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Breathing Idle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_IDLE, res);

	// 敵の歩くアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Walking.mv1");
	resourcesMap_.emplace(SRC::ENEMY_WALK, res);

	// 敵の攻撃アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Swiping.mv1");
	resourcesMap_.emplace(SRC::ENEMY_ATTACK, res);

	// 敵のタックルアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Run.mv1");
	resourcesMap_.emplace(SRC::ENEMY_TACKLE, res);

	// 敵のジャンプ攻撃アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Jump Attack.mv1");
	resourcesMap_.emplace(SRC::ENEMY_DASH_ATTACK, res);

	// 敵の弾の生成アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Shot Create.mv1");
	resourcesMap_.emplace(SRC::ENEMY_SHOT_CREATE, res);

	// 敵の遠距離攻撃アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Shot Attack.mv1");
	resourcesMap_.emplace(SRC::ENEMY_SHOT_ATTACK, res);

	// 敵のダメージヒットアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Zombie Reaction Hit.mv1");
	resourcesMap_.emplace(SRC::ENEMY_HIT, res);

	// 敵の死亡アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Dying.mv1");
	resourcesMap_.emplace(SRC::ENEMY_DEATH, res);

	// 敵の左旋回アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Left Turn 45.mv1");
	resourcesMap_.emplace(SRC::ENEMY_TURN_LEFT, res);

	// 敵の右旋回アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Mutant Right Turn 45.mv1");
	resourcesMap_.emplace(SRC::ENEMY_TURN_RIGHT, res);

	// 弾のモデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Shot/shot.mv1");
	resourcesMap_.emplace(SRC::SHOT_MODEL, res);

	// 剣のモデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Sword/uploads_files_4143503_sword(1).mv1");
	resourcesMap_.emplace(SRC::SWORD_MODEL, res);

	// 剣のモデル1
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Sword/sword.mv1");
	resourcesMap_.emplace(SRC::SWORD_MODEL1, res);

	// ロックオンカーソル
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "UI/enemyCursor.png",20,3,100,100);
	resourcesMap_.emplace(SRC::CURSOR, res);

	// エフェクト------------------------------------------------

	// 花火のエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Scene/Fireworks.efkefc");
	resourcesMap_.emplace(SRC::FIREWORKS_EFFECT, res);

	// プレイヤーの溜めのエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Player/Charge.efkefc");
	resourcesMap_.emplace(SRC::CHARGE_EFFECT, res);

	// プレイヤーの攻撃が当たった時のエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Player/Impact.efkefc");
	resourcesMap_.emplace(SRC::PLAYER_IMPACT_EFFECT, res);

	// 敵の攻撃が当たった時のエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Impact.efkefc");
	resourcesMap_.emplace(SRC::ENEMY_IMPACT_EFFECT, res);

	// 弾のエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Shot.efkefc");
	resourcesMap_.emplace(SRC::SHOT_EFFECT, res);

	// 敵の弾を作るときのエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Charge.efkefc");
	resourcesMap_.emplace(SRC::CREATE_EFFECT, res);

	// タックルのエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Tackle.efkefc");
	resourcesMap_.emplace(SRC::TACKLE_EFFECT, res);

	// タックルの攻撃範囲のエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Box.efkefc");
	resourcesMap_.emplace(SRC::TACKLE_RANGE_EFFECT, res);

	// ジャンプアタックのエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/JumpAttack.efkefc");
	resourcesMap_.emplace(SRC::JUMPATTACK_EFFECT, res);

	// ジャンプアタックの攻撃範囲のエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Ring.efkefc");
	resourcesMap_.emplace(SRC::JUMPATTACK_RANGE_EFFECT, res);

	// ジャンプアタックの攻撃範囲内に吸い込むエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Enemy/Tornade.efkefc");
	resourcesMap_.emplace(SRC::JUMPATTACK_TORNADE_EFFECT, res);

	// サウンド--------------------------------------------------
	
	// 溜める音
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/Charge.mp3");
	resourcesMap_.emplace(SRC::PLAYER_CHARGE_MUSIC, res);

	// 風を切る音１
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/slash1.mp3");
	resourcesMap_.emplace(SRC::SLASH_MUSIC1, res);

	// 風を切る音２
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/slash2.mp3");
	resourcesMap_.emplace(SRC::SLASH_MUSIC2, res);

	// プレイヤーの攻撃が当たった時の音１
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/impact1.mp3");
	resourcesMap_.emplace(SRC::PLAYER_IMPACT_MUSIC1, res);

	// プレイヤーの攻撃が当たった時の音２
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/impact2.mp3");
	resourcesMap_.emplace(SRC::PLAYER_IMPACT_MUSIC2, res);

	// プレイヤーの足音
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/footSteps.mp3");
	resourcesMap_.emplace(SRC::PLAYER_FOOTSTEPS_MUSIC, res);

	// 弾を作るときの音
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/Create.mp3");
	resourcesMap_.emplace(SRC::CREATE_MUSIC, res);

	// 弾を発射するときの音
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/Shot.mp3");
	resourcesMap_.emplace(SRC::SHOT_MUSIC, res);

	// タックルの音
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/Tackle.mp3");
	resourcesMap_.emplace(SRC::TACKLE_MUSIC, res);

	// 敵の足音
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/footSteps.mp3");
	resourcesMap_.emplace(SRC::ENEMY_FOOTSTEPS_MUSIC, res);

	// ジャンプアタックの音
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/JumpAttack.mp3");
	resourcesMap_.emplace(SRC::JUMPATTACK_MUSIC, res);

	// 地響きの音
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/earthQuake.mp3");
	resourcesMap_.emplace(SRC::EARTHQUAKE_MUSIC, res);

	// プレイヤーの攻撃ボイス１
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/slashVoice1.mp3");
	resourcesMap_.emplace(SRC::SLASH_VOICE_MUSIC1, res);

	// プレイヤーの攻撃ボイス２
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/slashVoice2.mp3");
	resourcesMap_.emplace(SRC::SLASH_VOICE_MUSIC2, res);

	// プレイヤーの溜め攻撃ボイス
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/chargeSlash.mp3");
	resourcesMap_.emplace(SRC::CHARGEATTACK_VOICE_MUSIC, res);

	// 回避音
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/roll.mp3");
	resourcesMap_.emplace(SRC::ROLL_MUSIC, res);

	// 回避ボイス１
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/rollVoice1.mp3");
	resourcesMap_.emplace(SRC::ROLL_VOICE_MUSIC1, res);

	// 回避ボイス２
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/rollVoice2.mp3");
	resourcesMap_.emplace(SRC::ROLL_VOICE_MUSIC2, res);

	// プレイヤーのダメージヒットボイス１
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/hitVoice1.mp3");
	resourcesMap_.emplace(SRC::PLAYER_HIT_VOICE_MUSIC1, res);

	// プレイヤーのダメージヒットボイス２
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/hitVoice2.mp3");
	resourcesMap_.emplace(SRC::PLAYER_HIT_VOICE_MUSIC2, res);

	// 敵の攻撃ボイス１
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/attackVoice1.mp3");
	resourcesMap_.emplace(SRC::ENEMY_ATTACK_VOICE_MUSIC1, res);

	// 敵の攻撃ボイス２
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/attackVoice2.mp3");
	resourcesMap_.emplace(SRC::ENEMY_ATTACK_VOICE_MUSIC2, res);

	// 敵のダメージヒットボイス１
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/hitVoice1.mp3");
	resourcesMap_.emplace(SRC::ENEMY_HIT_VOICE_MUSIC1, res);

	// 敵のダメージヒットボイス２
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/hitVoice2.mp3");
	resourcesMap_.emplace(SRC::ENEMY_HIT_VOICE_MUSIC2, res);

	// プレイヤーの死亡ボイス
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Player/death.mp3");
	resourcesMap_.emplace(SRC::PLAYER_DEATH, res);

	// 敵の死亡ボイス
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Enemy/death.mp3");
	resourcesMap_.emplace(SRC::ENEMY_DEATH, res);

	// タイトルシーンの音楽
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Scene/Title.mp3");
	resourcesMap_.emplace(SRC::TITLE_MUSIC, res);

	// ゲームシーンの音楽
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Scene/Game.mp3");
	resourcesMap_.emplace(SRC::GAME_MUSIC, res);

	// ゲームクリアシーンの音楽
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Scene/GameClear.mp3");
	resourcesMap_.emplace(SRC::GAME_CLEAR_MUSIC, res);

	// ゲームオーバーシーンの音楽
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "Scene/GameOver.mp3");
	resourcesMap_.emplace(SRC::GAME_OVER_MUSIC, res);

	// 決定音の音
	res = Resource(Resource::TYPE::MUSIC, Application::PATH_MUSIC + "UI/decision.mp3");
	resourcesMap_.emplace(SRC::DECISION_MUSIC, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}