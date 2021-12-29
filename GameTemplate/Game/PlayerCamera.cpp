#include "stdafx.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "AICar.h"

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �v���C���[�J�����̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsPlayerConstData::nsCameraConstData;

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

			m_targetOffsetUp = kTargetOffsetUp;

			// �v���C���[�̍��W�̎Q�Ƃ��Z�b�g
			m_playerRef = &player;

			// �J�����̃X�^�[�g�̎��_�ƒ����_��ݒ�
			m_camera->SetPosition(m_playerRef->GetPosition() + m_toCameraVec);
			m_camera->SetTarget(m_playerRef->GetPosition());

			// �o�l�J������������
			m_springCamera.Init(*m_camera, kCameraMaxSpeed, true, kCameraRadius);

			// �o�l�̌�������ݒ�
			m_springCamera.SetDampingRate(kSpringDampingRate);

			m_toCameraDistance = kDefaultToCameraDistance;

			return;
		}

		/**
		 * @brief �v���C���[�J�����N���X�̃A�b�v�f�[�g�����s����
		*/
		void CPlayerCamera::ExecuteUpdate()
		{
			if (m_isOnEnemyCamera)
			{
				// �G�̏�ɏ���Ă��鎞�̃J�����̏���
				OnEnemyCamera();
			}
			else
			{
				// �J�����̉�]���v�Z����
				CalcCameraRotation();

				// �����I�ɃJ�������v���C���[�̈ړ���Ɍ�����
				AutoTurnToPlayerDestination();
			}

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

			// �x�N�g����������Apply�ŉ񂵂Ă���ƁA������Ƃ��L�тĂ��邩��A���K�����ĐL�΂��B

			// �����𐳋K�����Ă���
			m_toCameraVec.Normalize();
			// �L�΂�
			m_toCameraVec.Scale(m_toCameraDistance);

			return;
		}


		/**
		 * @brief �����I�ɃJ�������v���C���[�̈ړ���Ɍ�����
		*/
		void CPlayerCamera::AutoTurnToPlayerDestination()
		{

			// �J�����̎����͂����邩�H���A
			// �ړ��̎����͂��Ȃ����H
			if (m_playerRef->GetInputData().inputCameraAxis == true ||
				m_playerRef->GetInputData().inputMoveAxis != true)
			{
				// �J�����̉�]�����邩�A�ړ����Ă��Ȃ����́A������]���Ȃ��B

				// �����ŃJ��������]���n�߂�^�C�}�[�����Z�b�g����^�C�}�[����莞�Ԑi��ł��Ȃ����H
				if (m_autoTurnStartTimerResetTimer < kAutoTurnStartTimerResetTime)
				{
					// �i��ł��Ȃ��B�^�C�}�[��i�߂�B
					m_autoTurnStartTimerResetTimer += nsTimer::GameTime().GetFrameDeltaTime();
					return;
				}
				// �i�񂾁B�^�C�}�[�����Z�b�g����E
				m_autoTurnStartTimer = 0.0f;
				// �������^�[��
				return;
			}

			// �J�����̉�]�̑�������Ă��Ȃ����A���A�ړ��̑�������Ă�����

			// �����ŃJ��������]���n�߂�^�C�}�[�����Z�b�g����^�C�}�[�����Z�b�g����
			m_autoTurnStartTimerResetTimer = 0.0f;

			// �����ŃJ��������]���n�߂�^�C�}�[���A��莞�Ԑi��ł��Ȃ����H
			if (m_autoTurnStartTimer < kAutoTurnStartTime)
			{
				// �i��ł��Ȃ��B�^�C�}�[��i�߂�B
				m_autoTurnStartTimer += nsTimer::GameTime().GetFrameDeltaTime();
				// �������^�[��
				return;
			}

			// �J�����̉�]�̑�������Ă��Ȃ����A���A�ړ��̑�������Ă����ԂŁA
			// ��莞�Ԍo�ߌ�A������]�̏������s��

			//////// �܂���Y���W���ړ������� ////////
			// Y���W��⊮���āA���X�ɍ����������悤�ɂ���
			m_toCameraVec.y = Math::Lerp(kAutoTurnYRate, m_toCameraVec.y, kDefaultToCameraVec.y);

			//////// ����XZ���ʂňړ������� ////////

			// �ڕW�́u�J�����ւ̃x�N�g���v
			Vector3 targetToCameraVec = kDefaultToCameraVec;
			// ���f���̉�]�ŉ�
			m_playerRef->GetRotation().Apply(targetToCameraVec);

			// XZ���ʂł́A�ڕW�́u�J�����ւ̕����x�N�g���v
			Vector3 targetToCameraDirXZ = targetToCameraVec;
			targetToCameraDirXZ.y = 0.0f;		// Y��������������
			targetToCameraDirXZ.Normalize();	// ���K������

			// XZ���ʂł́A���݂́u�J�����ւ̕����x�N�g���v
			Vector3 toCameraDirXZ = m_toCameraVec;
			toCameraDirXZ.y = 0.0f;				// Y��������������
			toCameraDirXZ.Normalize();			// ���K������

			// �ڕW�́u�J�����ւ̕����x�N�g���v�ƌ��݂́u�J�����ւ̕����x�N�g���v�̓��ς��������l�ȏ�
			if (Dot(targetToCameraDirXZ, toCameraDirXZ) >= kAutoTurnExecuteThreshold)
			{
				// �قړ��������̂��ߓ������K�v�Ȃ��B�������^�[���B
				return;
			}

			// XZ���ʂł́A�ڕW�́u�J�����ւ̕����x�N�g���v�́u�E�����̕����x�N�g���v
			Vector3 targetToCameraRightDirXZ = Cross(Vector3::Up, targetToCameraDirXZ);
			targetToCameraRightDirXZ.Normalize();	// ���K������

			// XZ���ʂł́A�ڕW�̃x�N�g���́u�E�����̕����x�N�g���v�ƌ��݂́u�J�����ւ̕����x�N�g���v�̓���
			float dotRightAndToCamDirXZ = Dot(targetToCameraRightDirXZ, toCameraDirXZ);

			// ��]���x�̌v�Z�B�v���C���[�̑��x�������قǁA������]����B

			// �Œ��ԗ��̑��x
			const float minRateSpeed = nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed;
			// �ō��⊮���̑��x
			const float maxRateSpeed = nsPlayerConstData::nsSwingActionConstData::kInitialSwingSpeed;
			// ��]�X�s�[�h�̕⊮���B�v���C���[�̑��x�ɉ����āA0.0f�`1.0f�̒l�ɐ��K������B
			float turnSpeedRate = max(0.0f, m_playerRef->GetPlayerMovement().GetXZSpeed() - minRateSpeed);
			turnSpeedRate = min(1.0f,turnSpeedRate / (maxRateSpeed - minRateSpeed));
			// �X�s�[�h�̕ω����w���֐��I�ɂ���
			turnSpeedRate = pow(turnSpeedRate, kAutoTurnSpeedRatePowPower);
			// ��]����X�s�[�h
			float turnSpeed = Math::Lerp(turnSpeedRate, kAutoTurnSpeedMin, kAutoTurnSpeedMax);


			// ���݂̃x�N�g�����A�ڕW�̃x�N�g���̉E���ɂ��邩�H
			if (dotRightAndToCamDirXZ >= 0.0f)
			{
				// ���݂̃x�N�g�����E���A�ڕW�̃x�N�g���������ɂ��邩��A�������ɉ�
				// ���Ό����ɉ�
				turnSpeed = -turnSpeed;
			}


			// ��]�N�H�[�^�j�I��
			Quaternion qRot;
			qRot.SetRotationY(turnSpeed);
			// XZ���ʂŁA�J�����ւ̃x�N�g������
			qRot.Apply(m_toCameraVec);

			return;
		}


		/**
		 * @brief �G�̏�ɏ���Ă��鎞�̃J�����̏���
		*/
		void CPlayerCamera::OnEnemyCamera()
		{
			const auto& enemy = m_playerRef->GetCatchEnemy().GetTargetEnemy();
			if (enemy == nullptr)
			{
				return;
			}
			const Quaternion& enemyRot = enemy->GetRotation();
			const Quaternion& playerRot = m_playerRef->GetRotation();
			m_toCameraVec = Vector3::Back;
			//enemyRot.Apply(m_toCameraVec);
			playerRot.Apply(m_toCameraVec);

			// ���������̃J�����̉�]���v�Z����
			// �����Ȏ�
			Vector3 axisH;
			// �O�ςŁAY���ƁA�����_���王�_�ւ̃x�N�g���ɁA��������x�N�g�������߂�
			axisH.Cross(Vector3::AxisY, m_toCameraVec);
			// ���K������
			axisH.Normalize();
			// �����Ȏ�����ŉ�]������
			Quaternion cameraVRot;
			cameraVRot.SetRotationDeg(
				axisH,
				kOnEnemyCameraVerticalAngle
			);
			// �����_���王�_�ւ̃x�N�g������]������
			cameraVRot.Apply(m_toCameraVec);

			// �����𐳋K�����Ă���
			m_toCameraVec.Normalize();
			// �L�΂�
			m_toCameraVec.Scale(kNearToCameraDistance);

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
			targetPos_out->y += m_targetOffsetUp;
			// �v���C���[�̏������̕��𒍎��_�Ƃ���
			*targetPos_out += m_camera->GetForward() * kTargetOffsetForward;

			//////// 2. �J�����̎��_���v�Z���� ////////

			*pos_out = *targetPos_out + m_toCameraVec;
			pos_out->y += m_cameraPositionOffsetUp;

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
