#include "stdafx.h"
#include "MapChip.h"
#include "Level3D.h"

namespace nsMyGame
{
	/**
	 * @brief 3D�̃��x���f�[�^�֘A�̃l�[���X�y�[�X
	*/
	namespace nsLevel3D
	{
		CMapChip::CMapChip(const SLevelObjectData& objData, const char* filePath)
		{
			// �}�b�v�`�b�v��`�悷�郂�f�������_���[�̐���
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			// �g�����X�t�H�[���Ɛݒ�
			m_modelRender->SetPosition(objData.position);
			m_modelRender->SetRotation(objData.rotation);
			m_modelRender->SetScale(objData.scale);

			// ���f�������_���[�̏�����
			m_modelRender->Init(filePath);

			//�ÓI�����I�u�W�F�N�g���쐬�B
			m_physicsStaticObject.CreateFromModel(
				m_modelRender->GetModel(),
				m_modelRender->GetModel().GetWorldMatrix()
			);

			return;
		}
	}
}