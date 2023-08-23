#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ImGuiManager.h"

SceneManager* SceneManager::GetInstance() {
	// 
	static SceneManager instance;
	return &instance;
}

void SceneManager::Initialize() {
	// 
	input_ = Input::GetInstance();

	// 
	scene_[TITLE_S] = std::make_unique<TitleScene>();
	scene_[GAME_S] = std::make_unique<GameScene>();
	sceneNo_ = TITLE_S;

	scene_[sceneNo_]->Initialize();
}

void SceneManager::Update() { 
	// 
	preSceneNo_ = sceneNo_;
	sceneNo_ = scene_[sceneNo_]->GetSceneNo();

	if (preSceneNo_ != sceneNo_) {
		scene_[sceneNo_]->Initialize();
	}


	scene_[sceneNo_]->Update();

	ImGui::Begin("Deb");
	ImGui::SliderInt("sceneNo", &sceneNo_, 0, 1);
	ImGui::SliderInt("pre", &preSceneNo_, 0, 1);

	ImGui::End();
		
}


void SceneManager::Draw() { 
	// 
	scene_[sceneNo_]->Draw();
}
