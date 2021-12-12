#pragma once

namespace nsMyGame
{
	/**
	 * @brief AI関連のネームスペース
	*/
	namespace nsAI
	{
		/**
		 * @brief パスを扱うクラス
		*/
		class CPath
		{
		private:	// privateな構造体
			/**
			 * @brief パスのセクション
			*/
			struct SSection {
				Vector3 startPos;	// セクションの開始座標。
				Vector3 endPos;		// セクションの終了座標。
				Vector3 direction;	// セクションの方向。
				float length;		// セクションの長さ。
			};
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPath() = default;
			/**
			 * @brief デストラクタ
			*/
			~CPath() = default;

		public:		// メンバ関数

			/**
			 * @brief パス上を移動する
			 * @details 物理ワールドが指定されていると、パス移動後に地面にレイキャストを行い、
			 * 座標を地面にスナップします。
			 * @param[in] pos 移動させる座標
			 * @param[in] moveSpeed 移動速度
			 * @param[out] isEnd パス移動終了したらtrueが設定される
			 * @param[in] physicsWorld 物理ワールド。
			 * @return 移動後の座標
			*/
			Vector3 Move(
				Vector3 pos,
				const float moveSpeed,
				bool& isEnd,
				const PhysicsWorld* physicsWorld = nullptr
			);

			/**
			 * @brief クリア
			*/
			void Clear()
			{
				m_sectionNo = 0;
				m_pointArray.clear();
				m_sectionArray.clear();
			}

			/**
			 * @brief ポイントの追加。
			 * @param[in] point ポイント
			*/
			void AddPoint(const Vector3& point)
			{
				m_pointArray.emplace_back(point);
			}

			/**
			 * @brief パスを構築
			*/
			void Build();

		private:	// データメンバ
			std::vector<Vector3>	m_pointArray;	//!< ポイントの配列
			std::vector< SSection >	m_sectionArray;	//!< セクションの配列。
			int m_sectionNo = 0;					//!< セクション番号。
		};

	}
}