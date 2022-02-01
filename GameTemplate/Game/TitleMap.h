#pragma once
#include "MapConstDatah.h"

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsPlayer { class CPlayer; }			// �v���C���[�N���X
	namespace nsGameState { class CGameMainState; }	// �Q�[�����C���X�e�[�g�N���X
	namespace nsSound { class CSoundCue; }			// �T�E���h�L���[�N���X
	namespace nsBackGround { class CBackGround; }	// �o�b�N�O���E���h�N���X
	namespace nsLevel3D { class CLevel3D; }			// 3D���x���N���X
	namespace nsGraphic { namespace nsSprite { class CSpriteRender; } }	// �X�v���C�g�����_���[�N���X

	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		/**
		 * @brief �^�C�g���}�b�v
		*/
		class CTitleMap : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CTitleMap() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CTitleMap() = default;

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

			/**
			 * @brief �w�i�X�e�[�W�N���X��ݒ肷��
			 * @param[in,out] backGround �w�i�X�e�[�W�N���X
			*/
			void SetBackGround(nsBackGround::CBackGround* backGround)
			{
				m_backGround = backGround;
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �v���C���[�̏�����
			*/
			void InitPlayer();

			/**
			 * @brief �T�E���h�̏�����
			*/
			void InitSound();

			/**
			 * @brief �X�v���C�g�̏�����
			*/
			void InitSprite();

			/**
			 * @brief �X�v���C�g�̍X�V
			*/
			void UpdateSprite();

			/**
			 * @brief �t�F�[�h�C���O�̍X�V
			*/
			void UpdateBeforeFadeIn();

			/**
			 * @brief �t�F�[�h�C���̍X�V
			*/
			void UpdateFadeIn();

			/**
			 * @brief �^�C�g���C���̍X�V
			*/
			void UpdateTitleIn();

			/**
			 * @brief �^�C�g���A�C�h���̍X�V
			*/
			void UpdateTitleIdle();

			/**
			 * @brief �^�C�g���A�E�g�̍X�V
			*/
			void UpdateTitleOut();

			/**
			 * @brief �X�C���O�̉��o�̍X�V
			*/
			void UpdateSwingDirecting();

			/**
			 * @brief �t�F�[�h�A�E�g�̍X�V
			*/
			void  UpdateFadeOut();

			/**
			 * @brief ���C���}�b�v�ɑJ��
			*/
			void ChangeToMainMap();

			/**
			 * @brief �X�e�[�g�J��
			 * @param[in] newState �V�����X�e�[�g
			*/
			void ChangeState(const nsMapConstData::EnTitleState newState);

		private:	// �f�[�^�����o
			std::unique_ptr<nsLevel3D::CLevel3D> m_playerLevel;	//!< �v���C���[�p���x��
			nsPlayer::CPlayer* m_player = nullptr;				//!< �v���C���[�N���X
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< �Q�[���X�e�[�g�N���X
			nsBackGround::CBackGround* m_backGround = nullptr;	//!< �o�b�N�O���E���h�N���X

			nsSound::CSoundCue* m_bgmSC = nullptr;				//!< �^�C�g������BGM
			nsSound::CSoundCue* m_decisionSC = nullptr;			//!< ���艹

			nsGraphic::nsSprite::CSpriteRender* m_titleSR = nullptr;		//!< �^�C�g���̃X�v���C�g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_titleStartSR = nullptr;	//!< �X�^�[�g�̃X�v���C�g�����_���[

			nsMapConstData::EnTitleState m_state = nsMapConstData::enTS_beforeFadeIn;	//!< �X�e�[�g
			float m_timer = 0.0f;														//!< �^�C�}�[

		};
	}
}

