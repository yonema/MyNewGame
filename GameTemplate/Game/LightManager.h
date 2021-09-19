#pragma once
#include "LightData.h"
#include "Noncopyable.h"

namespace nsMyGame
{
	/**
	 * @brief ライト関連のネームスペース
	*/
	namespace nsLight
	{
		// 前方宣言
		class CDirectionalLight;	// ディレクションライト
		class CPointLight;			// ポイントライト
		class CSpotLight;			// スポットライト

		/**
		 * @brief ライト管理クラス
		 * @note シングルトンパターンを使用
		*/
		class CLightManager : private nsUtil::Noncopyable
		{
		private:	// コンストラクタとデストラクタ。
					// シングルトンパターンのためprivateに隠す
			/**
			 * @brief コンストラクタ
			*/
			CLightManager();
			/**
			 * @brief デストラクタ
			*/
			~CLightManager();

		public:		// メンバ関数

			/**
			 * @brief アップデート関数。ゲームループで呼んでください。
			*/
			void Update();

			/**
			 * @brief ディレクションライトを追加する
			 * @param[in,out] light 追加するディレクションライト
			*/
			void AddLight(CDirectionalLight* light);

			/**
			 * @brief ポイントライトを追加する
			 * @param[in,out] light 追加するポイントライト
			*/
			void AddLight(CPointLight* light);

			/**
			 * @brief スポットライトを追加する
			 * @param[in,out] light 追加するスポットライト
			*/
			void AddLight(CSpotLight* light);

			/**
			 * @brief ディレクションライトを破棄する
			 * @param[in] light 破棄するディレクションライト
			*/
			void RemoveLight(const CDirectionalLight* light);

			/**
			 * @brief ポイントライトを破棄する
			 * @param[in] light 破棄するポイントライト
			*/
			void RemoveLight(const CPointLight* light);

			/**
			 * @brief スポットライトを破棄する
			 * @param[in] light 破棄するスポットライト
			*/
			void RemoveLight(const CSpotLight* light);

			/**
			 * @brief ライト達のデータの参照を得る
			 * @return ライト達のデータ
			*/
			nsLightData::SLightData& GetLightData()
			{
				return m_lightData;
			}

		public:		// staticなメンバ関数
			/**
			 * @brief 唯一のインスタンスを生成する
			 * @note シングルトンパターン
			*/
			static void CreateInstance()
			{
				m_instance = new CLightManager;
			}

			/**
			 * @brief 唯一のインスタンスを破棄する
			 * @note シングルトンパターン
			*/
			static void DeleteInstance()
			{
				if (m_instance)
					delete m_instance;
			}

			/**
			 * @brief 唯一のインスタンスの参照を得る
			 * @return 唯一のインスタンスの参照
			 * @note シングルトンパターン
			*/
			static CLightManager* GetInstance()
			{
				return m_instance;
			}

		private:	// データメンバ
			nsLightData::SLightData m_lightData;	//!< ライト達のデータ
			CDirectionalLight* m_directionalLightPtrs[nsLightConstData::kMaxDirectionalLightNum] = {};	//!< それぞれのディレクションライトへのポインタ
			CPointLight* m_pointLightPtrs[nsLightConstData::kMaxPointLightNum] = {};	//!< それぞれのポイントライトへのポインタ
			CSpotLight* m_spotLightPtrs[nsLightConstData::kMaxSpotLightNum] = {};		//!< それぞれのスポットライトへのポインタ

		private:	// staticなデータメンバ
			static CLightManager* m_instance;		//!< 唯一のインスタンス
		};
	}
}
