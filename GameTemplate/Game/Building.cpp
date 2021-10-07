#include "stdafx.h"
#include "Building.h"
#include "ModelRender.h"


namespace nsMyGame
{
	/**
	 * @brief �����֘A�̃l�[���X�y�[�X
	*/
	namespace nsBuilding
	{
		// �����֘A�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsBuildingConstData;

		/**
		 * @brief �R���X�g���N�^
		*/
		CBuilding::CBuilding()
		{
			// ���f�������_���[�����������Ă���
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			return;
		}


		/**
		 * @brief �j���������ɌĂ΂��֐�
		*/
		void CBuilding::OnDestroy()
		{
			DeleteGO(m_modelRender);

#ifdef MY_DEBUG
			for (auto& debugModel : m_debugModels)
			{
				DeleteGO(debugModel);
			}
#endif

			return;
		}

		/**
		 * @brief �A�b�v�f�[�g�֐�
		*/
		void CBuilding::Update()
		{
			return;
		}


		/**
		 * @brief ������
		 * @param[in] tkmFilePath tkm�t�@�C���p�X
		 * @param[in] pos ���W
		 * @param[in] rot ��]
		 * @param[in] scale �g�嗦
		*/
		void CBuilding::Init(
			const char* tkmFilePath,
			const Vector3& pos,
			const Quaternion rot,
			const Vector3& scale
		)
		{
			// ���f�������_���[�̃g�����X�t�H�[�����X�V
			m_modelRender->SetPosition(pos);
			m_modelRender->SetRotation(rot);
			m_modelRender->SetScale(scale);

			// ���f�������_���[��������
			m_modelRender->Init(tkmFilePath);

			// �X�C���O�^�[�Q�b�g��������
			m_swingTarget.Init(*m_modelRender);

			// �X�C���O�^�[�Q�b�g�̍��W���v�Z����
			m_swingTarget.CalcSwingingTargetPositions(
				kDefaultWidthSegments,
				kDefaultHeightSegments,
				kDefaultLengthSegments
			);
#ifdef TRUE
#ifdef MY_DEBUG

			// �v�Z���ꂽ�X�C���O�^�[�Q�b�g�̍��W�R���e�i���擾
			const std::vector<Vector3>& swingTargetPositions = m_swingTarget.GetSwingTargetPositions();

			// �X�C���O�^�[�Q�b�g�̍��W�ԍ�
			int stPosNo = 0;
			// �X�C���O�^�[�Q�b�g�̍��W�̐������L���p�V�e�B���m��
			m_debugModels.reserve(swingTargetPositions.size());
			// �S�Ă̍��W�𒲂ׂ�
			for (auto& stPos : swingTargetPositions)
			{
				nsGraphic::nsModel::CModelRender* debugModel = 
					NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
				debugModel->SetPosition(swingTargetPositions[stPosNo++]);
				debugModel->Init("Assets/modelData/debugModel/testBox.tkm");
				m_debugModels.emplace_back(debugModel);

			}
#endif
#endif

			return;
		}

	}
}