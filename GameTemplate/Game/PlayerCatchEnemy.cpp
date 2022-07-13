#include "stdafx.h"
#include "PlayerCatchEnemy.h"
#include "Player.h"
#include "GameMainState.h"
#include "AICar.h"
#include "SpriteRender.h"
#include "PlayerCommandInput.h"
#include "SoundCue.h"
#include "GameMainState.h"

namespace nsNinjaAttract
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

			// �T�E���h�̏�����
			InitSound();

			// �R�}���h���̓N���X�𐶐�����
			m_commandInput = std::make_unique<CPlayerCommandInput>();

			// �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
			std::random_device rnd;
			// �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h
			m_mt = std::make_unique<std::mt19937>(rnd());
			// �͈͂̈�l����
			m_fireVoiceRand = std::make_unique<std::uniform_int_distribution<>>(0, kFireVoiceSoundTypeNum - 1);
			m_failureVoiceRand = std::make_unique<std::uniform_int_distribution<>>(0, kFailureVoiceSoundTypeNum - 1);

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

			DeleteGO(m_QTEButtonFraneSR);
			DeleteGO(m_QTEButtonAfterImageSR);
			for (auto& qteResultSR : m_QTEResultSR)
			{
				DeleteGO(qteResultSR);
			}
			DeleteGO(m_QTEResultFrameIn);
			DeleteGO(m_QTEResultFrameOut);

			DeleteGO(m_ninjyutuEF);
			DeleteGO(m_sonarEF);

			DeleteGO(m_commandSuccessSC);
			DeleteGO(m_commandMissSC);
			DeleteGO(m_windowOpneSC);
			DeleteGO(m_windowCloseSC);
			DeleteGO(m_fireStartSC);
			DeleteGO(m_fireReleaseSC);
			DeleteGO(m_sonarSC);

			for (int i = 0; i < kFireVoiceSoundTypeNum; i++)
			{
				DeleteGO(m_fireVoiceSC[i]);
			}
			for (int i = 0; i < kFailureVoiceSoundTypeNum; i++)
			{
				DeleteGO(m_failureVoiceSC[i]);
			}

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
				nsCommonData::kGameObjectName[nsCommonData::enGN_MainGameState]
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
			// ���ł����ʂōs����X�V
			CommonUpdate();

			switch (m_catchEnemyState)
			{
			case enCE_FindTarget:

				if (m_playerRef->GetState() == nsPlayerConstData::enOnEnemy)
				{
					ChangeState(enCE_GoOnEnemy);
				}

				if (m_isStandUp && 
					m_playerRef->GetPlayerModelAnimation().GetAnimationState() == 
					nsPlayerConstData::nsModelAnimationConstData::enAnim_idle)
				{
					// �N���オ�蒆���A�A�j���[�V�������A�C�h����Ԃ܂ōs������

					// �N���オ�蒆������
					m_isStandUp = false;
					// ���͂��\�ɂ���
					m_playerRef->SetIsInputtable(true);
				}

				if (m_playerRef->GetInputData().actionSearchEnemy == true)
				{
					// �G��T�m����A�N�V�����������Ă�����A
					// �\�i�[�̃G�t�F�N�g���Đ�����
					m_sonarEF->SetPosition(m_playerRef->GetPosition());
					m_sonarEF->SetScale(Vector3::One);
					m_sonarEF->Play();
					m_sonarTimer = 0.0f;
					m_sonarSC->Play(false);
				}

				if (m_sonarEF->IsPlay())
				{
					// �\�i�[�̃G�t�F�N�g���Đ�����Ă�����A���X�Ɋg�債�Ă���
					const float rate = m_sonarTimer / kSonarEffectTime;
					const float scale = Math::Lerp<float>(rate, kSonarEffectMinScale, kSonarEffectMaxScale);
					Vector3 scaleVec = { scale, kSonarEffectYScale, scale };
					m_sonarEF->SetScale(scaleVec);
					m_sonarTimer += m_playerRef->GetDeltaTime();
				}

				// �^�[�Q�b�g��T��
				FindTarget();

				// �^�[�Q�b�g�Ƃ̋������X�V
				UpdateTargetLength();

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
			// ����A�܂����\�[�X�o���N����ĂȂ�������B
			for (int i = 0; i < enQTEButtonTypeNum; i++)
			{
				// �Q�[�����̓ǂݍ��݂𑬂����邽�߁A�ŏ��ɓǂݍ���ł����ɔj������B
				// ���\�[�X�o���N�ɓo�^�����B
				nsGraphic::nsSprite::CSpriteRender* sr = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFirst);
				sr->Init(
					kQTEButtonSpriteFilePath[i],
					0.0f, 0.0f
				);
				sr->Deactivate();
				DeleteGO(sr);
			}


			// QTE�Ɏg���{�^���̘g�̃X�v���C�g�̐����Ə�����
			m_QTEButtonFraneSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_QTEButtonFraneSR->Init(
				kQTEButtonFrameSpriteFilePath,
				kQTEButtonFrameSpriteWidth,
				kQTEButtonFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// ��\��
			m_QTEButtonFraneSR->Deactivate();

			// QTE�Ɏg���{�^���̎c���̃X�v���C�g�̐����Ə�����
			m_QTEButtonAfterImageSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_QTEButtonAfterImageSR->Init(
				kQTEButtonAfterImageSpriteFilePath,
				kQTEButtonAfterImageSpriteWidth,
				kQTEButtonAfterImageSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// �A���x�h�J���[�𐧌�\�ɂ���
			m_QTEButtonAfterImageSR->SetIsControlAlbedo(true);
			// ��\��
			m_QTEButtonAfterImageSR->Deactivate();

			int i = 0;	// �C���f�b�N�X
			for (auto& qteResultSR : m_QTEResultSR)
			{			
				// QTE�̌��ʂ̃X�v���C�g�̐����Ə�����
				qteResultSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFourth);
				qteResultSR->Init(
					kQTEResultSpriteFilePaths[i],
					kQTEResultSpriteWitdh,
					kQTEResultSpriteHeight,
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
				// ���W��ݒ�
				qteResultSR->SetPosition(kQTEResultSpriteStartPos);
				// ��\��
				qteResultSR->Deactivate();

				// �C���f�b�N�X��i�߂�
				i++;
			}

			// QTE�̌��ʂ̘g�i�����j�X�v���C�g�̐����Ə�����
			m_QTEResultFrameIn = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_QTEResultFrameIn->Init(
				kQTEResultFrameInSpriteFilePath,
				kQTEResultFrameSpriteWitdh,
				kQTEResultFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// ���W��ݒ�
			m_QTEResultFrameIn->SetPosition(kQTEResultSpriteEndPos);
			// ��\��
			m_QTEResultFrameIn->Deactivate();

			// QTE�̌��ʂ̘g�i�O���j�X�v���C�g�̐����Ə�����
			m_QTEResultFrameOut = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_QTEResultFrameOut->Init(
				kQTEResultFrameOutSpriteFilePath,
				kQTEResultFrameSpriteWitdh,
				kQTEResultFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			// ���W��ݒ�
			m_QTEResultFrameOut->SetPosition(kQTEResultSpriteEndPos);
			// ��\��
			m_QTEResultFrameOut->Deactivate();

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

			m_sonarEF = NewGO<Effect>(nsCommonData::enPrioritySecond);
			m_sonarEF->Init(kSonarEffectFilePath);

			return;
		}

		/**
		 * @brief �T�E���h��������
		*/
		void CPlayerCatchEnemy::InitSound()
		{
			m_commandSuccessSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_commandSuccessSC->Init(kCommandSuccessSoundFilePath, nsSound::CSoundCue::enSE);
			m_commandSuccessSC->SetVolume(kCommandSuccessSoundVolume);

			m_commandMissSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_commandMissSC->Init(kCommandMissSoundFilePath, nsSound::CSoundCue::enSE);
			m_commandMissSC->SetVolume(kCommandMissSoundVolume);

			m_windowOpneSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_windowOpneSC->Init(kWindowOpneSoundFilePath, nsSound::CSoundCue::enSE);
			m_windowOpneSC->SetVolume(kWindowOpenSoundVolume);

			m_windowCloseSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_windowCloseSC->Init(kWindowCloseSoundFilePath, nsSound::CSoundCue::enSE);
			m_windowCloseSC->SetVolume(kWindowCloseSoundVolume);

			m_fireStartSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_fireStartSC->Init(kFireStartSoundFilePath, nsSound::CSoundCue::enSE);
			m_fireStartSC->SetVolume(kFireStartSoundVolume);

			m_fireReleaseSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_fireReleaseSC->Init(kFireReleaseSoundFilePath, nsSound::CSoundCue::enSE);
			m_fireReleaseSC->SetVolume(kFireReleaseSoundVolume);

			m_sonarSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_sonarSC->Init(kSonarSoundFilePath, nsSound::CSoundCue::enSE);
			m_sonarSC->SetVolume(kSonarSoundVolume);

			for (int i = 0; i < kFireVoiceSoundTypeNum; i++)
			{
				m_fireVoiceSC[i] = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_fireVoiceSC[i]->Init(kFireVoiceSoundFilePath[i], nsSound::CSoundCue::enSE);
				m_fireVoiceSC[i]->SetVolume(kFireVoiceSoundVolume);
			}
			for (int i = 0; i < kFailureVoiceSoundTypeNum; i++)
			{
				m_failureVoiceSC[i] = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_failureVoiceSC[i]->Init(KFailureVoiceSoundFilePath[i], nsSound::CSoundCue::enSE);
				m_failureVoiceSC[i]->SetVolume(kFailureVoiceSoundVolume);
			}

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
		 * @brief ���ł����ʂōs����X�V
		*/
		void CPlayerCatchEnemy::CommonUpdate()
		{
			// QTE�Ɏg���{�^���̎c���̍X�V
			QTEButternAfterImageUpdate();

			// QTE�̌��ʂ̍X�V
			QTEResultUpdate();

			return;
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

				// �R�}���h���͂����������X�e�[�g�֑J��
				ChangeState(enCE_SuccessCommand);
				// QTE�Ɏg���{�^���̎c�����J�n����
				StartQTEButtonAfterImage(true);
				// QTE�̌��ʂ̃X�v���C�g�̉��o�𐬌��ŊJ�n����
				StartQTEResult(enQR_success);

				// �R�}���h���͐����̃T�E���h���Đ�
				m_commandSuccessSC->Play(false);

				return;
			}
			
			// �R�}���h���͂��A�܂��I����Ă��Ȃ��B

			if (m_commandInput->GetCommandResult() == 
				nsPlayerConstData::nsCommandInputConstData::enCR_Success)
			{
				// �R�}���h���͂��������Ă���B���̃R�}���h�ɐi�ށB

				// ���݂̃{�^���̃X�v���C�g���\���ɂ���
				m_QTEButtonSRs[m_oldCommandProgress]->Deactivate();
				// ���̃{�^���̃X�v���C�g���A�N�e�B�u���[�h�̏�Z�J���[�Ɗg�嗦�ɂ���
				m_QTEButtonSRs[m_oldCommandProgress + 1]->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
				m_QTEButtonSRs[m_oldCommandProgress + 1]->SetScale(kQTEButtonSpriteActionScale);
				// �{�^���̘g�����̃{�^���̈ʒu�ɂ���
				m_QTEButtonFraneSR->SetPosition(m_QTEButtonSRs[m_oldCommandProgress + 1]->GetPosition());

				// QTE�Ɏg���{�^���̎c�����J�n����
				StartQTEButtonAfterImage(true);

				// �R�}���h���͐����̃T�E���h���Đ�
				m_commandSuccessSC->Play(false);
			}
			else if (m_commandInput->GetCommandResult() ==
				nsPlayerConstData::nsCommandInputConstData::enCR_Miss)
			{
				// �R�}���h���͂����s���Ă���B

				// �R�}���h���͂����s�����X�e�[�g�֑J��
				ChangeState(enCE_FailureCommand);
				// QTE�Ɏg���{�^���̎c�����J�n����
				StartQTEButtonAfterImage(false);
				// QTE�̌��ʂ̃X�v���C�g�̉��o����̓~�X�ŊJ�n����
				StartQTEResult(enQR_miss);

				// �R�}���h���͎��s�̃T�E���h���Đ�
				m_commandMissSC->Play(false);
			}

			// �O�t���[���̃R�}���h�i�s�x���X�V����
			m_oldCommandProgress = m_commandInput->GetCommandProgress();

			// �^�C�}�[��i�߂�
			m_onEnemyTimer += m_playerRef->GetDeltaTime();

			// �o�[�̊g�嗦�B���X�ɏ��������Ă����B
			const float barRate = 1.0f - (m_onEnemyTimer / kOnEnemyTime);
			// �o�[�̊g�嗦��ݒ�
			m_onEnemyTimerBar->SetScale({ barRate ,1.0f,1.0f });


			if (m_onEnemyTimer >= kOnEnemyTime)
			{
				// ���Ԑ؂�

				// �R�}���h���͂����s�����X�e�[�g�֑J��
				ChangeState(enCE_FailureCommand);
				// QTE�̌��ʂ̃X�v���C�g�̉��o�����Ԑ؂�ŊJ�n����
				StartQTEResult(enQR_failed);

				// �R�}���h���͎��s�̃T�E���h���Đ�
				m_commandMissSC->Play(false);

				return;
			}

			return;
		}


		/**
		 * @brief QTE�Ɏg���{�^���̎c���̍X�V
		*/
		void CPlayerCatchEnemy::QTEButternAfterImageUpdate()
		{
			if (m_QTEButtonAfterImageSR->IsActive() != true)
			{
				// �c�オ�\������Ă��Ȃ��B�������^�[���B
				return;
			}

			// �c�����\������Ă��鎞���s

			// �^�C�}�[�̐i�ݗ�
			const float rate = m_buttonAfterImageTimer / kQTEButtonAfterImageTime;
			// �⊮���B�w���֐��I�ω��ɂ���B
			const float t = pow(rate, kQTEButtonAfterImagePowPower);

			// �A���t�@�l�B���񂾂񔖂��Ȃ��Ă����B
			const float alphaValue =
				Math::Lerp<float>(t, kQTEButtonAfterImageSpriteAlphaValue, 0.0f);
			// �g�嗦�B���񂾂�傫���Ȃ��Ă����B
			const float scale = Math::Lerp<float>(t, 1.0f, kQTEButtonAfterImageMaxScale);
			// �c���̃A���t�@�l�Ɗg�嗦��ݒ肷��
			m_QTEButtonAfterImageSR->SetAlphaValue(alphaValue);
			m_QTEButtonAfterImageSR->SetScale(scale);

			// �^�C�}�[��i�߂�
			m_buttonAfterImageTimer += m_playerRef->GetDeltaTime();

			if (m_buttonAfterImageTimer >= kQTEButtonAfterImageTime)
			{
				// �^�C�}�[�����Ԃ��߂�����A�c�����\���ɂ���B
				m_QTEButtonAfterImageSR->Deactivate();
			}


			return;
		}

		/**
		 * @brief QTE�̌��ʂ̍X�V
		*/
		void CPlayerCatchEnemy::QTEResultUpdate()
		{
			for (auto& qteResultSR : m_QTEResultSR)
			{
				if (qteResultSR->IsActive() != true)
				{
					// ���̎�ނ�QTE�̌��ʂ��A�\������Ă��Ȃ��B���ցB
					continue;
				}

				if (m_resultTimer <= kQTEResultFrameMoveTime)
				{
					// QTE�̌��ʂ̘g�̈ړ�����

					// ���Ԃ̐i�ݗ�
					const float t = m_resultTimer / kQTEResultFrameMoveTime;
					// QTE�̌��ʂ̘g�́A�����ƊO���A���ꂼ��̊g�嗦�B
					const float scaleIn = Math::Lerp<float>(t, kQTEResultFrameInSpriteMinScale, 1.0f);
					const float scaleOut = Math::Lerp<float>(t, kQTEResultFrameOutSpriteMaxScale, 1.0f);
					// ���ʂ̃X�v���C�g�̃A���t�@�l
					const float alphaValue = Math::Lerp<float>(t, kQTEResultSpriteStartAplhaValue, 1.0f);

					// QTE�̌��ʂ̘g�̃X�v���C�g�̊g�嗦�ƃA���t�@�l��ݒ�
					m_QTEResultFrameIn->SetScale(scaleIn);
					m_QTEResultFrameIn->SetAlphaValue(alphaValue);
					m_QTEResultFrameOut->SetScale(scaleOut);
					m_QTEResultFrameOut->SetAlphaValue(alphaValue);

					if (m_windowOpneSC->IsPlaying() != true)
					{
						m_windowOpneSC->Play(false);
					}
				}
				else if (m_resultTimer <= kQTEResultFrameMoveTime + kQTEResultMoveTime)
				{
					// QTE�̌��ʂ̈ړ�����

					// QTE�̌��ʂ̘g�̃X�v���C�g�̊g�嗦�ƃA���t�@�l���ŏI�l��ݒ�
					m_QTEResultFrameIn->SetScale(1.0f);
					m_QTEResultFrameIn->SetAlphaValue(1.0f);
					m_QTEResultFrameOut->SetScale(1.0f);
					m_QTEResultFrameOut->SetAlphaValue(1.0f);

					// �^�C�}�[�̐i�ݗ�
					const float t = (m_resultTimer - kQTEResultFrameMoveTime) / kQTEResultMoveTime;
					// ���ʂ̃X�v���C�g�̍��W
					Vector2 pos = Vector2::Zero;
					pos.Lerp(t, kQTEResultSpriteStartPos, kQTEResultSpriteEndPos);
					// ���ʂ̃X�v���C�g�̃A���t�@�l
					const float alphaValue = Math::Lerp<float>(t, kQTEResultSpriteStartAplhaValue, 1.0f);

					// ���ʂ̃X�v���C�g�̍��W�ƃA���t�@�l��ݒ�
					qteResultSR->SetPosition(pos);
					qteResultSR->SetAlphaValue(alphaValue);
				}
				else if (m_resultTimer <=
					kQTEResultFrameMoveTime + kQTEResultMoveTime + kQTEResultDisplayTime)
				{
					// QTE�̌��ʂ̕\������

					// ���ʂ̃X�v���C�g�̍��W�ƃA���t�@�l�̍ŏI�l��ݒ�
					qteResultSR->SetPosition(kQTEResultSpriteEndPos);
					qteResultSR->SetAlphaValue(1.0f);
				}
				else if (m_resultTimer <= kQTEResultFrameMoveTime + kQTEResultMoveTime +
					kQTEResultDisplayTime + kQTEResultBackTime)
				{
					//  QTE�̌��ʂ̖߂鎞��

					// �^�C�}�[�̐i�ݗ�
					const float t = (m_resultTimer - kQTEResultFrameMoveTime - 
						kQTEResultMoveTime - kQTEResultDisplayTime) /
						kQTEResultBackTime;
					// ���ʂ̃X�v���C�g�̍��W
					Vector2 pos = Vector2::Zero;
					pos.Lerp(t, kQTEResultSpriteEndPos, kQTEResultSpriteStartPos);
					// ���ʂ̃X�v���C�g�̃A���t�@�l
					const float alphaValue = Math::Lerp<float>(t, 1.0f, 0.0f);

					// ���ʂ̃X�v���C�g�̍��W�ƃA���t�@�l��ݒ�
					qteResultSR->SetPosition(pos);
					qteResultSR->SetAlphaValue(alphaValue);
				}
				else if (m_resultTimer <= kQTEResultFrameMoveTime + kQTEResultMoveTime +
					kQTEResultDisplayTime + kQTEResultBackTime + kQTEResultFrameBackTime)
				{
					// QTE�̌��ʂ̘g�̖߂鎞��

					// �^�C�}�[�̐i�ݗ��B
					const float t = (m_resultTimer - kQTEResultFrameMoveTime -
						kQTEResultMoveTime - kQTEResultDisplayTime - kQTEResultBackTime) /
						kQTEResultFrameBackTime;

					// QTE�̌��ʂ̘g�́A�����ƊO���A���ꂼ��̊g�嗦�B
					const float scaleIn = Math::Lerp<float>(t, 1.0f, kQTEResultFrameInSpriteMinScale);
					const float scaleOut = Math::Lerp<float>(t, 1.0f, kQTEResultFrameOutSpriteMaxScale);
					// ���ʂ̃X�v���C�g�̃A���t�@�l
					const float alphaValue = Math::Lerp<float>(t, 1.0f, 0.0f);

					// QTE�̌��ʂ̘g�̃X�v���C�g�̊g�嗦�ƃA���t�@�l��ݒ�
					m_QTEResultFrameIn->SetScale(scaleIn);
					m_QTEResultFrameIn->SetAlphaValue(alphaValue);
					m_QTEResultFrameOut->SetScale(scaleOut);
					m_QTEResultFrameOut->SetAlphaValue(alphaValue);

					if (m_windowCloseSC->IsPlaying() != true)
					{
						m_windowCloseSC->Play(false);
					}
				}


				// �^�C�}�[��i�߂�B
				m_resultTimer += m_playerRef->GetDeltaTime();

				if (m_resultTimer > kQTEResultFrameMoveTime + kQTEResultMoveTime +
					kQTEResultDisplayTime + kQTEResultBackTime + kQTEResultFrameBackTime)
				{
					// �^�C�}�[���A�S�Ă̎��Ԃ��߂�����A��\���ɂ��āA�I���B
					qteResultSR->Deactivate();
					m_QTEResultFrameIn->Deactivate();
					m_QTEResultFrameOut->Deactivate();
				}

				// ���ނ����A�X�V���Ȃ��͂��Ȃ̂ŁA���[�v�𔲂���B
				break;
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
			m_ninnjyutuEFTimer += m_playerRef->GetDeltaTime();
			
			if (m_ninnjyutuEFTimer >= kNinjyutuEffectTime)
			{
				// �^�C�}�[���A�E�p�̃G�t�F�N�g�̎��Ԃ𒴂�����A
				// �Ԃ�߂܂�����Ԃɂ��āA�E�p�̃G�t�F�N�g�̎Q�Ƃ�n���āA�X�e�[�g��J�ڂ���B
				m_targetRef->BeCaptured();
				m_targetRef->SetNinjyutuEffectRef(m_ninjyutuEF);
				ChangeState(enCE_End);

			}
			else if (m_ninnjyutuEFTimer >= kFireReleaseSoundTime)
			{
				if (m_fireReleaseSC->IsPlaying() != true)
				{
					m_fireReleaseSC->Play(false);
					m_fireVoiceSC[(*m_fireVoiceRand)(*m_mt)]->Play(false);
				}
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

			if (m_aiCarsRef->empty())
			{
				// �G�l�~�[�����Ȃ��Ȃ�A���ׂȂ��B�������^�[���B
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
		 * @brief �^�[�Q�b�g�Ƃ̋������X�V
		*/
		void CPlayerCatchEnemy::UpdateTargetLength()
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
		 * @brief QTE�Ɏg���{�^���̎c�����J�n����
		 * @param[in] isSuccess �R�}���h���͂������������H
		*/
		void CPlayerCatchEnemy::StartQTEButtonAfterImage(const bool isSuccess)
		{
			// �c���p�̃X�v���C�g��\������
			m_QTEButtonAfterImageSR->Activate();
			// �c���p�̃X�v���C�g��O�̃{�^���̈ʒu�Ɉړ�������
			m_QTEButtonAfterImageSR->SetPosition(m_QTEButtonSRs[m_oldCommandProgress]->GetPosition());
			// �c���p�̃X�v���C�g�̃A���t�@�l�A�g�嗦�A�^�C�}�[�����Z�b�g
			m_QTEButtonAfterImageSR->SetAlphaValue(kQTEButtonAfterImageSpriteAlphaValue);
			m_QTEButtonAfterImageSR->SetScale(1.0f);
			m_buttonAfterImageTimer = 0.0f;

			if (isSuccess)
			{
				// �R�}���h���͂��������Ă�����A
				// �c���p�̃X�v���C�g�̃A���x�h�J���[�𐬌����̃A���x�h�J���[�ɂ���B
				m_QTEButtonAfterImageSR->SetAlbedoColor(kQTEButtonAfterImageSpriteSuccessAlbedoColor);
			}
			else
			{
				// �R�}���h���͂����s���Ă�����A
				// �c���p�̃X�v���C�g�̃A���x�h�J���[�����s���̃A���x�h�J���[�ɂ���B
				m_QTEButtonAfterImageSR->SetAlbedoColor(kQTEButtonAfterImageSpriteMissAlbedoColor);
			}


			return;
		}

		/**
		 * @brief QTE�̌��ʂ̃X�v���C�g�̉��o���J�n����
		 * @param[in] qteResultType QTE�̌��ʂ̎��
		*/
		void CPlayerCatchEnemy::StartQTEResult(
			const nsPlayerConstData::nsCatchEnemyConstData::EnQTEResultType qteResultType
		)
		{
			// �X�v���C�g��L����
			m_QTEResultSR[qteResultType]->Activate();
			m_QTEResultFrameIn->Activate();
			m_QTEResultFrameOut->Activate();
			// ���W�ƃA���t�@�l�ƃ^�C�}�[�����Z�b�g
			m_QTEResultSR[qteResultType]->SetPosition(kQTEResultSpriteStartPos);
			m_QTEResultSR[qteResultType]->SetAlphaValue(0.0f);
			m_QTEResultFrameIn->SetScale(1.0f);
			m_QTEResultFrameOut->SetScale(1.0f);
			m_QTEResultFrameIn->SetAlphaValue(0.0f);
			m_QTEResultFrameOut->SetAlphaValue(0.0f);
			m_resultTimer = 0.0f;

			return;
		}


		/**
		 * @brief QTE�Ŏg���X�v���C�g��L��������
		*/
		void CPlayerCatchEnemy::ActivateQTESprite()
		{
			m_onEnemyTimerBar->Activate();
			m_onEnemyTimerBarFrame->Activate();
			m_QTEButtonFraneSR->Activate();

			return;
		}

		/**
		 * @brief QTE�Ŏg���X�v���C�g��L��������
		*/
		void CPlayerCatchEnemy::DeactivateQTESprite()
		{
			m_onEnemyTimerBar->Deactivate();
			m_onEnemyTimerBarFrame->Deactivate();
			m_QTEButtonFraneSR->Deactivate();

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
			m_catchEnemyState = enCE_FindTarget;
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
			DeactivateQTESprite();

			return;
		}

		/**
		 * @brief �X�e�[�g��J�ڂ���
		 * @param[in] newState �V�����X�e�[�g
		*/
		void CPlayerCatchEnemy::ChangeState(const nsPlayerConstData::nsCatchEnemyConstData::EnCatchEnemyState newState)
		{
			if (m_catchEnemyState == newState)
			{
				return;
			}


			m_catchEnemyState = newState;

			switch (m_catchEnemyState)
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
				ActivateQTESprite();
				// �{�^���̘g�̃X�v���C�g�̍��W���A��ԍ��̃{�^���̃X�v���C�g�̍��W�Ɠ����ɂ���B
				m_QTEButtonFraneSR->SetPosition(m_QTEButtonSRs[0]->GetPosition());
				break;

			case enCE_SuccessCommand:
				// �X�v���C�g���������ׂĔ�\���ɂ���
				for (auto& qteButtonSR : m_QTEButtonSRs)
				{
					qteButtonSR->Deactivate();
				}
				DeactivateQTESprite();

				// �E�p�̃G�t�F�N�g�̍��W�Ɖ�]��ݒ肵�āA�Đ�����B
				m_ninjyutuEF->SetPosition(m_targetRef->GetPosition() + Vector3::Up * kNinjyutuEffectPosBufHeight);
				m_ninjyutuEF->SetRotation(m_targetRef->GetRotation());
				m_ninjyutuEF->Play();

				m_fireStartSC->Play(false);
				break;

			case enCE_FailureCommand:
				// �R�}���h�~�X�̉񐔂��J�E���g����
				nsGameState::CGameMainState::GetInstance()->CountMissCommand();
				// �X�v���C�g���������ׂĔ�\���ɂ���
				for (auto& qteButtonSR : m_QTEButtonSRs)
				{
					qteButtonSR->Deactivate();
				}
				DeactivateQTESprite();

				// ��莞�ԓ��͕s�ɂ���
				m_playerRef->SetIsInputtable(false);
				// �N���オ�蒆�ɂ���
				m_isStandUp = true;

				m_failureVoiceSC[(*m_failureVoiceRand)(*m_mt)]->Play(false);

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