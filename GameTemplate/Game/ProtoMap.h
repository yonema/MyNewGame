#pragma once
#include "Level3D.h"

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }	// モデルレンダラークラス
	}
	namespace nsNature { class CSkyCube; }	// スカイクラス
	namespace nsPlayer { class CPlayer; }	// プレイヤークラス
	namespace nsGameState { class CGameMainState; }	// ゲームメインステートクラス
	namespace nsGoal { class CGoal; }	// ゴールクラス

	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		/**
		 * @brief プロトマップ（レベル）用のネームスペース
		*/
		namespace nsProtoMaps
		{
			/**
			 * @brief プロト用マップクラス
			*/
			class CProtoMap : public IGameObject
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CProtoMap() = default;
				/**
				 * @brief コンストラクタ
				*/
				~CProtoMap() = default;

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
				nsGoal::CGoal* m_goal = nullptr;			//!< ゴールクラス
				nsGameState::CGameMainState* m_gameState = nullptr;	//!< ゲームステートクラス
			};

		}
	}
}