#include "stdafx.h"
#include "SoundCue.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �T�E���h
	*/
	namespace nsSound
	{
		float CSoundCue::m_SEVolume = 1.0f;
		float CSoundCue::m_BGMVolume = 1.0f;

		/**
		 * @brief �j���������ɌĂ΂��֐�
		*/
		void CSoundCue::OnDestroy()
		{
			//CSoundCue�I�u�W�F�N�g��DeleteGO�����ہA�Đ����������ꍇ�A��~������B
			//if (m_soundSource)
			{
				DeleteGO(m_soundSource);
				m_soundSource = nullptr;
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

			// �T�E���h�^�C�v��o�^
			m_soundType = soundType;

			return;
		}

		/**
		 * @brief �T�E���h�Đ�
		 * @param[in] isLoop ���[�v�Đ����H
		*/
		void CSoundCue::Play(const bool isLoop)
		{


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

			// �Đ����ɂ���
			m_isPlaying = true;

			return;
		}

		/**
		 * @brief ���[�v�Đ��̏���
		*/
		void CSoundCue::PlayLoop()
		{
			// �T�E���h�\�[�X�����łɍ���Ă�����
			if (m_soundSource)
			{
				// �ꎞ��~�������̂��ĊJ����
				m_soundSource->Play(true);
				// �������^�[��
				return;
			}

			// ���[�v�Đ��p�̃T�E���h�\�[�X��
			// �܂����g�p�̏ꍇ��
			// �V��������čĐ�����B
			m_soundSource = NewGO<CSoundSource>(nsCommonData::enPriorityFirst);
			m_soundSource->Init(m_filePath);
			m_soundSource->Play(true);
			float vol = m_volume * GetTypeVolume();
			SetVolume(vol);

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

			// �����V���b�g�Đ����I�[�܂ōĐ����Ă��Ȃ��A����
			// �Đ����ł͂Ȃ�
			if (m_isOnshotEnd != true && m_isPlaying != true)
			{
				// �ꎞ��~�������̂��ĊJ����
				m_soundSource->Play(false);

				// �������^�[��
				return;
			}

			m_soundSource = NewGO<CSoundSource>(nsCommonData::enPriorityFirst);
			m_soundSource->Init(m_filePath);
			float vol = m_volume * GetTypeVolume();
			m_soundSource->SetVolume(vol);	// OS�̏ꍇ�͂����Ń{�����[����ݒ肷��
			m_soundSource->Play(false);
			m_isOnshotEnd = false;

			// �f�X�g���C�C�x���g��ݒ�
			m_soundSource->SetDestroyEvent(
				[&]()
				{
					if (m_soundSource == nullptr)
					{
						return;
					}
					// �T�E���h���Đ����ł͂Ȃ�������
					if (!m_soundSource->IsPlaying())
					{
						// nullptr������
						m_soundSource = nullptr;
						m_isPlaying = false;
						m_isOnshotEnd = true;
					}
				}
			);

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

			// �T�E���h�\�[�X���g�p����Ă��Ȃ�������
			if (!m_soundSource)
			{
				// �������Ȃ�
				return;
			}

			// ���[�v�Đ��p�̃T�E���h�\�[�X�̃{�����[����ݒ肷��
			float vol = m_volume * GetTypeVolume();
			m_soundSource->SetVolume(vol);

			return;
		}

	}
}