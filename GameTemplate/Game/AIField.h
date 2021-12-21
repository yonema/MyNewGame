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
		 * @brief AIの初期化データ
		*/
		struct SAICharacterInitData
		{
			const nsAI::CNaviMesh* naviMeshRef = nullptr;
			nsAI::CPathFinding* pathFindingRef = nullptr;
			std::vector<Vector3>* naviMeshTargetPointsRef = nullptr;
			const std::vector<Vector3>* wayPointsRef = nullptr;
		};

		/**
		 * @brief AIのフィールド
		*/
		class CAIField : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CAIField();
			/**
			 * @brief デストラクタ
			*/
			~CAIField() = default;

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

			/**
			 * @brief ウェイポイントの初期化
			*/
			void InitWayPoints();

		private:	// データメンバ
			SAICharacterInitData m_AICharaInitData;		//!< AIキャラクターの初期化データ
			nsAI::CNaviMesh m_naviMesh;					//!< ナビメッシュ
			nsAI::CPathFinding m_pathFinding;			//!< パス検索
			std::vector<Vector3> m_naviMeshTargetPoints;	//!< ナビメッシュのターゲットポイント
			std::vector<Vector3> m_wayPoints;			//!< ウェイポイント

		};

	}
}