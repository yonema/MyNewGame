#include "stdafx.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "PlayerConstData.h"

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �v���C���[�J�����̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsPlayerConstData::nsPlayerCameraConstData;

		/**
		 * @brief ������
		 * @param[in] player �v���C���[�̎Q��
		*/
		void CPlayerCamera::Init(const CPlayer& player)
		{
			// �J�������Z�b�g
			m_camera = g_camera3D;

			// �J�����̉����ʂ�ݒ�
			m_camera->SetFar(kDefaultCameraFar);

			// �f�t�H���g�̒����_���王�_�ւ̃x�N�g����ݒ�
			m_toCameraVec = kDefaultToCameraVec;

			// �v���C���[�̍��W�̎Q�Ƃ��Z�b�g
			m_playerRef = &player;

			// �J�����̃X�^�[�g�̎��_�ƒ����_��ݒ�
			m_camera->SetPosition(m_playerRef->GetPosition() + m_toCameraVec);
			m_camera->SetTarget(m_playerRef->GetPosition());

			// �o�l�J������������
			m_springCamera.Init(*m_camera, kCameraMaxSpeed, true, kCameraRadius);

			// �o�l�̌�������ݒ�
			m_springCamera.SetDampingRate(kSpringDampingRate);

			return;
		}

		/**
		 * @brief �v���C���[�J�����N���X�̃A�b�v�f�[�g�����s����
		*/
		void CPlayerCamera::ExecuteUpdate()
		{
			// �J�����̉�]���v�Z����
			CalcCameraRotation();

			// �J�����̎��_
			Vector3 cameraPos = Vector3::Back;
			// �J�����̒����_
			Vector3 camereTargetPos = Vector3::Zero;

			// �J�����̎��_�ƒ����_���v�Z����
			CalcCameraPositionAndTargetPos(&cameraPos,&camereTargetPos);

			// �o�l�J�������X�V����
			UpdateSpringCamera(cameraPos, camereTargetPos);


			return;
		}

		/**
		 * @brief �J�����̉�]���v�Z����
		*/
		void CPlayerCamera::CalcCameraRotation()
		{
			// ��]�O�̒����_���王�_�ւ̃x�N�g��
			Vector3 oldToCameraVec = m_toCameraVec;

			//////// 1.�J��������]������ ////////

			// �J�����̉�]�N�H�[�^�j�I��
			Quaternion cameraQRot;

			// ���������ւ̃J�����̉�]���v�Z����
			// �����Ȏ��AY�����ŉ�]������
			cameraQRot.SetRotationDegY(
				kCameraRotSpeed * m_playerRef->GetInputData().axisCameraRotHorizontal
			);
			// �����_���王�_�ւ̃x�N�g������]������
			cameraQRot.Apply(m_toCameraVec);

			// ���������̃J�����̉�]���v�Z����
			// �����Ȏ�
			Vector3 axisH;
			// �O�ςŁAY���ƁA�����_���王�_�ւ̃x�N�g���ɁA��������x�N�g�������߂�
			axisH.Cross(Vector3::AxisY, m_toCameraVec);
			// ���K������
			axisH.Normalize();
			// �����Ȏ�����ŉ�]������
			cameraQRot.SetRotationDeg(
				axisH,
				kCameraRotSpeed * m_playerRef->GetInputData().axisCameraRotVertical
			);
			// �����_���王�_�ւ̃x�N�g������]������
			cameraQRot.Apply(m_toCameraVec);


			//////// 2.�J�����̉�]�̏�����`�F�b�N���� ////////

			// �����_���王�_�ւ̕����x�N�g��
			Vector3 toCameraDir = m_toCameraVec;
			// �����x�N�g�������琳�K������
			toCameraDir.Normalize();

			// ������`�F�b�N
			if (toCameraDir.y < kMinToCameraDirY)
			{
				// �J������������߂�
				// ��]�O�̒����_���王�_�ւ̃x�N�g���ɖ߂�
				m_toCameraVec = oldToCameraVec;
			}
			// �������`�F�b�N
			else if (toCameraDir.y > kMaxToCameraDirY)
			{
				// �J�������������߂�
				// ��]�O�̒����_���王�_�ւ̃x�N�g���ɖ߂�
				m_toCameraVec = oldToCameraVec;
			}


			return;
		}

		/**
		 * @brief �J�����̎��_�ƒ����_���v�Z����
		 * @param[out] pos_out ���_���i�[�����
		 * @param[out] targetPos_out �����_���i�[�����
		*/
		void CPlayerCamera::CalcCameraPositionAndTargetPos(Vector3* pos_out, Vector3* targetPos_out)
		{
			//////// 1.�J�����̒����_���v�Z���� ////////

			// �ړ��O�̒����_�������
			*targetPos_out = m_playerRef->GetPosition();
			// �v���C���[�̑������班����𒍎��_�Ƃ���
			targetPos_out->y += kTargetOffsetUp;
			// �v���C���[�̏������̕��𒍎��_�Ƃ���
			*targetPos_out += m_camera->GetForward() * kTargetOffsetForward;

			//////// 2. �J�����̎��_���v�Z���� ////////

			*pos_out = *targetPos_out + m_toCameraVec;

			return;
		}

		/**
		 * @brief �o�l�J�������X�V����
		 * @param[in] pos ���_
		 * @param[in] targetPos �����_
		*/
		void CPlayerCamera::UpdateSpringCamera(const Vector3& pos, const Vector3& targetPos)
		{
			// �o�l�J�����̎��_�ƒ����_��ݒ�
			m_springCamera.SetPosition(pos);
			m_springCamera.SetTarget(targetPos);

			// �o�l�J�������X�V
			m_springCamera.Update();

			return;
		}

	}
}
