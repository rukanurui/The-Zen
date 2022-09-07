#pragma once
#include "PipeLineSet.h"
#include <DirectXMath.h>

class SpriteCommon
{
public:
	//�X�v���C�g�p�e�N�X�`������
	static const int spriteSRVCount = 512;

	//������
	void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

	//������
	void Initialize_Post(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);


	void PreDraw();

	void PreDraw_Post();
	//�ǂݍ���
	void SpriteCommonLoadTexture(UINT texnumber, const wchar_t* filename);

	//�擾
	ID3D12Resource* GetTexBuff(int texNumber);

	ID3D12Device* GetDevice() { return device_; }

	const DirectX::XMMATRIX& GetMatProjection() { return matProjection_; }

	ID3D12GraphicsCommandList* GetcmdList() { return cmdList_; }

	void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texnumber);

	void SetGraphicsRootDescriptorTable_Post(UINT rootParameterIndex, UINT texnumber);

	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState_Post;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature_Post;



	void CreateGraphicsPipelineState_Post();


	//�p�C�v���C���Z�b�g
	PipeLineSet pipelineSet_;


private:
	void CreateSprite2dpipe();

	void CreateSprite2dpipe_Post();


protected:

private:

	//�e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_[spriteSRVCount];

	//�ˉe�s��
	DirectX::XMMATRIX matProjection_{};

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap_;

	ID3D12Device* device_ = nullptr;
	//�R�}���h��
	ID3D12GraphicsCommandList* cmdList_ = nullptr;
};
