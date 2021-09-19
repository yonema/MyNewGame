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
		 * @brief ポイントライトクラス
		*/
		class CPointLight : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPointLight();
			/**
			 * @brief デストラクタ
			*/
			~CPointLight();

		public:		//メンバ関数

			/**
			 * @brief ライトデータのポインタにデータを設定する
			 * @param[in] lightData ライトデータ
			*/
			void SetLightDataPtr(nsLightData::SPointLightData& lightData)
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
				m_lightDataPtr->position = pos;
			}

			/**
			 * @brief ライトの座標を得る
			 * @return ライトの座標
			*/
			const Vector3& GetPosition() const
			{
				return m_lightDataPtr->position;
			}

			/**
			 * @brief ライトの影響範囲を設定
			 * @param[in] range ライトの影響範囲
			*/
			void SetRange(const float range) 
			{
				m_lightDataPtr->range = range;
			}

			/**
			 * @brief ライトのカラーを設定
			 * @param[in] color ライトのカラー
			*/
			void SetColor(const Vector4& color)
			{
				m_lightDataPtr->color = color;
			}

		private:	//データメンバ
			nsLightData::SPointLightData* m_lightDataPtr = nullptr;		//!< ライトデータのポインタ
			int m_controlNumber = -1;									//!< 制御ナンバー
		};

	}
}