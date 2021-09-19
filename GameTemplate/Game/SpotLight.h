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
		 * @brief スポットライトクラス
		*/
		class CSpotLight : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CSpotLight();
			/**
			 * @brief デストラクタ
			*/
			~CSpotLight();

		public:		//メンバ関数

			/**
			 * @brief ライトデータのポインタにデータを設定する
			 * @param[in] lightData ライトデータ
			*/
			void SetLightDataPtr(nsLightData::SSpotLightData& lightData)
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
			 * @brief ライトの座標を設定
			 * @param[in] pos ライトの座標
			*/
			void SetPosition(const Vector3& pos)
			{
				m_lightDataPtr->pointLightData.position = pos;
			}

			/**
			 * @brief ライトの座標を得る
			 * @return ライトの座標
			*/
			const Vector3& GetPosition() const
			{
				return m_lightDataPtr->pointLightData.position;
			}

			/**
			 * @brief ライトの影響範囲を設定
			 * @param[in] range ライトの影響範囲
			*/
			void SetRange(const float range)
			{
				m_lightDataPtr->pointLightData.range = range;
			}

			/**
			 * @brief ライトのカラーを設定
			 * @param[in] color ライトのカラー
			*/
			void SetColor(const Vector4& color)
			{
				m_lightDataPtr->pointLightData.color = color;
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
			 * @brief ライトの射出角度を設定する
			 * @param angle ライトの射出角度
			*/
			void SetAngle(const float angle)
			{
				m_lightDataPtr->angle = angle;
			}

		private:	//データメンバ
			nsLightData::SSpotLightData* m_lightDataPtr = nullptr;		//!< ライトデータのポインタ
			int m_controlNumber = -1;
		};

	}
}

