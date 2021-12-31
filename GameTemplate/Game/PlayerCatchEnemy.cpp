#include "stdafx.h"
#include "PlayerCatchEnemy.h"
#include "Player.h"
#include "GameMainState.h"
#include "AICar.h"
#include "SpriteRender.h"
#include "PlayerCommandInput.h"

namespace nsMyGame
{

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		using namespace nsPlayerConstData::nsCatchEnemyConstData;


		/**
		 * @brief �R���X�g���N�^
		*/
		CPlayerCatchEnemy::CPlayerCatchEnemy()
		{
			// �^�C�}�[�o�[�̃X�v���C�g�̏�����
			InitTimerBarSprite();

			// QTE�Ɏg���{�^���̃X�v���C�g�̏�����
			InitQTEButtonSprite();

			// �E�p�̃G�t�F�N�g��������
			InitNinjyutuEffect();

			// �R�}���h���̓N���X�𐶐�����
			m_commandInput = std::make_unique<CPlayerCommandInput>();

			return;
		}


		/**
		 * @brief �f�X�g���N�^
		*/
		CPlayerCatchEnemy::~CPlayerCatchEnemy()
		{
			DeleteGO(m_onEnemyTimerBar);
			DeleteGO(m_onEnemyTimerBarFrame);

			for (auto& qteButtonSR : m_QTEButtonSRs)
			{
				DeleteGO(qteButtonSR);
			}

			DeleteGO(m_onQTEButtonFraneSR);

			DeleteGO(m_ninjyutuEF);

			return;
		}

		/**
		 * @brief ����������
		 * @param[in,out] playerRef �v���C���[�̎Q��
		*/
		void CPlayerCatchEnemy::Init(CPlayer* playerRef)
		{
			m_playerRef = playerRef;

			m_commandInput->Init(*m_playerRef);


			// �Q�[���X�e�[�g�̎Q�Ƃ����������Ă���
			m_gameState = FindGO<nsGameState::CGameMainState>(
				nsCommonData::kGameObjectName[nsCommonData::enMainGameState]
				);

			// �Ԃ����̎Q�Ƃ����������Ă���
			m_aiCarsRef = m_gameState->GetAICar();

			return;
		}

		/**
		 * @brief ���C���[���G��߂܂��鏈���N���X�̃A�b�v�f�[�g�����s
		*/
		void CPlayerCatchEnemy::ExecuteUpdate()
		{
			switch (m_catceEnemyState)
			{
			case enCE_FindTarget:

				if (m_playerRef->GetState() == nsPlayerConstData::enOnEnemy)
				{
					ChangeState(enCE_GoOnEnemy);
				}

				// �^�[�Q�b�g��T��
				FindTarget();

				// ���̃N���X�Ŏg�����ʃf�[�^�̍X�V
				UpdateCommonData();

				// �G��߂܂��邩�ǂ������ׂ�
				CheckCatchEnemy();
				break;

			case enCE_GoOnEnemy:
				if (m_playerRef->GetPlayerMovement().GetPlayerOnEnemy().GetOnEnemyState() ==
					nsPlayerConstData::nsOnEnemyConstData::enOnEnemy)
				{
					ChangeState(enCE_InputingCommand);
				}
				break;

			case enCE_InputingCommand:
				OnEnemyUpdate();
				break;

			case enCE_SuccessCommand:
				SuccessCommandUpdate();
				break;

			case enCE_FailureCommand:
				ChangeState(enCE_End);
				break;

			case enCE_End:
				break;
			}

			return;
		}

		/**
		 * @brief �^�C�}�[�o�[�̃X�v���C�g�̏�����
		*/
		void CPlayerCatchEnemy::InitTimerBarSprite()
		{
			// �o�[�̐����Ə�����
			m_onEnemyTimerBar = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_onEnemyTimerBar->Init(
				kOnEnemyTimerBarSpriteFilePath,
				kOnEnemyTimerBarSpirteWidth,
				kOnEnemyTimerBarSpirteHeight,
				kOnEnemyTimerVarSpritePivot,
				AlphaBlendMode_Trans
			); 

			// �o�[�̍��W�B�s�{�b�g�����炷���߁A���W���v�Z����B
			Vector2 pos = kOnEnemyTimerVarSpritePosition;
			// �摜�̃n�[�t�T�C�Y
			float halfSize = 0.0f;
			halfSize = static_cast<float>(kOnEnemyTimerBarSpirteWidth) * 0.5f;
			// �s�{�b�g�̕������A���炷�B
			pos.x += halfSize * kOnEnemyTimerVarSpritePivot.x;
			// ���W��ݒ�
			m_onEnemyTimerBar->SetPosition(pos);
			// ��\���ɂ���
			m_onEnemyTimerBar->Deactivate();

			// �o�[�̃t���[���̐����Ə�����
			m_onEnemyTimerBarFrame = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_onEnemyTimerBarFrame->Init(
				kOnEnemyTimerBarFrameSpriteFilePath,
				kOnEnemyTimerBarFrameSpirteWidth,
				kOnEnemyTimerBarFrameSpirteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// ���W��ݒ�B������́A�s�{�b�g�����炵�ĂȂ����߁A���̂܂ܓ����B
			m_onEnemyTimerBarFrame->SetPosition(kOnEnemyTimerVarSpritePosition);
			// ��\���ɂ���
			m_onEnemyTimerBarFrame->Deactivate();

			return;
		}

		/**
		 * @brief QTE�Ɏg���{�^���̃X�v���C�g�̏�����
		*/
		void CPlayerCatchEnemy::InitQTEButtonSprite()
		{
			// QTE�Ɏg���{�^���̘g�̃X�v���C�g�̐����Ə�����
			m_onQTEButtonFraneSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_onQTEButtonFraneSR->Init(
				kQTEButtonFrameSpriteFilePath,
				kQTEButtonFrameSpriteWidth,
				kQTEButtonFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// ��\��
			m_onQTEButtonFraneSR->Deactivate();

			return;
		}


		/**
		 * @brief �E�p�̃G�t�F�N�g��������
		*/
		void CPlayerCatchEnemy::InitNinjyutuEffect()
		{
			// �E�p�̃G�t�F�N�g�̐����Ə�����
			m_ninjyutuEF = NewGO<Effect>(nsCommonData::enPrioritySecond);
			m_ninjyutuEF->Init(kNinjyutuEffectFilePath);

			return;
		}

		/**
		 * @brief QTE�Ɏg���{�^���̃X�v���C�g�̍ď�����
		 * @param[in] commandArray �R�}���h�̔z��
		*/
		void CPlayerCatchEnemy::ReInitQTEBUttonSprite(
			const std::vector<nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType>& commandArray
		)
		{
			if (m_QTEButtonSRs.empty() != true)
			{
				// �R���e�i����ł͂Ȃ�������ADeleteGO���Ă�
				for (auto& qteButtonSR : m_QTEButtonSRs)
				{
					DeleteGO(qteButtonSR);
				}
				// �����I�������N���A����
				m_QTEButtonSRs.clear();
			}

			// �V�����X�v���C�g�𐶐�����

			// �R�}���h�̐�
			const int commandNum = static_cast<int>(commandArray.size());
			// �R�}���h�̐��������T�[�u����
			m_QTEButtonSRs.reserve(commandNum);

			// ���[�̍��W
			const float leftEndPos = -kQTEButtonSpriteDistance * 0.5f * (commandNum - 1);
			// ���W
			Vector2 pos = { leftEndPos , kQTEButtonSpritePosY };

			for (const auto command : commandArray)
			{
				// �R�}���h��������o���Ă݂�

				// �X�v���C�g�����_���[�𐶐�����
				nsGraphic::nsSprite::CSpriteRender* qteButtonSR =
					NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
				// �R�}���h�ɂ������X�v���C�g�̃f�[�^�ŏ�����
				qteButtonSR->Init(
					kQTEButtonSpriteFilePath[command],
					kQTEButtonSpriteWidth[command] * kQTEButtonSizeScale[command],
					kQTEButtonSpriteHeight[command] * kQTEButtonSizeScale[command],
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
				// ���W��ݒ肷��
				qteButtonSR->SetPosition(pos);
				// �ҋ@���[�h�̏�Z�J���[��ݒ肷��
				qteButtonSR->SetMulColor(kQTEButtonSpriteStandByMulColor);
				// ���W�����̍��W�Ɍ����Ă��炷
				pos.x += kQTEButtonSpriteDistance;
				// �X�v���C�g�����_���[��z��ɓ����
				m_QTEButtonSRs.emplace_back(qteButtonSR);
			}

			// �擪�̃X�v���C�g���A�N�e�B�u�ȃ��[�h�̏�Z�J���[�Ɗg�嗦�ɂ���
			m_QTEButtonSRs[0]->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
			m_QTEButtonSRs[0]->SetScale(kQTEButtonSpriteActionScale);
		}

		/**
		 * @brief �G�̏�ɏ���Ă��鎞�̍X�V
		*/
		void CPlayerCatchEnemy::OnEnemyUpdate()
		{
			// �R�}���h���͂����s����
			m_commandInput->Execute();

			if (m_commandInput->IsEndCommandInput() == true)
			{
				// �R�}���h���͂��I�������B�R�}���h���͐����B
				ChangeState(enCE_SuccessCommand);
				return;
			}
			
			// �R�}���h���͂��A�܂��I����Ă��Ȃ��B

			if (m_oldCommandProgress != m_commandInput->GetCommandProgress())
			{
				// �O�t���[���̃R�}���h�i�s�x�ƈ������

				// ���݂̃X�v���C�g���\���ɂ���
				m_QTEButtonSRs[m_oldCommandProgress]->Deactivate();
				// ���̃X�v���C�g���A�N�e�B�u���[�h�̏�Z�J���[�Ɗg�嗦�ɂ���
				m_QTEButtonSRs[m_oldCommandProgress + 1]->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
				m_QTEButtonSRs[m_oldCommandProgress + 1]->SetScale(kQTEButtonSpriteActionScale);
				// �{�^���̘g�����̃{�^���̈ʒu�ɂ���
				m_onQTEButtonFraneSR->SetPosition(m_QTEButtonSRs[m_oldCommandProgress + 1]->GetPosition());
			}
			


			// �O�t���[���̃R�}���h�i�s�x���X�V����
			m_oldCommandProgress = m_commandInput->GetCommandProgress();

			// �^�C�}�[��i�߂�
			m_onEnemyTimer += nsTimer::GameTime().GetFrameDeltaTime();

			// �o�[�̊g�嗦�B���X�ɏ��������Ă����B
			const float barRate = 1.0f - (m_onEnemyTimer / kOnEnemyTime);
			// �o�[�̊g�嗦��ݒ�
			m_onEnemyTimerBar->SetScale({ barRate ,1.0f,1.0f });


			if (m_onEnemyTimer >= kOnEnemyTime)
			{
				ChangeState(enCE_FailureCommand);
				return;
			}

			return;
		}


		/**
		 * @brief �R�}���h�������������̍X�V
		*/
		void CPlayerCatchEnemy::SuccessCommandUpdate()
		{
			// �E�p�̃G�t�F�N�g�̍��W
			Vector3 pos = m_targetRef->GetPosition();
			// ������ɂ�����
			pos.y += kNinjyutuEffectPosBufHeight;

			// �E�p�̃G�t�F�N�g�̍��W�Ɖ�]���X�V����
			m_ninjyutuEF->SetPosition(pos);
			m_ninjyutuEF->SetRotation(m_targetRef->GetRotation());

			// �^�C�}�[��i�߂�
			m_ninnjyutuEFTimer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_ninnjyutuEFTimer >= kNinjyutuEffectTime)
			{
				// �^�C�}�[���A�E�p�̃G�t�F�N�g�̎��Ԃ𒴂�����A
				// �Ԃ�߂܂�����Ԃɂ��āA�E�p�̃G�t�F�N�g�̎Q�Ƃ�n���āA�X�e�[�g��J�ڂ���B
				m_targetRef->BeCaptured();
				m_targetRef->SetNinjyutuEffectRef(m_ninjyutuEF);
				ChangeState(enCE_End);
			}


			return;
		}

		/**
		 * @brief �^�[�Q�b�g��T��
		*/
		void CPlayerCatchEnemy::FindTarget()
		{
			if (m_targetRef)
			{
				// �^�[�Q�b�g������ꍇ�A�^�[�Q�b�g�Ƃ��Čp�����Ă悢�����ׂ�B

				// �^�[�Q�b�g��Ԃ��p�����邩���ׂ�
				CheckContinueTarget();
			}

			if (m_targetRef != nullptr)
			{
				// �^�[�Q�b�g������ꍇ�͒T���Ȃ��̂ŁA�������^�[���B
				return;
			}

			// �G�l�~�[1�̂����ׂ�
			for (auto& enemy : *m_aiCarsRef)
			{
				if (enemy->IsCaputred())
				{
					// �߂܂��Ă�����A���ցB
					continue;
				}
				// �v���C���[����G�l�~�[�ւ̃x�N�g��
				Vector3 playerToEnemyVec = enemy->GetPosition() - m_gameState->GetPlayer().GetPosition();
				// �v���C���[����G�l�~�[�ւ̋���
				const float playerToEnemyLen = playerToEnemyVec.Length();

				if (playerToEnemyLen >= kTargetMaxLength)
				{
					// ���������ȏ㗣��Ă��邽�߁A�^�[�Q�b�g�ɂ͂Ȃ�Ȃ��B���ցB
					continue;
				}

				// �J��������G�l�~�[�ւ̃x�N�g��
				Vector3 cameraToEnemyVec =
					enemy->GetPosition() - m_gameState->GetPlayer().GetCamera().GetCameraPosition();
				// �J��������G�l�~�[�ւ̕���
				Vector3 cameraToEnemyNorm = cameraToEnemyVec;
				cameraToEnemyNorm.Normalize();	// ���K������
				// �J��������G�l�~�[�ւ̕����ƁA�J�����̑O�����́A�p�x��
				float angleDiff =
					Dot(cameraToEnemyNorm, m_gameState->GetPlayer().GetCamera().GetCameraForward());
				angleDiff = acosf(angleDiff);	// ���ς��p�x�ɕϊ�

				if (angleDiff >= kTargetMaxAngle)
				{
					// �p�x�������ȏ�̑傫���̂��߁A�^�[�Q�b�g�ɂ͂Ȃ�Ȃ��B���ցB
					continue;
				}

				// @todo
				// �G�l�~�[�ւ̎������A�����ɎՂ��Ă��Ȃ������ׂ鏈����ǉ�����B
				// ����OBB�̓����蔻��Ōv�Z����B


				// �S�Ă̏����𖞂����Ă��邽�߁A�^�[�Q�b�g�Ƃ���B
				m_targetRef = enemy;
				// �^�[�Q�b�g�������������߁A���[�v���I������B
				break;
			}

			return;
		}

		/**
		 * @brief �^�[�Q�b�g��Ԃ��p�����邩���ׂ�
		*/
		void CPlayerCatchEnemy::CheckContinueTarget()
		{
			// �v���C���[����^�[�Q�b�g�ւ̃x�N�g��
			Vector3 playerToTargetVec = m_targetRef->GetPosition() - m_gameState->GetPlayer().GetPosition();
			// �v���C���[����^�[�Q�b�g�܂ł̋���
			const float playerToTargetLen = playerToTargetVec.Length();
			if (playerToTargetLen >= kTargetMaxLength)
			{
				// ���������ȏ㗣��Ă�����A�^�[�Q�b�g�����������B
				m_targetRef = nullptr;
				// ���������̂ŁA�������^�[��
				return;
			}

			// �J��������^�[�Q�b�g�ւ̃x�N�g��
			Vector3 cameraToTargetVec =
				m_targetRef->GetPosition() - m_gameState->GetPlayer().GetCamera().GetCameraPosition();
			// �J��������^�[�Q�b�g�ւ̕���
			Vector3 cameraToTargetNorm = cameraToTargetVec;
			cameraToTargetNorm.Normalize();	// ���K������
			// �J��������^�[�Q�b�g�ւ̕����ƁA�J�����̑O�����́A�p�x��
			float angleDiff =
				Dot(cameraToTargetNorm, m_gameState->GetPlayer().GetCamera().GetCameraForward());
			angleDiff = acosf(angleDiff);	// ���ς���p�x�֕ϊ�

			if (angleDiff >= kTargetMaxAngle)
			{
				// �p�x�������ȏ�̑傫���Ȃ�A�^�[�Q�b�g�����������B
				m_targetRef = nullptr;
				// ���������̂ŁA�������^�[��
				return;
			}

			return;
		}


		/**
		 * @brief ���̃N���X�Ŏg�����ʃf�[�^�̍X�V
		*/
		void CPlayerCatchEnemy::UpdateCommonData()
		{
			if (m_targetRef == nullptr)
			{
				// �^�[�Q�b�g���������Ă��Ȃ��ꍇ�́A����ȉ��̏����͍s��Ȃ��B
				return;
			}

			// �v���C���[����A�^�[�Q�b�g�ւ̋������v�Z����
			Vector3 playerToDiff = m_targetRef->GetPosition() - m_playerRef->GetPosition();
			m_targetLength = playerToDiff.Length();

			return;
		}

		/**
		 * @brief �G��߂܂��邩�ǂ������ׂ�
		*/
		void CPlayerCatchEnemy::CheckCatchEnemy()
		{
			if (m_targetRef == nullptr ||
				m_targetLength > kCanCatchMaxLength)
			{
				// �^�[�Q�b�g���������Ă��Ȃ��A�܂��́A
				// �^�[�Q�b�g�Ƃ̋��������ȉ��ł͂Ȃ��Ƃ�
				
				// �G��߂܂��邱�Ƃ��ł��Ȃ��B�������^�[���B
				return;
			}

			// �߂܂��邱�Ƃ��ł���

			if (m_playerRef->GetInputData().actionCatchEnemy)
			{
				// �G��߂܂�����͏�񂪂���
				// �G�̏�ɏ���Ԃ֑J�ڂ���
				m_playerRef->ChangeOnEnemyState();
			}

			return;
		}


		/**
		 * @brief QTE�Ŏg���X�v���C�g��L��������
		*/
		void CPlayerCatchEnemy::QTESpriteActivate()
		{
			m_onEnemyTimerBar->Activate();
			m_onEnemyTimerBarFrame->Activate();
			m_onQTEButtonFraneSR->Activate();

			return;
		}

		/**
		 * @brief QTE�Ŏg���X�v���C�g��L��������
		*/
		void CPlayerCatchEnemy::QTESpriteDeactivate()
		{
			m_onEnemyTimerBar->Deactivate();
			m_onEnemyTimerBarFrame->Deactivate();
			m_onQTEButtonFraneSR->Deactivate();

			return;
		}

		/**
		 * @brief �G��߂܂��鏈�����I���������̏���
		*/
		void CPlayerCatchEnemy::EndCatchEnemy()
		{
			// �G�t�F�N�g�^�C�}�[�����Z�b�g����
			m_ninnjyutuEFTimer = 0.0f;
			// �X�e�[�g�����Z�b�g����
			m_catceEnemyState = enCE_FindTarget;
			// �G�̏�ɏ���Ă���^�C�}�[�����Z�b�g����
			m_onEnemyTimer = 0.0f;
			// �^�[�Q�b�g�̓G�����Ȃ��悤�ɂ���
			m_targetRef = nullptr;
			// �G�̏�ɏ���Ă���N���X�����Z�b�g����
			m_playerRef->ResetOnEnemy();
			// �v���C���[�̃X�e�[�g���A�����Ƒ���ɑJ�ڂ���B
			m_playerRef->ChangeWalkAndRunState();
			// �R�}���h���̓N���X���I��������
			m_commandInput->EndCommandInput();
			// �O�t���[���̃R�}���h�i�s�x�����Z�b�g����
			m_oldCommandProgress = 0;

			// �X�v���C�g���������ׂĔ�\���ɂ���
			for (auto& qteButtonSR : m_QTEButtonSRs)
			{
				qteButtonSR->Deactivate();
			}
			QTESpriteDeactivate();

			return;
		}

		/**
		 * @brief �X�e�[�g��J�ڂ���
		 * @param[in] newState �V�����X�e�[�g
		*/
		void CPlayerCatchEnemy::ChangeState(const nsPlayerConstData::nsCatchEnemyConstData::EnCatchEnemyState newState)
		{
			if (m_catceEnemyState == newState)
			{
				return;
			}


			m_catceEnemyState = newState;

			switch (m_catceEnemyState)
			{
			case enCE_FindTarget:
				break;

			case enCE_GoOnEnemy:
				break;

			case enCE_InputingCommand:
				// �R�}���h������������
				m_commandInput->InitCommand(4, nsPlayerConstData::nsCommandInputConstData::enCT_type3);
				// QTE�Ɏg���{�^���̃X�v���C�g���ď�����
				ReInitQTEBUttonSprite(m_commandInput->GetCommandButtonTypeArray());
				// QTE�Ŏg���X�v���C�g�̗L����
				QTESpriteActivate();
				// �{�^���̘g�̃X�v���C�g�̍��W���A��ԍ��̃{�^���̃X�v���C�g�̍��W�Ɠ����ɂ���B
				m_onQTEButtonFraneSR->SetPosition(m_QTEButtonSRs[0]->GetPosition());
				break;

			case enCE_SuccessCommand:
				// �X�v���C�g���������ׂĔ�\���ɂ���
				for (auto& qteButtonSR : m_QTEButtonSRs)
				{
					qteButtonSR->Deactivate();
				}
				QTESpriteDeactivate();

				// �E�p�̃G�t�F�N�g�̍��W�Ɖ�]��ݒ肵�āA�Đ�����B
				m_ninjyutuEF->SetPosition(m_targetRef->GetPosition() + Vector3::Up * kNinjyutuEffectPosBufHeight);
				m_ninjyutuEF->SetRotation(m_targetRef->GetRotation());
				m_ninjyutuEF->Play();
				break;

			case enCE_FailureCommand:
				// �X�v���C�g���������ׂĔ�\���ɂ���
				for (auto& qteButtonSR : m_QTEButtonSRs)
				{
					qteButtonSR->Deactivate();
				}
				QTESpriteDeactivate();
				break;

			case enCE_End:
				// �G��߂܂��鏈�����I���������̏���
				EndCatchEnemy();
				break;
			}

			return;
		}

	}
}