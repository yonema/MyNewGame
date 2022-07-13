#include "stdafx.h"
#include "MainMap.h"
#include "MapConstDatah.h"
#include "Player.h"
#include "PlayerConstData.h"
#include "GameMainState.h"
#include "BGM.h"
#include "BackGround.h"
#include "AIField.h"
#include "Level3D.h"
#include "AICar.h"
#include "RenderingEngine.h"
#include "Fade.h"
#include "GameMainStateConstData.h"
#include "TitleMap.h"
#include "SoundCue.h"

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
		bool CMainMap::Start()
		{
			// �Q�[���X�e�[�g�̐���
			m_gameState = NewGO<nsGameState::CGameMainState>(
				nsCommonData::enPriorityFirst,
				nsCommonData::kGameObjectName[nsCommonData::enGN_MainGameState]
				);

			// �v���C���[�̏�����
			InitPlayer();

			// �Q�[���X�e�[�g��������
			m_gameState->Init(*m_player);

			// AI�t�B�[���h�N���X�̐���
			m_aiField = new nsAI::CAIField();

			// �Ԃ̏�����
			InitCar();

			if (m_backGround == nullptr)
			{
				// �w�i�X�e�[�W���ݒ肳��Ă��Ȃ���΁A
				// �w�i�X�e�[�W�𐶐�����B
				m_backGround = new nsBackGround::CBackGround;
			}
			// �^�C�g����ʂł͂Ȃ�
			m_backGround->SetIsTitle(false);

			// BGM�N���X�̐����Ə�����
			m_bgm = NewGO<nsBGM::CBGM>(nsCommonData::enPriorityFirst);
			m_bgm->Init(*m_player);

			// ���艹�̃T�E���h�̐����Ə�����
			m_decisionSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_decisionSC->Init(kDecisionSoundFilePath, nsSound::CSoundCue::enSE);
			// �J�n�{�C�X�̃T�E���h�̐����Ə�����
			m_startVoiceSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_startVoiceSC->Init(kStartVoiceSoundFilePath, nsSound::CSoundCue::enSE);
			m_startVoiceSC->SetVolume(kStartVoiceSoundVolume);

			// �t�F�[�h�C�����J�n
			nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeIn();

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CMainMap::OnDestroy()
		{
			DeleteGO(m_player);
			DeleteGO(m_gameState);
			DeleteGO(m_bgm);

			QueryGOs<nsAICharacter::CAICar>(
				nsCommonData::kGameObjectName[nsCommonData::enGN_Car],
				[&](nsAICharacter::CAICar* car)->bool
				{
					// �Ԃ�S����
					DeleteGO(car);
					return true;
				}
			);

			DeleteGO(m_decisionSC);
			DeleteGO(m_startVoiceSC);

			if (m_backGround)
			{
				// �w�i�X�e�[�W�N���X�̎Q�Ƃ��܂����L���Ă�����A�j������B
				delete m_backGround;
			}
			delete m_aiField;

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CMainMap::Update()
		{
			// �Q�[���X�e�[�g�ɉ����ď�����U�蕪����
			switch (m_gameState->GetGameMainStateState())
			{
			case nsGameState::nsGameMainStateConstData::enGS_startDirecting:
				// �J�n���o�̍X�V
				UpdateStartDirecting();

				break;
			case nsGameState::nsGameMainStateConstData::enGS_inGame:
				// �Q�[�����̍X�V
				UpdateInGame();

				break;
			case nsGameState::nsGameMainStateConstData::enGS_beforeClearDirecting:
				break;
			case nsGameState::nsGameMainStateConstData::enGS_startFadeOutToClearDirecting:
				// �N���A���o�̑O�ɁA�t�F�[�h�A�E�g���s���B
				nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeOut(kFadeOutToClearDirectingTime);
				// �Q�[���X�e�[�g���N���A���o�̑O�̃t�F�[�h�A�E�g���ɑJ��
				m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_fadeOutToClearDirecting);
				break;
			case nsGameState::nsGameMainStateConstData::enGS_fadeOutToClearDirecting:
				// �N���A���o�̑O�̃t�F�[�h�A�E�g�̍X�V
				UpdateFadeOutToClearDirecting();

				break;
			case nsGameState::nsGameMainStateConstData::enGS_clearDirecting:
				break;
			case nsGameState::nsGameMainStateConstData::enGS_result:
				break;
			case nsGameState::nsGameMainStateConstData::enGS_lastJump:
				// �Ō�̃W�����v�̍X�V
				UpdateLastJump();

				break;
			case nsGameState::nsGameMainStateConstData::enGS_goTitle:
				// �^�C�g���֑J�ڂ̍X�V
				UpdateGoTitle();

				break;
			}

			return;
		}

		/**
		 * @brief �v���C���[�̏�����
		*/
		void CMainMap::InitPlayer()
		{
			// �v���C���[�̐���
			// �����D��x�����Ȃ��ƁA���̈ʒu�������B
			m_player = NewGO<nsPlayer::CPlayer>(nsCommonData::enPrioritySecond);
			// �v���C���[����͕s�ɂ���
			m_player->SetIsInputtable(false);

			// �v���C���[�p�̃��x���𐶐�
			m_playerLevel = std::make_unique<nsLevel3D::CLevel3D>();
			// ���x�������[�h�B�}�b�v�`�b�v�̃C���X�^���X�͐������Ȃ��B
			m_playerLevel->Init(
				kLevelFilePath[enLevelPlayer],
				[&](nsLevel3D::SLevelObjectData& objData)
				{
					// �Q�[�����̃v���C���[�̃��x����̖��O
					if (objData.EqualObjectName(kPlayerLevelNameInGame))
					{
						objData.position.y = 2500.0f;
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
		 * @brief �Ԃ̏�����
		*/
		void CMainMap::InitCar()
		{
#ifdef _DEBUG
			int carNumForDebug = 0;
#endif

			// �Ԃ̑��������Z�b�g����
			nsAICharacter::CAICar::ResetCarTortalNumber();

			// �ԗp�̃��x���𐶐�
			m_carLevel = std::make_unique<nsLevel3D::CLevel3D>();
			// ���x�������[�h�B�}�b�v�`�b�v�̃C���X�^���X�͐������Ȃ��B
			m_carLevel->Init(
				kLevelFilePath[enLevelCar],
				[&](nsLevel3D::SLevelObjectData& objData)
				{
#ifdef _DEBUG
					if (carNumForDebug != 0)
					{
						return true;
					}
					carNumForDebug++;
#endif

					// �Ԃ𐶐�
					nsAICharacter::CAICar* car = NewGO<nsAICharacter::CAICar>(
						nsCommonData::enPriorityFirst, 
						nsCommonData::kGameObjectName[nsCommonData::enGN_Car]
						);

					// �Ԃ�������
					car->Init(
						m_aiField->GetAICharaInitData(),
						objData.position,
						objData.rotation,
						Vector3::One
					);

					return true;
				}
			);

			return;
		}

		/**
		 * @brief �J�n���o�̍X�V
		*/
		void CMainMap::UpdateStartDirecting()
		{
			// ���o�p�̃^�C�}�[��i�߂�
			m_directingTimer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_directingTimer >= kStartDirectingTime)
			{
				// �^�C�}�[���J�n���o�̃^�C���𒴂�����

				// �^�C�}�[�����Z�b�g
				m_directingTimer = 0.0f;
				// �v���C���[����͉\�ɂ���
				m_player->SetIsInputtable(true);
				// �~�b�V������\������
				m_gameState->ShowMission();
				// �Q�[���X�e�[�g���Q�[�����ɑJ��
				m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_inGame);
				m_startVoiceSC->Play(false);
			}

			return;
		}

		/**
		 * @brief �Q�[�����̍X�V
		*/
		void CMainMap::UpdateInGame()
		{
			if (m_player->GetState() == nsPlayer::nsPlayerConstData::enOnEnemy)
			{
				// �v���C���[�̃X�e�[�g���A�G�̏�ɂ����Ԃ̂Ƃ��A�������Ȃ��B�������^�[���B
				return;
			}

			// �v���C���[�̃X�e�[�g���A�G�̏�ɂ����Ԃł͂Ȃ��Ƃ�


			if (g_pad[0]->IsTrigger(enButtonStart))
			{
				// start�{�^������������A�~�b�V������\��
				m_gameState->ShowMission();
			}

			return;
		}


		/**
		 * @brief �N���A���o�̑O�̃t�F�[�h�A�E�g�̍X�V
		*/
		void CMainMap::UpdateFadeOutToClearDirecting()
		{
			if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd())
			{
				// �t�F�[�h�A�E�g���I��������

				// �t�F�[�h�C�����s��
				nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeIn(kFadeInToClearDirectingTime);
				// �Q�[���X�e�[�g���N���A���o�֑J��
				m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_clearDirecting);

				// ���U���g�̃T�E���h���Đ�����
				m_bgm->PlayResultSound();
			}

			return;
		}

		/**
		 * @brief �Ō�̃W�����v�̍X�V
		*/
		void CMainMap::UpdateLastJump()
		{
			// �v���C���[����͉\�ɂ���
			m_player->SetIsInputtable(true);

			if (g_pad[0]->IsTrigger(enButtonA))
			{
				// A�{�^���������ꂽ��

				// ���艹�������V���b�g�Đ�
				m_decisionSC->Play(false);
				// �Q�[���X�e�[�g���^�C�g���ֈړ��ɑJ��
				m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_goTitle);
			}
			return;
		}

		/**
		 * @brief �^�C�g���֑J�ڂ̍X�V
		*/
		void CMainMap::UpdateGoTitle()
		{
			// ���o�p�̃^�C�}�[��i�߂�
			m_directingTimer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_directingTimer > kFadeOutGoTitleMinActiveTime && m_directingTimer <= kFadeOutGoTitleMaxActiveTime)
			{
				// �^�C�}�[���^�C�g���֑J�ڂ��邽�߂̃t�F�[�h�A�E�g�̗L���^�C��
				if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd() == true)
				{
					// �t�F�[�h���ł͂Ȃ�������A�t�F�[�h�A�E�g�J�n
					nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeOut(kFadeOutGoTitleTime);
				}
			}

			if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd() != true)
			{
				// �t�F�[�h���Ȃ�

				// �t�F�[�h�̐i�s���ɉ����āA���U���g��BGM�̉��ʂ������Ă���
				const float volume = nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->GetFadeRate();
				m_bgm->SetResultSoundVolume(volume);
			}

			if (m_directingTimer < kGoTitleDirectingTime)
			{
				// �^�C�}�[���^�C�g���֍s�����߂̉��o�^�C����菬�����B
				// �܂��^�C�g���֍s���Ȃ��B�������^�[���B
				return;
			}

			// bgm�����ʂ����S�ɏ���
			m_bgm->SetResultSoundVolume(0.0f);

			// �^�C�g���}�b�v�𐶐�
			CTitleMap* titleMap = NewGO<CTitleMap>(
				nsCommonData::enPriorityFirst,
				nsCommonData::kGameObjectName[nsCommonData::enGN_TitleMap]
				);
			// �w�i�X�e�[�W�N���X�̎Q�Ƃ����n����
			titleMap->SetBackGround(m_backGround);

			// �Q�Ƃ��̂Ă�
			m_backGround = nullptr;
			// ���M��j������B
			DeleteGO(this);
			return;
		}
	}
}