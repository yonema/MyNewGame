#pragma once
#include "Level3D.h"

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }	// モデルレンダラークラス
	}
	namespace nsNature { class CSkyCube; }	//スカイクラス

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
			 * @brief Level3D用のテストマップ
			*/
			class CTestMapForLevel3D : public IGameObject
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CTestMapForLevel3D() = default;
				/**
				 * @brief コンストラクタ
				*/
				~CTestMapForLevel3D() = default;

			public:		// オーバーライドしたメンバ関数

				/**
				 * @brief スタート関数
				 * @return アップデートを行うか？
				*/
				bool Start() override final;

				/**
				 * @brief 破棄した時に呼ばれる関数
				*/
				void OnDestroy() override final;

				/**
				 * @brief アップデート関数
				*/
				void Update() override final;


			private:	// データメンバ
				nsLevel3D::CLevel3D m_level3D;		//!< 3Dレベルクラス
				nsNature::CSkyCube* m_skyCube = nullptr;
				float m_angleX = 0.0f;
				float m_angleY = 0.0f;
			};
		}
	}
}
