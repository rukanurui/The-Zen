#include "BoxCollider.h"
using namespace DirectX;

void BoxCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const XMMATRIX& matWorld = fbx3d->GetMatWorld();

	// ���̃����o�ϐ����X�V
	Box::minPos = matWorld.r[3] - offset;
	Box::maxPos = matWorld.r[3] + offset;
}
