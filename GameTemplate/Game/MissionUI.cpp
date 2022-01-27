#include "stdafx.h"
#include "MissionUI.h"
#include "SpriteRender.h"
#include "SpriteRenderConstData.h"
#include "FontRender.h"
#include "../../ExEngine/level2D/Level2D.h"
#include "GameTime.h"

namespace nsNinjaAttract
{
	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		using namespace nsMissionUIConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CMissionUI::Start()
		{
			// �X�v���C�g�̏�����
			InitSprite();

			// �t�H���g�̏�����
			InitFont();

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CMissionUI::OnDestroy()
		{
			// �X�v���C�g�̔j��
			QueryAllMissionSRs(
				[](nsGraphic::nsSprite::CSpriteRender* spriteRender) 
				{
					DeleteGO(spriteRender);
				}
			);

			// �t�H���g��̔j��
			DeleteGO(m_clearTimeFR);
			DeleteGO(m_numOfMissFR);

			return;
		}
		
		/**
		 * @brief �~�b�V������\������
		*/
		void CMissionUI::ShowMission()
		{
			ChangeState(enMS_showMission);
			return;
		}

		/**
		 * @brief �~�b�V��������N���A�������̏���
		 * @param[in] missionType �N���A�����~�b�V�����̃^�C�v
		*/
		void CMissionUI::ClearOneMission(const nsMissionUIConstData::EnMissionType missionType)
		{
			m_currentClearMissionType = missionType;
			ChangeState(enMS_clearOneMission);

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CMissionUI::Update()
		{
			switch (m_missionState)
			{
			case enMS_none:
				if (g_pad[0]->IsTrigger(enButtonA))
				{
					ShowMission();
				}
				else if (g_pad[0]->IsTrigger(enButtonB))
				{
					ClearOneMission(enMT_carSilver);
				}
				else if (g_pad[0]->IsTrigger(enButtonY))
				{
					ClearOneMission(enMT_carRed);
				}
				else if (g_pad[0]->IsTrigger(enButtonX))
				{
					ClearOneMission(enMT_carBlue);
				}
				break;
			case enMS_showMission:
				// �~�b�V������\�����鏈�����X�V
				UpdataShowMission();
				break;
			case enMS_hideMission:
				// �~�b�V�������\���ɂ��鏈�����X�V
				UpdateHideMission();
				break;
			case enMS_clearOneMission:
				// �~�b�V��������N���A�������̏����̍X�V
				UpdateClearOneMission();
				break;
			}
			return;
		}

		/**
		 * @brief �X�v���C�g�̏�����
		*/
		void CMissionUI::InitSprite()
		{
			// �~�b�V�����̎�ނ̐��������T�C�Y
			m_checkMarkSRs.resize(enMissionTypeNum);
			m_checkMarkOffsets.resize(enMissionTypeNum);
			m_checkMarkFlag.resize(enMissionTypeNum);

			// ���x��2D�̃C���X�^���X�𐶐�
			m_missionLevel = new Level2D;

			m_missionLevel->Init(
				kMissionLevel2DFilePath, 
				[&](Level2DObjectData& objData)->bool
				{
					if (objData.EqualObjectName(kMissionLevelObjName))
					{
						// �~�b�V�����E�B���h�E�̃X�v���C�g�̐����Ə�����
						m_missionWindowSR = 
							NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird + objData.numOfLyaer);
						m_missionWindowSR->Init(
							kMissionSpriteFilePath,
							objData.width,
							objData.height,
							nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
							AlphaBlendMode_Trans
						);
						// ���W��ݒ�
						m_missionWindowSR->SetPosition(objData.position + kMissionStartPosition);

						return true;
					}

					for (int missionType = 0; missionType < enMissionTypeNum; missionType++)
					{
						// �e�~�b�V�����̎�ޑS�Ă𒲂ׂ�
						if (objData.EqualObjectName(kCheckMarkLevelObjNames[missionType]))
						{
							m_checkMarkSRs[missionType] = 
								NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird + objData.numOfLyaer);
							
							// �`�F�b�N�}�[�N�̏�����
							m_checkMarkSRs[missionType]->Init(
								kCheckMarkSpriteFilePath,
								objData.width * objData.scale.x,
								objData.height * objData.scale.y,
								nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
								AlphaBlendMode_Trans
							);

							// �I�t�Z�b�g���i�[
							m_checkMarkOffsets[missionType] = objData.position;
							// �N���A�t���O���Z�b�g
							m_checkMarkFlag[missionType] = false;

							// ���W��ݒ�
							m_checkMarkSRs[missionType]->SetPosition(m_checkMarkOffsets[missionType] + kMissionPosition);

							return true;
						}
					}

					// �}�b�v�`�b�v�ŕ\������\�肪�Ȃ����߁A���ׂ�true��߂��B
					return true;
				});

			// �����g�p���Ȃ��̂ŁA�C���X�^���X��j���B
			delete m_missionLevel;

			// �~�b�V�������U���g�̘g�̃X�v���C�g�̐����Ə�����
			m_missionResultFrameSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFourth);
			m_missionResultFrameSR->Init(
				kMissionResultFrameFilePath,
				kMissionResultFrameSpriteWidth,
				kMissionResultFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			// �~�b�V�������U���g�̃e�L�X�g�̃X�v���C�g�̐����Ə�����
			m_missionResultTextSRs.reserve(enMissionResultTypeNum);
			for (int mrt = 0; mrt < enMissionResultTypeNum; mrt++)
			{
				m_missionResultTextSRs.emplace_back(NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFourth));
				m_missionResultTextSRs[mrt]->Init(
					kMissionResultTextSpriteFilepath[mrt],
					kMissionResultTextSpriteWidth,
					kMissionResultTextSpriteHeight,
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);

			}

			// �S�ẴX�v���C�g���\��
			QueryAllMissionSRs(
				[](nsGraphic::nsSprite::CSpriteRender* spriteRender)
				{
					spriteRender->Deactivate();
					spriteRender->SetAlphaValue(0.0f);
					return;
				}
			);

			return;
		}

		/**
		 * @brief �t�H���g�̏�����
		*/
		void CMissionUI::InitFont()
		{
			m_clearTimeFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFinal);
			m_clearTimeFR->SetText(L"01:00");
			m_clearTimeFR->SetPosition({ 100.0f,-100.0f });
			m_clearTimeFR->Deactivate();

			m_numOfMissFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFinal);
			m_numOfMissFR->SetText(L"0");
			m_numOfMissFR->SetPosition({ 100.0f,-120.0f });
			m_numOfMissFR->Deactivate();


			return;
		}


		/**
		 * @brief �~�b�V������\�����鏈�����X�V
		*/
		void CMissionUI::UpdataShowMission()
		{
			// �^�C�}�[��i�߂�
			m_timer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_timer <= kStartShowMissionTime)
			{
				const float rate = m_timer / kStartShowMissionTime;
				Vector3 pos = Vector3::Zero;
				pos.Lerp(rate, kMissionStartPosition, kMissionPosition);

				m_missionWindowSR->SetPosition(pos);
				m_missionWindowSR->SetAlphaValue(rate);

				for (int i = 0; i < enMissionTypeNum ; i++)
				{
					if (m_checkMarkFlag[i] != true)
					{
						// �N���A���ĂȂ��~�b�V�����͍X�V���Ȃ�
						continue;
					}
					m_checkMarkSRs[i]->SetPosition(pos + m_checkMarkOffsets[i]);
					m_checkMarkSRs[i]->SetAlphaValue(rate);
				}

				return;
			}
			else if (m_timer <= kShowMissionTime)
			{
				m_missionWindowSR->SetPosition(kMissionPosition);
				m_missionWindowSR->SetAlphaValue(1.0f);
				for (int i = 0; i < enMissionTypeNum; i++)
				{
					if (m_checkMarkFlag[i] != true)
					{
						// �N���A���ĂȂ��~�b�V�����͍X�V���Ȃ�
						continue;
					}
					m_checkMarkSRs[i]->SetPosition(kMissionPosition + m_checkMarkOffsets[i]);
					m_checkMarkSRs[i]->SetAlphaValue(1.0f);
				}
				return;
			}
			else
			{
				ChangeState(enMS_hideMission);
				return;
			}

			return;
		}

		/**
		 * @brief �~�b�V�������\���ɂ��鏈�����X�V
		*/
		void CMissionUI::UpdateHideMission()
		{
			// �^�C�}�[��i�߂�
			m_timer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_timer <= kHideMissionTime)
			{
				const float rate = m_timer / kHideMissionTime;
				Vector3 pos = Vector3::Zero;
				pos.Lerp(rate, kMissionPosition, kMissionStartPosition);

				const float alphaValue = 1.0f - rate;

				m_missionWindowSR->SetPosition(pos);
				m_missionWindowSR->SetAlphaValue(alphaValue);

				for (int i = 0; i < enMissionTypeNum; i++)
				{
					if (m_checkMarkFlag[i] != true)
					{
						// �N���A���ĂȂ��~�b�V�����͍X�V���Ȃ�
						continue;
					}
					m_checkMarkSRs[i]->SetPosition(pos + m_checkMarkOffsets[i]);
					m_checkMarkSRs[i]->SetAlphaValue(alphaValue);
				}
				return;
			}
			else
			{
				m_missionWindowSR->SetPosition(kMissionStartPosition);
				m_missionWindowSR->SetAlphaValue(0.0f);
				m_missionWindowSR->Deactivate();

				for (int i = 0; i < enMissionTypeNum; i++)
				{
					if (m_checkMarkFlag[i] != true)
					{
						// �N���A���ĂȂ��~�b�V�����͍X�V���Ȃ�
						continue;
					}
					m_checkMarkSRs[i]->SetPosition(kMissionStartPosition + m_checkMarkOffsets[i]);
					m_checkMarkSRs[i]->SetAlphaValue(0.0f);
				}
				ChangeState(enMS_none);
				return;
			}

			return;
		}

		/**
		 * @brief �~�b�V��������N���A�������̏����̍X�V
		*/
		void CMissionUI::UpdateClearOneMission()
		{

			UpdataShowMission();

			if (m_timer <= kWaitStartClearOneMissionTime)
			{
				if (m_timer <= 0.0f + FLT_EPSILON)
				{
					m_checkMarkFlag[m_currentClearMissionType] = true;
				}
				return;
			}
			else if (m_timer <= kInCheckMarkTime)
			{
				const float rate = (m_timer - kWaitStartClearOneMissionTime) / 
					(kInCheckMarkTime - kWaitStartClearOneMissionTime);
				const float scale = Math::Lerp<float>(rate, kInCheckMarkStartScale, kInCheckMarkEndScale);
				const float alphaValue = Math::Lerp<float>(rate, 0.0f, kInCheckMarkEndAlphaValue);
				m_checkMarkSRs[m_currentClearMissionType]->SetScale(scale);
				m_checkMarkSRs[m_currentClearMissionType]->SetAlphaValue(alphaValue);
				return;
			}
			else if (m_timer <= kScaleUpCheckMarkTime)
			{
				const float rate = 
					(m_timer - kInCheckMarkTime) / (kScaleUpCheckMarkTime - kInCheckMarkTime);
				const float scale = Math::Lerp<float>(rate, 1.0f, kScaleUpCheckMarkEndScale);
				m_checkMarkSRs[m_currentClearMissionType]->SetScale(scale);
				m_checkMarkSRs[m_currentClearMissionType]->SetAlphaValue(1.0f);
				return;
			}
			else if (m_timer <= kScaleuDownCheckMarkTime)
			{
				const float rate = 
					(m_timer - kScaleUpCheckMarkTime) / (kScaleuDownCheckMarkTime - kScaleUpCheckMarkTime);
				const float scale = Math::Lerp<float>(rate, kScaleUpCheckMarkEndScale, 1.0f);
				m_checkMarkSRs[m_currentClearMissionType]->SetScale(scale);
				return;
			}
			else if (m_timer <= kShowMissionTime)
			{
				m_checkMarkSRs[m_currentClearMissionType]->SetScale(1.0f);
				return;
			}

			return;
		}

		/**
		 * @brief �~�b�V�����̃X�v���C�g�����_���[�S�ĂɃN�G�����s��
		 * @param[in] func ���s����֐�
		*/
		void CMissionUI::QueryAllMissionSRs(
			const std::function<void(nsGraphic::nsSprite::CSpriteRender* spriteRender)> func
		)
		{
			for (auto& checkMarkSR : m_checkMarkSRs)
			{
				func(checkMarkSR);
			}
			func(m_missionWindowSR);
			func(m_missionResultFrameSR);
			for (auto& mrtSR : m_missionResultTextSRs)
			{
				func(mrtSR);
			}

			return;
		}

		/**
		 * @brief �X�e�[�g��J�ڂ���
		 * @param[in] newState �V�����X�e�[�g
		*/
		void CMissionUI::ChangeState(nsMissionUIConstData::EnMissionState newState)
		{
			if (m_missionState == newState)
			{
				return;
			}

			m_missionState = newState;

			m_timer = 0.0f;

			switch (m_missionState)
			{
			case enMS_none:
				break;
			case enMS_showMission:
				m_missionWindowSR->Activate();
				break;
			case enMS_hideMission:
				break;
			case enMS_clearOneMission:
				m_missionWindowSR->Activate();
				m_checkMarkSRs[m_currentClearMissionType]->Activate();
				break;
			}

			return;
		}
	}
}