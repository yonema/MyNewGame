#pragma once

namespace nsMyGame
{
	namespace nsPlayer { class CPlayer; }
	namespace nsSound { class CSoundCue; }

	namespace nsBGM
	{
		class CBGM : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CBGM() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CBGM() = default;

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
			 * @brief ������
			 * @param[in] player �v���C���[�̎Q��
			*/
			void Init(const nsPlayer::CPlayer& player);

		private:	// �f�[�^�����o
			const nsPlayer::CPlayer* m_playerRef = nullptr;		//!< �v���C���[�N���X�̎Q��

			nsSound::CSoundCue* m_swingBGMSC = nullptr;		//!< �X�C���O����BGM�̃T�E���h

		};

	}
}