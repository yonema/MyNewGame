#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief 3Dのレベルデータ関連のネームスペース
	*/
	namespace nsLevel3D
	{
		/**
		 * @brief tklファイルを読み込むクラス
		*/
		class CTklFile
		{
		public:		// データ
			
			/**
			 * @brief オブジェクトデータ
			*/
			struct SObject
			{
				std::unique_ptr<char[]> name;	//!< 骨の名前。
				int parentNo;					//!< 親の番号。
				float bindPose[4][3];			//!< バインドポーズ。
				float invBindPose[4][3];		//!< バインドポーズの逆数。
				int no;							//!< 自分の骨番号

				//スクリプトの方がまだ未実装。
				bool shadowcasterflag = false;
				bool shadowreceiverflag = false;
				std::vector<int> intData;
				std::vector<float> floatData;
				std::vector<char*> charData;
				std::vector<Vector3> Vector3Data;
			};

		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CTklFile() = default;

			/**
			 * @brief デストラクタ
			*/
			~CTklFile() = default;

		public:		// メンバ関数

			/**
			 * @brief tklファイルをロードする
			 * @param[in] filepath tklファイルパス
			*/
			void Load(const char* filepath);

			/**
			 * @brief クエリ関数
			 * @param[in] query 実行する関数オブジェクト
			*/
			void QuaryObject(std::function<void(SObject& obj)> query)
			{
				for (auto& obj : m_objectlist) {
					query(obj);
				}
			}

		private:	// データメンバ
			int m_version = 0;					//!< tklファイルのバージョン
			int m_objectCout = 0;				//!< オブジェクトの数
			std::vector<SObject> m_objectlist;	//!< オブジェクトのリスト
		};

	}
}


