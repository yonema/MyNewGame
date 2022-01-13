#include "stdafx.h"
#include "AIField.h"
#include "AIConstData.h"
#include "ModelRenderConstData.h"
#include "Level3D.h"

namespace nsNinjaAttract
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

			// �E�F�C�|�C���g�̏�����
			InitWayPoints();

			m_AICharaInitData.naviMeshRef = &m_naviMesh;
			m_AICharaInitData.pathFindingRef = &m_pathFinding;
			m_AICharaInitData.naviMeshTargetPointsRef = &m_naviMeshTargetPoints;
			m_AICharaInitData.wayPointsRef = &m_wayPoints;

			return;
		}

		/**
		 * @brief �E�F�C�|�C���g�̏�����
		*/
		void CAIField::InitWayPoints()
		{
			// �E�F�C�|�C���g�̐��������T�[�u
			m_wayPoints.reserve(kWayPointNum);

			// �E�F�C�|�C���g�̃��x���f�[�^�𐶐�
			nsLevel3D::CLevel3D wayPointLevel;
			wayPointLevel.Init(
				kWayPointFilePath,
				[&](nsLevel3D::SLevelObjectData& objData)
				{
					// ���x���f�[�^������W�����o��
					m_wayPoints.emplace_back(objData.position);
					return true;
				}
			);

			return;
		}

	}
}