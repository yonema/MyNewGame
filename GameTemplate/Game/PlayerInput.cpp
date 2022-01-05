#include "stdafx.h"
#include "PlayerInput.h"
#include "Player.h"

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �v���C���[���̓N���X�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsPlayerConstData::nsInputConstData;
		using nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType;

		/**
		 * @brief ������
		 * @param[in,out] player �v���C���[�̎Q��
		*/
		void CPlayerInput::Init(CPlayer* player)
		{
			// �Q�[���p�b�h�̎Q�Ƃ𓾂�
			m_pad = g_pad[0];

			// �v���C���[�̎Q�Ƃ��Z�b�g����
			m_playerRef = player;

			return;
		}

		/**
		 * @brief ���͏��������s����
		*/
		void CPlayerInput::ExecuteUpdate()
		{
			// ���͏�������������
			memset(&m_playerInputData, 0, sizeof(m_playerInputData));

			// �����͂��X�V����
			UpdateInputAxis();

			// �A�N�V�������͂��X�V����
			UpdateInputAction();

			// �R�}���h���͏��̍X�V
			UpdateCommandInputData();

			return;
		}


		/**
		 * @brief �����͏����X�V����
		*/
		void CPlayerInput::UpdateInputAxis()
		{
			// �ړ��̎����͏��̍X�V
			OnUpdateInputAxisMove();

			// �J�����̉�]�̎����͏��̍X�V
			OnUpdateInputAxisCameraRot();

			return;
		}


		/**
		 * @brief �A�N�V�������͏����X�V����
		*/
		void CPlayerInput::UpdateInputAction()
		{
			// A�{�^���g���K�[
			if (m_pad->IsTrigger(enButtonA))
			{
				// �W�����v
				m_playerInputData.actionJump = true;
			}

			// �E�X�e�B�b�N�g���K�[
			if (m_pad->IsTrigger(enButtonRB3))
			{
				// �G��T�m
				m_playerInputData.actionSearchEnemy = true;
			}

			// Y�{�^���g���K�[
			if (m_pad->IsTrigger(enButtonY))
			{
				m_playerInputData.actionCatchEnemy = true;
			}

			// RB2�{�^����������Ă��邩�H
			if (m_pad->IsPress(enButtonRB2))
			{
				// �_�b�V��
				m_playerInputData.actionDush = true;
				// �X�C���O
				m_playerInputData.actionSwing = true;
			}

			return;
		}

		/**
		 * @brief �ړ��̎����͏��̍X�V
		*/
		void CPlayerInput::OnUpdateInputAxisMove()
		{
			//////// 1.���X�e�B�b�N�̓��͏��𒲂ׂ� ////////

			// �p�b�h�̍��X�e�B�b�N�̎����͏��
			Vector2 padLStickF(m_pad->GetLStickXF(), m_pad->GetLStickYF());
			// �������K�����Ă���ĂȂ�����A���K������
			padLStickF.Normalize();

			// Y�������̓��͂𒲂ׂ�
			m_playerInputData.axisMoveForward = padLStickF.y;
			// X�������̓��͂𒲂ׂ�
			m_playerInputData.axisMoveRight = padLStickF.x;

			// �����͂��������ꍇ�͏\���L�[�𒲂ׂ�K�v���Ȃ����߁A�������^�[�����s��
			if (fabsf(m_playerInputData.axisMoveForward) > kInputAxisMin || 
				fabsf(m_playerInputData.axisMoveRight) > kInputAxisMin)
			{
				// ������
				m_playerInputData.inputMoveAxis = true;
				return;
			}


			//////// 2.�\���L�[�̓��͏��𒲂ׂ� ////////

			// ��A���L�[�̓��͏��𒲂ׂ�
			if (m_pad->IsPress(enButtonUp))
			{
				m_playerInputData.axisMoveForward = kDPadInputPower;
			}
			else if (m_pad->IsPress(enButtonDown))
			{
				m_playerInputData.axisMoveForward = -kDPadInputPower;
			}
			// �E�A���L�[�̓��͏��𒲂ׂ�
			if (m_pad->IsPress(enButtonRight))
			{
				m_playerInputData.axisMoveRight = kDPadInputPower;
			}
			else if (m_pad->IsPress(enButtonLeft))
			{
				m_playerInputData.axisMoveRight = -kDPadInputPower;
			}

			// �΂߈ړ��̈ړ��̏ꍇ�́A���[�g2�{���͗ʂ������Ȃ邽�߁A���[�g2�Ŋ���
			if (m_playerInputData.axisMoveForward != 0.0f && m_playerInputData.axisMoveRight != 0.0f)
			{
				m_playerInputData.axisMoveForward /= kSquareRootOfTwo;
				m_playerInputData.axisMoveRight /= kSquareRootOfTwo;
			}

			// �����͂����������H
			if (m_playerInputData.axisMoveForward != 0.0f || m_playerInputData.axisMoveRight != 0.0f)
			{
				// ������
				m_playerInputData.inputMoveAxis = true;
			}

			return;
		}

		/**
		 * @brief �J�����̉�]�̎����͏��̍X�V
		*/
		void CPlayerInput::OnUpdateInputAxisCameraRot()
		{
			// �E�X�e�B�b�N�̎����͏��𒲂ׂ�

			// Y�������̓��͂𒲂ׂ�
			m_playerInputData.axisCameraRotVertical = m_pad->GetRStickYF();
			// X�������̓��͂𒲂ׂ�
			m_playerInputData.axisCameraRotHorizontal = m_pad->GetRStickXF();

			// ���͂����������H
			if (fabsf(m_playerInputData.axisCameraRotVertical) > kInputAxisMin ||
				fabsf(m_playerInputData.axisCameraRotHorizontal) > kInputAxisMin)
			{
				// ������
				m_playerInputData.inputCameraAxis = true;
			}

			return;
		}


		/**
		 * @brief �R�}���h���͏��̍X�V
		*/
		void CPlayerInput::UpdateCommandInputData()
		{
			m_playerInputData.inputCommand = EnQTEButtonType::enQTE_None;

			// ABYX�{�^���̍X�V
			if (m_pad->IsTrigger(enButtonA))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_A;
			}
			else if (m_pad->IsTrigger(enButtonB))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_B;
			}
			else if (m_pad->IsTrigger(enButtonY))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_Y;
			}
			else if (m_pad->IsTrigger(enButtonX))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_X;
			}
			// �\���L�[�̍X�V
			else if (m_pad->IsTrigger(enButtonUp))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Up;
			}
			else if (m_pad->IsTrigger(enButtonDown))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Down;
			}
			else if (m_pad->IsTrigger(enButtonLeft))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Left;
			}
			else if (m_pad->IsTrigger(enButtonRight))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Right;
			}
			// ���X�e�B�b�N�̍X�V
			else if (m_pad->GetLStickYF() >= kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Up;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickYF() <= -kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Down;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickXF() <= -kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Left;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickXF() >= kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Right;
				m_canInputCommandAxis = false;
			}
			// L1,L2,R1,R2�{�^���̍X�V
			else if (m_pad->IsTrigger(enButtonLB1))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L1;
			}
			else if (m_pad->IsTrigger(enButtonLB2))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L2;
			}
			else if (m_pad->IsTrigger(enButtonRB1))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_R1;
			}
			else if (m_pad->IsTrigger(enButtonRB2))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_R2;
			}
			// �����͂̃t���O�̉�
			else if (fabsf(m_pad->GetLStickYF()) <= kInputAxisMin && fabsf(m_pad->GetLStickXF() <= kInputAxisMin))
			{
				m_canInputCommandAxis = true;
			}

			return;
		}
	}
}