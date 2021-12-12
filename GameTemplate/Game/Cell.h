#pragma once

namespace nsMyGame
{
	/**
	 * @brief AI関連のネームスペース
	*/
	namespace nsAI
	{
		/**
		 * @brief ナビメッシュのセルクラス
		*/
		class CCell
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CCell() = default;
			/**
			 * @brief デストラクタ
			*/
			~CCell() = default;

		public:		// メンバ関数

			/**
			 * @brief 頂点座標を設定
			 * @param[in] vertNo 頂点番号。0～2。
			 * @param[in] pos 座標
			*/
			void SetVertexPosition(const int vertNo, const Vector3& pos)
			{
				CheckInvalidNo(vertNo, "Invalid vertNo");

				m_vertexPosition[vertNo] = pos;
			}

			/**
			 * @brief 法線を設定
			 * @param[in] normal 法線
			*/
			void SetNormal(const Vector3& normal)
			{
				m_normal = normal;
			}

			/**
			 * @brief 隣接セル情報を設定
			 * @param[in] linkNo 隣接番号。0～2.
			 * @param[in] linkCell 隣接セル
			*/
			void SetLinkCell(const int linkNo, const CCell* linkCell)
			{
				CheckInvalidNo(linkNo, "Invalid linkNo");

				m_linkCell[linkNo] = linkCell;
			}

			/**
			 * @brief セルの中心座標を設定
			 * @param[in] centerPos セルの中心座標
			*/
			void SetCenterPosition(const Vector3& centerPos)
			{
				m_centerPos = centerPos;
			}

			/**
			 * @brief セルの中心座標を取得
			 * @return セルの中心座標
			*/
			const Vector3& GetCenterPosition() const
			{
				return m_centerPos;
			}

			/**
			 * @brief セル番号を取得
			 * @param[in] no セル番号
			*/
			void SetCellNo(const int no)
			{
				m_no = no;
			}

			/**
			 * @brief セル番号を取得
			 * @return セル番号
			*/
			int GetCellNo() const
			{
				return m_no;
			}

			/**
			 * @brief 隣接セルを取得
			 * @param[in] linkCellNo 隣接セル番号
			 * @return 隣接セル
			*/
			const CCell* GetLinkCell(int linkCellNo)const
			{
				return m_linkCell[linkCellNo];
			}

			/**
			 * @brief セルの頂点座標を取得する
			 * @param vertNo 頂点番号
			 * @return 頂点座標
			*/
			const Vector3& GetVertexPosition(int vertNo) const
			{
				return m_vertexPosition[vertNo];
			}

		private:	// privateなメンバ関数

			/**
			 * @brief 番号が有効かチェックする
			 * @param[in] No 番号
			 * @param[in] invalidTypeText 無効のタイプのテキスト
			*/
			void CheckInvalidNo(const int No, const char* invalidTypeText)
			{
#ifdef MY_DEBUG
				if (No >= 3)
				{
					MessageBoxA(nullptr, invalidTypeText, "アサート", MB_OK);
					std::abort();
				}
#endif
			}

		private:	// データメンバ
			Vector3 m_vertexPosition[3];				//!< セルの頂点座標
			Vector3 m_normal;							//!< 法線
			Vector3 m_centerPos;						//!< 中心座標
			const CCell* m_linkCell[3] = { nullptr };	//!< 隣接セル
			int m_no = 0;								//!< セル番号
		};

	}
}