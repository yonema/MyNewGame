#include "stdafx.h"
#include "Player.h"
#include "PlayerConstData.h"

namespace nsMyGame
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
			// �v���C���[���f���N���X�̏�����
			m_playerModel.Init(m_position, m_rotation);

			// �v���C���[�̓��͏��N���X�̏�����
			m_playerInput.Init(&m_playerInputData);

			// �v���C���[�J�����N���X�̏�����
			m_playerCamera.Init(m_position, m_playerInputData);

			// �v���C���[�ړ��N���X�̏�����
			m_playerMove.Init(
				kCapsuleRadius,
				kDapsuleHeight,
				&m_position,
				&m_rotation,
				m_playerCamera,
				m_playerInputData
				);

			return true;
		}

		/**
		 * @brief �j���������ɌĂ΂��֐�
		*/
		void CPlayer::OnDestroy()
		{


			return;
		}

		/**
		 * @brief �A�b�v�f�[�g�֐�
		*/
		void CPlayer::Update()
		{
			// ���͏��������s
			m_playerInput.ExecuteInput();

			// �v���C���[�̈ړ������s
			m_playerMove.ExecuteUpdate();

			// �J�����N���X���X�V
			m_playerCamera.ExecuteUpdate();

			// ���f���N���X���X�V
			m_playerModel.ExecuteUpdate();

			return;
		}
	}
}