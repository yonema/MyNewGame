#include "stdafx.h"
#include "PlayerStringModel.h"
#include "Player.h"
#include "PlayerConstData.h"
#include "GameTime.h"

namespace nsMyGame
{


	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �v���C���[�̎��N���X�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsPlayerConstData::nsStringModelConstData;

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

			// ���f�������_���[�̐���
			m_tipModelRender = NewGO <nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			// ������
			m_tipModelRender->Init(kStringModelFilePath);
			// ����������
			m_tipModelRender->Deactivate();

			return true;
		}

		/**
		 * @brief �X�V����
		*/
		void CPlayerStringModel::OnDestroy()
		{
			DeleteGO(m_modelRender);
			DeleteGO(m_tipModelRender);

			return;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CPlayerStringModel::Update()
		{
			// �L�т�J�n���W�̌v�Z
			m_startStretchPos = m_playerRef->GetPosition();
			m_startStretchPos.y += kStartStretchHeight;

			// ���f������]�����鏈��
			ModelRotation();
			
			// �L�΂�����

			// �L�΂����𑝂₷
			if (m_isStretched != true)
			{
				m_stretchRate += nsTimer::GameTime().GetFrameDeltaTime() / kStretchedTime;
			}

			if (m_stretchRate >= 1.0f)
			{
				// �L�т�����
				m_isStretched = true;
				// �ő嗦������ɂ���
				m_stretchRate = 1.0f;
			}

			// �L�΂����W�ւ̃x�N�g��
			const Vector3 toStretchPosVec = m_toStretchPos - m_startStretchPos;
			// �L�΂����W�ւ̋���
			const float toStretchPosDist = toStretchPosVec.Length();
			// �L�΂����ŁA�L�΂����������߂�
			float stretchLength = Math::Lerp<float>(m_stretchRate, 0.0f, toStretchPosDist);

			// �茳���̃��f���̊g�嗦
			float atHandModelScale = stretchLength;

			// �茳���̃��f���̊g�嗦���ő�l���傫�����H
			if (atHandModelScale > 500.0f)
			{
				// �ő�l�ɌŒ肷��
				atHandModelScale = 500.0f;

				// ��[���̃��f����L��������
				m_tipModelRender->Activate();

				// ��[���̃��f���̊g�嗦���v�Z
				float tipModelScale = stretchLength - atHandModelScale;
				m_tipModelRender->SetScale({ 1.0f,1.0f,tipModelScale });

				Vector3 toTipVec = Vector3::Front;
				m_tipModelRender->GetRotation().Apply(toTipVec);
				toTipVec.Normalize();
				toTipVec.Scale(500.0f - 60.0f);
				m_tipModelRender->SetPosition(m_startStretchPos + toTipVec);
			}

			// ���f���̊g�嗦�ƍ��W��ݒ肷��
			m_modelRender->SetScale({1.0f,1.0f,atHandModelScale });
			m_modelRender->SetPosition(m_startStretchPos);
						
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
			Vector3 toStretchVec = m_toStretchPos - m_startStretchPos;
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
			m_modelRender->SetPosition(m_startStretchPos);

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

			m_tipModelRender->Deactivate();

			m_stretchRate = 0.0f;

			return;
		}

		/**
		 * @brief ���f������]�����鏈��
		*/
		void CPlayerStringModel::ModelRotation()
		{
			// �L�΂���ւ̕���
			Vector3 toStretchPosDir = m_toStretchPos - m_startStretchPos;
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

			m_tipModelRender->SetRotation(qRot);

			return;
		}



	}
}