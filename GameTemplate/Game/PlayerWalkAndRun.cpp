#include "stdafx.h"
#include "PlayerWalkAndRun.h"
#include "PlayerConstData.h"
#include "PlayerCamera.h"
#include "PlayerInput.h"

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		/**
		 * @brief �v���C���[�̓����֘A�̃l�[���X�y�[�X
		*/
		namespace nsPlayerMovenent
		{
			// �v���C���[�̕����Ƒ���N���X�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsPlayerWalkAndRunConstData;

			/**
			 * @brief ������
			 * @param[in,out] moveVecForward �O�����̈ړ��x�N�g���̎Q��
			 * @param[in,out] moveVecRight �E�����̈ړ��x�N�g���̎Q��
			 * @param[in] camera �v���C���[�J����
			 * @param[in] playerInputData �v���C���[���͏��
			*/
			void CPlayerWalkAndRun::Init(
				Vector3* moveVecForward,
				Vector3* moveVecRight,
				const CPlayerCamera& camera,
				const SPlayerInputData& playerInputData
			)
			{
				// �O�����̈ړ��x�N�g���̎Q�Ƃ��Z�b�g
				m_moveVecForward = moveVecForward;
				// �E�����̈ړ��x�N�g���̎Q�Ƃ��Z�b�g
				m_moveVecRight = moveVecRight;
				// �v���C���[�̃J�������Z�b�g
				m_playerCamera = &camera;
				// �v���C���[�̓��͏����Z�b�g
				m_playerInputData = &playerInputData;

				return;
			}

			/**
			 * @brief �����Ƒ���̏��������s
			 * @param[in] isAir �󒆂��H
			*/
			void CPlayerWalkAndRun::Execute(const bool isAir)
			{
				// �O�A��ړ��̎�����
				const float inputAxisMoveForward = m_playerInputData->axisMoveForward;
				// �E�A���ړ��̎�����
				const float inputAxisMoveRight = m_playerInputData->axisMoveRight;

				// �O�A��ړ��̎����͂̐�Βl
				const float absInputAxisMoveForward = fabs(inputAxisMoveForward);
				// �E�A���ړ��̎����͂̐�Βl
				const float absInputAxisMoveRight = fabs(inputAxisMoveRight);

				// �ړ����x����������
				Acceleration(inputAxisMoveForward, inputAxisMoveRight);

				// ���C���v�Z����
				Friction(absInputAxisMoveForward, absInputAxisMoveRight, isAir);

				// �ړ����x�ɐ�����������
				LimitSpeed(absInputAxisMoveForward, absInputAxisMoveRight);


				return;
			}

			/**
			 * @brief �ړ����x������������
			 * @param inputMoveF �O�A��̎�����
			 * @param inputMoveR �E�A���̎�����
			*/
			void CPlayerWalkAndRun::Acceleration(const float inputMoveF, const float inputMoveR)
			{
				// �ړ��̑O����
				Vector3 moveForward = m_playerCamera->GetCameraForward();
				// �ړ��̉E����
				Vector3 moveRight = m_playerCamera->GetCameraRight();
				// Y������������XZ���ʂł̑O�����ƉE�����ɕϊ�����
				moveForward.y = 0.0f;
				moveForward.Normalize();
				moveRight.y = 0.0f;
				moveRight.Normalize();

				//�������ւ̈ړ����x�����Z�B
				*m_moveVecForward += moveForward * inputMoveF * kAcceleration;
				//�������ւ̈ړ����x�����Z�B
				*m_moveVecRight += moveRight * inputMoveR * kAcceleration;

				return;
			}

			/**
			 * @brief ���C�̌v�Z
			 * @param[in] absInputMoveF �O�A��̎����͂̐�Βl
			 * @param[in] absInputMoveR �E�A���̎����͂̐�Βl
			 * @param[in] isAir �󒆂��H
			*/
			void CPlayerWalkAndRun::Friction(
				const float absInputMoveF, const float absInputMoveR, const bool isAir
			)
			{
				// ���C��
				float friction = 0.0f;

				// �󒆂��H
				if (isAir)
				{
					// �󒆂̖��C��
					friction = kAirFriction;
				}
				else
				{
					// �n�ʏ�̖��C��
					friction = kGroundFriction;
				}

				// �O�����̈ړ��x�N�g���̖��C���v�Z����
				// �ړ����x���Œᑬ�x�ȉ��Ȃ�
				if (m_moveVecForward->Length() <= kMinSpeed)
				{
					// �ړ��[���ɂ���
					*m_moveVecForward = Vector3::Zero;
				}
				else if (absInputMoveF <= 0.001f)
				{
					// ���͂��Ȃ�������
					// ���C�Č�������
					m_moveVecForward->Scale(friction);
				}

				// �E�����̈ړ��x�N�g���̖��C���v�Z����
				// �ړ����x���Œᑬ�x�ȉ��Ȃ�
				if (m_moveVecRight->Length() <= kMinSpeed)
				{
					// �ړ��[���ɂ���
					*m_moveVecRight = Vector3::Zero;
				}
				else if (absInputMoveR <= 0.001f)
				{
					// ���͂��Ȃ�������
					// ���C�Č�������
					m_moveVecRight->Scale(friction);
				}

				return;
			}

			/**
			 * @brief �ړ����x�ɑ��x������������
			 * @param[in] absInputMoveF �O�A��̎����͂̐�Βl
			 * @param[in] absInputMoveR �E�A���̎����͂̐�Βl
			*/
			void CPlayerWalkAndRun::LimitSpeed(const float absInputMoveF, const float absInputMoveR)
			{
				// �����͂����邩�H
				if (absInputMoveF <= 0.001f && absInputMoveR <= 0.001f)
				{
					// �Ȃ��ꍇ�͑��x�����������Ȃ�
					// �������^�[��
					return;
				}

				// �O�����̈ړ����x���ō����x���I�[�o�[���Ă��邩�H
				if (m_moveVecForward->Length() > kMaxSpeed * absInputMoveF)
				{
					// �I�[�o�[���Ă�����ō����x���ێ�
					m_moveVecForward->Normalize();
					m_moveVecForward->Scale(kMaxSpeed * absInputMoveF);
				}

				// �E�����̈ړ����x���ō����x���I�[�o�[���Ă��邩�H
				if (m_moveVecRight->Length() > kMaxSpeed * absInputMoveR)
				{
					// �I�[�o�[���Ă�����ō����x���ێ�
					m_moveVecRight->Normalize();
					m_moveVecRight->Scale(kMaxSpeed * absInputMoveR);
				}
				

				return;
			}

		}
	}
}