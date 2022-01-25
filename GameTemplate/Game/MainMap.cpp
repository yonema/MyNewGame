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

			delete m_backGround;
			delete m_aiField;

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CMainMap::Update()
		{
			m_startFallTimer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_startFallTimer >= 5.0f)
			{
				// �v���C���[����͉\�ɂ���
				m_player->SetIsInputtable(true);
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
			// �ԗp�̃��x���𐶐�
			m_carLevel = std::make_unique<nsLevel3D::CLevel3D>();
			// ���x�������[�h�B�}�b�v�`�b�v�̃C���X�^���X�͐������Ȃ��B
			m_carLevel->Init(
				kLevelFilePath[enLevelCar],
				[&](nsLevel3D::SLevelObjectData& objData)
				{
#ifdef _DEBUG
					static int num = 0;
					if (num != 0)
					{
						return true;
					}
					num++;
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