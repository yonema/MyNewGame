#include "stdafx.h"
#include "DirectionalLight.h"

namespace nsNinjaAttract
{
	/**
	 * @brief ライト関連のネームスペース
	*/
	namespace nsLight
	{
		/**
		 * @brief コンストラクタ
		*/
		CDirectionalLight::CDirectionalLight()
		{
			//ライトマネージャーに自身を登録する
			CLightManager::GetInstance()->AddLight(this);
			return;
		}

		/**
		 * @brief デストラクタ
		*/
		CDirectionalLight::~CDirectionalLight()
		{
			// ライトマネージャーから自身の登録を破棄する
			CLightManager::GetInstance()->RemoveLight(this);
			return;
		}
	}
}