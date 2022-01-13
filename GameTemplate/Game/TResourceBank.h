#pragma once

#pragma once

#include <memory>
#include "Noncopyable.h"

namespace nsNinjaAttract 
{
	/**
	 * @brief Util関連のネームスペース
	*/
	namespace nsUtil
	{
		/**
		 * @brief リソースバンククラス
		 * @tparam TResource リソースにするクラス
		*/
		template<class TResource>
		class TResourceBank : public Noncopyable 
		{
		private:	// エイリアス宣言
			using TResourcePtr = std::unique_ptr<TResource>;	//!< リソースのユニークポインタ

		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			TResourceBank() = default;
			/**
			 * @brief デストラクタ
			*/
			~TResourceBank() = default;

		public:		// メンバ関数

			/**
			 * @brief バンクに登録されているリソースを得る。リソースが見つからない時はnullptrを戻す。
			 * @param[in] filePath リソースのファイルパス
			 * @return リソース
			*/
			TResource* Get(const char* filePath)
			{
				// ファイルパスからリソースを探す
				auto it = m_resourceMap.find(filePath);
				if (it != m_resourceMap.end()) 
				{
					// バンクに登録されている
					// リソースを戻す
					return it->second.get();
				}

				// 未登録
				// nullptrを戻す
				return nullptr;
			}

			/**
			 * @brief リソースを登録する
			 * @param[in] filePath リソースのファイルパス
			 * @param[in] resource リソースのポインタ
			*/
			void Regist(const char* filePath, TResource* resource)
			{
				// ファイルパスからリソースを探す
				auto it = m_resourceMap.find(filePath);
				if (it == m_resourceMap.end())
				{
					// 未登録
					// 登録する
					m_resourceMap.insert(
						std::pair< std::string, TResourcePtr>(filePath, resource)
					);
				}
				// 登録済みなら何もしない
			}
		private:
			std::map<std::string, TResourcePtr> m_resourceMap;	//!< リソースのマップ
		};
	}
}