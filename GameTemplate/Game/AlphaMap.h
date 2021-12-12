#pragma once
#include "Level3D.h"
#include "MapConstDatah.h"
#include "NaviMesh.h"
#include "PathFinding.h"


namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }	// モデルレンダラークラス
	}
	namespace nsNature { class CSkyCube; }			// スカイクラス
	namespace nsPlayer { class CPlayer; }			// プレイヤークラス
	namespace nsGameState { class CGameMainState; }	// ゲームメインステートクラス
	namespace nsGoal { class CGoal; }				// ゴールクラス
	namespace nsBuilding { class CBuildings; }		// 建物クラス

	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		/**
		 * @brief アルファマップ（レベル）用のネームスペース
		*/
		namespace nsAlphaMaps
		{
			/**
			 * @brief アルファ用マップクラス
			*/
			class CAlphaMap : public IGameObject
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CAlphaMap() = default;
				/**
				 * @brief コンストラクタ
				*/
				~CAlphaMap() = default;

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
				nsLevel3D::CLevel3D m_propsLevel3D[nsMapConstData::enPropsTypeNum];	//!< 小物用3Dレベルクラス
				nsNature::CSkyCube* m_skyCube = nullptr;	//!< スカイキューブ
				nsPlayer::CPlayer* m_player = nullptr;		//!< プレイヤークラス
				nsGoal::CGoal* m_goal = nullptr;			//!< ゴールクラス
				nsGameState::CGameMainState* m_gameState = nullptr;	//!< ゲームステートクラス
				nsBuilding::CBuildings* m_buildings = nullptr;	//!< 建物クラス
				nsAI::CNaviMesh m_naviMesh;					//!< ナビメッシュ
				nsAI::CPathFinding m_pathFinding;			//!< パス検索
			};
		}
	}
}
