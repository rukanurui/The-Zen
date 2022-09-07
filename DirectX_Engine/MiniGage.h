#pragma once
#include "Input.h"
class MiniGage
{
public:
	MiniGage(int window_width, int window_height,Input* input);

	void Initialize();
	void Update();
	void AutoMove();
	void TimeCount();

	float GetredXdir() { return redX; }
	float GetredYdir() { return redY; }

private:
	//赤の方向
	float redXdir;
	float redYdir;
	//赤の座標
	float redX;
	float redY;
	//自動方向
	float AutoXdir;
	float AutoYdir;

	float AutoX;
	float AutoY;
	//時間
	int Time;
	int Second;

	int RandX;
	int RandY;

	// スケーリング
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	// 入力クラスのポインタ
	Input* input;
};

