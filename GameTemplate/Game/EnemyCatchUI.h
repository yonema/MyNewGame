#pragma once
#include "UIConstData.h"

namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic {
		namespace nsSprite { class CSpriteRender; }	// �X�v���C�g�����_���[
		namespace nsFont { class CFontRender; }
	}
	namespace nsGameState { class CGameMainState; }
	namespace nsAICharacter { class CAICar; }

	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		/**
		 * @brief �G��߂܂��鏈���֘A��UI
		*/
		class CEnemyCatchUI : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CEnemyCatchUI() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CEnemyCatchUI() = default;

		public:		// �I�[�o�[���C�h���������o�֐�

			/**
			 * @brief Update�̒��O�ŌĂ΂��J�n����
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool Start() override final;

			/**
			 * @brief ��������鎞�ɌĂ΂�鏈��
			*/
			void OnDestroy() override final;

			/**
			 * @brief �X�V����
			*/
			void Update() override final;

		public:		// �����o�֐�

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̏�����
			*/
			void InitCanCatchEnemeySprite();

			/**
			 * @brief �^�[�Q�b�g�������X�v���C�g�̏�����
			*/
			void InitTargetSprite();

			/**
			 * @brief �^�[�Q�b�g�Ƃ̋����������t�H���g�̏�����
			*/
			void InitTargetLengthFont();

			/**
			 * @brief �^�[�Q�b�g��T��
			*/
			void FindTarget();

			/**
			 * @brief �^�[�Q�b�g��Ԃ��p�����邩���ׂ�
			*/
			void CheckContinueTarget();

			/**
			 * @brief ���̃N���X�Ďg�p���鋤�ʂ̃f�[�^���X�V
			*/
			void UpdateCommonData();

			/**
			 * @brief �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̍X�V
			*/
			void UpdateCanCatchEnemySprite();

			/**
			 * @brief �^�[�Q�b�g�������X�v���C�g�̍X�V
			*/
			void UpdateTargetSprite();

			/**
			 * @brief �^�[�Q�b�g�Ƃ̋����������t�H���g�̍X�V
			*/
			void UpdateTargetLengthFont();


		private:	// �f�[�^�����o
			//!< �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* 
				m_canCatchEnemySR[nsEnemyCatchUIConstData::enCanCatchEnemySpriteNum] = {};
			//!< �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�Ɏg���^�C�}�[
			float m_canCatchEnemyTimer = 0.0f;

			//!< �^�[�Q�b�g�������X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_targetSR = nullptr;
			//!< �^�[�Q�b�g�Ƃ̋����������t�H���g�����_���[
			nsGraphic::nsFont::CFontRender* m_targetLengthFR = nullptr;

			float m_targetLength = 0.0f;
			const nsAICharacter::CAICar* m_targetRef = nullptr;		//!< �^�[�Q�b�g�̎Q��
			Vector2 m_taraget2DPos = Vector2::Zero;					//!< �^�[�Q�b�g�̉�ʏ��2D���W
			std::vector<const nsAICharacter::CAICar*> m_aiCarsRef;	//!< �Ԃ�����const�Q��
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< �Q�[���X�e�[�g

		};

	}
}