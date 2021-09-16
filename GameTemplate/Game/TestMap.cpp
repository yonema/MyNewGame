#include "stdafx.h"
#include "TestMap.h"
#include "ModelRender.h"

namespace nsMyGame
{
	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		/**
		 * @brief �X�^�[�g�֐�
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CTestMap::Start()
		{
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			m_modelRender->Init("Assets/modelData/unityChan.tkm");

			return true;
		}

		/**
		 * @brief �j���������ɌĂ΂��
		*/
		void CTestMap::OnDestroy()
		{
			DeleteGO(m_modelRender);
			return;
		}

		/**
		 * @brief �A�b�v�f�[�g�֐�
		*/
		void CTestMap::Update()
		{
			return;
		}
	}
}