#include "stdafx.h"
#include "AIField.h"
#include "AIConstData.h"
#include "ModelRenderConstData.h"
#include "Level3D.h"

namespace nsMyGame
{
	/**
	 * @brief AI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAI
	{
		// AI�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsAIConstData;

		/**
		 * @brief �R���X�g���N�^
		*/
		CAIField::CAIField()
		{
			// ������
			Init();

			return;
		}


		/**
		 * @brief ������
		*/
		void CAIField::Init()
		{
			// �i�r���b�V���̐���
			m_naviMesh.Init(kNaviMeshFilePath);

			// �i�r���b�V���u���b�N�{�����[���̏������f�[�^
			//ModelInitData nmbbModelInitData;
			//nmbbModelInitData.m_tkmFilePath = kNaviMeshBlockBolumeFilePath;
			//// fx�͉��ł������͂�
			//nmbbModelInitData.m_fxFilePath =
			//	nsGraphic::nsModel::nsModelConstData::kTranslucentModelFxFilePath;
			//m_naviMeshBlockBolume.Init(nmbbModelInitData);

			//nsGraphic::nsModel::CModelRender* modelRender = NewGO< nsGraphic::nsModel::CModelRender>(10,"NMBB");
			//modelRender->IniTranslucent(kNaviMeshBlockBolumeFilePath);
			//modelRender->SetAlphaValue(0.5f);

			// NMBB���S�[�X�g�I�u�W�F�N�g�Ƃ��Đ���
			//m_noviMeshBlockGhostObject.CreateMesh(
			//	Vector3::Zero,
			//	Quaternion::Identity,
			//	m_naviMeshBlockBolume,
			//	Matrix::Identity
			//);
			
			// �i�r���b�V���̃^�[�Q�b�g�|�C���g�p�̃��x��
			nsLevel3D::CLevel3D naviMeshTargetPointLevel;
			naviMeshTargetPointLevel.Init(
				kNaviMeshTargetPointLevelFilePath,
				[&](nsLevel3D::SLevelObjectData& objData)
				{
					// �^�[�Q�b�g�|�C���g���i�[���Ă���
					m_naviMeshTargetPoints.emplace_back(objData.position);
					return true;
				}
			);


			m_AICharaInitData.naviMeshRef = &m_naviMesh;
			m_AICharaInitData.pathFindingRef = &m_pathFinding;
			m_AICharaInitData.naviMeshTargetPointsRef = &m_naviMeshTargetPoints;

			return;
		}

	}
}