#include "SceneAction.h"

SceneAction::SceneAction(int window_width, int window_height, Input* input)
{
	this->input = input;

	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void SceneAction::Initialize()
{
}

void SceneAction::Update()
{
	//�}�E�X�̓��͂��擾
	MouseGet();

	//�^�C�g���V�[���A�N�V����
	Title();
}

void SceneAction::MouseGet()
{

	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	GetCursorPos(&p);

	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(hwnd, &p);

	//�}�E�X�̍��W����
	mouseX = p.x;
	mouseY = p.y;
}

void SceneAction::Title()
{
	//�V�[����0�̎��ɂ����ɃJ�[�\��������ƃQ�[���{��
	if (scene == 0)
	{
		if (mouseX >= 425 && mouseX <= 855 && mouseY >= 480 && mouseY <= 720)
		{
			if (input->TriggerMouseLeft())
			{
				scene = 1;
			}
		}
	}

	
}
