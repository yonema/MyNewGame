#include "stdafx.h"
#include "Player.h"

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
			// �v���C���[���f���A�j���[�V�����N���X�̏�����
			m_playerModelAnimation.Init(*this);

			// �v���C���[�̓��͏��N���X�̏�����
			m_playerInput.Init(this);

			// �v���C���[�J�����N���X�̏�����
			m_playerCamera.Init(*this);

			// �v���C���[�ړ��N���X�̏�����
			m_playerMove.Init(
				kCapsuleRadius,
				kDapsuleHeight,
				this
				);

			// �v���C���[�̎��̃��f���N���X�̐����Ə�����
			m_playerStringModel = NewGO<CPlayerStringModel>(nsCommonData::enPriorityFirst);
			m_playerStringModel->Init(*this);

			return true;
		}

		/**
		 * @brief �j���������ɌĂ΂��֐�
		*/
		void CPlayer::OnDestroy()
		{
			// �v���C���[�̎��̃��f���N���X�̔j��
			DeleteGO(m_playerStringModel);

			return;
		}

		/**
		 * @brief �A�b�v�f�[�g�֐�
		*/
		void CPlayer::Update()
		{
			// ���͏��������s
			m_playerInput.ExecuteUpdate();

			// �v���C���[�̈ړ������s
			m_playerMove.ExecuteUpdate();

			// �J�����N���X���X�V
			m_playerCamera.ExecuteUpdate();

			// ���f���A�j���[�V�����N���X���X�V
			m_playerModelAnimation.ExecuteUpdate();

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
		 * @brief �����w�肵�����W�Ɍ������ĐL�т鏈�����I������
		*/
		void CPlayer::EndStringStretchToPos()
		{
			m_playerStringModel->EndStretchToPos();

			return;
		}

	}
}