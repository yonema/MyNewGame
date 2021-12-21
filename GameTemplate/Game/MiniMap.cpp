#include "stdafx.h"
#include "MiniMap.h"
#include "SpriteRender.h"
#include "SpriteRenderConstData.h"
#include "GameMainState.h"
#include "UIConstData.h"
#include "Player.h"
#include "RenderingEngine.h"

namespace nsMyGame
{
	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		using namespace nsGameMainUIConstData;

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
		
			// �~�j�}�b�v�̃X�v���C�g�̏�����
			InitMiniMapSprite();

			// �v���C���[�̃A�C�R���̃X�v���C�g�̏�����
			InitPlayerIconSprite();

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CMiniMap::OnDestroy()
		{
			DeleteGO(m_miniMapSR);
			DeleteGO(m_miniMapBackSR);
			DeleteGO(m_playerIconSR);

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CMiniMap::Update()
		{
			// �~�j�}�b�v�̊�_���X�V
			UpdateMiniMapPivot();

			// �~�j�}�b�v�̉�]���X�V
			UpdateMiniMapRotate();

			// �v���C���[�̃A�C�R������]���X�V
			UpdatePlayerIconRotate();

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

			m_playerIconSR->SetPosition(kPlayerIconSpritePosition);

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
		 * @brief �~�j�}�b�v�̊�_���X�V
		*/
		void CMiniMap::UpdateMiniMapPivot()
		{
			const Vector3& playerPos = m_gameState->GetPlayer().GetPosition();

			float xRate = (playerPos.x + 35000.0f) / 70000.0f;
			float yRate = (playerPos.z + 2800.0f) / 70000.0f;

			m_miniMapSR->SetPivot({ xRate, yRate });

			return;
		}

		/**
		 * @brief �~�j�}�b�v�̉�]���X�V
		*/
		void CMiniMap::UpdateMiniMapRotate()
		{
			// Y�����𔲂����J�����̑O����
			Vector3 cameraForward = g_camera3D->GetForward();
			cameraForward.y = 0.0f;
			cameraForward.Normalize();

			// ���W�A���p�x���o��
			float angle = acosf(Dot(Vector3::Front, cameraForward));

			Quaternion qRot = Quaternion::Identity;
			// X�����̎��A���]������
			if (cameraForward.x <= 0.0f)
			{
				angle = 3.14f * 2.0f - angle;
			}
			qRot.SetRotationZ(angle);

			// �~�j�}�b�v����]������
			m_miniMapSR->SetRotation(qRot);

			return;
		}

		/**
		 * @brief �v���C���[�̃A�C�R������]���X�V
		*/
		void CMiniMap::UpdatePlayerIconRotate()
		{
			// Y�����𔲂����J�����̑O����
			Vector3 cameraForward = g_camera3D->GetForward();
			cameraForward.y = 0.0f;
			cameraForward.Normalize();

			// Y�����𔲂����v���C���[�̑O����
			const Quaternion& playerRot = m_gameState->GetPlayer().GetRotation();
			Vector3 playerForward = Vector3::Front;
			playerRot.Apply(playerForward);
			playerForward.y = 0.0f;
			playerForward.Normalize();

			// ���W�A���p�x���o��
			float angle = acosf(Dot(playerForward, cameraForward));

			// �J�����̉E�����̃x�N�g��
			Vector3 CameraRight = Cross(Vector3::Up, cameraForward);
			// �v���C���[�̑O�����ƃJ�����̉E�����̓��ς����
			float dotPlForwardAndCamRight = Dot(playerForward, CameraRight);

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

	}
}