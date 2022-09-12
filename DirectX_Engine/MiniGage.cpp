#include "MiniGage.h"
#include <time.h>

MiniGage::MiniGage(int window_width, int window_height,Input* input,Collision* collision)
{
	this->input = input;
	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void MiniGage::Initialize()
{
	//������
	BlueSet();
}

void MiniGage::Update()
{
	//���Ԃ𑝉�
	TimeCount();

	//�����_���̈ړ������𐶐�
	AutoMove();

	//�}�E�X�̍��W����ω��ʂ��擾
	GetMouse();

	//Frma�O�ɏo���Ƃ��̏���
	FrameRimit();

	//�ԂƐ̓����蔻��
	CollisonSphere();

	//�ɂR�b����Ă�ꂽ�玟�Ɉړ�
	OnTimeOver();
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

	//�����_���ŕω�����ړ����������Z
	redX += AutoX;
	redY += AutoY;

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

void MiniGage::FrameRimit()
{
	//Frame�O�ɏo���璆�ɖ߂�
	if (redX < 50)
	{
		redX = 50;
	}
	else if (redX > 350)
	{
		redX = 350;
	}

	if (redY < 210)
	{
		redY = 210;
	}
	else if (redY > 510)
	{
		redY = 510;
	}
}

void MiniGage::GetMouse()
{
	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	redXdir = mouseMove.lX * scaleY * 100;
	redYdir = mouseMove.lY * scaleX * 100;


	//�}�E�X�̈ړ����������W�ɉ��Z
	redX += redXdir;
	redY += redYdir;
}

void MiniGage::BlueSet()
{
	//�������W
	blueX = 80;
	blueY = 400;
}

void MiniGage::CollisonSphere()
{
	//�ɓ������Ă��������Ă���J�E���g��i��
	//����Ă��Ȃ�������O�ɖ߂�
	if (collision->Check2DCollisionSphere(redX,redY,redR,blueX,blueY,blueR))
	{
		OnTime++;
	}
	else
	{
		OnTime = 0;
	}
}

void MiniGage::OnTimeOver()
{
	//�V�[�h�l�ݒ�
	srand(time(NULL));

	//�ɓ������Ă��鎞�Ԃ��R�b�ȏ�Ȃ�
	//�����_���ɍ��W��ς���
	if (OnTime > 180)
	{
		blueX = rand() % 280 + 60;
		blueY = rand() % 280 + 220;
		OnTime = 0;
	}
}
