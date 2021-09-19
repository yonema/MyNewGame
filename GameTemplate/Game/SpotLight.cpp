#include "stdafx.h"
#include "SpotLight.h"

namespace nsMyGame
{
	/**
	 * @brief ���C�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsLight
	{
		/**
		 * @brief �R���X�g���N�^
		*/
		CSpotLight::CSpotLight()
		{
			//���C�g�}�l�[�W���[�Ɏ��g��o�^����
			CLightManager::GetInstance()->AddLight(this);
			return;
		}

		/**
		 * @brief �f�X�g���N�^
		*/
		CSpotLight::~CSpotLight()
		{
			// ���C�g�}�l�[�W���[���玩�g�̓o�^��j������
			CLightManager::GetInstance()->RemoveLight(this);
			return;
		}
	}
}