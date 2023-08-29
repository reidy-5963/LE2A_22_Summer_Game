#pragma once

#include "BaseScene.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>

enum SelectButtons {
	START,
	END,
};

enum ButtonState {
	B_In,
	B_Root,
	B_Out
};

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public BaseScene {
public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// グローバル変数の初期化処理
	/// </summary>
	void InitializeGlobalVariavles();

	/// <summary>
	/// グローバル変数の取得
	/// </summary>
	void ApplyGlobalVariavles();

	/// <summary>
	/// キーマウでの更新処理
	/// </summary>
	void KeyMouseUpdate();

	/// <summary>
	/// コントローラー時の更新処理
	/// </summary>
	void JoyStateUpdate();

	/// <summary>
	/// もしボタン内にカーソルが入ったら
	/// </summary>
	/// <param name="buttonPos">ボタン位置</param>
	/// <param name="buttonSize">ボタンサイズ(直径)</param>
	/// <param name="mousePos">カーソル位置</param>
	/// <returns></returns>
	bool ButtonToCurSor(Vector2 buttonPos, Vector2 buttonSize, POINT mousePos);

	void StartButtonGimmick();
	void EndButtonGimmick();
	void TitleLogoGimmick();

	void TitleGimmick();

private: // メンバ変数
	// タイトルロゴ
	std::unique_ptr<Sprite> title_ = nullptr;
	// タイトルロゴのテクスチャ
	uint32_t titleTex_;
	// タイトルロゴの位置
	Vector2 titleLogoPos_{};

	//
	Vector2 titleLogo_in_S_pos_{};
	Vector2 titleLogo_in_E_pos_{};
	//
	Vector2 titleLogo_root_S_pos_{};
	Vector2 titleLogo_root_E_pos_{};
	//
	Vector2 titleLogo_out_S_pos_{};
	Vector2 titleLogo_out_E_pos_{};

	//
	bool isTitleLogoGimmick = false;
	bool isEnd_titleLogoGimmick = false;
	bool isRoottitleLogo = false;
	float titleLogo_t_ = 0.0f;
	int titleLogoState_ = B_In;



#pragma region Start
	// [始める]
	std::unique_ptr<Sprite> startButton_ = nullptr;
	// [始める]のテクスチャ
	uint32_t startButtonTex_;
	uint32_t startButton_select_Tex_;

	// [始める]の位置
	Vector2 startButtonPos_{};

	//
	Vector2 startButton_in_S_pos_{};
	Vector2 startButton_in_E_pos_{};
	//
	Vector2 startButton_root_S_pos_{};
	Vector2 startButton_root_E_pos_{};
	//
	Vector2 startButton_out_S_pos_{};
	Vector2 startButton_out_E_pos_{};

	//
	float startButton_t_ = 0.0f;
	int startButtonState_ = B_In;
	bool isRootStartButton = false;
	bool isStartButtonGimmick = false;
	bool isEnd_startButtonGimmick = false;

#pragma endregion

#pragma region End
	// [終わる]
	std::unique_ptr<Sprite> endButton_ = nullptr;
	// [終わる]のテクスチャ
	uint32_t endButtonTex_;
	uint32_t endButton_select_Tex_;

	// [終わる]の位置
	Vector2 endButtonPos_{};

	//
	Vector2 endButton_in_S_pos_{};
	Vector2 endButton_in_E_pos_{};
	//
	Vector2 endButton_root_S_pos_{};
	Vector2 endButton_root_E_pos_{};
	//
	Vector2 endButton_out_S_pos_{};
	Vector2 endButton_out_E_pos_{};

	//
	bool isEndButtonGimmick = false;
	bool isEnd_endButtonGimmick = false;
	bool isRootEndButton = false;
	float endButton_t_ = 0.0f;
	int endButtonState_ = B_In;

#pragma endregion

	// ゲームスタートフラグ
	bool isStart = false;

	// マウスの位置
	POINT mousePos_;

	// 選択時間の設定値
	int timerSet_ = 20;
	// コントローラー時の選択時間
	int timer_ = timerSet_;

	// もし選択していたらのフラグ
	bool isSelectButton_ = false;

	// 現在の選択
	int selectButton = START;

	// 前フレームのコントローラーのボタン
	WORD preJoyButton_;

	float b_in_t_offset = 0.1f;
	float b_root_t_offset = 0.1f;
	float b_out_t_offset = 0.1f;

	//
	bool isTitleGimmick_ = false;

	//
	float titleGimmickTimer_ = 0.0f;

	
	// 黒画面
	std::unique_ptr<Sprite> blackout_ = nullptr;
	// 黒画面のテクスチャ
	uint32_t blackoutTex_;
	float blackoutAlpha_ = 0.0f;
	float blackoutAlpha_offset_ = 0.1f;


	uint32_t BGMHandle_ = 0u;
	float bolume = 0.0f;

	uint32_t selectSEHandle_ = 0u;
	bool isStartSelect = false;
	bool isEndSelect = false;
};