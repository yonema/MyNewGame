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
			// �}�b�v�`�b�v��`�悷�郂�f�������_���[�̐����Ə�����
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			m_modelRender->Init(filePath);

			// ���f�������_���[�̍X�V
			m_modelRender->SetPosition(objData.position);
			m_modelRender->SetRotation(objData.rotation);
			m_modelRender->SetScale(objData.scale);

			// �e��s��
			Matrix mWorld, mTrans, mRotation, mScale;
			mWorld = m_modelRender->GetModel().GetWorldMatrix();
			mTrans.MakeTranslation(objData.position);
			mRotation.MakeRotationFromQuaternion(objData.rotation);
			mScale.MakeScaling(objData.scale);

			// �S������
			mWorld = mWorld * mScale * mRotation * mTrans;

			//�ÓI�����I�u�W�F�N�g���쐬�B
			m_physicsStaticObject.CreateFromModel(
				m_modelRender->GetModel(),
				/*m_modelRender->GetModel().GetWorldMatrix()*/mWorld
			);

			return;
		}
	}
}