#include "DebugCamera.h"
#include "Fbx3d.h"
#include <cassert>

using namespace DirectX;

DebugCamera::DebugCamera(int window_width, int window_height, Input* input)
	: Camera(window_width, window_height)
{
	assert(input);

	this->input = input;
	// 画面サイズに対する相対的なスケールに調整
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void DebugCamera::Update()
{
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	//sceneが0の時（タイトルの時のカメラワーク）
	if (scene==0)
	{
		float dx = 0;
		float dy = 0;
	    dy+= 1.5f * scaleY;

		angleX = 0;
		angleY = -dy * XM_PI;
		Allcamerax += angleY;
		dirty = true;
	}

	//タイトルー＞本編のカメラワーク
	if (scene == 1)
	{

		if (distance >= 18.0197)
		{
			distance -= 6.5f / 100.0f;
		}
		distance = max(distance, 1.0f);

		float dx = 0;
		float dy = 0;

		if (distance >= 18.0197)
		{
			dy += 1.5f * scaleY;
		}
		angleX = 0;
		angleY = -dy * XM_PI;
		Allcamerax += angleY;

	
		dirty = true;
	}

	// ホイール入力で距離を変更
	if (mouseMove.lZ != 0) {

		distance -= mouseMove.lZ / 100.0f;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (dirty ||viewDirty) {
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);
		// 累積の回転行列を合成
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい
		matRot = matRotNew * matRot;

		// 注視点から視点へのベクトルと、上方向ベクトル
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// ベクトルを回転
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// 注視点からずらした位置に視点座標を決定
		const XMFLOAT3& target = GetTarget();

		SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
		SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });
	}

	Camera::Update();
}

