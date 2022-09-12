#include "MiniGage.h"
#include <time.h>

MiniGage::MiniGage(int window_width, int window_height,Input* input,Collision* collision)
{
	this->input = input;
	// 画面サイズに対する相対的なスケールに調整
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void MiniGage::Initialize()
{
	//初期化
	BlueSet();
}

void MiniGage::Update()
{
	//時間を増加
	TimeCount();

	//ランダムの移動方向を生成
	AutoMove();

	//マウスの座標から変化量を取得
	GetMouse();

	//Frma外に出たときの処理
	FrameRimit();

	//赤と青の当たり判定
	CollisonSphere();

	//青に３秒乗ってられたら次に移動
	OnTimeOver();
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

	//ランダムで変化する移動方向を加算
	redX += AutoX;
	redY += AutoY;

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

void MiniGage::FrameRimit()
{
	//Frame外に出たら中に戻す
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
	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	redXdir = mouseMove.lX * scaleY * 100;
	redYdir = mouseMove.lY * scaleX * 100;


	//マウスの移動方向を座標に加算
	redX += redXdir;
	redY += redYdir;
}

void MiniGage::BlueSet()
{
	//初期青座標
	blueX = 80;
	blueY = 400;
}

void MiniGage::CollisonSphere()
{
	//青に当たっていたら乗っているカウントを進め
	//乗っていなかったら０に戻す
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
	//シード値設定
	srand(time(NULL));

	//青に当たっている時間が３秒以上なら
	//ランダムに座標を変える
	if (OnTime > 180)
	{
		blueX = rand() % 280 + 60;
		blueY = rand() % 280 + 220;
		OnTime = 0;
	}
}
