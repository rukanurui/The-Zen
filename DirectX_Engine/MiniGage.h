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
	//�Ԃ̕���
	float redXdir;
	float redYdir;
	//�Ԃ̍��W
	float redX=200;
	float redY=360;
	//��������
	float AutoXdir;
	float AutoYdir;

	float AutoX;
	float AutoY;

	//�̍��W
	float blueX;
	float blueY;

	//����
	int Time;
	int Second;
	int OnTime;

	int RandX;
	int RandY;

	float redR = 3.0f;
	float blueR = 15.0f;

	float Frame = 150;

	int scene;


	// �X�P�[�����O
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	// ���̓N���X�̃|�C���^
	Input* input;
	//�����蔻��N���X�̃|�C���^
	Collision* collision;
};

