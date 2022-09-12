#include "Sprite.h"
#include <d3dx12.h>
#include "WinApp.h"
#include "Input.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

using namespace::DirectX;

Sprite* Sprite::Create(SpriteCommon* spriteCommon, UINT texnumber, DirectX::XMFLOAT2 anchorpoint, bool isFlagX, bool isFlagY)
{
    Sprite* instance = new Sprite();

    instance->Initialize(spriteCommon, texnumber,
        anchorpoint, isFlagX, isFlagY);

    return instance;
}

void Sprite::Initialize(SpriteCommon* spriteCommon, UINT texnumber,
    DirectX::XMFLOAT2 anchorpoint, bool isFlagX, bool isFlagY)
{
    HRESULT result = S_FALSE;


    //�A���J�[�|�C���g���R�s�[
    anchorpoint_ = anchorpoint;

    //���]�t���O���R�s�[
    isFlagX_ = isFlagX;

    isFlagY_ = isFlagY;

    spriteCommon_ = spriteCommon;

    VertexPosUv vertices[] = {
        {{0.0f	,100.0f	,0.0f},{0.0f,1.0f} },
        {{0.0f	,0.0f	,0.0f},{0.0f,0.0f} },
        {{100.0f,100.0f	,0.0f},{1.0f,1.0f} },
        {{100.0f,0.0f	,0.0f},{1.0f,0.0f} },


    };




    //���_�o�b�t�@�̐���
    result = spriteCommon->GetDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff_)
    );

    texnumber_ = texnumber;

    //   if (spriteCommon_->texBuff_[sprite.texnumber])]

    if (spriteCommon_->GetTexBuff(texnumber_))
    {
        //�e�N�X�`�����̉摜���ǂݍ��ݍς݂Ȃ�
        D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetTexBuff(texnumber_)->GetDesc();

        //�X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
        size_ = { (float)resDesc.Width,(float)resDesc.Height };
        //  texSize = { (float)resDesc.Width,(float)resDesc.Height };
    }

    //���_�o�b�t�@�f�[�^�]��
    SpriteTransVertexBuffer();




    //���_�o�b�t�@�r���[�ւ̃f�[�^�]��
    VertexPosUv* vertMap = nullptr;
    result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
    memcpy(vertMap, vertices, sizeof(vertices));
    vertBuff_->Unmap(0, nullptr);

    //���_�o�b�t�@�r���[�̐���
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = sizeof(vertices);
    vbView_.StrideInBytes = sizeof(vertices[0]);

    //�萔�o�b�t�@�̐���
    result = spriteCommon_->GetDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData8) + 0xff) & ~0xff),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuff_));

    //�萔�o�b�t�@�Ƀf�[�^��]��
    ConstBufferData8* constMap = nullptr;
    result = constBuff_->Map(0, nullptr, (void**)&constMap);
    constMap->color8 = XMFLOAT4(1, 1, 1, 1);//�F�w��(R G B A)
    constMap->a8 = 0.0f;
    constMap->x8 = 0.0f;
    constMap->y8 = 0.0f;
    constMap->span8 = 0.0f;
    constMap->frame8 = 0.0f;
    constMap->scale8 = 0.0f;


    //���s���e�@
    constMap->mat8 = spriteCommon_->GetMatProjection();
    constBuff_->Unmap(0, nullptr);

}

void Sprite::SpriteTransVertexBuffer()
{
    HRESULT result = S_FALSE;

    VertexPosUv vertices[] = {
        {{},{} },
        {{},{} },
        {{},{} },
        {{},{} },

    };

    //	 ���� ���� �E�� �E��
    enum { LB, LT, RB, RT };

    float left = (0.0f - anchorpoint_.x) * size_.x;

    float right = (1.0f - anchorpoint_.x) * size_.x;

    float top = (0.0f - anchorpoint_.y) * size_.y;

    float bottom = (1.0f - anchorpoint_.y) * size_.y;

    if (isFlagX_ == true)
    {
        left = -left;
        right = -right;
    }

    if (isFlagY_ == true)
    {
        top = -top;
        bottom = -bottom;
    }

    vertices[LB].pos = { left,	bottom, 0.0f };
    vertices[LT].pos = { left,	top,	0.0f };
    vertices[RB].pos = { right,	bottom, 0.0f };
    vertices[RT].pos = { right,	top,	0.0f };

    //UV
    //�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
    if (spriteCommon_->GetTexBuff(texnumber_))
    {
        //�e�N�X�`�����擾
     //   D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[sprite.texnumber]->GetDesc();
        D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetTexBuff(texnumber_)->GetDesc();

        float tex_left = texLeftTop_.x / resDesc.Width;

        float tex_right = (texLeftTop_.x + texSize_.x) / resDesc.Width;

        float tex_top = texLeftTop_.y / resDesc.Height;

        float tex_bottom = (texLeftTop_.y + texSize_.y) / resDesc.Height;

        vertices[LB].uv = { tex_left,	tex_bottom };
        vertices[LT].uv = { tex_left,	tex_top };
        vertices[RB].uv = { tex_right,	tex_bottom };
        vertices[RT].uv = { tex_right,	tex_top };

    }

    //���_�o�b�t�@�ւ̃f�[�^�]��
    VertexPosUv* vertMap = nullptr;
    result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
    memcpy(vertMap, vertices, sizeof(vertices));
    vertBuff_->Unmap(0, nullptr);
}

void Sprite::SpriteDraw()
{
    if (isInvisible_ == true)
    {
        return;
    }
    ID3D12GraphicsCommandList* cmdList = spriteCommon_->GetcmdList();

    //���_�o�b�t�@���Z�b�g
    cmdList->IASetVertexBuffers(0, 1, &vbView_);

    //�萔�o�b�t�@���Z�b�g
    cmdList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

    spriteCommon_->SetGraphicsRootDescriptorTable(1, texnumber_);
    //
    //cmdList->SetGraphicsRootDescriptorTable(1,
       // CD3DX12_GPU_DESCRIPTOR_HANDLE(spriteCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
       //     texnumber_,
       //     spriteCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

    //�|���S���̕`��
    cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::Update()
{
    //���[���h�s��̍X�V
    matWorld_ = XMMatrixIdentity();

    //X����]
  //  matWorld_ *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));

    //Y����]
   // matWorld_ *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));

    //Z����]
    matWorld_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));




    //���s�ړ�
    matWorld_ *= XMMatrixTranslation(position_.x, position_.y, position_.z);

    //�萔�o�b�t�@�̓]��
    ConstBufferData8* constMap = nullptr;
    HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
    constMap->color8 = color_;
    constMap->mat8 = matWorld_ * spriteCommon_->GetMatProjection();

    /*
    if (constMap->Time8 < 2.0f)
    {
        constMap->Time8 += 0.02f;
    }
    else
    {
        constMap->Time8 = 0.1f;
    }
    */

 
    constBuff_->Unmap(0, nullptr);
}

void Sprite::SpriteDivDraw(float& DIVnum, float Divsize, float& sizex, float sizey, float& count)
{
    count++;
    if (count >= 100)
    {
        DIVnum++;
        SettexLeftTop({ 0 + Divsize * DIVnum,0 });
        SettexSize({ sizex,sizey });
        count = 0;
        SpriteTransVertexBuffer();
    }
    if (DIVnum == 4)
    {
        DIVnum = 0;
    }
}

const float Sprite::clearColor[4] = { 1.0f,1.0f,1.01f,0.0f };

Sprite* Sprite::PostCreate(SpriteCommon* spriteCommon, UINT texnumber, DirectX::XMFLOAT2 anchorpoint, bool isFlagX, bool isFlagY)
{
    Sprite* instance = new Sprite();

    instance->PostInitialize(spriteCommon, texnumber,
        anchorpoint, isFlagX, isFlagY);

    return instance;
}

void Sprite::PostInitialize(SpriteCommon* spriteCommon, UINT texnumber,
    DirectX::XMFLOAT2 anchorpoint, bool isFlagX, bool isFlagY)
{
    HRESULT result;



    //�A���J�[�|�C���g���R�s�[
    anchorpoint_ = anchorpoint;

    //���]�t���O���R�s�[
    isFlagX_ = isFlagX;

    isFlagY_ = isFlagY;

    spriteCommon_ = spriteCommon;

    VertexPosUv vertices[] = {
        {{0.0f	,720.0f	,0.0f},{0.0f,1.0f} },
        {{0.0f	,0.0f	,0.0f},{0.0f,0.0f} },
        {{1280.0f,720.0f,0.0f},{1.0f,1.0f} },
        {{1280.0f,0.0f	,0.0f},{1.0f,0.0f} },


    };




    //���_�o�b�t�@�̐���
    result = spriteCommon->GetDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff_)
    );

    texnumber_ = texnumber;

    //   if (spriteCommon_->texBuff_[sprite.texnumber])]

    if (spriteCommon_->GetTexBuff(texnumber_))
    {
        //�e�N�X�`�����̉摜���ǂݍ��ݍς݂Ȃ�
        D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetTexBuff(texnumber_)->GetDesc();

        //�X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
        size_ = { (float)resDesc.Width,(float)resDesc.Height };
        //  texSize = { (float)resDesc.Width,(float)resDesc.Height };
    }

    //���_�o�b�t�@�f�[�^�]��
    SpriteTransVertexBuffer();




    //���_�o�b�t�@�r���[�ւ̃f�[�^�]��
    VertexPosUv* vertMap = nullptr;
    result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
    memcpy(vertMap, vertices, sizeof(vertices));
    vertBuff_->Unmap(0, nullptr);

    //���_�o�b�t�@�r���[�̐���
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = sizeof(vertices);
    vbView_.StrideInBytes = sizeof(vertices[0]);

    //�萔�o�b�t�@�̐���
    result = spriteCommon_->GetDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData8) + 0xff) & ~0xff),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuff_));

    //�萔�o�b�t�@�Ƀf�[�^��]��
    ConstBufferData8* constMap = nullptr;
    result = constBuff_->Map(0, nullptr, (void**)&constMap);
    constMap->color8 = XMFLOAT4(1, 1, 1, 1);//�F�w��(R G B A)
    constMap->a8 = 0.0f;
    constMap->x8 = 0.0f;
    constMap->y8 = 0.0f;
    constMap->span8 = 0.0f;
    constMap->frame8 = 0.0f;
    constMap->scale8 = 0.0f;


    //���s���e�@
    constMap->mat8 = spriteCommon_->GetMatProjection();
    constBuff_->Unmap(0, nullptr);



    CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        DXGI_FORMAT_R8G8B8A8_UNORM,
        WinApp::window_width,
        (UINT)WinApp::window_height,
        1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
    );

    for (int i = 0; i < 2; i++)
    {
        //�e�N�X�`���o�b�t�@����
        result = spriteCommon->GetDevice()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
                D3D12_MEMORY_POOL_L0),
            D3D12_HEAP_FLAG_NONE,
            &texresDesc,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
            IID_PPV_ARGS(&texBuff[i])
        );


        const UINT pixelCount = WinApp::window_width * WinApp::window_height;

        const UINT rowPitch = sizeof(UINT) * WinApp::window_width;

        const UINT depthPitch = rowPitch * WinApp::window_height;

        UINT* img = new UINT[pixelCount];
        for (int j = 0; j < pixelCount; j++) { img[j] = 0xff0000ff; }

        result = texBuff[i]->WriteToSubresource(0, nullptr,
            img, rowPitch, depthPitch);

        delete[] img;
    }
    D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
    srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    srvDescHeapDesc.NumDescriptors = 2;

    result = spriteCommon->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;


    for (int i = 0; i < 2; i++)
    {
        spriteCommon->GetDevice()->CreateShaderResourceView(texBuff[i].Get(),
            &srvDesc,
            CD3DX12_CPU_DESCRIPTOR_HANDLE(
                descHeapSRV->GetCPUDescriptorHandleForHeapStart(), i,
                spriteCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    }


    //RTV
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
    rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDescHeapDesc.NumDescriptors = 2;

    result = spriteCommon->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));

    for (int i = 0; i < 2; i++)
    {

        spriteCommon->GetDevice()->CreateRenderTargetView(texBuff[i].Get(),
            nullptr,
            CD3DX12_CPU_DESCRIPTOR_HANDLE(
                descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
                spriteCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
    }

    //�[�x�o�b�t�@���\�[�X�ݒ�
    CD3DX12_RESOURCE_DESC depthResDesc =
        CD3DX12_RESOURCE_DESC::Tex2D(
            DXGI_FORMAT_D32_FLOAT,
            WinApp::window_width,
            WinApp::window_height,
            1, 0,
            1, 0,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
        );

    //�[�x�o�b�t�@����
    result = spriteCommon->GetDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depthResDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
        IID_PPV_ARGS(&depthBuff)
    );

    //DSV
    D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
    DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    DescHeapDesc.NumDescriptors = 1;

    result = spriteCommon->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));

    //�f�X�N���v�^�q�[�v��DSV�쐬
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    spriteCommon->GetDevice()->CreateDepthStencilView(depthBuff.Get(),
        &dsvDesc,
        descHeapDSV->GetCPUDescriptorHandleForHeapStart());


}

void Sprite::CreateGraphicsPipelineState_Post()
{
    /*
    HRESULT result = S_OK;



    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;// ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;//ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;//ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"Resources/shaders/SpriteVS.hlsl",  // �V�F�[�_�t�@�C����
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
        "main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
        0,
        &vsBlob, &errorBlob);


    if (FAILED(result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"Resources/shaders/PostPS.hlsl",   // �V�F�[�_�t�@�C����
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
        "main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
        0,
        &psBlob, &errorBlob);



    if (FAILED(result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    // ���_���C�A�E�g
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        }, // (1�s�ŏ������ق������₷��)


        {
            "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        }, // (1�s�ŏ������ق������₷��)
    };


    CD3DX12_DESCRIPTOR_RANGE  descRangeSRV;
    //descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);//b0 ���W�X�^
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//t0 ���W�X�^


    CD3DX12_ROOT_PARAMETER rootparams[2];
    rootparams[0].InitAsConstantBufferView(0);//�萔�o�b�t�@�r���[�Ƃ��ď�����(b0�@���W�X�^)
    rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

    // �O���t�B�b�N�X�p�C�v���C���ݒ�
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

    //���_�V�F�[�_�@�s�N�Z���V�F�[�_
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());


    //�u�����h
//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA�S�Ẵ`�����l����`��
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    //���ʐݒ�(BLEND)
    blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��

    //���Z����
    //blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	 //���Z
    //blenddesc.SrcBlend = D3D12_BLEND_ONE;	 //�\�[�X�̒l��100%�g��
    //blenddesc.DestBlend = D3D12_BLEND_ONE;	 //�f�X�g�̒l��100%�g��

    //����������
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    gpipeline.NumRenderTargets = 1; // �`��Ώۂ�1��
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
    gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

        //�W���I�Ȑݒ�(�|���S�����h��Ԃ� �[�x�N���b�s���O��L����)
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �w�ʂ��J�����O�Ȃ�

    //�f�v�X�X�e���V���X�e�[�g�̐ݒ�

    //�W���I�Ȑݒ�(�������݋���)
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//��U�W���l���Z�b�g
    gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//��ɏ㏑��
    gpipeline.DepthStencilState.DepthEnable = false;//�[�x�e�X�g���Ȃ�

    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

    //�f�X�N���v�^�e�[�u���̐ݒ�
    D3D12_DESCRIPTOR_RANGE descTblRange{};
    descTblRange.NumDescriptors = 1;
    descTblRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    descTblRange.BaseShaderRegister = 0;
    descTblRange.OffsetInDescriptorsFromTableStart =
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    //���[�g�p�����[�^
    D3D12_ROOT_PARAMETER rootparam = {};
    rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootparam.DescriptorTable.pDescriptorRanges = &descTblRange;
    rootparam.DescriptorTable.NumDescriptorRanges = 1;
    rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;



    //���[�g�V�O�l�`������
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;



    //���[�g�V�O�l�`���̐ݒ�
    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

    rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // PipelineSet pipelineSet;

    Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;

    result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
        &rootSigBlob, &errorBlob);

    result = spriteCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
        IID_PPV_ARGS(&pipelineSet_.rootsignature));

    gpipeline.pRootSignature = pipelineSet_.rootsignature.Get();

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;// ID3D12PipelineState* pipelinestate = nullptr;
    result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet_.pipelinestate));
    */

}

void Sprite::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
    // ID3D12GraphicsCommandList* cmdList = spriteCommon_->GetcmdList()
    for (int i = 0; i < 2; i++) {
        cmdList->ResourceBarrier(1,
            &CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
                D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                D3D12_RESOURCE_STATE_RENDER_TARGET));
    }


    D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2];
    for (int i = 0; i < 2; i++) {
        rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
            spriteCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
    }
    D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
        descHeapDSV->GetCPUDescriptorHandleForHeapStart();

    cmdList->OMSetRenderTargets(2, rtvHs, false, &dsvH);

    CD3DX12_VIEWPORT viewports[2];
    CD3DX12_RECT scissorRects[2];
    for (int i = 0; i < 2; i++)
    {
        viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);
        scissorRects[i] = CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height);
    }
    cmdList->RSSetViewports(2, viewports);

    cmdList->RSSetScissorRects(2, scissorRects);

    for (int i = 0; i < 2; i++) {
        cmdList->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
    }
    cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void Sprite::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
    for (int i = 0; i < 2; i++) {
        cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
    }
}

void Sprite::PostDraw()
{


    /*
    if (isInvisible_ == true)
    {
        return;
    }
    ID3D12GraphicsCommandList* cmdList = spriteCommon_->GetcmdList();

    //�p�C�v���C���X�e�[�g�̐ݒ�
   // cmdList->SetPipelineState(pipelineSet.pipelinestate.Get());

    //���[�g�V�O�l�`���̐ݒ�
   // /cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());

    //�v���~�e�B�u�`���ݒ�
    //cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //�e�N�X�`���p�f�X�N���v�^�q�[�v
    ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
    cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);



    //���_�o�b�t�@���Z�b�g
    cmdList->IASetVertexBuffers(0, 1, &vbView_);

    //�萔�o�b�t�@���Z�b�g
    cmdList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

    //spriteCommon_->SetGraphicsRootDescriptorTable(1, descHeapSRV);
    //
    //cmdList->SetGraphicsRootDescriptorTable(1,
       // CD3DX12_GPU_DESCRIPTOR_HANDLE(spriteCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
       //     texnumber_,
       //     spriteCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

    //�|���S���̕`��
    cmdList->DrawInstanced(4, 1, 0, 0);
    */

    // ���[���h�s��̍X�V
   // this->matWorld = XMMatrixIdentity();
   // this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
  //  this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

    // �萔�o�b�t�@�Ƀf�[�^�]��
  //  ConstBufferData* constMap = nullptr;
  //  HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
 //   if (SUCCEEDED(result)) {
 //       constMap->color = this->color;
 //       constMap->mat = this->matWorld * matProjection;	// �s��̍���	
 //       this->constBuff->Unmap(0, nullptr);
  //  }
//
    




    ID3D12GraphicsCommandList* cmdList = spriteCommon_->GetcmdList();


    //�p�C�v���C���X�e�[�g�̐ݒ�
   // cmdList->SetPipelineState(pipelineSet_.pipelinestate.Get());

    //���[�g�V�O�l�`���̐ݒ�
   // cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());

    //�v���~�e�B�u�`���ݒ�
   // cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


    // ���_�o�b�t�@�̐ݒ�
    cmdList->IASetVertexBuffers(0, 1, &this->vbView_);

    ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
    // �f�X�N���v�^�q�[�v���Z�b�g
    cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    // �萔�o�b�t�@�r���[���Z�b�g
    cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff_->GetGPUVirtualAddress());
    // �V�F�[�_���\�[�X�r���[���Z�b�g
   // cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber, descriptorHandleIncrementSize));
    cmdList->SetGraphicsRootDescriptorTable(1,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(
            descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 0,
            spriteCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
        ));



    cmdList->SetGraphicsRootDescriptorTable(2,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(
            descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1,
            spriteCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
        ));
    // �`��R�}���h
    cmdList->DrawInstanced(4, 1, 0, 0);
}
