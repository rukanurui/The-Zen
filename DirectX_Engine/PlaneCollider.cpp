#include "PlaneCollider.h"

using namespace DirectX;

void PlaneCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const XMMATRIX& matWorld = fbx3d->GetMatWorld();

	// ���ʂ̃����o�ϐ����X�V
	Plane::normal = matWorld.r[3] + offset;
    Plane::distance = distance;
}
