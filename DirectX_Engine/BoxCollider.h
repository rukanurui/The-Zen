#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// ���Փ˔���I�u�W�F�N�g
/// </summary>
class BoxCollider : public BaseCollider, public Box
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMVECTOR = DirectX::XMVECTOR;
public:
	BoxCollider(XMVECTOR offset = { 0,0,0,0 }) :
		offset(offset)
	{
		// ���`����Z�b�g
		shapeType = COLLISIONSHAPE_BOX;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	inline void SetX(float x) { this->x = x; }
	inline void SetY(float y) { this->y = y; }
	inline void SetZ(float z) { this->z = z; }

private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	XMVECTOR offset;
	// ���S�_����̍ő�l�ŏ��l�Q��
    float x;
	float y;
	float z;
};

