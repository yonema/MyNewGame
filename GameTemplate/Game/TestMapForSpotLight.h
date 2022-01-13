#pragma once

namespace nsNinjaAttract
{

	// 前方宣言
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }	// モデルレンダラークラス
	}
	namespace nsLight { class CSpotLight; }		// ポイントライトクラス

	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		/**
		 * @brief テストマップ（レベル）用のネームスペース
		*/
		namespace nsTestMaps
		{
			/**
			 * @brief スポットライトのテスト用マップ
			*/
			class CTestMapForSpotLight : public IGameObject
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CTestMapForSpotLight() = default;

				/**
				 * @brief デストラクタ
				*/
				~CTestMapForSpotLight() = default;

			public:		// オーバーライドしたメンバ関数
				/**
				 * @brief スタート関数
				 * @return アップデートを行うか？
				*/
				bool Start() override final;

				/**
				 * @brief 破棄した時に呼ばれる
				*/
				void OnDestroy() override final;

				/**
				 * @brief アップデート関数
				*/
				void Update() override final;

			private:	// データメンバ
				nsGraphic::nsModel::CModelRender* m_playerModel = nullptr;	//!< プレイヤーモデル
				nsGraphic::nsModel::CModelRender* m_bgModel = nullptr;		//!< バックグラウンドモデル
				nsGraphic::nsModel::CModelRender* m_spotLightModel = nullptr;//!< スポットライトモデル
				nsLight::CSpotLight* m_spotLight = nullptr;					//!< スポットライト
			};
		}
	}
}

