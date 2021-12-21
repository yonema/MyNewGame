#pragma once
#include "Noncopyable.h"
#include "NaviMesh.h"
#include "PathFinding.h"
#include "AICharacterBase.h"
#include "ModelRender.h"

namespace nsMyGame
{
	/**
	 * @brief AI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAI
	{
		/**
		 * @brief AI�̏������f�[�^
		*/
		struct SAICharacterInitData
		{
			const nsAI::CNaviMesh* naviMeshRef = nullptr;
			nsAI::CPathFinding* pathFindingRef = nullptr;
			std::vector<Vector3>* naviMeshTargetPointsRef = nullptr;
			const std::vector<Vector3>* wayPointsRef = nullptr;
		};

		/**
		 * @brief AI�̃t�B�[���h
		*/
		class CAIField : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CAIField();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CAIField() = default;

		public:		// �����o�֐�

			/**
			 * @brief AI�L�����N�^�[�̏������f�[�^�𓾂�
			 * @return AI�L�����N�^�[�̏������f�[�^
			*/
			SAICharacterInitData& GetAICharaInitData()
			{
				return m_AICharaInitData;
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief ������
			*/
			void Init();

			/**
			 * @brief �E�F�C�|�C���g�̏�����
			*/
			void InitWayPoints();

		private:	// �f�[�^�����o
			SAICharacterInitData m_AICharaInitData;		//!< AI�L�����N�^�[�̏������f�[�^
			nsAI::CNaviMesh m_naviMesh;					//!< �i�r���b�V��
			nsAI::CPathFinding m_pathFinding;			//!< �p�X����
			std::vector<Vector3> m_naviMeshTargetPoints;	//!< �i�r���b�V���̃^�[�Q�b�g�|�C���g
			std::vector<Vector3> m_wayPoints;			//!< �E�F�C�|�C���g

		};

	}
}