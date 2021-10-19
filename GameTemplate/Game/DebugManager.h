#pragma once
#include "VectorRender.h"
#include "DebugConstData.h"
#include "Noncopyable.h"

namespace nsMyGame
{
	/**
	 * @brief デバック関連のネームスペース
	*/
	namespace nsDebug
	{

		/**
		 * @brief デバックのマネージャー
		 * @note シングルトンパターンを使用
		*/
		class CDebugManager : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CDebugManager();
			/**
			 * @brief デストラクタ
			*/
			~CDebugManager();

		public:		// staticなメンバ関数

			/**
			 * @brief インスタンスを生成
			*/
			static void CreateInstance()
			{
				m_instance = new CDebugManager;
			}

			/**
			 * @brief インスタンスを破棄
			*/
			static void DeleteInstance()
			{
				if (m_instance)
					delete m_instance;
			}

			/**
			 * @brief インスタンスを取得
			 * @return インスタンス
			*/
			static CDebugManager* GetInstance()
			{
				return m_instance;
			}

		private:	// staticなデータメンバ
			static CDebugManager* m_instance;		//!< インスタンス

		public:		// メンバ関数

			/**
			 * @brief 描画するベクトルを追加する
			 * @param[in] vectorRenderData ベクトルの描画データ
			*/
			void AddVector(const CVectorRender::SVectorRenderData& vectorRenderData)
			{
#ifdef MY_DEBUG
				m_vectorRender->AddVector(vectorRenderData);
#endif
			}

			/**
			 * @brief ベクトルのフォントの拡大率を設定
			 * @param[in] scale 拡大率
			*/
			void SetVectorFontScale(const float scale)
			{
#ifdef MY_DEBUG
				m_vectorRender->SetFontScale(scale);
#endif
			}

			/**
			 * @brief 事前の破棄処理
			*/
			void PreDelete();

		private:	// データメンバ
			CVectorRender* m_vectorRender = nullptr;

		};


		/**
		 * @brief ベクトルを描画する（ベクトル描画データから）
		 * @param[in] vectorRenderData ベクトル描画データ
		*/
		static inline void DrawVector(const CVectorRender::SVectorRenderData& vectorRenderData)
		{
			CDebugManager::GetInstance()->AddVector(vectorRenderData);
		}

		/**
		 * @brief ベクトルを描画する（基点、ベクトル、名前から）
		 * @param[in] origin 基点
		 * @param[in] vector ベクトル
		 * @param[in] name ベクトルお名前
		*/
		static inline void DrawVector(
			const Vector3 origin,
			const Vector3 vector,
			const std::string name = nsDebugConstData::kDefaultVectorName
		)
		{
			CVectorRender::SVectorRenderData vectorRenderData =
			{ name,vector,origin, };

			CDebugManager::GetInstance()->AddVector(vectorRenderData);
		}


	}
}