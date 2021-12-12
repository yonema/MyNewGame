#pragma once
#include "Cell.h"
#include "BSP.h"

namespace nsMyGame
{
	/**
	 * @brief AI関連のネームスペース
	*/
	namespace nsAI
	{
		/**
		 * @brief ナビゲーションメッシュクラス
		*/
		class CNaviMesh
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CNaviMesh() = default;
			/**
			 * @brief デストラクタ
			*/
			~CNaviMesh() = default;

		public:		// メンバ関数

			/**
			 * @brief tknファイルからナビゲーションメッシュを構築する
			 * @param tknFilePath tknファイルのファイルパス
			*/
			void Init(const char* tknFilePath);

			/**
			 * @brief 指定した座標に最も近いセルを検索する
			 * @param[in] pos 指定座標
			 * @return 最も近いセル
			*/
			const CCell& FindNearestCell(const Vector3& pos) const;

			/**
			 * @brief セルの数を取得
			 * @return セルの数
			*/
			int GetNumCell() const
			{
				return static_cast<int>(m_cellArray.size());
			}

			/**
			 * @brief セルを取得
			 * @param[in] cellNo セルの番号
			 * @return セル
			*/
			const CCell& GetCell(const int cellNo) const
			{
				return m_cellArray.at(cellNo);
			}

		private:	// データメンバ

			std::vector<CCell> m_cellArray;			//!< セルの配列
			nsGeometry::CBSP m_cellCenterPosBSP;	//!< セルの中心座標で構築されたBSP

		};

	}
}
