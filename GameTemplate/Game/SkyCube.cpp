#include "stdafx.h"
#include "SkyCube.h"
#include "RenderingEngine.h"

namespace nsNinjaAttract
{
	/**
	 * @brief 自然関連のネームスペース
	*/
	namespace nsNature
	{
		// スカイキューブの定数データを使用可能にする
		using namespace nsSkyCubeConstData;

		/**
		 * @brief スタート関数
		 * @return アップデートを行うか？
		*/
		bool CSkyCube::Start()
		{


			return true;
		}

		/**
		 * @brief 破棄されたときに呼ばれる関数
		*/
		void CSkyCube::OnDestroy()
		{
			DeleteGO(m_skyModelRender);
			return;
		}

		/**
		 * @brief 初期化関数
		 * @attention オブジェクトを生成してすぐに呼ばないと効果がありません。
		 * @param[in] skyCubeType スカイキューブのタイプ
		 * @param[in] isIBL IBLを行うか？
		*/
		void CSkyCube::Init(const nsSkyCubeConstData::EnSkyCubeType skyCubeType, const bool isIBL)
		{
			// 指定したスカイキューブのタイプでテクスチャを初期化
			m_skyTexture.InitFromDDSFile(GetSkyCubeTextureDDSFilePath(skyCubeType));

			nsMyEngine::CRenderingEngine::GetInstance()->ReInitIBL(GetSkyCubeTextureDDSFilePath(skyCubeType), 1.0f,isIBL);

			// スカイのモデルレンダラーの生成
			m_skyModelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			// テクスチャの初期化
			InitTexture();

			// モデルレンダラーの初期化
			InitModelRender();
		}

		/**
		 * @brief タイトル画面か？を設定
		 * @param[in] isTitle タイトル画面か？
		*/
		void CSkyCube::SetIsTitle(const bool isTitle)
		{
			// 空のモデルの乗算カラー
			float mul = 1.0f;

			if (isTitle)
			{
				// タイトル画面では少し暗くする
				mul = 0.9f;
			}

			// 乗算カラーを設定
			m_skyModelRender->SetMulColor({ mul ,mul ,mul ,1.0f });

			return;
		}

		/**
		 * @brief テクスチャの初期化関数
		*/
		void CSkyCube::InitTexture()
		{
			// スカイキューブのテクスチャが初期化されていないか？
			if (!m_skyTexture.IsValid())
			{
				// 初期化されていない
				// デフォルトのスカイキューブのテクスチャで初期化する
				m_skyTexture.InitFromDDSFile(kDefaultDDSPath);
			}

			return;
		}

		/**
		 * @brief モデルレンダラーの初期化関数
		*/
		void CSkyCube::InitModelRender()
		{
			// モデルの初期化データ
			ModelInitData modelInitData;
			modelInitData.m_tkmFilePath = kTkmFilePath;	// tkmファイルパスを設定
			modelInitData.m_fxFilePath = kFxFilePath;	// fxファイルパスを設定
			// SRVにスカイキューブマップのテクスチャを設定
			modelInitData.m_expandShaderResoruceView[0] = &m_skyTexture;
			// メインレンダリングターゲットのからフォーマットに合わせる
			modelInitData.m_colorBufferFormat[0] =
				nsMyEngine::CRenderingEngine::GetInstance()->GetMainRenderTargetFormat();

			// スカイのモデルレンダラーをフォワードレンダリングで描画するように初期化
			m_skyModelRender->InitForwardRendering(modelInitData);

			// 座標と拡大率を設定する
			m_skyModelRender->SetPosition(m_position);
			m_skyModelRender->SetScale(m_scale);

			return;
		}
	}
}