#include <vector>
#include <string>
#include <fstream>
#include <DirectXMath.h>



#include <DirectXTex.h>
#include <wrl.h>

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

#include "Object3d.h"
#include "Model.h"
#include"SpriteCommon.h"
#include"Sprite.h"
using namespace DirectX;
using namespace Microsoft::WRL;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region WindowsAPI初期化
	WinApp* winApp = nullptr;

	winApp = new WinApp();
	winApp->Initialize();

	MSG msg{};  // メッセージ
#pragma endregion WindowsAPI初期化

#pragma region DirectX初期化処理

	DirectXCommon* dxCommon = nullptr;

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	Input* input = nullptr;
	input = new Input();
	input->Initialize(winApp);

	Object3d::StaticInitialize(dxCommon->GetDev(), winApp->window_width, winApp->window_height);

#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

	SpriteCommon* spriteCommon = new SpriteCommon();
	spriteCommon->initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);
	
	spriteCommon->LoadTexture(0, L"Resources/texture.png");
	spriteCommon->LoadTexture(1, L"Resources/house.png");

	Sprite* sprite = new Sprite();
	sprite->initialize(spriteCommon, 0);

	Model* modelPost = Model::LoadFromOBJ("posuto");
	Model* modelChr = Model::LoadFromOBJ("chr_sword");

	Object3d* objPost = Object3d::Create();
	Object3d* objChr = Object3d::Create();


	objPost->SetModel(modelChr);
	objChr->SetModel(modelPost);

	objPost->SetPosition({ -10,30,-20 });
	objChr->SetPosition({ +10,0,+5 });

	objPost->Update();
	objChr->Update();

#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター
	XMFLOAT3 pPos = { -10,30,-20 };
	//int flag = 0;
	float G = 9.80665f;
	float time = 0;

	while (true)  // ゲームループ
	{
#pragma region ウィンドウメッセージ処理
		if (winApp->ProcessMessage()) {
			break;
		}
#pragma endregion ウィンドウメッセージ処理

#pragma region DirectX毎フレーム処理
		// DirectX毎フレーム処理　ここから

		input->Update();

		const int cycle = 540; // 繰り返しの周期
		counter++;
		counter %= cycle; // 周期を超えたら0に戻る
		float scale = (float)counter / cycle; // [0,1]の数値
		time+=0.05f;
		scale *= 360.0f;
		
		if (input->TriggerKey(DIK_0)) // 数字の0キーが押されていたら
		{
			OutputDebugStringA("Hit 0\n");  // 出力ウィンドウに「Hit 0」と表示

		}

		float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色

		if (input->PushKey(DIK_SPACE))     // スペースキーが押されていたら
		{
			// 画面クリアカラーの数値を書き換える
			clearColor[1] = 1.0f;
			pPos = { -10,30,-20 };
			time = 0;
			//objPost->SetModel(modelChr);
		//	objChr->SetModel(modelChr);
		}
		pPos.y -= (1.0f / 2.0f) * G * (time * time);
		// 座標操作
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{

		}


		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{

		}
		objPost->SetPosition(pPos);

		objPost->Update();
		objChr->Update();
		// DirectX毎フレーム処理　ここまで
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド

		dxCommon->PreDraw();

		Object3d::PreDraw(dxCommon->GetCmdList());
		objPost->Draw();
	//	objChr->Draw();
		Object3d::PostDraw();

		spriteCommon->PreDraw();

		// ４．描画コマンドここまで
		dxCommon->PostDraw();


	}
	// XAudio2解放
   // xAudio2.Reset();
	// 音声データ解放
   // SoundUnload(&soundData1);

	
#pragma region WindowsAPI後始末
	winApp->Finalize();
#pragma endregion WindowsAPI後始末
	delete input;
	delete winApp;
	delete spriteCommon;
	delete modelPost;
	delete modelChr;
	delete objChr;
	delete objPost;
	return 0;
}