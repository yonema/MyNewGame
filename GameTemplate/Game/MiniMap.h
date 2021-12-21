#pragma once

namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic {
		namespace nsSprite { class CSpriteRender; }	// �X�v���C�g�����_���[
	}
	namespace nsGameState { class CGameMainState; }

	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		/**
		 * @brief �~�j�}�b�v�\���N���X
		*/
		class CMiniMap : public IGameObject
		{
		private:	// �\����
			/**
			 * @brief �~�j�}�b�v�̃X�v���C�g�p�̒萔�o�b�t�@
			*/
			struct SMiniMapSpriteCB
			{
				Vector2 minPos = Vector2::Zero;		//!< �\������ŏ����W
				Vector2 maxPos = Vector2::Zero;		//!< �\������ő���W
			};

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CMiniMap() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CMiniMap() = default;

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
			 * @brief �~�j�}�b�v�̃X�v���C�g�̏�����
			*/
			void InitMiniMapSprite();

			/**
			 * @brief �v���C���[�̃A�C�R���̃X�v���C�g�̏�����
			*/
			void InitPlayerIconSprite();

			/**
			 * @brief �~�j�}�b�v�̍ŏ����W�ƍő���W���v�Z
			*/
			void CalcMinAndMaxPos();

			/**
			 * @brief �~�j�}�b�v�̊�_���X�V
			*/
			void UpdateMiniMapPivot();

			/**
			 * @brief �~�j�}�b�v�̉�]���X�V
			*/
			void UpdateMiniMapRotate();

			/**
			 * @brief �v���C���[�̃A�C�R������]���X�V
			*/
			void UpdatePlayerIconRotate();

		private:	// �f�[�^�����o
			//!< �~�j�}�b�v�̔w�i�p�X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_miniMapBackSR = nullptr;
			//!< �~�j�}�b�v�ɃX�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_miniMapSR = nullptr;
			//!< �v���C���[�̃A�C�R���̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_playerIconSR = nullptr;

			SMiniMapSpriteCB m_miniMapSpriteCB;					//!< �~�j�}�b�v�p�̒萔�o�b�t�@
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< �Q�[���X�e�[�g
		};

	}
}