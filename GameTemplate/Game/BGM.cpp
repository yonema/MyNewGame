#include "stdafx.h"
#include "BGM.h"
#include "Player.h"
#include "SoundCue.h"
#include "GameTime.h"

namespace nsNinjaAttract
{
	namespace nsBGM
	{
		using namespace nsBGMConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CBGM::Start()
		{
			for (int i = 0; i < enBGMTypeNum; i++)
			{
				// �eBGM�𐶐����ď�����
				m_bgmSC[i] = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_bgmSC[i]->Init(kBGMSoundFilePath[i], nsSound::CSoundCue::enBGM);
				// ���ʂ�0�ɂ��čĐ����Ă���
				m_bgmSC[i]->SetVolume(0.0f);
				m_bgmSC[i]->Play(true);
			}

			// ���݂�BGM�ƃX�e�[�g���m�[�}���ɂ��Ă���
			m_currentBGMType = enBT_Normal;
			m_bgmState = enBT_Normal;
			m_bgmSC[m_currentBGMType]->SetVolume(kBGMSoundVolume[m_currentBGMType]);

			m_resultSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_resultSC->Init(kResultSoundFilePath, nsSound::CSoundCue::enSE);

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CBGM::OnDestroy()
		{
			for (int i = 0; i < enBGMTypeNum; i++)
			{
				DeleteGO(m_bgmSC[i]);
			}
			DeleteGO(m_resultSC);
			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CBGM::Update()
		{
			if (m_isResultSound)
			{
				// ���U���g�T�E���h���́A�X�V���Ȃ��B�������^�[���B
				return;
			}
			// BGM�̃X�e�[�g���X�V����
			UpdateBGMState();

			// �N���X�t�F�[�h���X�V
			UpdateCrossFade();

			return;
		}

		/**
		 * @brief ������
		 * @param[in] player �v���C���[�̎Q��
		*/
		void CBGM::Init(const nsPlayer::CPlayer& player)
		{
			m_playerRef = &player;

			return;
		}

		/**
		 * @brief ���U���g�̃T�E���h�̍Đ�
		*/
		void CBGM::PlayResultSound()
		{
			// ���U���g�̃T�E���h�������V���b�g�Đ�
			m_resultSC->Play(false);
			// ���U���g�̃T�E���h�̉��ʂ�ݒ�
			m_resultSC->SetVolume(kResultSoundVolume);

			// ���U���g�T�E���h���ɂ���
			m_isResultSound = true;

			// ���U���g�T�E���h����BGM�����[�v�Đ��Đ�
			m_bgmSC[enBT_Normal]->Play(true);
			// ���U�A���g�T�E���h����BGM�̉��ʂ�ݒ�
			m_bgmSC[enBT_Normal]->SetVolume(kBGMSoundVolume[m_currentBGMType] * kResultBGMVolume);

			return;
		}

		/**
		 * @brief ���U���g�̃T�E���h�̉��ʂ�ݒ�
		 * @param[in] volume ����
		*/
		void CBGM::SetResultSoundVolume(const float volume)
		{
			m_bgmSC[enBT_Normal]->SetVolume(kBGMSoundVolume[m_currentBGMType] * kResultBGMVolume * volume);

			return;
		}

		/**
		 * @brief �N���X�t�F�[�h��ݒ肷��
		 * @param[in] bgmType ����BGM�̎��
		*/
		void CBGM::SetCrossFade(const nsBGMConstData::EnBGMType bgmType)
		{
			if (m_currentBGMType == bgmType)
			{
				m_currentBGMType = m_nextBGMType;
			}
			// ����BGM��ݒ�
			m_nextBGMType = bgmType;
			// �N���X�t�F�[�h�̃^�C�}�[�ƃ^�C��������������
			m_crossFadeTimer = 0.0f;
			m_crossFadeTime = kCrossFadeTime[m_nextBGMType];
			// �N���X�t�F�[�h�A�I��
			m_isCrossFade = true;

			for (int i = 0; i < enBGMTypeNum; i++)
			{
				// �N���X�t�F�[�h�Ɏg��BGM�ȊO�͉���0�ɂ���
				if (i != m_currentBGMType &&
					i != m_nextBGMType)
				{
					m_bgmSC[i]->SetVolume(0.0f);
				}
			}

			return;
		}

		/**
		 * @brief BGM�̃X�e�[�g���X�V����
		*/
		void CBGM::UpdateBGMState()
		{
			if (m_playerRef->GetState() == nsPlayer::nsPlayerConstData::enSwing)
			{
				// �X�C���O���
				ChangeState(enBT_Swing);
			}
			else if (m_playerRef->GetState() == nsPlayer::nsPlayerConstData::enOnEnemy)
			{
				// QTE���
				ChangeState(enBT_qte);
			}
			else if (m_bgmState == enBT_Swing &&
				m_playerRef->GetPlayerMovement().IsAir())
			{
				// �X�C���O���ɁA�󒆂ŃX�C���O���ł͂Ȃ��Ȃ��Ă��A�n�ʂɍ~���܂ł�
				// �m�[�}���ɖ߂炸�A�X�C���O��Ԃ��ێ����邽�߂�if
			}
			else
			{
				// �m�[�}�����
				ChangeState(enBT_Normal);
			}
			return;
		}

		/**
		 * @brief �N���X�t�F�[�h���X�V
		*/
		void CBGM::UpdateCrossFade()
		{
			if (m_isCrossFade != true)
			{
				// �N���X�t�F�[�h���Ȃ��B�������^�[���B
				return;
			}

			if (m_crossFadeTimer >= m_crossFadeTime)
			{
				// �^�C�}�[���^�C�����I�[�o�[������I��

				// �N���X�t�F�[�h�A�I�t�B
				m_isCrossFade = false;
				// ���݂�BGM�̉��ʂƎ���BGM�̉��ʂ����S�ɐ؂�ւ���
				m_bgmSC[m_currentBGMType]->SetVolume(0.0f);
				m_bgmSC[m_nextBGMType]->SetVolume(1.0f);

				// ����BGM�����݂�BGM�ɂ���
				m_currentBGMType = m_nextBGMType;
			}

			// �^�C�}�[�̐i��ł��闦
			const float rate = m_crossFadeTimer / m_crossFadeTime;

			// �w���֐��I�ω��ɂ�����B�����傫����Ԃ������Ȃ�悤�ɂ���B
			float t = pow(rate, 2.0f);	// �⊮��
			// ���ʂ���`�⊮
			float volume = Math::Lerp<float>(t, kBGMSoundVolume[m_currentBGMType], 0.0f);
			// ���݂�BGM�̉��ʂ�ݒ�
			m_bgmSC[m_currentBGMType]->SetVolume(volume);

			// �w���֐��I�ω��ɂ�����B�����傫����Ԃ������Ȃ�悤�ɂ���B
			t = 1.0f - pow((1.0f - rate), 2.0f);
			// ���ʂ���`�⊮
			volume = Math::Lerp<float>(rate, 0.0f, kBGMSoundVolume[m_nextBGMType]);
			// ����BGM�̉��ʂ�ݒ�
			m_bgmSC[m_nextBGMType]->SetVolume(volume);

			// �^�C�}�[��i�߂�
			m_crossFadeTimer += nsTimer::GameTime().GetFrameDeltaTime();

			return;
		}

		/**
		 * @brief �X�e�[�g�J��
		 * @param[in] newState �V�����X�e�[�g
		*/
		void CBGM::ChangeState(const nsBGMConstData::EnBGMType newState)
		{
			if (m_bgmState == newState)
			{
				return;
			}

			m_bgmState = newState;

			// �X�e�[�g���J�ڂ�����A�N���X�t�F�[�h���s��
			SetCrossFade(m_bgmState);

			switch (m_bgmState)
			{
			case enBT_Normal:
				break;
			case enBT_Swing:
				break;
			}

		}

	}
}