#include "stdafx.h"
#include "MapChip.h"
#include "Level3D.h"

namespace nsNinjaAttract
{
	/**
	 * @brief 3D�̃��x���f�[�^�֘A�̃l�[���X�y�[�X
	*/
	namespace nsLevel3D
	{
		CMapChip::CMapChip(const SLevelObjectData& objData,const char* filePath)
		{
			m_mapChipDataVector.reserve(objData.numMapChipReserve);
			m_physicsStaticObjectPtrVector.reserve(objData.numMapChipReserve);

			// ���f���̃t�@�C���p�X���擾
			m_filePath = std::make_unique<const char*>(filePath);
			// �������`�悩�H���擾
			m_isTranslucent = objData.isTranslucent;
			// �D��x���擾
			m_priority = objData.priority;
			// ���[�U�[��`�̃R���W�����������擾
			m_userIndex = objData.userIndex;
			// LOD�̐؂�ւ��̋������擾
			m_distanceLOD = objData.distanceLOD;

			// LOD�p�̃��f���̃t�@�C���p�X���w�肳��Ă�����A�t�@�C���p�X���擾
			if (objData.lodModelFilePath)
			{
				m_lodModelFilePath = std::make_unique<const char*>(objData.lodModelFilePath);
			}

			// �V���h�E�L���X�^�[���H���擾
			m_shadowCaster = objData.shadowCaster;

			// �}�b�v�`�b�v�f�[�^��ǉ�����
			AddMapChipData(objData);

			return;
		}

		void CMapChip::Init()
		{
#ifdef MY_DEBUG
			// �L���p�V�e�B�m�F�p
			const int cap = static_cast<int>(m_mapChipDataVector.capacity());
#endif
			// �}�b�v�`�b�v�f�[�^�̐�
			const int mapChipDataNum = static_cast<int>(m_mapChipDataVector.size());

			// �}�b�v�`�b�v��`�悷�郂�f�������_���[�̐���
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(m_priority);

			if (mapChipDataNum == 1)
			{
				// ������̂��߁A�ʏ�`��

				// �ʏ�̃��f���̏�����
				InitModel();

				return;
			}

			// �C���X�^���V���O�`��̃��f���̏�����
			InitModelInstancing(mapChipDataNum);



			return;
		}

		/**
		 * @brief �}�b�v�`�b�v�f�[�^�̒ǉ�
		 * @param[in] objData ���x���I�u�W�F�N�g�f�[�^
		*/
		void CMapChip::AddMapChipData(const SLevelObjectData& objData)
		{
			auto mapChipData = std::make_unique< SMapChipData>();
			mapChipData->position = objData.position;
			mapChipData->rotation = objData.rotation;
			mapChipData->scale = objData.scale;
			// �}�b�v�`�b�v�f�[�^�̒ǉ�
			m_mapChipDataVector.emplace_back(std::move(mapChipData));

			return;
		}

		/**
		 * @brief �ʏ�̃��f���̏�����
		*/
		void CMapChip::InitModel()
		{
			// ���f���̏�����
			m_modelRender->SetPosition(m_mapChipDataVector[0]->position);
			m_modelRender->SetRotation(m_mapChipDataVector[0]->rotation);
			m_modelRender->SetScale(m_mapChipDataVector[0]->scale);

			if (m_isTranslucent != true)
			{
				// ���f�������_���[�̏�����
				m_modelRender->Init(*m_filePath.get());
			}
			else
			{
				// �������`��Ń��f�������_���[�̏�����
				m_modelRender->IniTranslucent(*m_filePath.get());
			}

			// LOD�p�̃��f�����w�肳��Ă��邩�H
			if (m_lodModelFilePath)
			{
				// LOD�p�̃��f���̏�����
				m_modelRender->InitLODModel(*m_lodModelFilePath);
				// LOD��L����
				m_modelRender->SetIsEnableLOD(true);
				if (m_distanceLOD > 0.0f)
				{
					// LOD�̐؂�ւ��̋������w�肳��Ă�����A���f�������_���[�ɐݒ肷��
					m_modelRender->SetDistanceLOD(m_distanceLOD);
				}
			}

			auto p = std::make_unique<PhysicsStaticObject>();
			//�ÓI�����I�u�W�F�N�g���쐬�B
			p->CreateFromModel(m_modelRender->GetModel(), m_modelRender->GetModel().GetWorldMatrix(), m_userIndex);
			m_physicsStaticObjectPtrVector.emplace_back(std::move(p));

			return;
		}

		/**
		 * @brief �C���X�^���V���O�`��̃��f���̏�����
		 * @param[in] numInstance �C���X�^���X��
		*/
		void CMapChip::InitModelInstancing(const int numInstance)
		{
			//�C���X�^���V���O�`��p�Ƀ��f�����������B

			if (m_isTranslucent != true)
			{
				// ���f�������_���[�̏�����
				m_modelRender->Init(*m_filePath.get(), nullptr, 0, numInstance);
			}
			else
			{
				// �������`��Ń��f�������_���[�̏�����
				m_modelRender->IniTranslucent(*m_filePath.get(), nullptr, 0, numInstance);
			}

			// LOD�p�̃��f�����w�肳��Ă��邩�H
			if (m_lodModelFilePath)
			{
				// LOD�p�̃��f���̏�����
				m_modelRender->InitLODModel(*m_lodModelFilePath);
				// LOD��L����
				m_modelRender->SetIsEnableLOD(true);
				if (m_distanceLOD > 0.0f)
				{
					// LOD�̐؂�ւ��̋������w�肳��Ă�����A���f�������_���[�ɐݒ肷��
					m_modelRender->SetDistanceLOD(m_distanceLOD);
				}
			}

			// �V���h�E�L���X�^�[���H��ݒ�
			m_modelRender->SetIsShadowCaster(m_shadowCaster);

			for (auto& mapChipData : m_mapChipDataVector)
			{
				//���f�������_�[�̃C���X�^���V���O�p�̃f�[�^���X�V�B
				m_modelRender->UpdateInstancingData(mapChipData->position, mapChipData->rotation, mapChipData->scale);

				//���[���h�s����v�Z����B
				Matrix worldMatrix = m_modelRender->GetModel().CalcWorldMatrix(
					mapChipData->position, mapChipData->rotation, mapChipData->scale
				);
				auto p = std::make_unique<PhysicsStaticObject>();
				//�ÓI�����I�u�W�F�N�g���쐬�B
				p->CreateFromModel(m_modelRender->GetModel(), worldMatrix, m_userIndex);
				m_physicsStaticObjectPtrVector.emplace_back(std::move(p));
			}

			return;
		}


	}
}