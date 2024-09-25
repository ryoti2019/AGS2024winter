#pragma once

class Fader
{

public:

	// �t�F�[�h���i�ޑ���
	static constexpr float SPEED_ALPHA = 5.0f;

	// ���
	enum class STATE
	{
		NONE, 
		FADE_OUT,	// ���X�ɈÓ]
		FADE_IN		// ���X�ɖ��]
	};

	Fader();

	~Fader() = default;

	void Update();
	void Draw();


	// ��Ԃ̎擾
	const STATE& GetState() const { return state_; };

	// �t�F�[�h�������I�����Ă��邩
	bool IsEnd() const { return isEnd_; };

	// �w��t�F�[�h���J�n����
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

	// ���
	STATE state_;

	// �����x
	float alpha_;

	// ���(STATE)��ۂ����܂܏I��������s�����߁A
	// Update->Draw->Update��1�t���[������p
	bool isPreEnd_;

	// �t�F�[�h�����̏I������
	bool isEnd_;

};
