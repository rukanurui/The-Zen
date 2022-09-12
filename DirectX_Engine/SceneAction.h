#pragma once
#include "Input.h"
class SceneAction
{
public:
	SceneAction(int window_width, int window_height, Input* input);

	void Initialize();
	void Update();
	void MouseGet();
	void Title();

	int GetScene() { return scene; }
	void SetHwnd(HWND hwnd_) { hwnd=hwnd_; }
private:

	// スケーリング
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	float mouseX;
	float mouseY;

	Input* input;
	int scene = 0;

	// マウス座標を取得する
	POINT p;
	
	//ウィンドウハンドル
	HWND hwnd = nullptr;

};

