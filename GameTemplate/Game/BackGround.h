#pragma once
#include "Noncopyable.h"
#include "MapConstDatah.h"

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsLevel3D { class CLevel3D; }			// 3Dレベルクラス
	namespace nsNature { class CSkyCube; }			// スカイクラス
	namespace nsBuilding { class CBuildings; }		// 建物クラス


	/**
	 * @brief 背景ステージ用のネームスペース
	*/
	namespace nsBackGround
	{
		/**
		 * @brief 背景ステージクラス
		*/
		class CBackGround : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CBackGround();
			/**
			 * @brief デストラクタ
			*/
			~CBackGround();

		public:		// メンバ関数

			/**
			 * @brief タイトル画面か？を設定
			 * @param[in] isTitle タイトル画面か？
			*/
			void SetIsTitle(const bool isTitle);

		private:	// privateなメンバ関数

			/**
			 * @brief レベルをロードする
			*/
			void LoadLevel3D();

		private:	// データメンバ
			std::unique_ptr<nsLevel3D::CLevel3D> m_buildingLevel3D;	//!< 建物用3Dレベルクラス
			//!< 小物用3Dレベルクラス
			std::unique_ptr <nsLevel3D::CLevel3D> m_propsLevel3D[nsMaps::nsMapConstData::enPropsTypeNum];
			nsNature::CSkyCube* m_skyCube = nullptr;		//!< スカイキューブ
			nsBuilding::CBuildings* m_buildings = nullptr;	//!< 建物クラス

		};
	}
}


