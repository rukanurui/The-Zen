#include "SceneAction.h"

SceneAction::SceneAction(int window_width, int window_height, Input* input)
{
	this->input = input;

	// 画面サイズに対する相対的なスケールに調整
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void SceneAction::Initialize()
{
}

void SceneAction::Update()
{
	//マウスの入力を取得
	MouseGet();

	//タイトルシーンアクション
	Title();
}

void SceneAction::MouseGet()
{

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	GetCursorPos(&p);

	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(hwnd, &p);

	//マウスの座標を代入
	mouseX = p.x;
	mouseY = p.y;
}

void SceneAction::Title()
{
	//シーンが0の時にここにカーソルを入れるとゲーム本編
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
