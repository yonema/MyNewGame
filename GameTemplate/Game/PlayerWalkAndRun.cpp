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
			 * @param[in,out] addMoveVec ���Z�ړ��x�N�g���̎Q��
			 * @param[in] camera �v���C���[�J����
			 * @param[in] playerInputData �v���C���[���͏��
			*/
			void CPlayerWalkAndRun::Init(
				Vector3* addMoveVec,
				const CPlayerCamera& camera,
				const SPlayerInputData& playerInputData
			)
			{
				// ���Z�ړ��x�N�g���̎Q�Ƃ��Z�b�g
				m_addMoveVec = addMoveVec;
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
				const float absInputAxisMoveForward = fabsf(inputAxisMoveForward);
				// �E�A���ړ��̎����͂̐�Βl
				const float absInputAxisMoveRight = fabsf(inputAxisMoveRight);

				// �ړ����x����������
				Acceleration(inputAxisMoveForward, inputAxisMoveRight);

				// ���C���v�Z����
				Friction(absInputAxisMoveForward, absInputAxisMoveRight, isAir);

				// �ړ����x�ɐ�����������
				LimitSpeed(absInputAxisMoveForward, absInputAxisMoveRight);

				// �O�̃t���[���̑��x���X�V
				m_oldVelocity = m_addMoveVec->Length();

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
				*m_addMoveVec += moveForward * inputMoveF * kAcceleration;
				//�������ւ̈ړ����x�����Z�B
				*m_addMoveVec += moveRight * inputMoveR * kAcceleration;

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

				// ���C���v�Z����
				if (m_addMoveVec->Length() <= kMinSpeed)
				{
					// �ړ����x���Œᑬ�x�ȉ��Ȃ�
					// �ړ��[���ɂ���
					*m_addMoveVec = Vector3::Zero;
				}
				else if (absInputMoveF <= 0.001f && absInputMoveR <= 0.001f)
				{
					// ���͂��Ȃ�������
					// ���C�Č�������
					m_addMoveVec->Scale(friction);
				}
				else if (m_addMoveVec->Length() < m_oldVelocity)
				{
					// �O�̃t���[����葬�x�������Ă�����
					// ���C�Č�������
					m_addMoveVec->Scale(friction);
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


				// �ړ����x���ō����x���I�[�o�[���Ă��邩�H
				if (m_addMoveVec->Length() > kMaxSpeed)
				{
					// �I�[�o�[���Ă�����ō����x���ێ�
					m_addMoveVec->Normalize();
					m_addMoveVec->Scale(kMaxSpeed);
				}

				return;
			}

		}
	}
}