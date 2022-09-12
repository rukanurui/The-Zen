#pragma once
#include "Input.h"
#include "Collision.h"
class MiniGage
{
public:
	MiniGage(int window_width, int window_height,Input* input,Collision*collision);

	void Initialize();
	void Update();
	void AutoMove();
	void TimeCount();
	void FrameRimit();
	void GetMouse();

	void BlueSet();
	void CollisonSphere();
	void OnTimeOver();

	float GetredXdir() { return redX; }
	float GetredYdir() { return redY; }

	float GetblueX() { return blueX; }
	float GetblueY() { return blueY; }

	void SetScene(int scene_) { scene = scene_; }

private:
	//赤の方向
	float redXdir;
	float redYdir;
	//赤の座標
	float redX=200;
	float redY=360;
	//自動方向
	float AutoXdir;
	float AutoYdir;

	float AutoX;
	float AutoY;

	//青の座標
	float blueX;
	float blueY;

	//時間
	int Time;
	int Second;
	int OnTime;

	int RandX;
	int RandY;

	float redR = 3.0f;
	float blueR = 15.0f;

	float Frame = 150;

	int scene;


	// スケーリング
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	// 入力クラスのポインタ
	Input* input;
	//当たり判定クラスのポインタ
	Collision* collision;
};

