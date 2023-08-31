#pragma once

#include "BaseScene.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>

class OverScene : public BaseScene {
	// ボタン
	enum SelectButtons {
		RETRAY,
		BACKTITLE,
		END,
	};

	// ボタンの状態
	enum ButtonState { B_In, B_Root, B_Out };


public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	OverScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~OverScene();

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
	/// グローバル変数の取得
	/// </summary>
	void ApplyGlobalVariavles();

	/// <summary>
	///
	/// </summary>
	void InitializeGlobalVariavles();

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
	bool ButtonToCurSor(Vector2 buttonPos, Vector2 buttonSize);

	/// <summary>
	/// キーマウでのボタン処理
	/// </summary>
	/// <param name="buttonPos"></param>
	/// <param name="buttonSize"></param>
	/// <param name="isSelect"></param>
	/// <param name="whatDO"></param>
	/// <param name="sprite"></param>
	/// <param name="texture"></param>
	void KeyMouse_ButtonUpdate(
	    Vector2 buttonPos, Vector2 buttonSize, bool& isSelect, bool& whatDO, Sprite* sprite,
	    uint32_t texture[2]);

	void JoyState_ButtonUpdate(
	    int selectState, Sprite* start, Sprite* end, uint32_t startTex, uint32_t endTex,
	    bool& whatDO);
	void BackTitleBGimmick();
	void LogoGimmick();

	void OverGimmick();
	void EndButtonGimmick();

private: // メンバ変数
#pragma region ロゴ
	// タイトルロゴ
	std::unique_ptr<Sprite> logo_ = nullptr;
	// タイトルロゴのテクスチャ
	uint32_t logoTex_;
	// タイトルロゴの位置
	Vector2 logoPos_{};

	//
	Vector2 logo_in_S_pos_{};
	Vector2 logo_in_E_pos_{};
	//
	Vector2 logo_root_S_pos_{};
	Vector2 logo_root_E_pos_{};
	//
	Vector2 logo_out_S_pos_{};
	Vector2 logo_out_E_pos_{};

	//
	bool isLogoGimmick = false;
	bool isEnd_LogoGimmick = false;
	bool isRootLogo = false;
	float logo_t_ = 0.0f;
	int logoState_ = B_In;

#pragma endregion

#pragma region[タイトルに戻る]
	// [タイトルに戻る]
	std::unique_ptr<Sprite> backTitle_ = nullptr;
	// タイトルロゴのテクスチャ
	uint32_t backTitleTex_[2];
	// タイトルロゴの位置
	Vector2 backTitlePos_{};

	//
	Vector2 backTitle_in_S_pos_{};
	Vector2 backTitle_in_E_pos_{};
	//
	Vector2 backTitle_root_S_pos_{};
	Vector2 backTitle_root_E_pos_{};
	//
	Vector2 backTitle_out_S_pos_{};
	Vector2 backTitle_out_E_pos_{};

	//
	bool isBackTitleGimmick = false;
	bool isEnd_BackTitleGimmick = false;
	bool isRootBackTitle_ = false;
	float backTitle_t_ = 0.0f;
	int backTitleState_ = B_In;

#pragma endregion

#pragma region[タイトルに戻る]
	// [終わる]
	std::unique_ptr<Sprite> endButton_ = nullptr;
	// [終わる]のテクスチャ
	uint32_t endButtonTex_[2];

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

	int i = 0;

	bool isTitle_ = false;
	bool isStart_ = false;
	POINT mousePos_;

	// 選択時間の設定値
	int timerSet_ = 20;
	// コントローラー時の選択時間
	int timer_ = timerSet_;

	// もし選択していたらのフラグ
	bool isSelectButton_ = false;

	// 現在の選択
	int selectButton = BACKTITLE;

	float b_in_t_offset = 0.1f;
	float b_root_t_offset = 0.1f;
	float b_out_t_offset = 0.1f;

	uint32_t BGMHandle_ = 0u;
	float bolume = 0.0f;

	uint32_t selectSEHandle_ = 0u;
	bool isStartSelect = false;
	bool isEndSelect = false;

	//
	bool isOverGimmick_ = false;
	//
	float overGimmickTimer_ = 0.0f;

	// 黒画面
	std::unique_ptr<Sprite> blackout_ = nullptr;
	// 黒画面のテクスチャ
	uint32_t blackoutTex_;
	float blackoutAlpha_ = 0.0f;
	float blackoutAlpha_offset_ = 0.1f;
};
