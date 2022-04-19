#pragma once
#include <Windows.h>

class WinApp
{
public://静的メンバ関数
	static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public://メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();
	//終了
	void Finalize();
	//メッセージ処理
	bool ProcessMessage();
	
	//getter
	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetHInstance() { return w.hInstance; }


public://静的メンバ変数

	// ウィンドウサイズ
	static const int window_width = 1280;  // 横幅
	static const int window_height = 720;  // 縦幅

private://メンバ変数
	HWND hwnd = nullptr;
	WNDCLASSEX w{}; // ウィンドウクラスの設定
};

