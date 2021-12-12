#pragma once

namespace nsMyGame
{
	/**
	 * @brief AI関連のネームスペース
	*/
	namespace nsAI
	{
		/**
		 * @brief tknファイル（ナビゲーションメッシュ）クラス
		*/
		class CTknFile
		{
		public:		// 構造体
			/**
			 * @brief tknファイルのヘッダー
			*/
			struct SHeader
			{
				std::uint32_t numCell;	//!< セルの数
			};

			/**
			 * @brief セルのデータ
			*/
			struct SCell
			{
				Vector3 vertexPosition[3];	//!< セルの頂点座標
				Vector3 normal;				//!< セルの法線
				std::int32_t linkCellNo[3];	//!< 隣接セルの番号
			};

		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CTknFile() = default;
			/**
			 * @brief デストラクタ
			*/
			~CTknFile() = default;

		public:		// メンバ関数

			/**
			 * @brief tknファイルをロードする
			 * @param[in] filePath tknファイルのファイルパス
			 * @return ロードできたか？falseが戻ってきたらロード失敗。
			*/
			bool Load(const char* filePath);

			/**
			 * @brief セルの数を取得
			 * @return セルの数
			*/
			int GetNumCell() const
			{
				return static_cast<int>(m_header.numCell);
			}

			/**
			 * @brief セルを取得
			 * @param[in] cellNo セルの番号
			 * @return セル
			*/
			const SCell& GetCell(const int cellNo) const
			{
				return m_cellArray.at(cellNo);
			}

		private:	// データメンバ

			SHeader m_header;					//!< ヘッダ
			std::vector<SCell> m_cellArray;		//!< セルの配列

		};

	}
}