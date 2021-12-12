#include "stdafx.h"
#include "AICharacterBase.h"
#include "ModelRender.h"
#include "NaviMesh.h"
#include "PathFinding.h"

namespace nsMyGame
{
	/**
	 * @brief AI�L�����N�^�[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CAICharacterBase::Start()
		{
			// ���N���X�̊J�n����
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			// �h���N���X�̊J�n����
			return StartSub();
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CAICharacterBase::OnDestroy()
		{
			// �h���N���X�̏�������鎞�ɌĂ΂�鏈��
			OnDestroySub();

			// ���N���X�̏�������鎞�ɌĂ΂�鏈��
			DeleteGO(m_modelRender);

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CAICharacterBase::Update()
		{
			// ���N���X�̍X�V����

			// �h���N���X�̍X�V����
			UpdateSub();

			return;
		}

		/**
		 * @brief
		 * @param filePath
		 * @param pos
		 * @param rot
		 * @param scale
		*/
		void CAICharacterBase::InitModel(
			const char* filePath,
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3& scale
		)
		{
			m_modelRender->SetPosition(pos);
			m_modelRender->SetRotation(rot);
			m_modelRender->SetScale(scale);
			m_modelRender->Init(filePath);

			return;
		}

		/**
		 * @brief �p�X��������
		 * @param[in] targetPos �ړ��ڕW���W
		*/
		void CAICharacterBase::PathFinding(const Vector3& targetPos)
		{
			// �p�X����
			m_pathFindingRef->Execute(
				m_path,							// �\�z���ꂽ�p�X�̊i�[��
				*m_naviMeshRef,					// �i�r���b�V��
				m_modelRender->GetPosition() ,	// �J�n���W
				targetPos,						// �ړ��ڕW���W
				PhysicsWorld::GetInstance(),	// �����G���W��	
				50.0f,							// AI�G�[�W�F���g�̔��a
				200.0f							// AI�G�[�W�F���g�̍����B
			);

			m_isEndPathMove = false;

			return;
		}

		/**
		 * @brief �p�X����ړ�����
		 * @param[in] moveSpeed �ړ����x
		 * @param[in] physicsWorld �������[���h�B
		*/
		void CAICharacterBase::MoveOnPath(
			const float moveSpeed,
			const PhysicsWorld* physicsWorld
		)
		{
			if (m_isEndPathMove == true)
			{
				return;
			}

			Vector3 pos = m_path.Move(m_modelRender->GetPosition(), moveSpeed, m_isEndPathMove, physicsWorld);
			m_modelRender->SetPosition(pos);

			return;
		}

	}
}