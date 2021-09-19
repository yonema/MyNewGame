#pragma once
#include "LightManager.h"

namespace nsMyGame
{
	/**
	 * @brief ライト関連のネームスペース
	*/
	namespace nsLight
	{
		/**
		 * @brief ディレクションライトクラス
		*/
		class CDirectionalLight : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CDirectionalLight();
			/**
			 * @brief デストラクタ
			*/
			~CDirectionalLight();

		public:		// メンバ関数

			/**
			 * @brief ライトデータのポインタにデータを設定する
			 * @param[in] lightData ライトデータ
			*/
			void SetLightDataPtr(nsLightData::SDirectionalLightData& lightData)
			{
				m_lightDataPtr = &lightData;
			}

			/**
			 * @brief 制御ナンバーを設定する
			 * @param[in] controlNumber 制御ナンバー
			*/
			void SetControlNumber(const int controlNumber)
			{
				m_controlNumber = controlNumber;
			}

			/**
			 * @brief 制御ナンバーを得る
			 * @return 制御ナンバー
			*/
			int GetControlNumber() const
			{
				return m_controlNumber;
			}

			/**
			 * @brief ライトの方向を設定する
			 * @param[in] direction ライトの方向
			*/
			void SetDirection(const Vector3& direction)
			{
				m_lightDataPtr->direction = direction;
			}

			/**
			 * @brief ライトが影を生成するか？を設定。0:生成しない、1:生成する
			 * @param[in] castShadow 影を生成するか？
			*/
			void SetCastShadow(const int castShadow)
			{
				m_lightDataPtr->castShadow = castShadow;
			}

			/**
			 * @brief ライトのカラーを設定
			 * @param[in] color ライトのカラー
			*/
			void SetColor(const Vector4& color)
			{
				m_lightDataPtr->color = color;
			}

		private:	// データメンバ
			nsLightData::SDirectionalLightData* m_lightDataPtr = nullptr;	//!< ライトデータのポインタ
			int m_controlNumber = -1;										//!< 制御ナンバー
		};

	}
}