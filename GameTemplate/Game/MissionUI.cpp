#include "stdafx.h"
#include "MissionUI.h"
#include "UIConstData.h"
#include "SpriteRender.h"
#include "SpriteRenderConstData.h"
#include "FontRender.h"
#include "../../ExEngine/level2D/Level2D.h"

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
			for (auto& checkMarkSR : m_checkMarkSRs)
			{
				DeleteGO(checkMarkSR);
			}
			DeleteGO(m_missionWindowSR);
			DeleteGO(m_missionResultFrameSR);
			for (auto& missionResultTextSR : m_missionResultTextSRs)
			{
				DeleteGO(missionResultTextSR);
			}

			// �t�H���g��̔j��
			DeleteGO(m_clearTimeFR);
			DeleteGO(m_numOfMissFR);

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CMissionUI::Update()
		{
			return;
		}

		/**
		 * @brief �X�v���C�g�̏�����
		*/
		void CMissionUI::InitSprite()
		{
			// �~�b�V�����̎�ނ̐��������T�[�u
			m_checkMarkSRs.reserve(enMissionTypeNum);
			for (int missionType = 0; missionType < enMissionTypeNum; missionType++)
			{
				// ��ɃC���X�^���X�𐶐����Ă����B�������̓��x���̏������̒��ōs���B
				//m_checkMarkSRs.emplace_back(NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFourth));
				m_checkMarkSRs.emplace_back(nullptr);
			}

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
						m_missionWindowSR->SetPosition(objData.position);

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
							// ���W��ݒ�
							m_checkMarkSRs[missionType]->SetPosition(objData.position);
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

			m_numOfMissFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFinal);
			m_numOfMissFR->SetText(L"0");
			m_numOfMissFR->SetPosition({ 100.0f,-120.0f });

			return;
		}
	}
}