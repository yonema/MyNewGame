#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"


namespace nsMyGame
{
	// �O���錾

	namespace nsPlayer { class CPlayer; }
	namespace nsGameState { class CGameMainState; }
	namespace nsAICharacter { class CAICar; }
	namespace nsGraphic { namespace nsSprite { class CSpriteRender; } }

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		/**
		 * @brief �v���C���[���G��߂܂��鏈���N���X
		*/
		class CPlayerCatchEnemy : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerCatchEnemy();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CPlayerCatchEnemy();

		public:		// �����o�֐�

			/**
			 * @brief ����������
			 * @param[in,out] playerRef �v���C���[�̎Q��
			*/
			void Init(CPlayer* playerRef);

			/**
			 * @brief ���C���[���G��߂܂��鏈���N���X�̃A�b�v�f�[�g�����s
			*/
			void ExecuteUpdate();

			/**
			 * @brief �^�[�Q�b�g�̓G�̎Q�Ƃ𓾂�
			 * @return �^�[�Q�b�g�̓G�̎Q��
			*/
			const nsAICharacter::CAICar* GetTargetEnemy() const
			{
				return m_targetRef;
			}

			/**
			 * @brief �^�[�Q�b�g�Ƃ̋����𓾂�
			 * @return �^�[�Q�b�g�Ƃ̋���
			*/
			float GetTargetLength() const
			{
				return m_targetLength;
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �^�C�}�[�o�[�̃X�v���C�g�̏�����
			*/
			void InitTimerBarSprite();

			/**
			 * @brief QTE�Ɏg���{�^���̃X�v���C�g�̏�����
			*/
			void InitQTEButtonSprite();

			/**
			 * @brief �G�̏�ɏ���Ă��鎞�̍X�V
			*/
			void OnEnemyUpdate();

			/**
			 * @brief �^�[�Q�b�g��T��
			*/
			void FindTarget();

			/**
			 * @brief �^�[�Q�b�g��Ԃ��p�����邩���ׂ�
			*/
			void CheckContinueTarget();

			/**
			 * @brief ���̃N���X�Ŏg�����ʃf�[�^�̍X�V
			*/
			void UpdateCommonData();

			/**
			 * @brief �G��߂܂��邩�ǂ������ׂ�
			*/
			void CheckCatchEnemy();

		private:	// �f�[�^�����o
			CPlayer* m_playerRef = nullptr;	//!< �v���C���[�̎Q��

			float m_targetLength = 0.0f;	//!< �^�[�Q�b�g�Ƃ̋���

			float m_onEnemyTimer = 0.0f;	//!< �G�̏�ɏ���Ă���Ԃ̃^�C�}�[

			//!< �G�̏�ɏ���Ă��鎞�̃^�C�}�[�o�[�̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_onEnemyTimerBar = nullptr;
			//!< �G�̏�ɏ���Ă��鎞�̃^�C�}�[�o�[�̃t���[���̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_onEnemyTimerBarFrame = nullptr;

			//!< QTE�Ɏg���{�^�������̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* 
				m_onQTEButtonSRs[nsPlayer::nsPlayerConstData::nsCatchEnemyConstData::enQTEButtouTypeNum]
				= {};
			//!< QTE�Ɏg���{�^���̘g�̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_onQTEButtonFraneSR = nullptr;


			nsAICharacter::CAICar* m_targetRef = nullptr;		//!< �^�[�Q�b�g�̎Q��
			std::vector<nsAICharacter::CAICar*>* m_aiCarsRef = nullptr;	//!< �Ԃ����̎Q��
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< �Q�[���X�e�[�g
		};


	}
}
