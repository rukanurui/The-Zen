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

	// �X�P�[�����O
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	float mouseX;
	float mouseY;

	Input* input;
	int scene = 0;

	// �}�E�X���W���擾����
	POINT p;
	
	//�E�B���h�E�n���h��
	HWND hwnd = nullptr;

};

