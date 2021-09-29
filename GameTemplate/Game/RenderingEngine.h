#pragma once
#include "RenderingEngineConstData.h"
#include "TResourceBank.h"
#include "PostEffect.h"
#include "ShadowMapRender.h"
#include "LightConstData.h"

namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic { class CRender; };

	/**
	 * @brief このゲーム用のゲームエンジンネームスペース
	*/
	namespace nsMyEngine
	{
		/**
		 * @brief レンダリングエンジンクラス。GraphicsEngineのラップクラス。
		 * @note シングルトンパターンを使用
		*/
		class CRenderingEngine : private nsUtil::Noncopyable
		{
		private:	// コンストラクタとデストラクタ。
					// シングルトンパターンのためprivateに隠す
			/**
			 * @brief コンストラクタ
			*/
			CRenderingEngine();
			/**
			 * @brief デストラクタ
			*/
			~CRenderingEngine();

		public:		// メンバ関数

			void AddRenderObject(nsGraphic::CRender* renderObject)
			{
#ifdef MY_DEBUG
				if (m_renderObjects.size() >= nsRenderingEngineConstData::m_kRenderObjectsCapacityNum)
					MessageBoxA(nullptr, "レンダリングオブジェトが予想より多く作られてる", "警告", MB_OK);
#endif
				m_renderObjects.emplace_back(renderObject);
			}

			/**
			 * @brief メインレンダリングターゲットのカラーバッファーフォーマットを取得
			 * @return メインレンダリングターゲットのカラーバッファーフォーマット
			*/
			DXGI_FORMAT GetMainRenderTargetFormat() const
			{
				return m_mainRenderTarget.GetColorBufferFormat();
			}

			/**
			 * @brief IBLを行うか？を設定する
			 * @param[in] isIBL IBLを行うか？
			*/
			void SetIsIBL(const bool isIBL)
			{
				m_defferdLightingCB.isIBL = isIBL;
			}

			/**
			 * @brief tkmファイルのリソースをリソースバンクから取得。未登録の場合はnullptrを戻す。
			 * @param[in] filePath tkmファイルパス
			 * @return リソース
			*/
			TkmFile* GetTkmFileFromBank(const char* filePath)
			{
				return m_tkmFileBank.Get(filePath);
			}

			/**
			 * @brief tkmファイルのリソースをリソースバンクに登録
			 * @param[in] filePath tkmファイルパス
			 * @param[in] tkmFile tkmファイルリソース
			*/
			void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
			{
				m_tkmFileBank.Regist(filePath, tkmFile);
			}

			/**
			 * @brief シェーダーのリソースをリソースバンクから取得。未登録の場合はnullptrを戻す。
			 * @param[in] filePath シェーダーファイルパス
			 * @param[in] entryPointFuncName エントリーポイント
			 * @return リソース
			*/
			Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
			{
				std::string programName = filePath;
				programName += entryPointFuncName;
				return m_shaderBank.Get(programName.c_str());
			}

			/**
			 * @brief シェーダーのリソースをリソースバンクに登録
			 * @param filePath シェーダーファイルパス
			 * @param entryPointFuncName エントリーポイント
			 * @param shader シェーダーリソース
			*/
			void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
			{
				std::string programName = filePath;
				programName += entryPointFuncName;
				m_shaderBank.Regist(programName.c_str(), shader);
			}

			/**
			 * @brief 指定したGBufferのカラーフォーマットを取得
			 * @param[in] enGBufferNo GBufferの番号
			 * @return カラーフォーマット
			*/
			DXGI_FORMAT GetGBufferColorFormat(const nsRenderingEngineConstData::EnGBuffer enGBufferNo) const
			{
				return m_GBuffer[enGBufferNo].GetColorBufferFormat();
			}

			/**
			 * @brief IBLを再初期化する
			 * @param[in] ddsFilePath IBLのテクスチャのddsファイルパス
			 * @param[in] luminance IBLの明るさ
			 * @param[in] isIBL IBLを行うか？
			*/
			void ReInitIBL(const wchar_t* ddsFilePath, const float luminance,const bool isIBL);

			/**
			 * @brief レンダリングエンジンを実行
			*/
			void Execute();

		public:		// staticなメンバ関数
			/**
			 * @brief 唯一のインスタンスを生成する
			 * @note シングルトンパターン
			*/
			static void CreateInstance()
			{
				m_instance = new CRenderingEngine;
			}

			/**
			 * @brief 唯一のインスタンスを破棄する
			 * @note シングルトンパターン
			*/
			static void DeleteInstance()
			{
				if (m_instance)
					delete m_instance;
			}

			/**
			 * @brief 唯一のインスタンスの参照を得る
			 * @return 唯一のインスタンスの参照
			 * @note シングルトンパターン
			*/
			static CRenderingEngine* GetInstance()
			{
				return m_instance;
			}
		private:	// privateなメンバ関数

			/**
			 * @brief メインレンダリングターゲットの初期化
			*/
			void InitMainRenderTarget();

			/**
			 * @brief GBufferを初期化
			*/
			void InitGBuffer();

			/**
			 * @brief メインレンダリングターゲットをフレームバッファにコピーするためのスプライトの初期化
			*/
			void InitCopyMainRenderTargetToFrameBufferSprite();

			/**
			 * @brief シャドウマップレンダラーの初期化
			*/
			void InitShadowMapRender();

			/**
			 * @brief ディファ―ドライティングを行うためのスプライトの初期化
			*/
			void InitDefferdLightingSprite();

			/**
			 * @brief シャドウマップに描画する
			 * @param[in] rc レンダリングコンテキスト
			*/
			void RenderToShadowMap(RenderContext& rc);

			/**
			 * @brief GBufferに描画する
			 * @param rc レンダリングコンテキスト
			*/
			void RenderToGBuffer(RenderContext& rc);

			/**
			 * @brief ディファ―ドライティング
			 * @param rc レンダリングコンテキスト
			*/
			void DefferdLighting(RenderContext& rc);

			/**
			 * @brief フォワードレンダリング
			 * @param[in] rc レンダリングコンテキスト
			*/
			void ForwardRendering(RenderContext& rc);

			/**
			 * @brief 2Dを描画する
			 * @param rc レンダリングコンテキスト
			*/
			void Render2D(RenderContext& rc);

			/**
			 * @brief メインレンダリングターゲットの内容をフレームバッファにコピーする
			 * @param rc レンダリングコンテキスト
			*/
			void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

			/**
			 * @brief IBLのデータを初期化する
			 * @param[in] ddsFilePath IBLのテクスチャのddsファイルパス
			 * @param[in] luminance IBLの明るさ
			*/
			void InitIBLData(const wchar_t* ddsFilePath, const float luminance);

		private:	// データメンバ
			std::vector<nsGraphic::CRender*> m_renderObjects;	//!< 描画するオブジェクト

			RenderTarget m_mainRenderTarget;	//!< メインレンダリングターゲット
			RenderTarget m_GBuffer[nsRenderingEngineConstData::enGBufferNum];	//!< GBuffer

			Sprite m_copyMainRtToFrameBufferSprite;	//!< メインレンダリングターゲットをフレームバッファにコピーするためのスプライト
			Sprite m_diferredLightingSprite;	//!< ディファードライティングを行うためのスプライト

			//!< シャドウマップレンダラー
			nsGraphic::nsShadow::CShadowMapRender m_shadowMapRenders[nsLight::nsLightConstData::kMaxDirectionalLightNum];
			nsGraphic::nsPostEffect::CPostEffect m_postEffect;	//!< ポストエフェクトクラス

			//!< ディファードライティング用の定数バッファ
			nsRenderingEngineConstData::SDefferdLightingCB m_defferdLightingCB;
			Texture m_IBLTexture;	//!< IBLに使用するテクスチャ

			nsUtil::TResourceBank<TkmFile> m_tkmFileBank;	//!< tkmファイルバンク
			nsUtil::TResourceBank<Shader> m_shaderBank;		//!< シェーダーバンク

		private:	// staticなデータメンバ
			static CRenderingEngine* m_instance;		//!< 唯一のインスタンス
		};
	}
}

