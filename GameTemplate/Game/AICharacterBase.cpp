#include "stdafx.h"
#include "AICharacterBase.h"
#include "ModelRender.h"
#include "NaviMesh.h"
#include "PathFinding.h"
#include "GameTime.h"

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

			// �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
			std::random_device rnd;
			// �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h
			m_mt = std::make_unique<std::mt19937>(rnd());
			// [0, 99] �͈͂̈�l����
			m_rand = std::make_unique<std::uniform_int_distribution<>>(0, 99);

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
		 * @param[in] targetPos �p�X�̖ڕW���W
		*/
		void CAICharacterBase::PathFinding(const Vector3& targetPos)
		{
			// �p�X����
			m_pathFindingRef->Execute(
				m_path,							// �\�z���ꂽ�p�X�̊i�[��
				*m_naviMeshRef,					// �i�r���b�V��
				m_modelRender->GetPosition() ,	// �J�n���W
				targetPos,						// �ړ��ڕW���W
				//PhysicsWorld::GetInstance(),	// �����G���W��
				nullptr,
				5.0f,							// AI�G�[�W�F���g�̔��a
				200.0f							// AI�G�[�W�F���g�̍����B
			);

			m_isEndPathMove = false;

			return;
		}

		/**
		 * @brief �^�[�Q�b�g�̒����烉���_���Ńp�X��������
		*/
		void CAICharacterBase::RandomTargetPathFinding()
		{
			const int randTargetNum = (*m_rand)(*m_mt);
			const Vector3 randTargetPos = (*m_naviMeshTargetPointsRef)[randTargetNum];

			PathFinding(randTargetPos);
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

			const Vector3 posBeforeMove = m_modelRender->GetPosition();
			const Vector3 posAfterMove = m_path.Move(
				posBeforeMove,
				moveSpeed * nsTimer::CGameTime().GetFrameDeltaTime(),
				m_isEndPathMove,
				physicsWorld
			);
			Quaternion frontQRot = Quaternion::Identity;


			Rotating(&frontQRot, posBeforeMove, posAfterMove);

			m_modelRender->SetPosition(posAfterMove);
			m_modelRender->SetRotation(frontQRot);

			return;
		}

		/**
		 * @brief �ړ������ɉ�]����
		 * @param[in] frontQRotOut �O�����̉�]�̊i�[��
		 * @param[in] posBefoerMove �ړ��O�̍��W
		 * @param[in] posAfterMove �ړ���̍��W
		*/
		void CAICharacterBase::Rotating(Quaternion* frontQRotOut, const Vector3& posBeforeMove, const Vector3& posAfterMove)
		{
			Vector3 moveDir = posAfterMove - posBeforeMove;
			if (moveDir.LengthSq() <= 1.0f)
			{
				return;
			}
			moveDir.Normalize();
			frontQRotOut->SetRotation(Vector3::Front, moveDir);

			return;
		}

	}
}