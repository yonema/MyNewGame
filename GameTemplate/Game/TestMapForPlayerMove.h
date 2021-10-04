#pragma once
#include "Level3D.h"

namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }	// モデルレンダラークラス
	}
	namespace nsNature { class CSkyCube; }	// スカイクラス
	namespace nsPlayer { class CPlayer; }	// プレイヤークラス

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
			 * @brief プレイヤーの動きのテストマップ
			*/
			class CTestMapForPlayerMove : public IGameObject
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CTestMapForPlayerMove() = default;
				/**
				 * @brief コンストラクタ
				*/
				~CTestMapForPlayerMove() = default;

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
				nsLevel3D::CLevel3D m_level3D;				//!< 3Dレベルクラス
				nsNature::CSkyCube* m_skyCube = nullptr;	//!< スカイキューブ
				nsPlayer::CPlayer* m_player = nullptr;		//!< プレイヤークラス
			};

		}
	}
}