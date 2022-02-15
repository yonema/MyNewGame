#include "stdafx.h"
#include "BlockingVolume.h"
#include "ModelRender.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �W�I���g���֘A�̃l�[���X�y�[�X
	*/
	namespace nsGeometry
	{
		const char* const CBlockingVolume::m_kBlockingVolumeModelFilePath = 
			"Assets/modelData/preset/blockingVolume.tkm";


		/**
		 * @brief ������
		 * @param[in] pos ���W
		 * @param[in] rot ��]
		 * @param[in] scale �g��
		*/
		void CBlockingVolume::Init(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
		{
			// �u���b�L���O�{�����[���p�̃��f���𐶐�
			nsGraphic::nsModel::CModelRender* model =
				NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			// �g�����X�t�H�[����ݒ�
			model->SetPosition(pos);
			model->SetRotation(rot);
			model->SetScale(scale);

			// ������
			model->Init(m_kBlockingVolumeModelFilePath);

			// ���f������A�u���b�L���O�{�����[���𐶐�
			m_physicsStaticObject = std::make_unique<PhysicsStaticObject>();
			m_physicsStaticObject->CreateFromModel(model->GetModel(), model->GetModel().GetWorldMatrix(), enCollisionAttr_DoesNotCollideCamera);

			// ���f���͂���Ȃ����߁A�j���B
			DeleteGO(model);

			return;
		}

	}
}