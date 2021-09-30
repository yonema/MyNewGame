#pragma once
#include "Noncopyable.h"
#include "ModelRender.h"


namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }	// モデルレンダラークラス
	}

	/**
	 * @brief 3Dのレベルデータ関連のネームスペース
	*/
	namespace nsLevel3D
	{
		// 前方宣言
		struct SLevelObjectData;		// レベルオブジェクトデータ

		/**
		 * @brief マップチップクラス
		*/
		class CMapChip : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			 * @param[in] objData レベルオブジェクトデータ
			 * @param[in] filePath ファイルパス
			*/
			CMapChip(const SLevelObjectData& objData, const char* filePath);
			/**
			 * @brief デストラクタ
			*/
			~CMapChip()
			{
				// モデルレンダラーの破棄
				DeleteGO(m_modelRender);
			}

		public:		// メンバ関数

		private:	// データメンバ
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< モデルレンダラー
			PhysicsStaticObject m_physicsStaticObject;					//!< 静的物理オブジェクト。
		};

	}
}