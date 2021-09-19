#include "stdafx.h"
#include "SpotLight.h"

namespace nsMyGame
{
	/**
	 * @brief ライト関連のネームスペース
	*/
	namespace nsLight
	{
		/**
		 * @brief コンストラクタ
		*/
		CSpotLight::CSpotLight()
		{
			//ライトマネージャーに自身を登録する
			CLightManager::GetInstance()->AddLight(this);
			return;
		}

		/**
		 * @brief デストラクタ
		*/
		CSpotLight::~CSpotLight()
		{
			// ライトマネージャーから自身の登録を破棄する
			CLightManager::GetInstance()->RemoveLight(this);
			return;
		}
	}
}