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
			switch (m_gameState->GetGameMainStateState())
			{
			case nsGameState::nsGameMainStateConstData::enGS_startDirecting:
				m_directingTimer += nsTimer::GameTime().GetFrameDeltaTime();

				if (m_directingTimer >= 4.0f)
				{
					// �v���C���[����͉\�ɂ���
					m_player->SetIsInputtable(true);
					m_gameState->ShowMission();
					m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_inGame);
				}
				break;
			case nsGameState::nsGameMainStateConstData::enGS_inGame:
				if (m_player->GetState() != nsPlayer::nsPlayerConstData::enOnEnemy)
				{
					if (g_pad[0]->IsTrigger(enButtonStart))
					{
						m_gameState->ShowMission();
					}
				}

				m_directingTimer = 0.0f;
				break;
			case nsGameState::nsGameMainStateConstData::enGS_beforeClearDirecting:
				break;
			case nsGameState::nsGameMainStateConstData::enGS_startFadeOutToClearDirecting:
				nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeOut(0.5f);
				m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_fadeOutToClearDirecting);
				break;
			case nsGameState::nsGameMainStateConstData::enGS_fadeOutToClearDirecting:
				if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd())
				{
					m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_clearDirecting);
					nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeIn(0.5f);
					m_bgm->PlayResultSound();
				}
				break;
			case nsGameState::nsGameMainStateConstData::enGS_clearDirecting:
				break;
			case nsGameState::nsGameMainStateConstData::enGS_result:
				break;
			case nsGameState::nsGameMainStateConstData::enGS_lastJump:
				// �v���C���[����͉\�ɂ���
				m_player->SetIsInputtable(true);

				if (g_pad[0]->IsTrigger(enButtonA))
				{
					m_decisionSC->Play(false);
					m_gameState->ChangeState(nsGameState::nsGameMainStateConstData::enGS_goTitle);
				}
				break;
			case nsGameState::nsGameMainStateConstData::enGS_goTitle:
				m_directingTimer += nsTimer::GameTime().GetFrameDeltaTime();

				if (m_directingTimer > 1.5f && m_directingTimer <= 1.7f)
				{
					if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd() == true)
					{
						nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->StartFadeOut(0.5f);
					}
				}
				if (nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->IsFadeEnd() != true)
				{
					const float volume = nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->GetFadeRate();
					m_bgm->SetResultSoundVolume(volume);
				}
				if (m_directingTimer < 2.5f)
				{
					m_bgm->SetResultSoundVolume(0.0f);
					return;
				}
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
	}
}