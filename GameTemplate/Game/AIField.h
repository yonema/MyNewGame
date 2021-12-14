#pragma once
#include "Noncopyable.h"
#include "NaviMesh.h"
#include "PathFinding.h"
#include "AICharacterBase.h"
#include "ModelRender.h"

namespace nsMyGame
{
	/**
	 * @brief AI関連のネームスペース
	*/
	namespace nsAI
	{
		/**
		 * @brief AIのフィールド
		*/
		class CAIField : private nsUtil::Noncopyable
		{
		private:	// エイリアス宣言
			using SAICharacterInitData = nsAICharacter::CAICharacterBase::SAICharacterInitData;
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CAIField();
			/**
			 * @brief デストラクタ
			*/
			//~CAIField() = default;
			~CAIField() {
				DeleteGO(FindGO<nsGraphic::nsModel::CModelRender>("NMBB"));
			};

		public:		// メンバ関数

			/**
			 * @brief AIキャラクターの初期化データを得る
			 * @return AIキャラクターの初期化データ
			*/
			SAICharacterInitData& GetAICharaInitData()
			{
				return m_AICharaInitData;
			}

		private:	// privateなメンバ関数

			/**
			 * @brief 初期化
			*/
			void Init();

		private:	// データメンバ
			SAICharacterInitData m_AICharaInitData;		//!< AIキャラクターの初期化データ
			nsAI::CNaviMesh m_naviMesh;					//!< ナビメッシュ
			nsAI::CPathFinding m_pathFinding;			//!< パス検索
			Model m_naviMeshBlockBolume;				//!< ナビメッシュをブロックするボリューム
			PhysicsGhostObject m_noviMeshBlockGhostObject;	//!< ナビメッシュをブロックするゴーストオブジェクト
			std::vector<Vector3> m_naviMeshTargetPoints;	//!< ナビメッシュのターゲットポイント
		};

	}
}