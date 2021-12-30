#include "stdafx.h"
#include "MiniMap.h"
#include "SpriteRender.h"
#include "SpriteRenderConstData.h"
#include "GameMainState.h"
#include "UIConstData.h"
#include "Player.h"
#include "RenderingEngine.h"
#include "AICar.h"
#include "GameTime.h"

namespace nsMyGame
{
	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		using namespace nsMiniMapConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CMiniMap::Start()
		{
			// �Q�[���X�e�[�g�̎Q�Ƃ����������Ă���
			m_gameState = FindGO<nsGameState::CGameMainState>(
				nsCommonData::kGameObjectName[nsCommonData::enMainGameState]
				);

			// �Ԃ����̎Q�Ƃ����������Ă���
			m_aiCarsRef = m_gameState->GetAICar();

			// �v���C���[�̍��W�̎Q�Ƃ����������Ă���
			m_playerPositionRef = &m_gameState->GetPlayer().GetPosition();
		
			// �~�j�}�b�v�̃X�v���C�g�̏�����
			InitMiniMapSprite();

			// �v���C���[�̃A�C�R���̃X�v���C�g�̏�����
			InitPlayerIconSprite();

			// �Ԃ̃A�C�R���̃X�v���C�g�̏�����
			InitCarIconSprite();

			// �Ԃ̃~�j�A�C�R���̃X�v���C�g�̏�����
			InitCarMiniIconSprite();

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CMiniMap::OnDestroy()
		{
			DeleteGO(m_miniMapSR);
			DeleteGO(m_miniMapFrameSR);
			DeleteGO(m_miniMapBackSR);
			DeleteGO(m_playerIconSR);

			for (auto& carIcon : m_carIconSRs)
			{
				DeleteGO(carIcon);
			}

			for (auto& carMiniIcon : m_carMiniIconSRs)
			{
				DeleteGO(carMiniIcon);
			}

			for (auto& carMiniIconOut : m_carMiniIconOutSRs)
			{
				DeleteGO(carMiniIconOut);
			}

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CMiniMap::Update()
		{

			// �N���X���̂����ȂƂ���ŋ��ʂ��Ďg�p�����f�[�^�̍X�V
			UpdateCommonData();

			// �~�j�}�b�v�̊�_���X�V
			UpdateMiniMapPivot();

			// �~�j�}�b�v�̉�]���X�V
			UpdateMiniMapRotate();

			// �v���C���[�̃A�C�R������]���X�V
			UpdatePlayerIconRotate();

			// �Ԃ̃A�C�R�����X�V
			UpdateCarIcon();

			// �Ԃ̃~�j�A�C�R�����X�V
			UpdateCarMiniIcon();

			return;
		}


		/**
		 * @brief �~�j�}�b�v�̃X�v���C�g�̏�����
		*/
		void CMiniMap::InitMiniMapSprite()
		{
			// �~�j�}�b�v�̔w�i�p�X�v���C�g�̐����Ə�����
			m_miniMapBackSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond);
			m_miniMapBackSR->Init(kMiniMapBackSpriteFilePath, kMiniMapDrawWidth, kMiniMapDrawHeight);

			// �~�j�}�b�v�̘g�p�X�v���C�g�̐����Ə�����
			m_miniMapFrameSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond);
			m_miniMapFrameSR->Init(
				kMiniMapFrameSpriteFilePath,
				kMiniMapFrameSpriteWidth, 
				kMiniMapFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			// �~�j�}�b�v�̃X�v���C�g�̐���
			m_miniMapSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond);

			// �X�v���C�g�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsGraphic::nsSprite::nsSpriteConstData;
			// �~�j�}�b�v�p�̃X�v���C�g�̏������f�[�^
			SpriteInitData initData;
			initData.m_ddsFilePath[0] = kMiniMapSpriteFilePath;
			initData.m_width = kMiniMapSpriteWidth;
			initData.m_height = kMiniMapSpriteHeight;
			initData.m_fxFilePath = kMiniMapFxFilePath;
			initData.m_psEntryPoinFunc = kPsEntryPointFunc;
			initData.m_vsEntryPointFunc = kVsEntryPointFunc;
			// ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[�t�H�[�}�b�g�ƍ��킹��
			initData.m_colorBufferFormat[0] = nsMyEngine::CRenderingEngine::GetInstance()->GetMainRenderTargetFormat();
			// �~�j�}�b�v�̍ŏ����W�ƍő���W���v�Z
			CalcMinAndMaxPos();
			// �萔�o�b�t�@��ݒ�
			initData.m_expandConstantBuffer[0] = &m_miniMapSpriteCB;
			initData.m_expandConstantBufferSize[0] = sizeof(m_miniMapSpriteCB);
			// �~�j�}�b�v�̃X�v���C�g�̏�����
			m_miniMapSR->Init(initData);

			// ���W��ݒ�
			m_miniMapBackSR->SetPosition(kMiniMapSpritePosition);
			m_miniMapFrameSR->SetPosition(kMiniMapFrameSpritePosition);
			m_miniMapSR->SetPosition(kMiniMapSpritePosition);

			return;
		}

		/**
		 * @brief �v���C���[�̃A�C�R���̃X�v���C�g�̏�����
		*/
		void CMiniMap::InitPlayerIconSprite()
		{
			// �v���C���[�̃A�C�R���̃X�v���C�g�̐����Ə�����
			m_playerIconSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond);
			m_playerIconSR->Init(
				kPlayerIconSpriteFilePath,
				kPlayerIconSpriteWidth,
				kPlayerIconSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			// ���W�͍ŏ��̈ʒu�ŌŒ�
			m_playerIconSR->SetPosition(kPlayerIconSpritePosition);

			return;
		}

		/**
		 * @brief �Ԃ̃A�C�R���̃X�v���C�g�̏�����
		*/
		void CMiniMap::InitCarIconSprite()
		{
			// �Ԃ̐��������T�[�u
			m_carIconSRs.reserve(m_aiCarsRef->size());

			// �Ԃ̐������X�v���C�g�𐶐�
			for (int i = 0; i < m_aiCarsRef->size();i++)
			{
				m_carIconSRs.emplace_back(NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond));
				m_carIconSRs[i]->Init(
					kCarIconSpriteFilePath,
					kCarIconSpriteWidth,
					kCarIconSpriteHeight, 
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
				m_carIconSRs[i]->SetIsControlAlbedo(true);
				m_carIconSRs[i]->SetAlbedoColor(kCarIconDefaultColor);
				// �ŏ��͑S����\��
				m_carIconSRs[i]->Deactivate();
			}


			return;
		}

		/**
		 * @brief �Ԃ̃~�j�A�C�R���̃X�v���C�g�̏�����
		*/
		void CMiniMap::InitCarMiniIconSprite()
		{
			// �Ԃ̐��������T�[�u
			m_carMiniIconSRs.reserve(m_aiCarsRef->size());

			// �Ԃ̐������X�v���C�g�𐶐�
			for (int i = 0; i < m_aiCarsRef->size(); i++)
			{
				m_carMiniIconSRs.emplace_back(NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond));
				m_carMiniIconSRs[i]->Init(
					kCarMiniIconSpriteFilePath,
					kCarMiniIconSpriteWidth,
					kCarMiniIconSpriteHeight,
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
				m_carMiniIconSRs[i]->SetIsControlAlbedo(true);
				m_carMiniIconSRs[i]->SetAlbedoColor(kCarIconDefaultColor);
				m_carMiniIconOutSRs.emplace_back(NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond));
				m_carMiniIconOutSRs[i]->Init(
					kCarMiniIconOutSpriteFilePath,
					kCarMiniIconOutSpriteWidth,
					kCarMiniIconOutSpriteHeight,
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
				m_carMiniIconOutSRs[i]->SetIsControlAlbedo(true);
				m_carMiniIconOutSRs[i]->SetAlbedoColor(kCarIconDefaultColor);
			}

			return;
		}

		/**
		 * @brief �~�j�}�b�v�̍ŏ����W�ƍő���W���v�Z
		*/
		void CMiniMap::CalcMinAndMaxPos()
		{
			// �~�j�}�b�v�̃X�v���C�g�̒��S���W���v�Z
			Vector2 centerPos = {
				g_graphicsEngine->GetFrameBufferWidth() * 0.5f,
				g_graphicsEngine->GetFrameBufferHeight() * 0.5f
			};
			centerPos.x += kMiniMapSpritePosition.x;
			centerPos.y -= kMiniMapSpritePosition.y;
			// �~�j�}�b�v�̍ŏ����W���v�Z
			m_miniMapSpriteCB.minPos = centerPos;
			m_miniMapSpriteCB.minPos.x -= kMiniMapDrawWidth * 0.5f;
			m_miniMapSpriteCB.minPos.y -= kMiniMapDrawHeight * 0.5f;
			// �~�j�}�b�v�̍ő���W���v�Z
			m_miniMapSpriteCB.maxPos = centerPos;
			m_miniMapSpriteCB.maxPos.x += kMiniMapDrawWidth * 0.5f;
			m_miniMapSpriteCB.maxPos.y += kMiniMapDrawHeight * 0.5f;

			return;
		}

		/**
		 * @brief �N���X���̂����ȂƂ���ŋ��ʂ��Ďg�p�����f�[�^�̍X�V
		*/
		void CMiniMap::UpdateCommonData()
		{
			// XZ���ʂł̃J�����̑O�����X�V
			m_cameraForwardXZ = g_camera3D->GetForward();
			// Y����������
			m_cameraForwardXZ.y = 0.0f;
			// ���K�����Ȃ���
			m_cameraForwardXZ.Normalize();

			// ������ƃJ������XZ���ʂ̑O�����̊O�ς���A
			// XZ���ʂ̃J�����̉E�����̃x�N�g�����쐬
			m_cameraRightXZ = Cross(Vector3::Up, m_cameraForwardXZ);
			// ���K������
			m_cameraRightXZ.Normalize();

			return;
		}

		/**
		 * @brief �~�j�}�b�v�̊�_���X�V
		*/
		void CMiniMap::UpdateMiniMapPivot()
		{
			// �X�e�[�W�̕��ƃv���C���[�̍��W����A
			// �v���C���[���X�e�[�W�̂ǂ̕ӂɂ���̂��A���K�������l���o��
			float xRate = (m_playerPositionRef->x + kPlayerCenterBuffWidth) / kStageWidth;
			float yRate = (m_playerPositionRef->z + kPlayerCenterBuffHeight) / kStageHeight;

			// �v���C���[�̈ʒu���}�b�v�̒��S�ɂ��邽�߁A�}�b�v�̊�_���v���C���[�̈ʒu�ɍ��킹��
			m_miniMapSR->SetPivot({ xRate, yRate });

			return;
		}

		/**
		 * @brief �~�j�}�b�v�̉�]���X�V
		*/
		void CMiniMap::UpdateMiniMapRotate()
		{
			// ���W�A���p�x���o��
			float angle = acosf(Dot(Vector3::Front, m_cameraForwardXZ));

			// X�����̎��A���]������
			if (m_cameraForwardXZ.x <= 0.0f)
			{
				angle = 3.14f * 2.0f - angle;
			}

			// �p�x����Z������̃N�H�[�^�j�I����ݒ�
			m_miniMapRotatioin.SetRotationZ(angle);

			// �~�j�}�b�v����]������
			m_miniMapSR->SetRotation(m_miniMapRotatioin);

			return;
		}

		/**
		 * @brief �v���C���[�̃A�C�R������]���X�V
		*/
		void CMiniMap::UpdatePlayerIconRotate()
		{
			// �v���C���[��XZ���ʂł̑O�����̃x�N�g�����쐬����

			// �v���C���[�̉�]���
			const Quaternion& playerRot = m_gameState->GetPlayer().GetRotation();
			// Y�����𔲂����v���C���[�̑O����
			Vector3 playerForward = Vector3::Front;
			// ��]�őO�����x�N�g������
			playerRot.Apply(playerForward);
			// Y����������
			playerForward.y = 0.0f;
			// ���K������
			playerForward.Normalize();

			// �v���C���[�̑O�����ƃJ�����̑O�����̃��W�A���p�x���o��
			float angle = acosf(Dot(playerForward, m_cameraForwardXZ));

			// �v���C���[�̑O�����ƃJ�����̉E�����̓��ς����
			float dotPlForwardAndCamRight = Dot(playerForward, m_cameraRightXZ);

			Quaternion qRot = Quaternion::Identity;
			// �v���C���[�̑O�������A�J�����̉E�����Ɠ��������������Ă�����A���]����B
			if (dotPlForwardAndCamRight >= 0.0f)
			{
				angle = 3.14f * 2.0f - angle;
			}
			qRot.SetRotationZ(angle);

			// �~�j�}�b�v����]������
			m_playerIconSR->SetRotation(qRot);

			return;
		}

		/**
		 * @brief �Ԃ̃A�C�R�����X�V
		*/
		void CMiniMap::UpdateCarIcon()
		{

			if (m_gameState->GetPlayer().GetInputData().actionSeachEnemy == true)
			{
				// �G��T�m����A�N�V��������������A�A�C�R����\������悤�ɂ���B
				m_isDisplayCarIcon = true;
				// �^�C�}�[�̃��Z�b�g
				m_displayCarIconTimer = 0.0f;
				for (int i = 0; i < m_carIconSRs.size(); i++)
				{
					if ((*m_aiCarsRef)[i]->IsCaputred() != true)
					{
						// �G���߂܂��Ă��Ȃ�������A�\������
						m_carIconSRs[i]->Activate();
					}
				}
			}

			if (m_isDisplayCarIcon != true)
			{
				// �Ԃ̃A�C�R����\�����Ȃ�

				return;
			}

			if (m_displayCarIconTimer >= kDisplayCarIconTime)
			{
				// ��莞�Ԃ�������A�C�R�����\��
				m_isDisplayCarIcon = false;

				for (auto& icon : m_carIconSRs)
				{
					// �S�ẴA�C�R�����\��
					icon->Deactivate();
				}

				return;
			}

			// �^�C�}�[��i�߂�
			m_displayCarIconTimer += nsTimer::GameTime().GetFrameDeltaTime();


			// �C���f�b�N�X
			int i = 0;
			// �ԑS�����̍X�V
			for (const auto& carIconSR : m_carIconSRs)
			{
				if ((*m_aiCarsRef)[i]->IsCaputred())
				{
					// �߂܂��Ă�����A�X�L�b�v
					i++;
					continue;
				}

				// �A�C�R���̍��W
				Vector2 iconPos = Vector2::Zero;

				// �J�������O�ɂ��邩�H
				bool isCameraFront = true;
				// �g�̏ォ�H
				bool isOnFrame = false;
				// �g�Ɉړ�������O�̍��W
				Vector2 prevPos;
				// �Ԃ̍��W
				Vector3 carPos = (*m_aiCarsRef)[i++]->GetPosition();
				// ������ɂ�����
				carPos.y += kCarIconSpriteWorldPosYBuff;

				// ���[���h���W����X�N���[�����W�֕ϊ��B
				g_camera3D->CalcScreenPositionFromWorldPositionBackCullOnFrame(
					iconPos,
					prevPos,
					carPos,
					&isCameraFront,
					&isOnFrame,
					kCarIconSpriteWidth * 0.5f
				);

				// �J���������̑Ώۂ́A�g��ɕ\������
				if (isCameraFront == false)
				{
					// �g��t���O�����Ă�
					isOnFrame = true;
					// ��ʂ̉���
					const float half_w = (float)g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
					// Y���W�𔽓]������
					iconPos.y *= -1.0f;
					prevPos.y *= -1.0f;
					// X���W�𔽓]�����āA��ʂ̉����ɍ��킹��
					if (iconPos.x <= 0.0f)
					{
						iconPos.x = half_w;
						prevPos.x = half_w;
					}
					else
					{
						iconPos.x = -half_w;
						prevPos.x = -half_w;
					}
				}

				// �g�O�ɂ�����A�g��ɕ\������B
				if (isOnFrame)
				{
					// �A�C�R�����͂ݏo���Ȃ��悤�Ɋ�_�𒲐�����B
					Vector2 toPrevPosNorm = prevPos;
					toPrevPosNorm.Normalize();
					// �P�ʃx�N�g����-1.0f�`1.0f�Ȃ̂ŁA0.0f�`1.0f�Ɋۂ߂�B
					toPrevPosNorm.x = toPrevPosNorm.x * 0.5f + 0.5f;
					toPrevPosNorm.y = toPrevPosNorm.y * 0.5f + 0.5f;
					carIconSR->SetPivot(toPrevPosNorm);
				}
				else
				{
					carIconSR->SetPivot(nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot);
				}

				// ���W�X�V
				carIconSR->SetPosition(iconPos);
			}

			return;
		}

		/**
		 * @brief �Ԃ̃~�j�A�C�R�����X�V
		*/
		void CMiniMap::UpdateCarMiniIcon()
		{
			// �ԑS�����X�V
			int i = 0;
			for (const auto& carMiniIconSR : m_carMiniIconSRs)
			{
				if ((*m_aiCarsRef)[i]->IsCaputred())
				{
					// �G���߂܂��Ă�����A��\���ɂ��āA���ցB
					carMiniIconSR->Deactivate();
					m_carMiniIconOutSRs[i]->Deactivate();
					i++;
					continue;
				}

				//////// 1.�Ԃ̃A�C�R���̍��W���v�Z���� ////////

				// �v���C���[����Ԃւ̃x�N�g��
				Vector3 playerToCarVec = (*m_aiCarsRef)[i]->GetPosition() - *m_playerPositionRef;
				// ���K�����ꂽ�v���C���[����Ԃւ̃x�N�g��
				float xRate = (playerToCarVec.x) / kStageWidth;
				float yRate = (playerToCarVec.z) / kStageHeight;

				// ���K�����ꂽ�x�N�g������A�}�b�v�ɍ��킹���x�N�g���ɕϊ�
				Vector3 toCarVecOnMap = Vector3::Zero;
				toCarVecOnMap.x = kMiniMapSpriteWidth * xRate;
				toCarVecOnMap.y = kMiniMapSpriteHeight * yRate;
				// �~�j�}�b�v�̉�]�Ńx�N�g������
				m_miniMapRotatioin.Apply(toCarVecOnMap);
				// �Ԃ̃A�C�R���̍��W
				// �v���C���[�̃A�C�R���̍��W����A�}�b�v�ɍ��킹���Ԃւ̃x�N�g���𑫂��B
				Vector2 iconPos = kPlayerIconSpritePosition;
				iconPos.x += toCarVecOnMap.x;
				iconPos.y += toCarVecOnMap.y;
				Vector2 prevIconPos = iconPos;

				//////// 2.�Ԃ̍��W���~�j�}�b�v�O�Ȃ�[�����ɌŒ肷�� ////////
 
				// �v���C���[�A�C�R������ԃA�C�R���ւ̐����̃x�N�g��
				Vector2 pIconToCIconVec = Vector2::Zero;
				pIconToCIconVec.x = iconPos.x - kPlayerIconSpritePosition.x;
				pIconToCIconVec.y = iconPos.y - kPlayerIconSpritePosition.y;

				// �������Ă��邩�H
				bool isIntersect = false;
				// �����ƁA�~�j�}�b�v�̎l�p�`�̓����蔻������
				for (int i = 0; i < 4; i++)
				{
					// Vector2���m�̌�������
					isIntersect = IsIntersectVector2ToVector2(
						kPlayerIconSpritePosition,
						pIconToCIconVec,
						kMiniMapVert[i],
						kMiniMapEdge[i],
						&iconPos
					);
					if (isIntersect)
					{
						// �������Ă�����A�����[�v�𔲂���
						break;
					}
				}

				// �Ԃ̃~�j�A�C�R���̍��W��ݒ肷��
				carMiniIconSR->SetPosition(iconPos);

				//////// 3.�Ԃ̃~�j�A�C�R���̉�ʂ݂͂����p���X�V���� ////////

				// �Ԃ̃~�j�A�C�R���̉�ʂ݂͂����p���X�V
				UpdateCarMiniIconOut(i, isIntersect, playerToCarVec, iconPos);

				//////// 4.�Ԃ̃A�C�R�������̃J���[���X�V ////////

				// �Ԃ̃A�C�R�������̐F���X�V
				UpdateCarIconsColor(carMiniIconSR, isIntersect, prevIconPos, iconPos);
				UpdateCarIconsColor(m_carMiniIconOutSRs[i], isIntersect, prevIconPos, iconPos);
				// ���C����ʂ̕��̎Ԃ̃A�C�R���̐F�������ōX�V����
				UpdateCarIconsColor(m_carIconSRs[i], isIntersect, prevIconPos, iconPos);

				i++;
			}

			return;
		}

		/**
		 * @brief �Ԃ̃~�j�A�C�R���̉�ʂ݂͂����p���X�V�B
		 * ���̊֐��́AUpdateCarMiniIcon�ŌĂ΂��B
		 * @param[in] index �Ԃ̃C���f�b�N�X
		 * @param[in] isIntersect �������Ă��邩�H
		 * @param[in] playerToCarVec �v���C���[����Ԃւ̃x�N�g��
		 * @param[in] iconPos �Ԃ̃A�C�R���̍��W
		*/
		void CMiniMap::UpdateCarMiniIconOut(
			const int index,
			const bool isIntersect,
			const Vector3& playerToCarVec,
			const Vector2& iconPos)
		{
			if (isIntersect != true)
			{
				// �������ĂȂ��������\��
				m_carMiniIconOutSRs[index]->Deactivate();
				// �������^�[��
				return;
			}

			// �������Ă�����\��
			m_carMiniIconOutSRs[index]->Activate();

			// XZ���ʂł̃v���C���[����Ԃւ̕����x�N�g��
			Vector3 playerToCarXZVec = playerToCarVec;
			playerToCarXZVec.y = 0.0f;
			playerToCarXZVec.Normalize();

			// ���W�A���p�x���o��
			float angle = acosf(Dot(playerToCarXZVec, m_cameraForwardXZ));

			// ���C���[����Ԃւ̕����ƃJ�����̉E�����̓��ς����
			float dotPlToCarAndCamRight = Dot(playerToCarXZVec, m_cameraRightXZ);

			Quaternion qRot = Quaternion::Identity;
			// �v���C���[�̑O�������A�J�����̉E�����Ɠ��������������Ă�����A���]����B
			if (dotPlToCarAndCamRight >= 0.0f)
			{
				angle = 3.14f * 2.0f - angle;
			}
			qRot.SetRotationZ(angle);

			// ���ʂ̎ԃA�C�R���Ƃ�����Ƃ��炷
			Vector3 up = Vector3::Up;
			qRot.Apply(up);
			up.Scale(kCarMiniIconOutBuff);
			Vector2 iconOutPos = iconPos;
			iconOutPos.x += up.x;
			iconOutPos.y += up.y;

			m_carMiniIconOutSRs[index]->SetRotation(qRot);
			m_carMiniIconOutSRs[index]->SetPosition(iconOutPos);

			return;
		}

		// �N���X�O�֐�

		/**
		 * @brief �Ԃ̃A�C�R�����w�肵�ĐF���X�V
		 * ���̊֐��́AUpdateCarMiniIcon�ŌĂ΂��B
		 * @param[out] spriteRender �F���X�V����X�v���C�g�����_���[
		 * @param[in] isIntersect �������Ă��邩�H
		 * @param[in] prevIconPos �����_�ֈړ��O�̃A�C�R���̍��W
		 * @param[in] iconPos �A�C�R���̍��W
		*/
		void UpdateCarIconsColor(
			nsGraphic::nsSprite::CSpriteRender* spriteRender,
			const bool isIntersect,
			const Vector2& prevIconPos,
			const Vector2& iconPos
		)
		{
			// �A���x�h�J���[
			Vector3 albedo = kCarIconDefaultColor;

			// �������Ă��邩�H
			if (isIntersect)
			{
				// �A�C�R���́A�����_�ֈړ��O�̍��W�ƁA�����_�ֈړ���̍��W�̋����ɉ�����
				// �J���[����`�⊮����B
				Vector3 v1, v2;
				v1.x = prevIconPos.x;
				v1.y = prevIconPos.y;
				v2.x = iconPos.x;
				v2.y = iconPos.y;
				Vector3 diffV = v1 - v2;
				float rate = diffV.Length() / kCarIconOutMaxRage;
				albedo.Lerp(min(rate, 1.0f), kCarIconOutColorNear, kCarIconOutColorFar);
			}

			// �A���x�h�J���[��ݒ�
			spriteRender->SetAlbedoColor(albedo);

			return;
		}

	}
}