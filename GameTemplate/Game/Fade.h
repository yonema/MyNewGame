#pragma once

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsGraphic { namespace nsSprite { class CSpriteRender; } }

	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �t�F�[�h�N���X
		*/
		class CFade : public IGameObject
		{
		private:	// �萔
			static const char* const kFadeSpriteFilePath;	//!< �t�F�[�h�p�̃X�v���C�g�̃t�@�C���p�X
			static const float kDefaultFadeTime;			//!< �f�t�H���g�̃t�F�[�h�^�C��
			/**
			 * @brief �t�F�[�h�̏��
			*/
			enum EnFadeState
			{
				enFS_fadeIn,
				enFS_fadeOut,
				enFS_blackOut,
				enFS_fadeEnd
			};

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CFade();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CFade() = default;

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
			 * @brief �t�F�[�h�C���̊J�n
			 * @param[in] fadeTime �t�F�[�h�ɂ����鎞��
			*/
			void StartFadeIn(const float fadeTime = kDefaultFadeTime)
			{
				m_fadeState = enFS_fadeIn;
				m_fadeTime = fadeTime;
				m_fadeTimer = 0.0f;
			};

			/**
			 * @brief �t�F�[�h�A�E�g�̊J�n
			 * @param[in] fadeTime �t�F�[�h�ɂ����鎞��
			*/
			void StartFadeOut(const float fadeTime = kDefaultFadeTime)
			{
				m_fadeState = enFS_fadeOut;
				m_fadeTime = fadeTime;
				m_fadeTimer = 0.0f;
			};

			/**
			 * @brief �����Ƀt�F�[�h�A�E�g����
			*/
			void BlackOut()
			{
				m_fadeState = enFS_blackOut;
			}

			/**
			 * @brief �t�F�[�h���I�����Ă��邩�H�𓾂�
			 * @return �t�F�[�h���I�����Ă��邩�H
			*/
			bool IsFadeEnd() const
			{
				return (m_fadeState == enFS_fadeEnd);
			};

			/**
			 * @brief �t�F�[�h�̐i�s���𓾂�
			 * @return �t�F�[�h�̐i�s��
			*/
			float GetFadeRate() const
			{
				return min(1.0f, m_fadeTimer / m_fadeTime);
			}

		private:	// �f�[�^�����o
			//!< �t�F�[�h�p�̃X�v���C�g
			nsGraphic::nsSprite::CSpriteRender* m_fadeSR = nullptr;	//!< �t�F�[�h�Ɏg�p����X�v���C�g�����_���[
			float m_fadeTimer = 0.0f;								//!< �t�F�[�h�Ɏg�p����^�C�}�[
			float m_fadeTime = kDefaultFadeTime;					//!< �t�F�[�h�ɂ�����^�C��
			EnFadeState m_fadeState = enFS_fadeEnd;					//!< �t�F�[�h�̃X�e�[�g
		};
	}
}
