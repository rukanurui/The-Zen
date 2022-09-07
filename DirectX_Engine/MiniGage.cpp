#include "MiniGage.h"
#include <time.h>

MiniGage::MiniGage(int window_width, int window_height,Input* input)
{
	this->input = input;
	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void MiniGage::Initialize()
{
	//������

}

void MiniGage::Update()
{
	
	//���Ԃ𑝉�
	TimeCount();

	//�����_���̈ړ������𐶐�
	AutoMove();

	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	redXdir = mouseMove.lX * scaleY*100;
	redYdir = mouseMove.lY * scaleX*100;

	
	//�}�E�X�̈ړ����������W�ɉ��Z
	redX += redXdir;
	redY += redYdir;

	//�����_���ŕω�����ړ����������Z
	redX += AutoX;
	redY += AutoY;
}

void MiniGage::AutoMove()
{
	//�V�[�h�l�ݒ�
	srand(time(NULL));

	//-+�����߂�
	RandX = (rand() % 2)+1;
	RandY = (rand() % 2)+1;

	//�����̑傫�������߂�
	AutoXdir = (rand() % 2 * (Time/2))+1;
	AutoYdir = (rand() % 2 * (Time/2))+1;

	//1�Ȃ�-�Q�Ȃ�{�Ŋ���10����
	if (RandX != 1)
	{
	 AutoXdir = AutoXdir / 10;
	}
	else
	{
		AutoXdir = -(AutoXdir / 10);
	}
	
	if (RandY != 1)
	{
		AutoYdir = AutoYdir / 10;
	}
	else
	{
		AutoYdir = -(AutoYdir / 10);
	}
	//��������
	AutoX = AutoXdir;
	AutoY = AutoYdir;
}

void MiniGage::TimeCount()
{
	//�P�b60�t���[���Ȃ̂�Second��60��
	//�Ȃ�����Time��1����
	if (Second < 60)
	{
		Second += 1;
	}
	else
	{
		Second = 0;
		Time += 1;
	}
}
