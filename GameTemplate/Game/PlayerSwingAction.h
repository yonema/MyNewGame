#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"

#ifdef MY_DEBUG
#include "FontRender.h"
#endif

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �O���錾
		class CPlayer;			// �v���C���[�N���X

		/**
		 * @brief �v���C���[�̓����֘A�̃l�[���X�y�[�X
		*/
		namespace nsPlayerMovenent
		{
			// �O���錾
			class CPlayerMovement;	// �v���C���[�̓����N���X

			/**
			 * @brief �v���C���[�̃X�C���O�̃A�N�V�����N���X
			*/
			class CPlayerSwingAction : private nsUtil::Noncopyable
			{
			private:	// �G�C���A�X�錾
				// �X�C���O�A�N�V�����̃X�e�[�g
				using EnSwingActionState = nsPlayerConstData::nsPlayerSwingActionConstData::EnSwingActionState;

			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CPlayerSwingAction()// = default;
				{
#ifdef MY_DEBUG
					m_debugFont = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFirst);
					m_debugFont->SetText(L"StringLen:");
					m_debugFont->SetPosition({ -100.0f,-100.0f });
#endif
				}
				/**
				 * @brief �f�X�g���N�^
				*/
				~CPlayerSwingAction()// = default;
				{
#ifdef MY_DEBUG
					DeleteGO(m_debugFont);
#endif
				}

			public:		// �����o�֐�

				/**
				 * @brief ������
				 * @param[in.out] player �v���C���[
				 * @param[in,out] playerMovement �v���C���[�ړ��N���X�̎Q��
				*/
				void Init(
					CPlayer* player,
					CPlayerMovement* playerMovement
				);

				/**
				 * @brief �X�C���O�A�N�V���������s
				 * @return �����Ƒ���̏������s�����H
				*/
				bool Execute();

			private:	// private�ȃ����o�֐�

				/**
				 * @brief �X�C���O�^�[�Q�b�g��T��
				*/
				void FindSwingTarget();

				/**
				 * @brief �X�C���O�A�N�V�����̏���
				*/
				void SwingAction();

			private:	// �f�[�^�����o
				CPlayer* m_playerRef = nullptr;					//!< �v���C���[�N���X�̎Q��
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< �v���C���[�ړ��N���X�̎Q��
				const Vector3* m_swingTargetPos = nullptr;		//!< �X�C���O�^�[�Q�b�g�̍��W
				//!< �X�C���O�A�N�V�����̃X�e�[�g
				EnSwingActionState m_swingActionState = 
					nsPlayerConstData::nsPlayerSwingActionConstData::enFindSwingTarget;
				float m_swingRadAngle = 0.0f;						//!< �X�C���O�̃��W�A���p�x

#ifdef MY_DEBUG
				nsGraphic::nsFont::CFontRender* m_debugFont = nullptr;
#endif

			};
		}
	}
}
