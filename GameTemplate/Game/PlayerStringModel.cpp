#include "stdafx.h"
#include "PlayerStringModel.h"
#include "Player.h"
#include "PlayerConstData.h"
#include "ModelRender.h"

namespace nsMyGame
{


	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �v���C���[�̎��N���X�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsPlayerConstData::nsPlayerStringModelConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CPlayerStringModel::Start()
		{
			// ���f�������_���[�̐���
			m_modelRender = NewGO <nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			// ������
			m_modelRender->Init(kStringModelFilePath);

			// ����������
			m_modelRender->Deactivate();

			return true;
		}

		/**
		 * @brief �X�V����
		*/
		void CPlayerStringModel::OnDestroy()
		{
			DeleteGO(m_modelRender);

			return;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CPlayerStringModel::Update()
		{
			// �L�т����Ă��邩�H
			if (m_isStretched == true)
			{
				// �L�т����Ă�
				// �������^�[��
				return;
			}

			// �L�т����ĂȂ�

			// ���f������]�����鏈��
			ModelRotation();
			
			// �L�΂�����

			Vector3 scale = m_modelRender->GetScale();

			scale.z += 50.0f;

			Vector3 distVec = m_toStretchPos - m_playerRef->GetPosition();

			const float distLen = distVec.Length();

			if (scale.z > distLen)
			{
				scale.z = distLen;
			}

			m_modelRender->SetScale(scale);
			m_modelRender->SetPosition(m_playerRef->GetPosition());

			



			return;
		}

		/**
		 * @brief �w�肵�����W�Ɍ������ĐL�т鏈�����J�n����
		 * @param[in] pos �L�т��̍��W
		*/
		void CPlayerStringModel::StartStretchToPos(const Vector3& pos)
		{
			// �L�т����Ă��Ȃ��悤�ɂ���
			m_isStretched = false;

			// �L�т��̍��W��ݒ肷��
			m_toStretchPos = pos;

			// ���g��L��������
			Activate();

			// ���f����L��������
			m_modelRender->Activate();

			// ���f������]�����鏈��
			ModelRotation();

			m_modelRender->SetScale(Vector3::One);
			m_modelRender->SetPosition(m_playerRef->GetPosition());

			return;
		}

		/**
		 * @brief �w�肵�����W�Ɍ������ĐL�т鏈�����I������
		*/
		void CPlayerStringModel::EndStretchToPos()
		{
			// �L�т����Ă���悤�ɂ���
			m_isStretched = true;

			// ���g�𖳌�������
			Deactivate();

			// ���f���𖳌�������
			m_modelRender->Deactivate();

			return;
		}

		/**
		 * @brief ���f������]�����鏈��
		*/
		void CPlayerStringModel::ModelRotation()
		{
			// �L�΂���ւ̕���
			Vector3 toStretchPosDir = m_toStretchPos - m_playerRef->GetPosition();
			// ���K������
			toStretchPosDir.Normalize();

			// �O�����ƐL�΂���ւ̕����̊O�ςŁA��]�������߂�B
			Vector3 rotAxis = Cross(Vector3::Front, toStretchPosDir);
			// ���K������
			rotAxis.Normalize();

			// �O�����ƐL�΂���ւ̕����̓��ς����߂�
			const float dotFrontAndToStretchPosDir = Dot(Vector3::Front, toStretchPosDir);
			// ���ς��烉�W�A���p�����߂�
			const float radAngle = acosf(dotFrontAndToStretchPosDir);

			// ��]�N�H�[�^�j�I��
			Quaternion qRot;
			// �C�ӂ̎�����ŉ�]������
			qRot.SetRotation(rotAxis, radAngle);

			// ���f���ɉ�]��ݒ肷��
			m_modelRender->SetRotation(qRot);

			return;
		}



	}
}