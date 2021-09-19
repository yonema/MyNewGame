#include "stdafx.h"
#include "PointLight.h"

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
		CPointLight::CPointLight()
		{
			//ライトマネージャーに自身を登録する
			CLightManager::GetInstance()->AddLight(this);
			return;
		}

		/**
		 * @brief デストラクタ
		*/
		CPointLight::~CPointLight()
		{
			// ライトマネージャーから自身の登録を破棄する
			CLightManager::GetInstance()->RemoveLight(this);
			return;
		}
	}
}