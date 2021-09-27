#include "stdafx.h"
#include "RenderingEngine.h"
#include "system/system.h"
#include "Render.h"
#include "LightManager.h"

namespace nsMyGame
{

	/**
	 * @brief このゲーム用のゲームエンジンネームスペース
	*/
	namespace nsMyEngine
	{
		CRenderingEngine* CRenderingEngine::m_instance = nullptr;	// 唯一のインスタンス

		// レンダリングエンジンの定数データを使用可能にする
		using namespace nsRenderingEngineConstData;

		/**
		 * @brief コンストラクタ
		*/
		CRenderingEngine::CRenderingEngine()
		{
			// 描画できるオブジェクトのキャパシティを確保する
			m_renderObjects.reserve(m_kRenderObjectsCapacityNum);

			InitMainRenderTarget();	// メインレンダリングターゲットの初期化
			InitGBuffer();	// GBufferを初期化
			// メインレンダリングターゲットをフレームバッファにコピーするためのスプライトの初期化
			InitCopyMainRenderTargetToFrameBufferSprite();
			InitDefferdLightingSprite();	// ディファ―ドライティングを行うためのスプライトの初期化

			m_postEffect.Init(m_mainRenderTarget);	// ポストエフェクトの初期化

			return;
		}
		/**
		 * @brief デストラクタ
		*/
		CRenderingEngine::~CRenderingEngine()
		{
			return;
		}

		/**
		 * @brief メインレンダリングターゲットの初期化
		*/
		void CRenderingEngine::InitMainRenderTarget()
		{
			m_mainRenderTarget.Create(
				g_graphicsEngine->GetFrameBufferWidth(),
				g_graphicsEngine->GetFrameBufferHeight(),
				1,
				1,
				DXGI_FORMAT_R16G16B16A16_FLOAT,	// 16bit浮動小数点型
				DXGI_FORMAT_UNKNOWN
			);
			return;
		}

		/**
		 * @brief GBufferを初期化
		*/
		void CRenderingEngine::InitGBuffer()
		{
			int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
			int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

			// アルベドカラーを出力用のレンダリングターゲットを初期化する
			m_GBuffer[enGBufferAlbedoDepth].Create(
				frameBuffer_w,
				frameBuffer_h,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,	// 32bit浮動小数点型
				DXGI_FORMAT_D32_FLOAT
			);

			// 法線出力用のレンダリングターゲットを初期化する
			m_GBuffer[enGBufferNormal].Create(
				frameBuffer_w,
				frameBuffer_h,
				1,
				1,
				DXGI_FORMAT_R8G8B8A8_SNORM,	// 8bit符号付き正規化整数
				DXGI_FORMAT_UNKNOWN
			);


			// メタリック、影パラメータ、スムース出力用のレンダリングターゲットを初期化する    
			m_GBuffer[enGBufferMetaricShadowSmooth].Create(
				frameBuffer_w,
				frameBuffer_h,
				1,
				1,
				DXGI_FORMAT_R8G8B8A8_UNORM,	// 8bit符号なし正規化整数
				DXGI_FORMAT_UNKNOWN
			);

			return;
		}

		/**
		 * @brief メインレンダリングターゲットをフレームバッファにコピーするためのスプライトの初期化
		*/
		void CRenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
		{
			SpriteInitData spriteInitData;

			// テクスチャはメインレンダリングターゲット
			spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

			// レンダリング先がフレームバッファーなので、解像度はフレームバッファーと同じ
			spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
			spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
			spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

			// ガンマ補正ありの2D描画のシェーダーを指定する
			spriteInitData.m_fxFilePath = m_kSpriteFxFilePath;

			// 初期化オブジェクトを使って、スプライトを初期化する
			m_copyMainRtToFrameBufferSprite.Init(spriteInitData);

			return;
		}

		/**
		 * @brief ディファ―ドライティングを行うためのスプライトの初期化
		*/
		void CRenderingEngine::InitDefferdLightingSprite()
		{
			// スプライトを初期化データ
			SpriteInitData spriteInitData;

			// 画面全体にレンダリングするので幅と高さはフレームバッファーの幅と高さと同じ
			spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
			spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

			// ディファードライティングで使用するテクスチャを設定
			int texNo = 0;
			for (auto& GBuffer : m_GBuffer)
			{
				spriteInitData.m_textures[texNo++] = &GBuffer.GetRenderTargetTexture();
			}

			// fxファイルパスを設定
			spriteInitData.m_fxFilePath = m_kDefferdLightingSpriteFxFilePath;

			// 定数バッファの登録
			// ライト情報を登録
			spriteInitData.m_expandConstantBuffer[0] = 
				&nsLight::CLightManager::GetInstance()->GetLightData();
			spriteInitData.m_expandConstantBufferSize[0] =
				sizeof(nsLight::CLightManager::GetInstance()->GetLightData());
			// ディファードレンダリング用の定数バッファを登録
			spriteInitData.m_expandConstantBuffer[1] = &m_defferdLightingCB;
			spriteInitData.m_expandConstantBufferSize[1] = sizeof(spriteInitData);

			// メインレンダリングターゲットに描画するため
			// メインレンダリングターゲットとカラーフォーマットを合わせる
			spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();

			// IBLを行うか？、かつ
			// IBLに使用するテクスチャが有効か？
			if (m_defferdLightingCB.isIBL == true && m_IBLTexture.IsValid())
			{
				// IBLに使用するテクスチャを設定設定する
				spriteInitData.m_textures[texNo++] = &m_IBLTexture;
			}

			// 初期化データを使ってスプライトを初期化
			m_diferredLightingSprite.Init(spriteInitData);

			return;
		}

		/**
		 * @brief IBLを再初期化する
		 * @param[in] ddsFilePath IBLのテクスチャのddsファイルパス
		 * @param[in] luminance IBLの明るさ
		 * @param[in] isIBL IBLを行うか？
		*/
		void CRenderingEngine::ReInitIBL(const wchar_t* ddsFilePath, const float luminance, const bool isIBL)
		{
			// IBLのデータを初期化する
			InitIBLData(ddsFilePath, luminance);

			// IBLを行うか？を設定
			m_defferdLightingCB.isIBL = isIBL;

			// ディファードライティングを行うためのスプライトを初期化する
			InitDefferdLightingSprite();

			return;
		}


		/**
		 * @brief レンダリングエンジンを実行
		*/
		void CRenderingEngine::Execute()
		{
			// レンダリングコンテキスト
			RenderContext& rc = g_graphicsEngine->GetRenderContext();

			// 描画オブジェクトの登録
			GameObjectManager::GetInstance()->ExecuteAddRender();

			// GBufferに描画する
			RenderToGBuffer(rc);

			// ディファ―ドライディング
			DefferdLighting(rc);

			// フォワードレンダリング
			ForwardRendering(rc);

			// エフェクトの描画
			EffectEngine::GetInstance()->Draw();

			// ポストエフェクトを実行
			m_postEffect.Render(rc, m_mainRenderTarget);

			// 2Dを描画する
			Render2D(rc);

			// メインレンダリングターゲットの内容をフレームバッファにコピーする
			CopyMainRenderTargetToFrameBuffer(rc);

			m_renderObjects.clear();

			return;
		}

		/**
		 * @brief GBufferに描画する
		 * @param rc レンダリングコンテキスト
		*/
		void CRenderingEngine::RenderToGBuffer(RenderContext& rc)
		{
			// レンダリングターゲットをG-Bufferに変更
			RenderTarget* rts[enGBufferNum] = {
				&m_GBuffer[enGBufferAlbedoDepth],         // 0番目のレンダリングターゲット
				&m_GBuffer[enGBufferNormal],              // 1番目のレンダリングターゲット
				&m_GBuffer[enGBufferMetaricShadowSmooth], // 2番目のレンダリングターゲット
			};

			// まず、レンダリングターゲットとして設定できるようになるまで待つ
			rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

			// レンダリングターゲットを設定
			rc.SetRenderTargets(ARRAYSIZE(rts), rts);

			// レンダリングターゲットをクリア
			rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

			// 描画
			for (nsGraphic::CRender* renderObject : m_renderObjects)
			{
				renderObject->OnRenderToGBuffer(rc);
			}

			// レンダリングターゲットへの書き込み待ち
			rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

			return;
		}

		/**
		 * @brief ディファ―ドライティング
		 * @param rc レンダリングコンテキスト
		*/
		void CRenderingEngine::DefferdLighting(RenderContext& rc)
		{
			// ディファードライティングに必要なデータを更新する
			// ビュープロジェクション行列の逆行列更新
			m_defferdLightingCB.mViewProjInv.Inverse(g_camera3D->GetViewProjectionMatrix());

			// レンダリング先をメインレンダリングターゲットにする
			// メインレンダリングターゲットを設定
			rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
			rc.SetRenderTargetAndViewport(m_mainRenderTarget);

			// G-Bufferの内容を元にしてディファードライティング
			m_diferredLightingSprite.Draw(rc);

			// メインレンダリングターゲットへの書き込み終了待ち
			rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

			return;
		}

		/**
		 * @brief フォワードレンダリング
		 * @param[in] rc レンダリングコンテキスト
		*/
		void CRenderingEngine::ForwardRendering(RenderContext& rc)
		{
			// レンダリング先をメインレンダリングターゲットにする
			// 深度バッファはGBufferのものを使用する
			rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
			rc.SetRenderTarget(
				m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
				m_GBuffer[enGBufferAlbedoDepth].GetDSVCpuDescriptorHandle()
			);

			// 描画
			for (nsGraphic::CRender* renderObject : m_renderObjects)
			{
				renderObject->OnForwardRender(rc);
			}

			// メインレンダリングターゲットへの書き込み終了待ち
			rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

			return;
		}

		/**
		 * @brief 2Dを描画する
		 * @param rc レンダリングコンテキスト
		*/
		void CRenderingEngine::Render2D(RenderContext& rc)
		{
			for (nsGraphic::CRender* renderObject : m_renderObjects)
			{
				renderObject->OnRender2D(rc);
			}
			return;
		}

		/**
		 * @brief メインレンダリングターゲットの内容をフレームバッファにコピーする
		 * @param rc レンダリングコンテキスト
		*/
		void CRenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
		{
			// メインレンダリングターゲットの絵をフレームバッファーにコピー
			rc.SetRenderTarget(
				g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
				g_graphicsEngine->GetCurrentFrameBuffuerDSV()
			);

			// ビューポートを指定する
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferWidth());
			viewport.Height = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferHeight());
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			rc.SetViewportAndScissor(viewport);
			m_copyMainRtToFrameBufferSprite.Draw(rc);

			return;
		}

		/**
		 * @brief IBLのデータを初期化する
		 * @param[in] ddsFilePath IBLのテクスチャのddsファイルパス
		 * @param[in] luminance IBLの明るさ
		*/
		void CRenderingEngine::InitIBLData(const wchar_t* ddsFilePath, const float luminance)
		{
			// IBLに使用するテクスチャの初期化
			m_IBLTexture.InitFromDDSFile(ddsFilePath);

			// IBLの明るさを設定
			m_defferdLightingCB.IBLLuminance = luminance;

			return;
		}
	}
}