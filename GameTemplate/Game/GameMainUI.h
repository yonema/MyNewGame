#pragma once

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsGraphic {
		namespace nsFont { class CFontRender; }		// �t�H���g�����_���[
		namespace nsSprite { class CSpriteRender; }	// �X�v���C�g�����_���[
	}
	namespace nsUI { 
		class CMiniMap;
		class CEnemyCatchUI;
		class CMissionUI;
	}

	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		/**
		 * @brief �Q�[�����C����UI�N���X
		*/
		class CGameMainUI : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CGameMainUI() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CGameMainUI() = default;

		public:		// �I�[�o�[���C�h���������o�֐�

			/**
			 * @brief Update�̒��O�ŌĂ΂��J�n����
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool Start() override final;

			/**
			 * @brief �X�V����
			*/
			void OnDestroy() override final;

			/**
			 * @brief ��������鎞�ɌĂ΂�鏈��
			*/
			void Update() override final;

		public:		// �����o�֐�

			/**
			 * @brief �^�C�}�[�̃e�L�X�g��ݒ肷��
			 * @param[in] time �^�C��
			*/
			void SetTimerText(const float time);

			/**
			 * @brief �S�[������
			*/
			void Goal();

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �^�C�}�[�̃t�H���g��������
			*/
			void InitTimerFont();

			/**
			 * @brief �S�[���̃X�v���C�g�̏�����
			*/
			void InitGoalSprite();

		private:	// �f�[�^�����o
			nsGraphic::nsFont::CFontRender* m_timerFR = nullptr;	//!< �^�C�}�[�̃t�H���g�����_���[
			nsGraphic::nsSprite::CSpriteRender* m_goalSR = nullptr;	//!< �S�[���̃X�v���C�g�����_���[
			CMiniMap* m_miniMap = nullptr;							//!< �~�j�}�b�v�\���N���X
			CEnemyCatchUI* m_enemyCatchUI = nullptr;				//!< �G��߂܂��鏈���֘A��UI�N���X
			CMissionUI* m_missionUI = nullptr;						//!< �~�b�V����UI�N���X

		};
	}
}

