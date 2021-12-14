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
		 * @brief AI�̃t�B�[���h
		*/
		class CAIField : private nsUtil::Noncopyable
		{
		private:	// �G�C���A�X�錾
			using SAICharacterInitData = nsAICharacter::CAICharacterBase::SAICharacterInitData;
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CAIField();
			/**
			 * @brief �f�X�g���N�^
			*/
			//~CAIField() = default;
			~CAIField() {
				DeleteGO(FindGO<nsGraphic::nsModel::CModelRender>("NMBB"));
			};

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

		private:	// �f�[�^�����o
			SAICharacterInitData m_AICharaInitData;		//!< AI�L�����N�^�[�̏������f�[�^
			nsAI::CNaviMesh m_naviMesh;					//!< �i�r���b�V��
			nsAI::CPathFinding m_pathFinding;			//!< �p�X����
			Model m_naviMeshBlockBolume;				//!< �i�r���b�V�����u���b�N����{�����[��
			PhysicsGhostObject m_noviMeshBlockGhostObject;	//!< �i�r���b�V�����u���b�N����S�[�X�g�I�u�W�F�N�g
			std::vector<Vector3> m_naviMeshTargetPoints;	//!< �i�r���b�V���̃^�[�Q�b�g�|�C���g
		};

	}
}