#include <stdio.h>
#include <stdlib.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <cstdlib>      // srand,rand
#include <time.h>
using namespace DirectX;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include<DirectXTex.h>
#include <wrl.h>

using namespace Microsoft::WRL;

#include<d3dx12.h>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

#include "3d/Object3d.h"
#include"Model.h"


#include "Collision.h"
#include "CollisionPrimitive.h"


#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>

#include "WinApp.h"
#include "Audio.h"
#include"SpriteCommon.h"
#include "Sprite.h"
#include "DebugText.h"

//#include "FbxLoader.h"
#include "3d/FbxLoader.h"
#include "Fbx3d.h"
#include "Camera.h"
#include "DebugCamera.h"
//#include "fbxsdk.h"
#include <vector>

#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "BoxCollider.h"
#include "CollisionManager.h"

#include "MiniGage.h"

Model* modelPlane = nullptr;
Model* modelBox = nullptr;
Model* modelPyramid = nullptr;

std::vector<Object3d*> objects;


Sphere sphere;

Plane plane;

Triangle triangle;


//
DirectXCommon* dxCommon = nullptr;
SpriteCommon* spriteCommon = new SpriteCommon();
SpriteCommon* spriteCommon2= new SpriteCommon();

class CollisionManager;
class Bullet;

//ComPtr<ID3D12Device> dev;

LRESULT CALLBACK WindowProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


#ifdef _DEBUG
#   define MyOutputDebugString( str, ... ) \
      { \
        TCHAR c[256]; \
        _stprintf( c, str, __VA_ARGS__ ); \
        OutputDebugString( c ); \
      }
#else
#    define MyOutputDebugString( str, ... ) // 空実装
#endif



//カメラ
DebugCamera* camera = nullptr;
//# Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    //FbxManager* fbxManager = FbxManager::Create();

    //ポインタ置き場
    Input* input = nullptr;
    WinApp* winApp = nullptr;
    Audio* audio = nullptr;
    GamePad* gamepad = nullptr;
    //WindowsAPIの初期化
    winApp = new WinApp();
    winApp->Initialize();

    gamepad = new GamePad();
   
// DirectX初期化処理　ここから

    ComPtr<IXAudio2> xAudio2;
    IXAudio2MasteringVoice* masterVoice;

    //DirectXの初期化
    dxCommon = new DirectXCommon();
    dxCommon->Initialize(winApp);
    HRESULT result;
     


     input = new Input();
    //入力の初期化
    input->Intialize(winApp);


 
    camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);

    MiniGage* gage = nullptr;
    gage = new MiniGage(WinApp::window_width, WinApp::window_height, input);

    //3D初期化
   Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);


    //読み込み

    //生成

    //ひもづけ

    //位置指定

    result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

    //マスターボイスを作成 
    result = xAudio2->CreateMasteringVoice(&masterVoice);
    
    //スプライト初期化
  
   spriteCommon->Initialize(dxCommon->GetDev(),dxCommon->GetCmdList(),winApp->window_width,winApp->window_height);
   spriteCommon->Initialize_Post(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);



    //ポストエフェクト用
  //Sprite*sprite=Sprite::Create(100, "Resources/Red.png");
   Sprite* sprite100 = Sprite::PostCreate(spriteCommon, 100);
   spriteCommon->SpriteCommonLoadTexture(100, L"Resources/Red.png");

    //スプライト
    Sprite* sprite = Sprite::Create(spriteCommon,0);

   spriteCommon-> SpriteCommonLoadTexture(0, L"Resources/Red.png"); 
   sprite->SetPosition({ 640,362.5,0 });
   sprite->SetSize({ 20,20 });
   sprite->SettexSize({ 70,70 });

 

    //デバックテキスト
    DebugText* debugtext=nullptr;
    debugtext = new DebugText();

    DebugText* debugtext2 = nullptr;
    debugtext2 = new DebugText();


    const int debugTextTexNumber = 2;

    spriteCommon->SpriteCommonLoadTexture(debugTextTexNumber,L"Resources/ASC_white1280.png");
    //SpriteCommonLoadTexture(spriteCommon, debugTextTexNumber, L"Resources/ASC_white1280.png", dxCommon->GetDev());

    debugtext->debugTextInit(spriteCommon,debugTextTexNumber);
    debugtext2->debugTextInit(spriteCommon, debugTextTexNumber);



    // DirectX初期化処理　ここまで

    FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
 
    //3D初期値設定

    //オーディオ初期化
    audio = new Audio();
    audio->Initialize();

 //   audio->SoundLoadWave("Resources/Shot.wav");//正解の音
  
    //パイプライン生成
 
    BYTE key[256] = {};
    BYTE olds[256] = {};

    int debug_x = 600;
    int debug_y = 80;

    int debug2_x = 300;
    int debug2_y = 24;

    //ゲームシーン処理関連

    int GameScene = 0;

    char moji[64];
    char moji2[64];

   Model* model1 = nullptr;
   Fbx3d* fbx3d1 = nullptr;

 

   model1 = FbxLoader::GetInstance()->LoadModelFromFile("zazen");
   


   //モデル読み込み
   Fbx3d::SetDevice(dxCommon->GetDev());

   Fbx3d::SetCamera(camera);

   Fbx3d::CreateGraphicsPipeline();

 
  //3Dオブジェクト生成とモデルのセット
  fbx3d1= new Fbx3d(input);
  fbx3d1->Initialize();
  fbx3d1->SetModel(model1);
  

  fbx3d1->SetPosition({ 0, -10, 0 });
  fbx3d1->SetRotate({ 0,0,0 });
  fbx3d1->SetScale({ 0.05,0.05,0.05 });


  //衝突マネージャー
  CollisionManager* collisionManager = nullptr;
  collisionManager = CollisionManager::GetInstance();

  float radius = 5.0f;


  XMFLOAT2 GageOrigin = { 200,360 };
 
    while (true)  // ゲームループ
    {
     //3d更新   
     //スプライト

    sprintf_s(moji, "X=%f", gage->GetredXdir());
    sprintf_s(moji2, "Y=%f", gage->GetredYdir());
    //sprintf_s(moji2, "camera=%f", camera->GetPositionY());
    //sprintf_s(moji2,"%d",camera->GetAngleY());
     
        if (winApp->ProcessMessage())
        {
            //ゲームループを抜ける
            break;
        }

        // DirectX毎フレーム処理　ここから
      
    
        //初期化処理

        //ゲーム内の動作  
        gage->Update();
        //更新
        input->Update();

        fbx3d1->Update();
        camera->Update();
      
        //レンダ―テクスチャの描画
        sprite100->PreDrawScene(dxCommon->GetCmdList());
        ////スプライト共通コマンド
        spriteCommon->PreDraw();
       
        //FBX描画
      //  fbx3d1->();
        fbx3d1->Draw2(dxCommon->GetCmdList());
        //ポストエフェクトここまで
        sprite100->PostDrawScene(dxCommon->GetCmdList());

        //バックバッファの番号を取得（2つなので0番か1番）
        dxCommon->PreDraw();


        spriteCommon->PreDraw_Post();
        sprite100->Update();
        sprite100->PostDraw();
        
        //3D描画前処理
        //Object3d::PreDraw(dxCommon->GetCmdList());
        //3D描画
        //ここに処理追加できる
      
        //3D描画後処理
        //Object3d::PostDraw();
       

        ////スプライト共通コマンド
        spriteCommon->PreDraw();
        debugtext->Print(moji, 100, 100);
        debugtext->DrawAll();//的カウント

        debugtext2->Print(moji2, 100, 200);
        debugtext2->DrawAll();//的カウント

        sprite->SetPosition({ GageOrigin.x+gage->GetredXdir(),GageOrigin.y+gage->GetredYdir(),0 });
        sprite->SpriteTransVertexBuffer();
        sprite->Update();
        sprite->SpriteDraw();
        // ４．描画コマンドここまで
        collisionManager->CheckAllCollisions();
        // DirectX毎フレーム処理　ここまで
        dxCommon->PostDraw();
    }
    //3D解放

    //入力開放
    delete input;

    //DirectX解放
    delete dxCommon;


    winApp->Finalize();

    //WindowsAPI解放
    delete winApp;
    winApp = nullptr;

   //3Dモデル解放

    //スプライト
    delete spriteCommon;

  //  delete sprite;

    //テキスト解放
    delete debugtext;

    audio->Finalize();
    delete audio;

   // delete fbx3d1;
   // delete model1;

    FbxLoader::GetInstance()->Finalize();


    for (auto object : objects) {
        delete(object);
    }

    delete(modelPlane);
    delete(modelBox);
    delete(modelPyramid);

    return 0;
}
