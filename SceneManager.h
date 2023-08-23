#pragma once
#include "BaseScene.h"

class SceneManager {
public:
	static SceneManager* GetInstance();

public:
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	const SceneManager& operator=(const SceneManager&) = delete;


	void Initialize();

	void Update();

	void Draw();


private:
	std::unique_ptr<BaseScene> scene_[3];
	Input* input_ = nullptr;

	int sceneNo_;
	int preSceneNo_;
};