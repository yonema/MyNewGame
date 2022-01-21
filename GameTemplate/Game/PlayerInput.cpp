#include "stdafx.h"
#include "PlayerInput.h"
#include "Player.h"
#include "SavedPlayerInputData.h"

namespace nsNinjaAttract
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
		 * @brief �R���X�g���N�^
		*/
		CPlayerInput::CPlayerInput()
		{
			m_savedPlayerInputData = new nsExternalData::CSavedPlayerInputData;
			return;
		}

		/**
		 * @brief �f�X�g���N�^
		*/
		CPlayerInput::~CPlayerInput()
		{
			delete m_savedPlayerInputData;
			return;
		}

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
			memset(&m_playerInputDataAndDeltaTime, 0, sizeof(m_playerInputDataAndDeltaTime));

			// ���͏����X�V����
			UpdateInput();

			if (m_isStartSaveData)
			{
				// �ۑ��f�[�^�̎��W���J�n���Ă���Ȃ�A�ۑ�����f�[�^��ǉ�����B
				m_savedPlayerInputData->PushBuckData(m_playerInputDataAndDeltaTime);
			}

			if (m_playerRef->IsInputtable() != true || m_isUsingSavedData)
			{
				// �v���C���[�����͕s�̎��A�܂��́A�ۑ��������̓f�[�^���g�p���̎�
				// ���͏�������������
				memset(
					&m_playerInputDataAndDeltaTime.playerInputData,
					0, 
					sizeof(m_playerInputDataAndDeltaTime.playerInputData));
			}

			if (m_isUsingSavedData)
			{
				// �ۑ��������̓f�[�^���g�p���̎�
				// ���݂̐i���̃��[�h�����f�[�^�����o���āA�i����i�߂�B
				SPlayerInputDataAndDeltaTime* loadData = m_savedPlayerInputData->GetAndProgressLoadData();
				if (loadData)
				{
					// �f�[�^������΁A���̓f�[�^�ɔ��f����B
					m_playerInputDataAndDeltaTime = *loadData;
				}
			}

			return;
		}

		/**
		 * @brief �ۑ�������̓f�[�^�̎�����I�����āA�ۑ�����B
		 * @param[in] �ۑ��t�@�C���̎��
		*/
		void CPlayerInput::EndSaveDataAndSave(
			const nsExternalData::nsSavedPlayerInputDataConstData::EnSavedFileType saveFileType
		)
		{
			// �ۑ��f�[�^�̎��W���J�n���Ă��邩�H�̃t���O��܂�
			m_isStartSaveData = false;

			// ���W�����f�[�^��ۑ�����
			m_savedPlayerInputData->SaveData(saveFileType);

			// ���W�����f�[�^���N���A����
			m_savedPlayerInputData->ClearSaveData();

			return;
		}

		/**
		 * @brief �ۑ������f�[�^�̎g�p���J�n����
		 * @param[in] saveFileType �ۑ��t�@�C���̎��
		*/
		void CPlayerInput::StartUsingSavedData(
			const nsExternalData::nsSavedPlayerInputDataConstData::EnSavedFileType saveFileType
		)
		{
			// �ۑ��f�[�^���g�p���t���O�����Ă�
			m_isUsingSavedData = true;

			// �ۑ����Ă���v���C���[�̓��̓f�[�^�����[�h����
			m_savedPlayerInputData->LoadData(saveFileType);

			return;
		}

		/**
		 * @brief �ۑ������f�[�^�̎g�p���I������
		*/
		void CPlayerInput::EndUsingSavedData()
		{
			// �ۑ��f�[�^���g�p���t���O��܂�
			m_isUsingSavedData = false;

			// ���[�h���Ă���f�[�^���N���A
			m_savedPlayerInputData->ClearLoadData();

			return;
		}

		/**
		 * @brief ���[�h�����f�[�^�̐i�����A�Ō�܂ōs���Ă��邩�H�𓾂�B
		 * @return ���[�h�����f�[�^�̐i�����A�Ō�܂ōs���Ă��邩�H
		*/
		bool CPlayerInput::IsEndLoadDataProgress() const
		{
			return m_savedPlayerInputData->IsEndLoadDataProgress();
		}


		/**
		 * @brief ���͏����X�V����
		*/
		void CPlayerInput::UpdateInput()
		{
			// �����͂��X�V����
			UpdateInputAxis();

			// �A�N�V�������͂��X�V����
			UpdateInputAction();

			// �R�}���h���͏��̍X�V
			UpdateCommandInputData();

			// �f���^�^�C�����擾
			m_playerInputDataAndDeltaTime.deltaTime = nsTimer::GameTime().GetFrameDeltaTime();

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
				m_playerInputDataAndDeltaTime.playerInputData.actionJump = true;
			}

			// �E�X�e�B�b�N�g���K�[
			if (m_pad->IsTrigger(enButtonRB3))
			{
				// �G��T�m
				m_playerInputDataAndDeltaTime.playerInputData.actionSearchEnemy = true;
			}

			// Y�{�^���g���K�[
			if (m_pad->IsTrigger(enButtonY))
			{
				m_playerInputDataAndDeltaTime.playerInputData.actionCatchEnemy = true;
			}

			// RB2�{�^����������Ă��邩�H
			if (m_pad->IsPress(enButtonRB2))
			{
				// �_�b�V��
				m_playerInputDataAndDeltaTime.playerInputData.actionDush = true;
				// �X�C���O
				m_playerInputDataAndDeltaTime.playerInputData.actionSwing = true;
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
			m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward = padLStickF.y;
			// X�������̓��͂𒲂ׂ�
			m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight = padLStickF.x;

			// �����͂��������ꍇ�͏\���L�[�𒲂ׂ�K�v���Ȃ����߁A�������^�[�����s��
			if (fabsf(m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward) > kInputAxisMin ||
				fabsf(m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight) > kInputAxisMin)
			{
				// ������
				m_playerInputDataAndDeltaTime.playerInputData.inputMoveAxis = true;
				return;
			}


			//////// 2.�\���L�[�̓��͏��𒲂ׂ� ////////

			// ��A���L�[�̓��͏��𒲂ׂ�
			if (m_pad->IsPress(enButtonUp))
			{
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward = kDPadInputPower;
			}
			else if (m_pad->IsPress(enButtonDown))
			{
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward = -kDPadInputPower;
			}
			// �E�A���L�[�̓��͏��𒲂ׂ�
			if (m_pad->IsPress(enButtonRight))
			{
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight = kDPadInputPower;
			}
			else if (m_pad->IsPress(enButtonLeft))
			{
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight = -kDPadInputPower;
			}

			// �΂߈ړ��̈ړ��̏ꍇ�́A���[�g2�{���͗ʂ������Ȃ邽�߁A���[�g2�Ŋ���
			if (m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward != 0.0f && 
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight != 0.0f)
			{
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward /= kSquareRootOfTwo;
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight /= kSquareRootOfTwo;
			}

			// �����͂����������H
			if (m_playerInputDataAndDeltaTime.playerInputData.axisMoveForward != 0.0f || 
				m_playerInputDataAndDeltaTime.playerInputData.axisMoveRight != 0.0f)
			{
				// ������
				m_playerInputDataAndDeltaTime.playerInputData.inputMoveAxis = true;
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
			m_playerInputDataAndDeltaTime.playerInputData.axisCameraRotVertical = m_pad->GetRStickYF();
			// X�������̓��͂𒲂ׂ�
			m_playerInputDataAndDeltaTime.playerInputData.axisCameraRotHorizontal = m_pad->GetRStickXF();

			// ���͂����������H
			if (fabsf(m_playerInputDataAndDeltaTime.playerInputData.axisCameraRotVertical) > kInputAxisMin ||
				fabsf(m_playerInputDataAndDeltaTime.playerInputData.axisCameraRotHorizontal) > kInputAxisMin)
			{
				// ������
				m_playerInputDataAndDeltaTime.playerInputData.inputCameraAxis = true;
			}

			return;
		}


		/**
		 * @brief �R�}���h���͏��̍X�V
		*/
		void CPlayerInput::UpdateCommandInputData()
		{
			m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_None;

			// ABYX�{�^���̍X�V
			if (m_pad->IsTrigger(enButtonA))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_A;
			}
			else if (m_pad->IsTrigger(enButtonB))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_B;
			}
			else if (m_pad->IsTrigger(enButtonY))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_Y;
			}
			else if (m_pad->IsTrigger(enButtonX))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_X;
			}
			// �\���L�[�̍X�V
			else if (m_pad->IsTrigger(enButtonUp))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Up;
			}
			else if (m_pad->IsTrigger(enButtonDown))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Down;
			}
			else if (m_pad->IsTrigger(enButtonLeft))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Left;
			}
			else if (m_pad->IsTrigger(enButtonRight))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Right;
			}
			// ���X�e�B�b�N�̍X�V
			else if (m_pad->GetLStickYF() >= kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Up;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickYF() <= -kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Down;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickXF() <= -kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Left;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickXF() >= kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Right;
				m_canInputCommandAxis = false;
			}
			// L1,L2,R1,R2�{�^���̍X�V
			else if (m_pad->IsTrigger(enButtonLB1))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L1;
			}
			else if (m_pad->IsTrigger(enButtonLB2))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_L2;
			}
			else if (m_pad->IsTrigger(enButtonRB1))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_R1;
			}
			else if (m_pad->IsTrigger(enButtonRB2))
			{
				m_playerInputDataAndDeltaTime.playerInputData.inputCommand = EnQTEButtonType::enQTE_R2;
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