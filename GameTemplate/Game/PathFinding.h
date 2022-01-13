#pragma once

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsAI
	{
		class CPath;
		class CCell;
		class CNaviMesh;
	}

	/**
	 * @brief AI関連のネームスペース
	*/
	namespace nsAI
	{
		/**
		 * @brief パス検索処理
		*/
		class CPathFinding
		{
		private:	// privateな構造体
			struct CellWork 
			{
				void Init(const CCell* cell);

				const CCell* cell;
				CellWork* parentCell;	// 親のセル。
				float costFromStartCell;
				Vector3 pathPoint;
				float cost;				// 移動コスト
				bool isOpend;			// 開かれた？
				bool isClosed;			// 閉じられた？
				bool isSmooth;			// スムースされる？
			};
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPathFinding() = default;
			/**
			 * @brief デストラクタ
			*/
			~CPathFinding() = default;

		public:		// メンバ関数

			/**
			 * @brief パスの検索処理を実行。
			 * @details 物理ワールドが指定されていると、物理ワールドに配置されているオブジェクトと
			 * 衝突判定を行い、パスのスムージングが改善します。
			 * この衝突判定では、agentRadiusとagentHeightのパラメータが使用されます。
			 * @param path 見つかったパス
			 * @param naviMesh ナビゲーションメッシュ
			 * @param startPos 検索開始座標
			 * @param endPos 検索終了座標
			 * @param physicsWorld 物理ワールド。
			 * @param agentRadius AIエージェントの半径。エージェントのカプセルコライダーを作成するのに利用されます。
			 * @param agentHeight AIエージェントの高さ。エージェントのカプセルコライダーを作成するのに利用されます。
			*/
			void Execute(
				CPath& path,
				const CNaviMesh& naviMesh,
				const Vector3& startPos,
				const Vector3& endPos,
				PhysicsWorld* physicsWorld = nullptr,
				float agentRadius = 50.0f,
				float agentHeight = 200.0f
			);

		private:	// privateなメンバ関数

			/**
			 * @brief 次のセルに移動するコストを計算
			 * @param[out] totalCost 
			 * @param[out] costFromStartCell 
			 * @param[in] nextCell 
			 * @param[in] prevCell 
			 * @param[in] endCell 
			*/
			void CalcCost(
				float& totalCost,
				float& costFromStartCell,
				const CellWork* nextCell,
				const CellWork* prevCell,
				const CCell* endCell
			);

			/**
			 * @brief スムージング
			 * @param cellList 
			 * @param[in] physicsWorld 
			 * @param[in] agentRadius AIエージェントの半径。エージェントのカプセルコライダーを作成するのに利用されます。
			 * @param[in] agentHeight AIエージェントの高さ。エージェントのカプセルコライダーを作成するのに利用されます。
			*/
			void Smoothing(
				std::list<CellWork*>& cellList,
				PhysicsWorld* physicsWorld,
				float agentRadius,
				float agentHeight
			);

			/**
			 * @brief レイとセルの交差判定
			 * @param rayStartPos レイの始点
			 * @param rayEndPos レイの終点
			 * @param  
			 * @return 
			*/
			bool IsIntercetRayToCell(Vector3 rayStartPos, Vector3 rayEndPos, CellWork*) const;
		private:	// データメンバ

			std::vector< CellWork > m_cellWork;

		};

	}
}