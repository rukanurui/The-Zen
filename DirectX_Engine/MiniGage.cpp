#include "MiniGage.h"
#include <time.h>

MiniGage::MiniGage(int window_width, int window_height,Input* input)
{
	this->input = input;
	// 画面サイズに対する相対的なスケールに調整
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void MiniGage::Initialize()
{
	//初期化

}

void MiniGage::Update()
{
	
	//時間を増加
	TimeCount();

	//ランダムの移動方向を生成
	AutoMove();

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	redXdir = mouseMove.lX * scaleY*100;
	redYdir = mouseMove.lY * scaleX*100;

	
	//マウスの移動方向を座標に加算
	redX += redXdir;
	redY += redYdir;

	//ランダムで変化する移動方向を加算
	redX += AutoX;
	redY += AutoY;
}

void MiniGage::AutoMove()
{
	//シード値設定
	srand(time(NULL));

	//-+を決める
	RandX = (rand() % 2)+1;
	RandY = (rand() % 2)+1;

	//方向の大きさを決める
	AutoXdir = (rand() % 2 * (Time/2))+1;
	AutoYdir = (rand() % 2 * (Time/2))+1;

	//1なら-２なら＋で割る10する
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
	//方向を代入
	AutoX = AutoXdir;
	AutoY = AutoYdir;
}

void MiniGage::TimeCount()
{
	//１秒60フレームなのでSecondが60に
	//なったらTimeを1増加
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
