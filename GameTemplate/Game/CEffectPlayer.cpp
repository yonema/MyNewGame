#include "stdafx.h"
#include "EffectPlayer.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �G�t�F�N�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsEffect
	{
		/**
		 * @brief ��ɌĂ΂��A�b�v�f�[�g�֐�
		*/
		void CEffectPlayer::AlwaysUpdate() 
		{
			// �G�t�F�N�g�̍X�V
			m_effect.Update();

			return;
		}

		/**
		 * @brief �������֐�
		 * @param[in] filePath �t�@�C���p�X
		*/
		void CEffectPlayer::Init(const char16_t* filePath)
		{
			// �G�t�F�N�g�̏�����
			m_effect.Init(filePath);

			return;
		}

		/**
		 * @brief �G�t�F�N�g���Đ�����
		*/
		void CEffectPlayer::Play()
		{
			// �G�t�F�N�g�̍Đ�
			m_effect.Play();

			return;
		}
	}
}