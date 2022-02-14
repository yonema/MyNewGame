#pragma once
#include "Noncopyable.h"
#include "TklFile.h"

namespace nsNinjaAttract
{
	/**
	 * @brief 3Dのレベルデータ関連のネームスペース
	*/
	namespace nsLevel3D
	{

		/**
		 * @brief レベルオブジェクトデータの構造体
		*/
		struct SLevelObjectData : public nsUtil::Noncopyable
		{
			Vector3 position;		//!< 座標。
			Quaternion rotation;	//!< 回転。
			Vector3 scale;			//!< 拡大率。
			const wchar_t* name;	//!< 名前。
			int number;

			// 追加。ここから。
			bool isTranslucent = false;	//!< 半透明か？
			int priority = nsCommonData::enPriorityFirst;	//!< 優先度
			int numMapChipReserve = 1;	//!< マップチップの予約数
			//!< ユーザー定義のコリジョン属性
			EnCollisionAttr userIndex = EnCollisionAttr::enCollisionAttr_None;
			const char* lodModelFilePath = nullptr;		//!< LOD用のモデルのファイルパス
			bool shadowCaster = false;					//!< シャドウキャスターか？
			//!< LODの切り替えの距離。デフォルトでは負の数。
			//! 負の数ならモデルレンダラーにLODの切り替えの距離を指定しない
			float distanceLOD = -1.0f;
			const char* collisionModelFilePath = nullptr;	//!< コリジョン用モデルのファイルパス
			// 追加。ここまで。

			/// <summary>
			/// 引数で渡したオブジェクト名のオブジェクトか調べる。
			/// </summary>
			/// <param name="objName">調べる名前。</param>
			/// <returns>名前が同じ場合にtrueを返します。</returns>


			/**
			 * @brief 引数で渡したオブジェクト名のオブジェクトか調べる（ワイド文字列）
			 * @param[in] objName 調べる名前（ワイド文字列）
			 * @return 名前が同じか？
			*/
			bool EqualObjectName(const wchar_t* objName)
			{
				return wcscmp(objName, name) == 0;
			}

			/**
			 * @brief 引数で渡したオブジェクト名のオブジェクトか調べる（マルチバイト文字列）
			 * @param[in] objName 調べる名前（マルチバイト文字列）
			 * @return 名前が同じか？
			*/
			bool EqualObjectName(const char* objName)
			{
				wchar_t objNameW[256];
				// sizeof(objName)だとポインタのサイズが戻ってくるんだぜ。
				// 日本語上手くいかないんだぜ
				mbstowcs(objNameW, objName, std::strlen(objName) + 1);
				return wcscmp(objNameW, name) == 0;
			}

			/**
			 * @brief 名前が前方一致するか調べる
			 * @param[in] n 調べる名前
			 * @return 名前が前方一致するか？
			*/
			bool ForwardMatchName(const wchar_t* n)
			{
				auto len = wcslen(n);
				auto namelen = wcslen(name);
				if (len > namelen) {
					//名前が長い。不一致。
					return false;
				}
				return wcsncmp(n, name, len) == 0;
			}
		};

		// 前方宣言
		class CMapChip;		// マップチップクラス

		/**
		 * @brief 3Dレベルクラス
		*/
		class CLevel3D
		{
		private:	// エイリアス宣言
			using MapChipPtr = std::shared_ptr<CMapChip>;

		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CLevel3D();
			/**
			 * @brief デストラクタ
			*/
			~CLevel3D() = default;

		public:		// メンバ関数

			/**
			 * @brief レベルを初期化。
			 * @param[in] filePath tklファイルのファイルパス
			 * @param[in] hookFunc オブジェクトを作成する時の処理をフックするための関数オブジェクト
			 * @details フックしないならnullptrを指定すればよい。
			 * この関数オブジェクトがfalseを返すと、オブジェクトの情報から、
			 * 静的オブジェクトのMapChipクラスのインスタンスが生成されます。
			 * オブジェクトの名前などで、ドアやジャンプ台、アイテムなどの特殊なクラスのインスタンスを
			 * 生成したときに、デフォルトで作成されるMapChipクラスのインスタンスが不要な場合はtrueを
			 * 返してください。
			 * 例えば、フック関数の中で、渡されたオブジェクトデータの名前のモデルを描画するクラスの
			 * インスタンスを生成したときに、falseを返してしまうと、同じモデルが二つ描画されることになります。
			*/
			void Init(
				const char* filePath,
				std::function<bool(SLevelObjectData& objData)> hookFunc
			);

			/**
			 * @brief tklファイルがあるディレクトリのパスを設定
			 * @param[in] directoryPaht ディレクトリのパス
			*/
			void SetDirectoryPath(const wchar_t* directoryPaht)
			{
				// 今あるパスをクリア
				memset(m_directoryPath, 0, sizeof(m_directoryPath));
				// 新しいパスをセット
				swprintf_s(m_directoryPath, directoryPaht);
			}

		private:	// privateなメンバ関数

			/**
			 * @brief マップチップを作成。
			 * @param[in] objData レベルオブジェクトデータ
			 * @param[in] filePath ファイルパス
			*/
			void CreateMapChip(const SLevelObjectData& objData, const char* filePath);

			/**
			 * @brief Tklファイルの行列を変換する。
			*/
			void MatrixTklToLevel();

		private:	// 定数
			static const int m_kMaxPathSize = 256;			//!< ファイルパスの最大サイズ
			static const wchar_t* m_kDefaultDirectoryPath;	//!< デフォルトのディレクトリのパス
			static const wchar_t* m_kDirectoryPathTail;		//!< ディレクトリのパスの末尾
		public:		// publicな定数
			static const Vector3 m_kLevelObjectOffset;		//!< レベルオブジェクトのオフセット
		private:	// データメンバ
			using BonePtr = std::unique_ptr<Bone>;		//!< ボーンPtr
			std::vector<BonePtr> m_bonelist;			//!< ボーンのリスト
			std::unique_ptr<Matrix[]> m_matrixlist;		//!< 行列のリスト
			std::map<std::string, MapChipPtr> m_mapChipPtrs;	//!< マップチップの可変長配列。
			CTklFile m_tklFile;							//!< tklファイル
			wchar_t m_directoryPath[m_kMaxPathSize] = {};	//!< tklファイルがあるディレクトリのパス

		};

	}
}