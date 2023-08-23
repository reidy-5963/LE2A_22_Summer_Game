#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include <memory>

enum SCENE {
	TITLE_S,
	GAME_S,
};

class BaseScene {
public:


public:
	BaseScene();

	~BaseScene();

	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

	void SetSceneNo(int sceneNo) { scenedNo_ = sceneNo; }
	int GetSceneNo() { return scenedNo_; }

protected:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	int scenedNo_ = 0;

};