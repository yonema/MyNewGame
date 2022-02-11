#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"


namespace nsNinjaAttract
{
	// �O���錾

	namespace nsPlayer { 
		class CPlayer; 
		class CPlayerCommandInput;
	}
	namespace nsGameState { class CGameMainState; }
	namespace nsAICharacter { class CAICar; }
	namespace nsGraphic { namespace nsSprite { class CSpriteRender; } }
	namespace nsSound { class CSoundCue; }

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

			/**
			 * @brief �G��߂܂���X�e�[�g�𓾂�
			 * @return �G��߂܂���X�e�[�g
			*/
			nsPlayerConstData::nsCatchEnemyConstData::EnCatchEnemyState GetCatchEnemyState() const
			{
				return m_catchEnemyState;
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
			 * @brief �E�p�̃G�t�F�N�g��������
			*/
			void InitNinjyutuEffect();

			/**
			 * @brief �T�E���h��������
			*/
			void InitSound();

			/**
			 * @brief QTE�Ɏg���{�^���̃X�v���C�g�̍ď�����
			 * @param[in] commandArray �R�}���h�̔z��
			*/
			void ReInitQTEBUttonSprite(
				const std::vector<nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType>& commandArray
			);

			/**
			 * @brief ���ł����ʂōs����X�V
			*/
			void CommonUpdate();

			/**
			 * @brief �G�̏�ɏ���Ă��鎞�̍X�V
			*/
			void OnEnemyUpdate();

			/**
			 * @brief QTE�Ɏg���{�^���̎c���̍X�V
			*/
			void QTEButternAfterImageUpdate();

			/**
			 * @brief QTE�̌��ʂ̍X�V
			*/
			void QTEResultUpdate();

			/**
			 * @brief �R�}���h�������������̍X�V
			*/
			void SuccessCommandUpdate();

			/**
			 * @brief �^�[�Q�b�g��T��
			*/
			void FindTarget();

			/**
			 * @brief �^�[�Q�b�g��Ԃ��p�����邩���ׂ�
			*/
			void CheckContinueTarget();

			/**
			 * @brief �^�[�Q�b�g�Ƃ̋������X�V
			*/
			void UpdateTargetLength();

			/**
			 * @brief �G��߂܂��邩�ǂ������ׂ�
			*/
			void CheckCatchEnemy();

			/**
			 * @brief QTE�Ɏg���{�^���̎c�����J�n����
			 * @param[in] isSuccess �R�}���h���͂������������H
			*/
			void StartQTEButtonAfterImage(const bool isSuccess);

			/**
			 * @brief QTE�̌��ʂ̃X�v���C�g�̉��o���J�n����
			 * @param[in] qteResultType QTE�̌��ʂ̎��
			*/
			void StartQTEResult(
				const nsPlayerConstData::nsCatchEnemyConstData::EnQTEResultType qteResultType
			);

			/**
			 * @brief QTE�Ŏg���X�v���C�g��L��������
			*/
			void ActivateQTESprite();

			/**
			 * @brief QTE�Ŏg���X�v���C�g��L��������
			*/
			void DeactivateQTESprite();

			/**
			 * @brief �G��߂܂��鏈�����I���������̏���
			*/
			void EndCatchEnemy();

			/**
			 * @brief �X�e�[�g��J�ڂ���
			 * @param[in] newState �V�����X�e�[�g
			*/
			void ChangeState(const nsPlayerConstData::nsCatchEnemyConstData::EnCatchEnemyState newState);

		private:	// �f�[�^�����o
			CPlayer* m_playerRef = nullptr;	//!< �v���C���[�̎Q��
			//!< �G��߂܂���N���X�̃X�e�[�g
			nsPlayerConstData::nsCatchEnemyConstData::EnCatchEnemyState m_catchEnemyState =
				nsPlayerConstData::nsCatchEnemyConstData::enCE_FindTarget;

			float m_targetLength = 0.0f;	//!< �^�[�Q�b�g�Ƃ̋���

			float m_onEnemyTimer = 0.0f;	//!< �G�̏�ɏ���Ă���Ԃ̃^�C�}�[
			float m_ninnjyutuEFTimer = 0.0f;	//!< �E�p�̃G�t�F�N�g�^�C�}�[
			bool m_isStandUp = false;			//!< �N���オ�蒆�H

			//!< �G�̏�ɏ���Ă��鎞�̃^�C�}�[�o�[�̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_onEnemyTimerBar = nullptr;
			//!< �G�̏�ɏ���Ă��鎞�̃^�C�}�[�o�[�̃t���[���̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_onEnemyTimerBarFrame = nullptr;

			//!< QTE�Ɏg���{�^�������̃X�v���C�g�����_���[
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_QTEButtonSRs;
			//!< QTE�Ɏg���{�^���̘g�̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_QTEButtonFraneSR = nullptr;
			//!< QTE�Ɏg���{�^���̎c���X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_QTEButtonAfterImageSR = nullptr;
			float m_buttonAfterImageTimer = 0.0f;	//!< �{�^���̎c���̃^�C�}�[
			//!< QTE�̌��ʂ̃X�v���C�g�����_���[�B
			nsGraphic::nsSprite::CSpriteRender* 
				m_QTEResultSR[nsPlayerConstData::nsCatchEnemyConstData::enQTEResultTypeNum] = {};
			//!< QTE�̌��ʂ̘g�i�����j�̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_QTEResultFrameIn = nullptr;
			//!< QTE�̌��ʂ̘g�i�O���j�̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_QTEResultFrameOut = nullptr;
			float m_resultTimer = 0.0f;	//!< �{�^���̎c���̃^�C�}�[

			std::unique_ptr<CPlayerCommandInput> m_commandInput;	//!< �R�}���h���̓N���X
			int m_oldCommandProgress = 0;							//!< �O�t���[���̃R�}���h�i�s�x

			Effect* m_ninjyutuEF = nullptr;	//!< �E�p�̃G�t�F�N�g
			Effect* m_sonarEF = nullptr;	//!< �\�i�[�̃G�t�F�N�g
			float m_sonarTimer = 0.0f;

			nsAICharacter::CAICar* m_targetRef = nullptr;		//!< �^�[�Q�b�g�̎Q��
			std::vector<nsAICharacter::CAICar*>* m_aiCarsRef = nullptr;	//!< �Ԃ����̎Q��
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< �Q�[���X�e�[�g

			// �T�E���h
			nsSound::CSoundCue* m_commandSuccessSC = nullptr;	//!< �R�}���h���͐����̃T�E���h
			nsSound::CSoundCue* m_commandMissSC = nullptr;		//!< �R�}���h���͎��s�̃T�E���h
			nsSound::CSoundCue* m_windowOpneSC = nullptr;		//!< �E�B���h�E���J���T�E���h
			nsSound::CSoundCue* m_windowCloseSC = nullptr;		//!< �E�B���h�E�����T�E���h
			nsSound::CSoundCue* m_fireStartSC = nullptr;		//!< �Γق̍ŏ��̃T�E���h
			nsSound::CSoundCue* m_fireReleaseSC = nullptr;		//!< �Γق���T�E���h
			nsSound::CSoundCue* m_sonarSC = nullptr;			//!< �\�i�[�̃T�E���h


		};


	}
}
