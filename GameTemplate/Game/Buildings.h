#pragma once
#include "SwingTarget.h"
#include "BuildingConstData.h"

namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic { namespace nsModel { class CModelRender; } }	// モデルレンダラー

	/**
	 * @brief 建物関連のネームスペース
	*/
	namespace nsBuilding
	{
		/**
		 * @brief 建物のデータ
		*/
		struct SBuildingData
		{
			Vector3 pos = Vector3::Zero;			//!< 座標
			Quaternion rot = Quaternion::Identity;	//!< 回転
			Vector3 scale = Vector3::One;			//!< 拡大率
			std::unique_ptr<nsStringActionTarget::nsSwingTarget::CSwingTarget> swingTarget;	//!< スイングターゲット
			std::unique_ptr<PhysicsStaticObject> staticObject;	//!< 静的物理オブジェクト
		};

		/**
		 * @brief 建物クラス
		*/
		class CBuildings : public IGameObject
		{
		private:	// エイリアス宣言
			using SBuildingDataPtr = std::unique_ptr<SBuildingData>;	// 建物のデータのスマートポインタ
			using SBuildingDataVector = std::vector<SBuildingDataPtr>;	// 建物のデータのコンテナ

		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CBuildings();
			/**
			 * @brief デストラクタ
			*/
			~CBuildings() = default;

		public:		// オーバーライドしたメンバ関数
			
			/**
			 * @brief 破棄した時に呼ばれる関数
			*/
			void OnDestroy() override final;

			/**
			 * @brief アップデート関数
			*/
			void Update() override final;

		public:		// メンバ関数

			/**
			 * @brief 建物を追加する
			 * @param[in] buildingType 建物の種類
			 * @param[in] pos 座標
			 * @param[in] rot 回転
			 * @param[in] scale 拡大率
			*/
			void AddBuliding(
				const nsBuildingConstData::EnBuildingType buildingType,
				const Vector3& pos,
				const Quaternion& rot,
				const Vector3& scale
			);

			/**
			 * @brief 初期化
			*/
			void Init();

		private:	// privateなメンバ関数

			/**
			 * @brief 通常のモデルの初期化
			 * @param[in] buildingType 建物の種類
			*/
			void InitModel(const nsBuildingConstData::EnBuildingType buildingType);

			/**
			 * @brief インスタンシング描画のモデルの初期化
			 * @param[in] buildingType 建物の種類
			 * @param[in] numInstance インスタンスの数
			*/
			void InitModelInstance(
				const nsBuildingConstData::EnBuildingType buildingType,
				const int numInstance
			);

		private:	// データメンバ
			//!< モデルレンダラーのマップ
			std::unordered_map < nsBuildingConstData::EnBuildingType, nsGraphic::nsModel::CModelRender*> m_modelRenderMap;
			//!< 建物のデータのマップ
			std::unordered_map<nsBuildingConstData::EnBuildingType, SBuildingDataVector> m_buildingDataMap;

#ifdef MY_DEBUG
			std::vector<nsGraphic::nsModel::CModelRender*> m_debugModels;	//!< デバック用モデル
#endif

		};
	}
}



