#include "stdafx.h"
#include "PlayerStringModel.h"
#include "Player.h"
#include "PlayerConstData.h"
#include "ModelRender.h"
#include "GameTime.h"

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

			// ���f������]�����鏈��
			ModelRotation();
			
			// �L�΂�����

			// �g�嗦���擾
			Vector3 scale = m_modelRender->GetScale();
			// Z�����i�O�����j�Ɋg�傷��
			scale.z += m_stretchSpeed;
			// �L�т��ւ̋����x�N�g��
			const Vector3 distVec = m_toStretchPos - m_playerRef->GetPosition();
			// �L�т��ւ̋����̑傫��
			const float distLen = distVec.Length();
			// �g�嗦���A�L�т��ւ̋����̑傫�����傫�����H
			if (scale.z > distLen)
			{
				// �傫��
				// �g�嗦�������ɍ��킹��
				scale.z = distLen;
				// �L�т�����
				m_isStretched = true;
			}

			// ���f���̊g�嗦�ƍ��W��ݒ肷��
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

			// �L�т��ւ̃x�N�g��
			Vector3 toStretchVec = m_toStretchPos - m_playerRef->GetPosition();
			// �L�т鑬�x��ݒ肷��
			m_stretchSpeed =
				toStretchVec.Length() * nsTimer::CGameTime().GetFrameDeltaTime() / kStretchedTime;

			// ���g��L��������
			Activate();

			// ���f����L��������
			m_modelRender->Activate();

			// ���f������]�����鏈��
			ModelRotation();

			// ���f���̊g�嗦��������
			m_modelRender->SetScale(Vector3::One);
			// ���f���̍��W��ݒ肷��
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