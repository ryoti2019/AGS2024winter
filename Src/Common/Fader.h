#pragma once

class Fader
{

public:

	// フェードが進む速さ
	static constexpr float SPEED_ALPHA = 5.0f;

	// 状態
	enum class STATE
	{
		NONE, 
		FADE_OUT,	// 徐々に暗転
		FADE_IN		// 徐々に明転
	};

	Fader();

	~Fader() = default;

	void Update();
	void Draw();


	// 状態の取得
	const STATE& GetState() const { return state_; };

	// フェード処理が終了しているか
	bool IsEnd() const { return isEnd_; };

	// 指定フェードを開始する
	const void SetFade(const STATE& state)
	{

		state_ = state;
		if (state_ != STATE::NONE)
		{
			isPreEnd_ = false;
			isEnd_ = false;
		};

	};

private:

	// 状態
	STATE state_;

	// 透明度
	float alpha_;

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;

};
