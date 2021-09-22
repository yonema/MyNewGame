#pragma once

#include "HID/GamePad.h"

class GraphicsEngine;

class TkEngine {
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~TkEngine();
	/// <summary>
	/// フレームの開始時に呼ばれる処理。
	/// </summary>
	void BeginFrame();
	/// <summary>
	/// フレームの終了時に呼ばれる処理。
	/// </summary>
	void EndFrame();
	/// <summary>
	/// ゲームエンジンの初期化。
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
private:
	GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。
	// 自分で用意した方を使う。
	//GameTime m_gameTime;							//ゲームタイム。

	// 追加
public:

	/**
	 * @brief tkEngineの初期化
	*/
	void Init();
	
};

//extern TkEngine* g_engine;	//TKエンジン。
//extern GameTime* g_gameTime;