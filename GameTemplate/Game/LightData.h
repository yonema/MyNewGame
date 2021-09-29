#pragma once
#include "LightConstData.h"
#include "ShadowConstDatah.h"

namespace nsMyGame
{
	/**
	 * @brief ライト関連のネームスペース
	*/
	namespace nsLight
	{
		/**
		 * @brief ライトのデータのネームスペース
		*/
		namespace nsLightData
		{
			//ライトの定数データを使用可能にする
			using namespace nsLightConstData;

			/**
			 * @brief ディレクションライトのデータ
			*/
			struct SDirectionalLightData
			{
				Vector3 direction = GetDefaultDirection();		//!< ライトの方向
				int castShadow = 1;								//!< 影を生成するか？0:生成しない、1:生成する
				Vector4 color = kDefaultDirectionalLightColor;	//!< ライトのカラー
			};

			/**
			 * @brief ポイントライトのデータ
			*/
			struct SPointLightData
			{
				Vector3 position = Vector3::Zero;			//!< ライトの座標
				float range = kDefaultPointLightRange;		//!< ライトの影響範囲
				Vector4 color = kDefaultPointLightColor;	//!< ライトのカラー
			};

			/**
			 * @brief スポットライトのデータ
			*/
			struct SSpotLightData
			{
				SPointLightData pointLightData;				//!< ポイントライトのデータ
				Vector3 direction = GetDefaultDirection();	//!< ライトの射出方向
				float angle = kDefaultSpotLightAngle;		//!< ライトの射出角度（radian単位）
			};

			/**
			 * @brief ライトのデータ
			*/
			struct SLightData
			{
				SDirectionalLightData directionalLightData[kMaxDirectionalLightNum];	//!< ディレクションライトのデータ
				SPointLightData pointLightData[kMaxPointLightNum];	//!< ポイントライトのデータ
				SSpotLightData spotLightData[kMaxSpotLightNum];		//!< スポットライトのデータ
				Vector3 eyePos = Vector3::Zero;						//!< 視点
				int directionalLightNum = 0;						//!< ディレクションライトの数
				Vector3 ambientLight = GetDefaultAmbientLight();	//!< アンビエントライト
				int pointLightNum = 0;								//!< ポイントライトの数
				//!< ライトビュープロジェクション行列
				Matrix mlvp[kMaxDirectionalLightNum][nsGraphic::nsShadow::nsShadowConstData::enShadowMapArea_num];
				int spotLightNum = 0;								//!< スポットライトの数
			};
		}
	}
}
