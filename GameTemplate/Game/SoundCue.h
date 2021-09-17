#pragma once
#include "SoundSource.h"

namespace nsMyGame
{
	/**
	 * @brief �T�E���h
	*/
	namespace nsSound
	{

		/**
		 * @brief �T�E���h�L���[�N���X
		 * �T�E���h�Đ��N���X�ACSoundSource���g���Ă��悢�B
		*/
		class CSoundCue : public IGameObject
		{
		public:		//�񋓌^

			/**
			 * @brief �T�E���h�^�C�v
			*/
			enum EnSoundType
			{
				enSE,	// SE
				enBGM	// BGM
			};
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CSoundCue() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CSoundCue();
		public:		// �����o�֐�

			/**
			 * @brief �������֐�
			 * @param filePath wav�t�@�C���p�X
			 * @param soundType �T�E���h�^�C�v
			*/
			void Init(const wchar_t* filePath, const EnSoundType soundType);

			/**
			 * @brief �T�E���h�Đ�
			 * @param[in] isLoop ���[�v�Đ����H
			*/
			void Play(const bool isLoop);

			/**
			 * @brief �{�����[����ݒ�B1.0f���ʏ�̃{�����[���B
			 * @attention ���̒l������ƁA���̐�Βl�����f�����
			 * @param volume �{�����[��
			*/
			void SetVolume(const float volume);

			/**
			 * @brief �{�����[�����擾
			 * @return �{�����[��
			*/
			float GetVolume() const
			{
				return m_volume;
			}

			/**
			 * @brief �T�E���h�̍Đ����~����B
			 * @details ���̊֐��Œ�~����ƁA���ɍĐ��������ŏ�����̍Đ��ɂȂ�B
			 * @attention ���̎d�l���ƁA���[�v�Đ��͒�~�ł��邪�����V���b�g�Đ��͒�~�ł��Ȃ��B
			*/
			void Stop()
			{
				//���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă�����
				if (!m_loopSoundSource)
					return;
				//�T�E���h���~����
				m_loopSoundSource->Stop();
				m_isPlaying = false;
				return;
			}

			/**
			 * @brief �T�E���h�̍Đ����ꎞ��~����
			 * @derails ���̊֐��ňꎞ��~����ƁA���ɍĐ��������r������̍Đ��ɂȂ�B
			 * @attention ���̎d�l���ƁA���[�v�Đ��͈ꎞ��~�ł��邪�����V���b�g�Đ��͈ꎞ��~�ł��Ȃ��B
			*/
			void Pause()
			{
				//���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă�����
				if (!m_loopSoundSource)
					return;
				//�T�E���h���ꎞ��~����
				m_loopSoundSource->Pause();
				m_isPlaying = false;
				return;
			}

			/**
			 * @brief ���Đ������H
			 * @attention ���̎d�l���ƁA���[�v�Đ��̍Đ����͌��m�ł��邪�����V���b�g�Đ��̍Đ����͌��m�ł��Ȃ��B
			*/
			bool IsPlaying() const
			{
				return  m_isPlaying;
			}

			/**
			 * @brief �T�E���h�̃^�C�v��ݒ�
			 * @details CSoundCue::enSE��CSoundCue::enBGM�Ń^�C�v��ݒ肷��B
			 * @param soundType �T�E���h�^�C�v
			*/
			void SetSoundType(const EnSoundType soundType)
			{
				m_soundType = soundType;
			}

		public:		//static�ȃ����o�֐�

			/**
			 * @brief SE�̃{�����[����ݒ肷��B1.0f���ʏ�B
			 * @param SEVolume SE�̃{�����[��
			*/
			static void SetSEVolume(const float SEVolume)
			{
				m_SEVolume = SEVolume;
			}

			/**
			 * @brief BGM�̃{�����[����ݒ肷��B1.0f���ʏ�
			 * @param BGMVolume 
			*/
			static void SetBGMVolume(const float BGMVolume)
			{
				m_BGMVolume = BGMVolume;
			}
			

		private:	//private�ȃ����o�֐�

			/**
			 * @brief ���[�v�Đ��̏���
			*/
			void PlayLoop();

			/**
			 * @brief �����V���b�g�Đ��̏���
			*/
			void PlayOneShot();

			/**
			 * @brief �^�C�v�ʂ̃{�����[�����擾
			 * @return �^�C�v�ʂ̃{�����[��
			*/
			float GetTypeVolume()
			{
				float volume = 1.0f;

				switch (m_soundType)
				{
				case enSE:
					volume = m_SEVolume;
					break;
				case enBGM:
					volume = m_BGMVolume;
					break;
				}

				return volume;
			}


		private:	//�f�[�^�����o
			CSoundSource* m_loopSoundSource = nullptr;	//!< ���[�v�Đ��p�̃T�E���h�\�[�X
			wchar_t m_filePath[256];					//!< wav�t�@�C���p�X
			float m_volume = 1.0f;						//!< �{�����[��
			bool m_isPlaying = false;					//!< �Đ������H
			EnSoundType m_soundType = enSE;				//!< �T�E���h�^�C�v

		private:	//static�ȃf�[�^�����o
			static float m_SEVolume;					//!< SE�̃{�����[��
			static float m_BGMVolume;					//!< BGM�̃{�����[��

		};
	}
}

