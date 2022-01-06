#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"

namespace nsMyGame
{
	// �O���錾
	namespace nsPlayer
	{
		class CPlayer;
		namespace nsPlayerMovenent { class CPlayerMovement; }
	}
	namespace nsAICharacter { class CAICar; }
	namespace nsSound { class CSoundCue; }

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
			/**
			 * @brief �G�̏�ɏ�铮���N���X
			*/
			class CPlayerOnEnemy : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CPlayerOnEnemy();
				/**
				 * @brief �f�X�g���N�^
				*/
				~CPlayerOnEnemy();

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
				 * @brief �v���C���[�̓G�̏�ɏ�鏈�������s
				*/
				void Execute();

				/**
				 * @brief ������Ԃɖ߂�
				*/
				void Reset();

				/**
				 * @brief �G�̏�ɏ���Ă���X�e�[�g�𓾂�
				 * @return �G�̏�ɏ���Ă���X�e�[�g
				*/
				nsPlayerConstData::nsOnEnemyConstData::EnOnEnemyState GetOnEnemyState() const
				{
					return m_onEnemyState;
				}

			private:	// private�ȃ����o�֐�

				/**
				 * @brief �T�E���h��������
				*/
				void InitSound();

				/**
				 * @brief �G�Ɍ������Ď���L�΂��n�߂鏈��
				*/
				void StartStretchToEnemy();

				/**
				 * @brief �G�l�~�[�Ɍ������Ď���L�΂�����
				*/
				void StretchingToEnemy();

				/**
				 * @brief �G�̏�ɍs������
				*/
				void GoOnEnemy();

				/**
				 * @brief �G�̏�ɂ��鏈��
				*/
				void OnEnemy();

				/**
				 * @brief �G�̕�����������
				*/
				void LookAtEnemy();

				/**
				 * @brief �X�e�[�g��ύX����
				 * @param[in] newState �V�����X�e�[�g
				*/
				void ChangeState(const nsPlayerConstData::nsOnEnemyConstData::EnOnEnemyState newState);

			private:	// �f�[�^�����o
				CPlayer* m_playerRef = nullptr;					//!< �v���C���[�N���X�̎Q��
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< �v���C���[�ړ��N���X�̎Q��
				//!< �G�̏�ɏ��X�e�[�g
				nsPlayerConstData::nsOnEnemyConstData::EnOnEnemyState m_onEnemyState =
					nsPlayerConstData::nsOnEnemyConstData::enStartStretchToEnemy;
				const nsAICharacter::CAICar* m_enemyRef = nullptr;	//!< �G�̎Q��

				Vector3 m_startGoOnEnemyPos = Vector3::Zero;	//!< �G�̏�ɍs���Ƃ��̍ŏ��̍��W
				float m_goOnEnemyTimer = 0.0f;	//!< �G�̏�ɍs���܂ł̃^�C�}�[


				// �T�E���h
				nsSound::CSoundCue* m_chainPutOutSC = nullptr;		//!< �����o���T�E���h
				nsSound::CSoundCue* m_goOnEnemySC = nullptr;		//!< �G�̏�ɏ��ɍs���T�E���h
				nsSound::CSoundCue* m_landingOnEnemySC = nullptr;	//!< �G�̏�ɏ�����T�E���h

			};

		}
	}
}