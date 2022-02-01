#include "stdafx.h"
#include "Player.h"
#include "GameMainState.h"
#include "GameMainStateConstData.h"
#include "RenderingEngine.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// ���ʃf�[�^���g�p�\�ɂ���
		using namespace nsCommonData;
		// �v���C���[�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsPlayerConstData;

		/**
		 * @brief �X�^�[�g�֐�
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CPlayer::Start()
		{
			m_playerModelAnimation = std::make_unique<CPlayerModelAnimation>();
			m_playerInput = std::make_unique<CPlayerInput>();
			m_playerCamera = std::make_unique<CPlayerCamera>();
			m_playerMove = std::make_unique<nsPlayerMovenent::CPlayerMovement>();
			m_playerCatchEnemy = std::make_unique<CPlayerCatchEnemy>();

			// �v���C���[���f���A�j���[�V�����N���X�̏�����
			m_playerModelAnimation->Init(*this);

			// �v���C���[�̓��͏��N���X�̏�����
			m_playerInput->Init(this);

			// �v���C���[�J�����N���X�̏�����
			m_playerCamera->Init(*this);

			// �v���C���[�ړ��N���X�̏�����
			m_playerMove->Init(
				kCapsuleRadius,
				kDapsuleHeight,
				this,
				m_playerCamera.get(),
				m_playerModelAnimation.get()
				);

			// �v���C���[�̎��̃��f���N���X�̐����Ə�����
			m_playerStringModel = NewGO<CPlayerStringModel>(nsCommonData::enPriorityFirst);
			m_playerStringModel->Init(*this);
			
			// �v���C���[���G��߂܂��鏈���N���X�̏�����
			m_playerCatchEnemy->Init(this);


			if (m_isTitleMode != true)
			{
				m_playerMove->AddMoveVec({ 0.0f,2500.0f,500.0f });
				ChangeState(enStartFall);
			}

			return true;
		}

		/**
		 * @brief �j���������ɌĂ΂��֐�
		*/
		void CPlayer::OnDestroy()
		{
			m_playerModelAnimation.reset();
			m_playerInput.reset();
			m_playerCamera.reset();
			m_playerMove.reset();
			m_playerCatchEnemy.reset();

			// �v���C���[�̎��̃��f���N���X�̔j��
			DeleteGO(m_playerStringModel);

			return;
		}

		/**
		 * @brief �A�b�v�f�[�g�֐�
		*/
		void CPlayer::Update()
		{
			nsDebug::DrawTextPanel(L"[CPlayer::Update()]");

			nsDebug::DrawTextPanel(m_position, L"pos");

			// �X�e�[�g�̍X�V
			UpdateState();

			// ���͏��������s
			m_playerInput->ExecuteUpdate();

			// �v���C���[�̈ړ������s
			m_playerMove->ExecuteUpdate();

			// �J�����N���X���X�V
			m_playerCamera->ExecuteUpdate();

			// ���f���A�j���[�V�����N���X���X�V
			m_playerModelAnimation->ExecuteUpdate();

			// �v���C���[���G��߂܂��鏈���N���X�����s
			m_playerCatchEnemy->ExecuteUpdate();

			// �����_�����O�G���W���Ƀv���C���[�̍��W��`����
			nsMyEngine::CRenderingEngine::GetInstance()->SetPlayerPosition(g_camera3D->GetPosition());

			return;
		}

		/**
		 * @brief �����Ƒ����Ԃ֑J�ڂ���
		*/
		void CPlayer::ChangeWalkAndRunState()
		{
			ChangeState(enWalkAndRun);

			return;
		}

		/**
		 * @brief �X�C���O��Ԃ֑J�ڂ���
		*/
		void CPlayer::ChangeSwingState()
		{
			ChangeState(enSwing);

			return;
		}

		/**
		 * @brief �Ǒ����Ԃ֑J�ڂ���
		*/
		void CPlayer::ChangeWallRunState()
		{
			ChangeState(enWallRun);

			return;
		}

		/**
		 * @brief �G�̏�ɏ���Ă����Ԃ֑J�ڂ���
		*/
		void CPlayer::ChangeOnEnemyState()
		{
			m_playerMove->ResetSwing();
			ChangeState(enOnEnemy);
			return;
		}


		/**
		 * @brief �����w�肵�����W�Ɍ������ĐL�т鏈�����J�n����
		 * @param[in] pos �L�т��̍��W
		*/
		void CPlayer::StartStringStretchToPos(const Vector3& pos)
		{
			m_playerStringModel->StartStretchToPos(pos);

			return;
		}

		/**
		 * @brief �L�т��̍��W��ݒ肷��
		 * @param[in] pos ���W
		*/
		void CPlayer::SetStringStretchPos(const Vector3& pos)
		{
			m_playerStringModel->SetToStretchPos(pos);

			return;
		}

		/**
		 * @brief �����w�肵�����W�Ɍ������ĐL�т鏈�����I������
		*/
		void CPlayer::EndStringStretchToPos()
		{
			m_playerStringModel->EndStretchToPos();

			return;
		}

		/**
		 * @brief �X�e�[�g�̍X�V
		*/
		void CPlayer::UpdateState()
		{
			if (nsGameState::CGameMainState::GetInstance()->GetGameMainStateState() == 
				nsGameState::nsGameMainStateConstData::enGS_clearDirecting)
			{
				ChangeState(enClearDirecting);
			}

			if (nsGameState::CGameMainState::GetInstance()->GetGameMainStateState() ==
				nsGameState::nsGameMainStateConstData::enGS_lastJump)
			{
				ChangeState(enLastJump);
			}


			if (IsInputtable() == true &&
				GetState() == enStartFall)
			{
				ChangeWalkAndRunState();
			}

			return;
		}

		/**
		 * @brief �X�e�[�g�J��
		 * @param newState[in] �V�����X�e�[�g
		*/
		void CPlayer::ChangeState(nsPlayerConstData::EnPlayerState newState)
		{
			if (m_playerState == newState)
			{
				// �X�e�[�g�������Ȃ�A�������Ȃ��B�������^�[��
				return;
			}

			switch (m_playerState)
			{
			case enWalkAndRun:
				// �����Ƒ���̃T�E���h���~����
				m_playerMove->StopWalkAndRunSound();
				break;
			case enSwing:
				break;
			case enWallRun:
				break;
			case enOnEnemy:
				break; 
			}

			m_playerState = newState;

			switch (newState)
			{
			case enWalkAndRun:
				// �����Ƒ���̃N���X�̈ړ��p�����[�^�����킹��
				m_playerMove->MuchWalkAndRunMoveParam();
				break;
			case enSwing:
				break;
			case enWallRun:
				break;
			case enOnEnemy:
				break;
			case enClearDirecting:
				SetIsInputtable(false);
				m_playerCamera->SetIsControl(false);
				m_playerMove->ResetMoveVecX();
				m_playerMove->ResetMoveVecY();
				m_playerMove->ResetMoveVecZ();
				// �����Ƒ���̃N���X�̈ړ��p�����[�^�����킹��
				m_playerMove->MuchWalkAndRunMoveParam();
				static const Vector3 pos = { 0.0f,20.0f,0.0f };
				static const Vector3 camPos = {100.0f, 50.0f, -200.0f};
				m_playerMove->SetDirectPosition(pos);
				Quaternion qRot;
				qRot.SetRotationDegY(180.0f);
				SetRotation(qRot);
				g_camera3D->SetPosition(camPos);
				g_camera3D->SetTarget(camPos + Vector3::Front * 100.0f);
				break;
			}

			return;
		}

	}
}