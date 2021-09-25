#include "stdafx.h"
#include "SkyCube.h"

namespace nsMyGame
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
			// テクスチャの初期化
			InitTexture();

			// モデルレンダラーの初期化
			InitModelRender();

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
		*/
		void CSkyCube::Init(const nsSkyCubeConstData::EnSkyCubeType skyCubeType)
		{
			// 指定したスカイキューブのタイプでテクスチャを初期化
			m_skyTexture.InitFromDDSFile(GetSkyCubeTextureDDSFilePath(skyCubeType));
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
			// スカイのモデルレンダラーの初期化
			m_skyModelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			// モデルの初期化データ
			ModelInitData modelInitData;
			modelInitData.m_tkmFilePath = kTkmFilePath;	// tkmファイルパスを設定
			modelInitData.m_fxFilePath = kFxFilePath;	// fxファイルパスを設定
			// SRVにスカイキューブマップのテクスチャを設定
			modelInitData.m_expandShaderResoruceView[0] = &m_skyTexture;

			// スカイのモデルレンダラーをフォワードレンダリングで描画するように初期化
			m_skyModelRender->InitForwardRendering(modelInitData);

			// 座標と拡大率を設定する
			m_skyModelRender->SetPosition(m_position);
			m_skyModelRender->SetScale(m_scale);

			return;
		}
	}
}