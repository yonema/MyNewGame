#pragma once
#include "VectorRender.h"

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
			static CDebugManager* m_instance;

		public:		// メンバ関数
			void AddVector(const CVectorRender::SVectorRenderData& vectorRenderData)
			{
				m_vectorRender->AddVector(vectorRenderData);
			}

			/**
			 * @brief 
			*/
			void PreDelete();

		private:	// データメンバ
			CVectorRender* m_vectorRender = nullptr;
		};



		static inline void DrawVector(const CVectorRender::SVectorRenderData& vectorRenderData)
		{
			CDebugManager::GetInstance()->AddVector(vectorRenderData);
		}

		static inline void DrawVector(const Vector3 origin, const Vector3 vector, const std::string name)
		{
			CVectorRender::SVectorRenderData vectorRenderData =
			{
				name,
				vector,
				origin,
			};

			CDebugManager::GetInstance()->AddVector(vectorRenderData);
		}
	}
}