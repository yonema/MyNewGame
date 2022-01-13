#include "stdafx.h"
#include "Goal.h"
#include "GoalConstData.h"
#include "ModelRender.h"
#include "GameMainState.h"
#include "Player.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �S�[���֘A�̃l�[���X�y�[�X
	*/
	namespace nsGoal
	{
		// �S�[���N���X�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsGoalConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CGoal::Start()
		{
			return true;
		}

		/**
		 * @brief �X�V����
		*/
		void CGoal::OnDestroy()
		{
			DeleteGO(m_goalMR);

			return;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CGoal::Update()
		{
			// �N���N����]��������
			//Rotationg();

			// �S�[�����Ă��邩�𒲂ׂ�
			//CheckIsGoal();

			return;
		}


		/**
		 * @brief ������
		 * @param[in] pos ���W
		 * @param[in] rot ��]
		 * @param[in] scale �g�嗦
		 * @param[in] player �v���C���[
		*/
		void CGoal::Init(
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3 scale,
			nsPlayer::CPlayer& player
		)
		{
			// �S�[���̃��f�������_���[�𐶐�
			m_goalMR = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			// �g�����X�t�H�[����ݒ�
			//m_goalMR->SetPosition(pos);
			m_goalMR->SetPosition({1000.0f,10.0f,0.0f});
			m_goalMR->SetRotation(rot);
			m_goalMR->SetScale(scale);
			// ������
			m_goalMR->Init(kGoalModelFilePath);

			// �v���C���[���Z�b�g
			m_player = &player;

			return;
		}

		/**
		 * @brief �N���N����]��������
		*/
		void CGoal::Rotationg()
		{
			m_rotateAngle += kRotateSpeed;
			if (m_rotateAngle > 360.0f)
			{
				m_rotateAngle -= 360.0f;
			}

			Quaternion rotatingQRot = Quaternion::Identity;
			rotatingQRot.SetRotationDegY(m_rotateAngle);
			Quaternion tiltQRot = Quaternion::Identity;
			tiltQRot.SetRotationDegZ(kTiltAngle);

			m_goalMR->SetRotation(rotatingQRot * tiltQRot);

			return;
		}

		/**
		 * @brief �S�[�����Ă��邩�𒲂ׂ�
		*/
		void CGoal::CheckIsGoal()
		{
			// �v���C���[�ւ̃x�N�g��
			Vector3 toPlayerVec = m_player->GetPosition() - m_goalMR->GetPosition();

			// �v���C���[�Ƃ̋������S�[���̂������l�ȉ����H
			if (toPlayerVec.Length() <= kGoalThreshold)
			{
				// �S�[���̂������l�ȉ�
				// �S�[��
				nsGameState::GameMainState()->Goal();
			}

			return;
		}
	}
}