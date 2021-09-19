#include "stdafx.h"
#include "PointLight.h"

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
		CPointLight::CPointLight()
		{
			//���C�g�}�l�[�W���[�Ɏ��g��o�^����
			CLightManager::GetInstance()->AddLight(this);
			return;
		}

		/**
		 * @brief �f�X�g���N�^
		*/
		CPointLight::~CPointLight()
		{
			// ���C�g�}�l�[�W���[���玩�g�̓o�^��j������
			CLightManager::GetInstance()->RemoveLight(this);
			return;
		}
	}
}