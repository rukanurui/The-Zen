#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include "Mesh.h"

#include<fbxsdk.h>

struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//��]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//�ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//�ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};

class Model
{

public:
	//�ǂݍ���
	static Model* LoadFromOBJ(const std::string& modelname);

	friend class FbxLoader;

	static const int MAX_BONE_INDICES = 4;

	FbxScene* fbxScene = nullptr;

	FbxScene* GetFbxScene() { return fbxScene; }

	~Model();
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	std::string name;

	std::vector<Node>nodes;

	// DirectX::����
	
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMettadata=DirectX::TexMetadata;
	using ScratchImage=DirectX::ScratchImage;
	
	using string = std::string;
//	template<class T> using vector =
		template <class T> using vector = std::vector<T>;



public: // �T�u�N���X
// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT3 normal; // �@���x�N�g��
		DirectX::XMFLOAT2 uv;  // uv���W
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};

	//�}�e���A��
	struct Material
	{
		std::string name;//�}�e���A����
		DirectX::XMFLOAT3 ambient;//�A���r�G���g�e���x
		DirectX::XMFLOAT3 diffuse;//�f�B�t���[�Y�e���x
		DirectX::XMFLOAT3 specular;//�X�y�L�����[�e���x
		float alpha;//��
		std::string textureFilename;//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	//�}�e���A��
	 Material material;

	 //�萔�o�b�t�@�p�f�[�^�\����B�P
	 struct ConstBufferDataB1
	 {
		 DirectX::XMFLOAT3 ambient;//�A���r�G���g�W��
		 float pad1;//�p�f�B���O
		 DirectX::XMFLOAT3 diffuse;//�f�B�t���[�Y�W��
		 float pad2;//�p�f�B���O
		 DirectX::XMFLOAT3 specular;//�X�؃L�����[�W��
		 float alpha;//��
	 };


	 //�m�[�h
	 /*
	 struct Node
	 {
		 //���O
		 std::string name;
		 //���[�J���X�P�[��
		 DirectX::XMVECTOR scaling = { 1,1,1,0 };
		 //��]�p
		 DirectX::XMVECTOR rotation = { 0,0,0,0 };
		 //�ړ�
		 DirectX::XMVECTOR translation = { 0,0,0,1 };
		 //�ό`�s��
		 DirectX::XMMATRIX transform;
		 //�O���[�o���ό`�s��
		 DirectX::XMMATRIX globalTransform;
		 //�e�m�[�h
		 Node* parent = nullptr;
	 };
	 */
/*
	 struct VetexPosNormalUvSkin
	 {
		 DirectX::XMFLOAT3 pos;
		 DirectX::XMFLOAT3 normal;
		 DirectX::XMFLOAT2 uv;
		 UINT boneIndex[MAX_BONE_INDICES];//�ԍ�
		 float boneWeight[MAX_BONE_INDICES];//�d��
	 };
	 */
	 //�{�[���\����
	 struct Bone
	 {
		 //���O
		 std::string name;
		 //�����p���̋t
		 DirectX::XMMATRIX invInitialPose;
		 //�N���X�^�[
		 FbxCluster* fbxCluster;
		 //�R���X�g���N�^
		 Bone(const std::string& name)
		 {
			 this->name = name;
		 }
	 };

	 //��ssh�������m�[�h
	 Node* meshNode = nullptr;
	 //���_�f�[�^�z��
	 std::vector<VertexPosNormalUv>vertices;
	 //���_�C���f�b�N�X�z��
	 std::vector<unsigned short>indices;
public:


	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <returns>����</returns>
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	 static ID3D12Device* device;

	static void SetDevice(ID3D12Device* device) { Model::device = device; }

	//bool InitializeDescriptorHeap();

	//void CreateBuffers();

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	/// <returns></returns>
	bool InitializeDescriptorHeap();

	void Draw3(ID3D12GraphicsCommandList* cmdList);

	void CreateBuffers2(ID3D12Device* device);

	void Draw2(ID3D12GraphicsCommandList* cmdList);

	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

    //inline  const std::vector<Mesh>& GetMeshes() { return  ;}

	
	//std::vector<unsigned short>indices;

	// ���_�f�[�^�z��
	//std::vector<VertexPosNormalUv> vertices;
	//�{�[��
	std::vector<Bone> bones;

	//getter
	std::vector<Bone>& GetBones() { return bones; }

	/// <summary>
/// ���_�z����擾
/// </summary>
/// <returns>���_�z��</returns>
	inline const std::vector<VertexPosNormalUv>& GetVertices() { return vertices; }

	/// <summary>
	/// �C���f�b�N�X�z����擾
	/// </summary>
	/// <returns>�C���f�b�N�X�z��</returns>
	inline const std::vector<unsigned short>& GetIndices() { return indices; }

private://�����o�ϐ�
	
	
	//ID3D12Device* device;
	
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;

	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	//�f�B�q���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	//�e�N�X�`���f�[�^
	DirectX::TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};

	//����J
	void LoadFromOBJInternal(const std::string& modelname);

	
	
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1; // �萔�o�b�t�@

	

	//public:

		
	
};


