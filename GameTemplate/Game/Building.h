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
		 * @brief 建物クラス
		*/
		class CBuilding : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CBuilding();
			/**
			 * @brief デストラクタ
			*/
			~CBuilding() = default;

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
			 * @brief 初期化
			 * @param[in] tkmFilePath tkmファイルパス
			 * @param[in] pos 座標
			 * @param[in] rot 回転
			 * @param[in] scale 拡大率
			*/
			void Init(
				const char* tkmFilePath,
				const Vector3& pos,
				const Quaternion rot = Quaternion::Identity,
				const Vector3& scale = Vector3::One
			);

		private:	// データメンバ
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;			//!< モデルレンダラー
			nsStringActionTarget::nsSwingTarget::CSwingTarget m_swingTarget;	//!< スイングターゲット

#ifdef MY_DEBUG
			std::vector<nsGraphic::nsModel::CModelRender*> m_debugModels;	//!< デバック用モデル
#endif

		};
	}
}



