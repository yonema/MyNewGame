#include "stdafx.h"
#include "SoundCue.h"

namespace nsMyGame
{
	/**
	 * @brief �T�E���h
	*/
	namespace nsSound
	{
		float CSoundCue::m_SEVolume = 1.0f;
		float CSoundCue::m_BGMVolume = 1.0f;

		/**
		 * @brief �f�X�g���N�^
		*/
		CSoundCue::~CSoundCue()
		{
			//CSoundCue�I�u�W�F�N�g��DeleteGO�����ہA�Đ����������ꍇ�A��~������B
			if (m_loopSoundSource) {
				DeleteGO(m_loopSoundSource);
			}

			return;
		}


		/**
		 * @brief �������֐�
		 * @param filePath wav�t�@�C���p�X
		 * @param soundType �T�E���h�^�C�v
		*/
		void CSoundCue::Init(const wchar_t* filePath, const EnSoundType soundType)
		{
			//�����œn���ꂽ const wchat_t* ���A
			//�f�[�^�����o�� wchar �̔z��ɓ����
			swprintf_s(m_filePath, filePath);

			//��x�A����Ĕj�����邱�ƂŁA
			//Flyweight�p�^�[�����g�p���Ă���WaveFileBank��
			//�o�^������B
			CSoundSource* ss = NewGO<CSoundSource>(nsCommonData::enPriorityFirst);
			ss->Init(m_filePath);
			DeleteGO(ss);

			return;
		}

		/**
		 * @brief �T�E���h�Đ�
		 * @param[in] isLoop ���[�v�Đ����H
		*/
		void CSoundCue::Play(const bool isLoop)
		{
			// �Đ����ɂ���
			m_isPlaying = true;

			// ���[�v�Đ��������V���b�g�Đ����ŏ�����U�蕪����
			if (isLoop)
			{
				// ���[�v�Đ�
				PlayLoop();
			}
			else
			{
				// �����V���b�g�Đ�
				PlayOneShot();
			}

			return;
		}

		/**
		 * @brief ���[�v�Đ��̏���
		*/
		void CSoundCue::PlayLoop()
		{
			// ���[�v�Đ��p�̃T�E���h�\�[�X��
			// ���łɍ���Ă�����
			if (m_loopSoundSource)
			{
				// �ꎞ��~�������̂��ĊJ����
				m_loopSoundSource->Play(true);

			}
			else
			{
				// ���[�v�Đ��p�̃T�E���h�\�[�X��
				// �܂����g�p�̏ꍇ��
				// �V��������čĐ�����B
				m_loopSoundSource = NewGO<CSoundSource>(nsCommonData::enPriorityFirst);
				m_loopSoundSource->Init(m_filePath);
				m_loopSoundSource->Play(true);
				float vol = m_volume * GetTypeVolume();
				SetVolume(vol);
			}

			return;
		}

		/**
		 * @brief �����V���b�g�Đ��̏���
		*/
		void CSoundCue::PlayOneShot()
		{
			// ����A�T�E���h�\�[�X������čĐ�������
			// Flyweight�p�^�[�����g�p���Ă��邩��
			// �d���Ȃ�Ȃ����I
			CSoundSource* ss = NewGO<CSoundSource>(nsCommonData::enPriorityFirst);
			ss->Init(m_filePath);
			float vol = m_volume * GetTypeVolume();
			ss->SetVolume(vol);	// OS�̏ꍇ�͂����Ń{�����[����ݒ肷��
			ss->Play(false);

			return;
		}

		/**
		 * @brief �{�����[����ݒ�B1.0f���ʏ�̃{�����[���B
		 * @attention ���̒l������ƁA���̐�Βl�����f�����
		 * @param volume �{�����[��
		*/
		void CSoundCue::SetVolume(const float volume)
		{
			// �{�����[����ێ�
			m_volume = volume;

			// ���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă�����
			if (!m_loopSoundSource)
				return;

			// ���[�v�Đ��p�̃T�E���h�\�[�X�̃{�����[����ݒ肷��
			float vol = m_volume * GetTypeVolume();
			m_loopSoundSource->SetVolume(vol);

			return;
		}

	}
}