#include "stdafx.h"
#include "TitleMap.h"
#include "Player.h"
#include "GameMainState.h"
#include "BackGround.h"
#include "Level3D.h"
#include "SoundCue.h"
#include "SpriteRender.h"
#include "SpriteRenderConstData.h"
#include "BGMConstData.h"
#include "MainMap.h"
#include "GameTime.h"
#include "RenderingEngine.h"
#include "Fade.h"
#include "SavedPlayerInputDataConstData.h"

// ���̓f�[�^��ۑ�����
//#define SAVE_INPUTDATA


namespace nsNinjaAttract
{
	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		using namespace nsMapConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CTitleMap::Start()
		{
			// ��������
			//constexpr float kWorldSoundVolume = 1.0f;
			constexpr float kWorldSoundVolume = 0.2f;
			nsSound::CSoundCue::SetBGMVolume(kWorldSoundVolume);
			nsSound::CSoundCue::SetSEVolume(kWorldSoundVolume);

			// �Q�[���X�e�[�g�̐���
			m_gameState = NewGO<nsGameState::CGameMainState>(
				nsCommonData::enPriorityFirst,
				nsCommonData::kGameObjectName[nsCommonData::enGN_MainGameState]
				);

			// �v���C���[�̏�����
			InitPlayer();

			// �Q�[���X�e�[�g��������
			m_gameState->Init(*m_player);

			// �T�E���h�̏�����
			InitSound();

			// �X�v���C�g�̏�����
			InitSprite();

			// �w�i�X�e�[�W�̐���
			m_backGround = new nsBackGround::CBackGround();
			// �^�C�g�����
			m_backGround->SetIsTitle(true);

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CTitleMap::OnDestroy()
		{
			DeleteGO(m_gameState);
			DeleteGO(m_player);
			DeleteGO(m_bgmSC);
			DeleteGO(m_decisionSC);
			DeleteGO(m_titleSR);
			DeleteGO(m_titleStartSR);

			if (m_backGround)
			{
				// �w�i�X�e�[�W�N���X�̎Q�Ƃ��܂����L���Ă�����A�j������B
				delete m_backGround;
			}

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CTitleMap::Update()
		{
			// �X�v���C�g�̍X�V
			UpdateSprite();

			return;
		}

		/**
		 * @brief �v���C���[�̏�����
		*/
		void CTitleMap::InitPlayer()
		{
			// �v���C���[�̐���
			// �����D��x�����Ȃ��ƁA���̈ʒu�������B
			m_player = NewGO<nsPlayer::CPlayer>(nsCommonData::enPrioritySecond);
			// �v���C���[���^�C�g�����[�h�ɂ���
			m_player->TitleMode();

			// �v���C���[�p�̃��x���𐶐�
			m_playerLevel = std::make_unique<nsLevel3D::CLevel3D>();
			// ���x�������[�h�B�}�b�v�`�b�v�̃C���X�^���X�͐������Ȃ��B
			m_playerLevel->Init(
				kLevelFilePath[enLevelPlayer],
				[&](nsLevel3D::SLevelObjectData& objData)
				{
					// �^�C�g�����̃v���C���[�̃��x����̖��O
					if (objData.EqualObjectName(kPlayerLevelNameInTitle))
					{
						// �Q�[�����̃v���C���[�̍��W�Ɖ�]��ݒ�
						m_player->SetPosition(objData.position);
						m_player->SetRotation(objData.rotation);
					}
					return true;
				}
			);

			return;
		}

		/**
		 * @brief �T�E���h�̏�����
		*/
		void CTitleMap::InitSound()
		{
			// BGM�̃T�E���h�̏�����
			m_bgmSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_bgmSC->Init(
				nsBGM::nsBGMConstData::kBGMSoundFilePaht[nsBGM::nsBGMConstData::enBT_Swing],
				nsSound::CSoundCue::enBGM
			);

			// ���艹�̃T�E���h�̏�����
			m_decisionSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_decisionSC->Init(
				kDecisionSoundFilePath,
				nsSound::CSoundCue::enBGM
			);

			return;
		}

		/**
		 * @brief �X�v���C�g�̏�����
		*/
		void CTitleMap::InitSprite()
		{
			// �^�C�g���̃X�v���C�g�̐����Ə�����
			m_titleSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_titleSR->Init(
				kTitleSpriteFilePath,
				kTitleSpriteWidth,
				kTitleSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			// �X�^�[�g�̃X�v���C�g�̐����Ə�����
			m_titleStartSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_titleStartSR->Init(
				kTitleStartSpriteFilePath,
				kTitleStartSpriteWidth,
				kTitleStartSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			// ������o���Ă���悤�ɓ��������߁AY���W�͍ŏ��͒�߂ɂ��Ă���
			Vector2 titlePos = kTitleSpritePosition;
			titlePos.y = kTitleSpriteInitialHeight;
			// ���W�ƃA���t�@�l��ݒ�B�ŏ��͓����B
			m_titleSR->SetPosition(titlePos);
			m_titleSR->SetAlphaValue(0.0f);
			m_titleStartSR->SetPosition(kTitleStartSpritePosition);
			m_titleStartSR->SetAlphaValue(0.0f);

			return;
		}


		/**
		 * @brief �X�v���C�g�̍X�V
		*/
		void CTitleMap::UpdateSprite()
		{
			// �^�C�}�[��i�߂�
			m_timer += nsTimer::GameTime().GetFrameDeltaTime();

			// �X�e�[�g�ɂ���ď�����U�蕪����
			switch (m_state)
			{
			case enTS_beforeFadeIn:
				// �t�F�[�h�C���O�̍X�V
				UpdateBeforeFadeIn();
				break;
			case enTS_fadeIn:
				// �t�F�[�h�C���̍X�V
				UpdateFadeIn();
				break;
			case enTS_titleIn:
				// �^�C�g���C���̍X�V
				UpdateTitleIn();
				break;
			case enTS_titleIdle:
				// �^�C�g���A�C�h���̍X�V
				UpdateTitleIdle();
				break;
			case enTS_titleOut:
				// �^�C�g���A�E�g�̍X�V
				UpdateTitleOut();
				break;
			case enTS_swingDirecting:
				// �X�C���O�̉��o�̍X�V
				UpdateSwingDirecting();
				break;
			case enTS_fadeOut:
				// �t�F�[�h�A�E�g�̍X�V
				UpdateFadeOut();
				break;
			}


			return;
		}


		/**
		 * @brief �t�F�[�h�C���O�̍X�V
		*/
		void CTitleMap::UpdateBeforeFadeIn()
		{
			// �t�F�[�h�C���J�n
			nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeIn();
			// BGM���Đ�
			m_bgmSC->Play(true);
			// �X�e�[�g���t�F�[�h�C���ɑJ��
			ChangeState(enTS_fadeIn);

			return;
		}

		/**
		 * @brief �t�F�[�h�C���̍X�V
		*/
		void CTitleMap::UpdateFadeIn()
		{
			if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd())
			{
				// �t�F�[�h�C�����I�������A�X�e�[�g���^�C�g���C���֑J��
				ChangeState(enTS_titleIn);
			}

			return;
		}

		/**
		 * @brief �^�C�g���C���̍X�V
		*/
		void CTitleMap::UpdateTitleIn()
		{
			// �^�C�}�[�̐i�ݗ�
			float rate = min(1.0f, m_timer / kTitleInTime);
			// �^�C�g���̍��W
			Vector2 titlePos = kTitleSpritePosition;
			// ���񂾂�Ə�ɏオ���Ă���
			titlePos.y = Math::Lerp<float>(rate, kTitleSpriteInitialHeight, kTitleSpritePosition.y);

			// ���W�ƃA���t�@�l��ݒ�B�A���t�@�l�͏��X�ɕs�����ɂȂ��Ă����B
			m_titleSR->SetPosition(titlePos);
			m_titleSR->SetAlphaValue(rate);

			if (m_timer >= kTitleInTime)
			{
				// �^�C�}�[����莞�Ԃ��߂�����A�X�e�[�g���^�C�g���A�C�h����Ԃ֑J��
				ChangeState(enTS_titleIdle);
			}
			return;
		}

		/**
		 * @brief �^�C�g���A�C�h���̍X�V
		*/
		void CTitleMap::UpdateTitleIdle()
		{
			// �^�C�}�[�̐i�ݗ��B0.0f�`1.0f�͈̔͂���A0.0f�`2.0f�͈̔͂ɕϊ��B
			float rate = min(2.0f, m_timer / (kTitleIdleLoopTime * 0.5f));
			// �A���t�@�l
			float alphaValue = 0.0f;
			if (rate < 1.0f)
			{
				// �^�C�}�[�̐i�ݗ���0.0f�`1.0f�̎��́A���񂾂�ƕs�����ɂȂ��Ă����B
				alphaValue = rate;
			}
			else
			{
				// �^�C�}�[�̐i�ݗ���1.0f�`2.0f�̎��́A���񂾂�Ɠ����ɂȂ��Ă����B
				alphaValue = 1.0f - (rate - 1.0f);
			}

			// �X�^�[�g�̃X�v���C�g�̃A���t�@�l��ݒ�
			m_titleStartSR->SetAlphaValue(alphaValue);

			if (m_timer >= kTitleIdleLoopTime)
			{
				// �^�C�}�[����莞�ԉ߂�����A���Z�b�g�B
				m_timer = 0.0f;
			}

			if (g_pad[0]->IsTrigger(enButtonA))
			{
				// A�{�^���������ꂽ��A�X�e�[�g���^�C�g���A�E�g�ɑJ��
				ChangeState(enTS_titleOut);
				// ���艹���Đ�
				m_decisionSC->Play(false);
			}
			return;
		}


		/**
		 * @brief �^�C�g���A�E�g�̍X�V
		*/
		void CTitleMap::UpdateTitleOut()
		{
			// �^�C�}�[�̐i�ݗ�
			float rate = min(1.0f, m_timer / kTitleOutTime);
			// �A���t�@�l�B���񂾂�Ə������Ȃ�B
			float alphaValue = 1.0f - rate;

			// �X�v���C�g�̃A���t�@�l��ݒ�B���񂾂�Ɠ����ɂȂ�B
			m_titleSR->SetAlphaValue(alphaValue);
			m_titleStartSR->SetAlphaValue(alphaValue);

			if (m_timer >= kTitleOutTime)
			{
				// �^�C�}�[����莞�Ԃ��߂�����A�X�e�[�g���X�C���O���o�ɑJ��
				ChangeState(enTS_swingDirecting);
			}

			return;
		}


		/**
		 * @brief �X�C���O�̉��o�̍X�V
		*/
		void CTitleMap::UpdateSwingDirecting()
		{
#ifdef SAVE_INPUTDATA

			if (g_pad[0]->IsTrigger(enButtonStart))
			{
				// start�{�^���ŁA���͏��̕ۑ����I������B
				m_player->EndSaveInputDataAndSave(
					nsExternalData::nsSavedPlayerInputDataConstData::enST_titleDirecting
				);
				// �X�e�[�g���t�F�[�h�A�E�g�ɑJ��
				ChangeState(enTS_fadeOut);
			}
#else
			if (m_player->IsEndLoadDataProgress())
			{
				// ���[�h�������͏�񂪍Ō�܂Ői�񂾂�A
				// ���[�h�������͏��̎g�p���I������B
				m_player->EndUsingSavedInputData();

				// �X�e�[�g���t�F�[�h�A�E�g�ɑJ��
				ChangeState(enTS_fadeOut);
			}
#endif



			return;
		}

		/**
		 * @brief �t�F�[�h�A�E�g�̍X�V
		*/
		void  CTitleMap::UpdateFadeOut()
		{
			if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd() != true)
			{
				// �t�F�[�h���I������܂ŁA���X��BGM�����������Ă����B
				m_bgmSC->SetVolume(
					1.0f - 1.0f * nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->GetFadeRate()
				);
				// �������^�[��
				return;
			}

			// ���S�̉��ʂ�0�ɂ���
			m_bgmSC->SetVolume(0.0f);

			// ���C���}�b�v�ɑJ��
			ChangeToMainMap();

			return;
		}

		/**
		 * @brief ���C���}�b�v�ɑJ��
		*/
		void CTitleMap::ChangeToMainMap()
		{

			// ���C���}�b�v�𐶐�
			CMainMap* mainMap = NewGO<CMainMap>(
				nsCommonData::enPriorityFirst,
				nsCommonData::kGameObjectName[nsCommonData::enGN_MainMap]
				);
			// �w�i�X�e�[�W�N���X�̎Q�Ƃ����n����
			mainMap->SetBackGround(m_backGround);
			// �Q�Ƃ��̂Ă�
			m_backGround = nullptr;
			// ���M��j������B
			DeleteGO(this);


			return;
		}

		/**
		 * @brief �X�e�[�g�J��
		 * @param[in] newState �V�����X�e�[�g
		*/
		void CTitleMap::ChangeState(const nsMapConstData::EnTitleState newState)
		{
			if (m_state == newState)
			{
				return;
			}

			m_state = newState;
			m_timer = 0.0f;

			switch (m_state)
			{
			case enTS_beforeFadeIn:
				break;
			case enTS_fadeIn:
				break;
			case enTS_titleIn:
				break;
			case enTS_titleIdle:
				break;
			case enTS_titleOut:
				break;
			case enTS_swingDirecting:
#ifdef SAVE_INPUTDATA
				// ���͏��̕ۑ����J�n����
				m_player->StartSaveInputData();
				// �v���C���[�̓��͂�L���ɂ���
				m_player->SetIsInputtable(true);
#else
				// �ۑ����Ă�����͏��̎g�p���J�n����
				m_player->StartUsingSavedInputData(
					nsExternalData::nsSavedPlayerInputDataConstData::enST_titleDirecting
				);
#endif
				break;
			case enTS_fadeOut:
				// �t�F�[�h�A�E�g���J�n
				nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeOut();
				break;
			}

			return;
		}


	}
}