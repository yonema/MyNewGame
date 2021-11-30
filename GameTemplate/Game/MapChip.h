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
		 * @brief マップチップのデータ
		*/
		struct SMapChipData
		{
			Vector3 position;		//!< 座標。
			Vector3 scale;			//!< 大きさ。
			Quaternion rotation;	//!< 回転。
			//std::unique_ptr<PhysicsStaticObject> physicsStaticObject;
		};

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

			/**
			 * @brief 初期化処理
			*/
			void Init();

			/**
			 * @brief マップチップデータの追加
			 * @param[in] objData レベルオブジェクトデータ
			*/
			void AddMapChipData(const SLevelObjectData& objData);

		private:	// privateなメンバ関数

			/**
			 * @brief 通常のモデルの初期化
			*/
			void InitModel();

			/**
			 * @brief インスタンシング描画のモデルの初期化
			 * @param[in] numInstance インスタンス数
			*/
			void InitModelInstancing(const int numInstance);

		private:	// データメンバ
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< モデルレンダラー
			std::vector< std::unique_ptr<SMapChipData>> m_mapChipDataVector;	//!< マップチップデータのリスト
			std::unique_ptr<const char*> m_filePath;					//!< tkmファイルパス
			//!< 静的物理オブジェクトの配列。
			std::vector<std::unique_ptr<PhysicsStaticObject>> m_physicsStaticObjectPtrVector;
			bool m_isTranslucent = false;								//!< 半透明描画か？
			int m_priority = nsCommonData::enPriorityFirst;				//!< 優先度
			//!< ユーザー定義のコリジョン属性
			EnCollisionAttr m_userIndex = EnCollisionAttr::enCollisionAttr_None;

		};

	}
}