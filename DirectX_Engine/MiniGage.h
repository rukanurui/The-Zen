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
	//�Ԃ̕���
	float redXdir;
	float redYdir;
	//�Ԃ̍��W
	float redX;
	float redY;
	//��������
	float AutoXdir;
	float AutoYdir;

	float AutoX;
	float AutoY;
	//����
	int Time;
	int Second;

	int RandX;
	int RandY;

	// �X�P�[�����O
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	// ���̓N���X�̃|�C���^
	Input* input;
};

