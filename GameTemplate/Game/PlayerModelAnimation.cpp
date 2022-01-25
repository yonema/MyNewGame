#include "stdafx.h"
#include "PlayerModelAnimation.h"
#include "Player.h"
#include "ModelRender.h"

namespace nsNinjaAttract
{

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// ���ʃf�[�^���g�p�\�ɂ���
		using namespace nsCommonData;
		// �v���C���[�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsPlayerConstData::nsModelAnimationConstData;

		/**
		* @brief �f�X�g���N�^
		*/
		CPlayerModelAnimation::~CPlayerModelAnimation()
		{
			// ���f�����
			ReleaseModel();

			return;
		}

		/**
		 * @brief ���f��������
		 * @param[in] player �v���C���[�̎Q��
		*/
		void CPlayerModelAnimation::Init(const CPlayer& player)
		{
			// �v���C���[�̎Q�Ƃ��Z�b�g
			m_playerRef = &player;

			// �A�j���[�V����������
			InitAnimation();
			// ���f��������
			InitModel();

			// ��̃{�[��ID���擾
			m_handBoneID = m_playerModel->FindBoneID(kHandBoneName);

			return;
		}

		/**
		 * @brief �v���C���[���f���N���X�̃A�b�v�f�[�g�����s����
		*/
		void CPlayerModelAnimation::ExecuteUpdate()
		{
			// �g�����X�t�H�[�����X�V
			UpdateTransform();
			// �A�j���[�V�����J�ڂ��X�V
			UpdateAnimationTransition();

			return;
		}



		/**
		 * @brief �A�j���[�V����������
		*/
		void CPlayerModelAnimation::InitAnimation()
		{
			// �A�j���[�V�����̏�����

			for (int i = 0; i < enAnim_num; i++)
			{
				// �A�j���[�V�����̃��[�h
				m_animationClips[i].Load(kAnimationFilePath[i]);
				//m_animationClips[i].SetLoopFlag(true);
				// ���[�v�t���O��ݒ�
				switch (i)
				{
				// ���[�v����A�j���[�V����
				case enAnim_idle:
				case enAnim_ninjaIdle:
				case enAnim_walk:
				case enAnim_run:
				case enAnim_airIdle:
				case enAnim_swinging:
					m_animationClips[i].SetLoopFlag(true);
					break;
				// ���[�v���Ȃ��A�j���[�V����
				default:
					m_animationClips[i].SetLoopFlag(false);
					break;
				}
			}

			return;
		}

		/**
		 * @brief ���f��������
		*/
		void CPlayerModelAnimation::InitModel()
		{

			// �v���C���[���f�������_���[�̐���
			m_playerModel = NewGO<nsGraphic::nsModel::CModelRender>(enPriorityFirst);

			// ���W�Ɖ�]��ݒ�
			m_playerModel->SetPosition(m_playerRef->GetPosition());
			m_playerModel->SetRotation(m_playerRef->GetRotation());

			// �v���C���[���f�������_���[�̏�����
			//m_playerModel->Init(kPlayerModelFilePath, m_animationClips, enAnim_num);
			m_playerModel->IniTranslucent(
				kPlayerModelFilePath,
				m_animationClips,
				enAnim_num
				);
			// �V���h�E�L���X�^�[��L���ɂ���
			m_playerModel->SetIsPlayerShadowCaster(true);

			return;
		}

		/**
		* @brief ���f�����
		*/
		void CPlayerModelAnimation::ReleaseModel()
		{
			// ���������Ă��Ȃ����H
			if (!m_playerModel)
			{
				// ���������Ă��Ȃ�
				// �������^�[��
				return;
			}

			// �v���C���[���f�������_���[�̔j��
			DeleteGO(m_playerModel);

			return;
		}

		/**
		 * @brief �g�����X�t�H�[�����X�V
		*/
		void CPlayerModelAnimation::UpdateTransform()
		{
			// ���W�Ɖ�]���X�V
			m_playerModel->SetPosition(m_playerRef->GetPosition());
			m_playerModel->SetRotation(m_playerRef->GetRotation());
			return;
		}


		/**
		 * @brief �A�j���[�V�����J�ڂ��X�V
		*/
		void CPlayerModelAnimation::UpdateAnimationTransition()
		{
			// �A�j���[�V�����X�e�[�g���X�V
			UpdateAnimationState();

			// �A�j���[�V�����̃X�s�[�h��ݒ�
			m_playerModel->SetAnimationSpeed(m_aninSpeed);

			// �A�j���[�V�����Đ�
			// �O�t���[���Ɠ����A�j���[�V������Play�����ꍇ�A�Ȃɂ����Ȃ�
			m_playerModel->PlayAnimation(m_animState, m_AnimInterpolateTime);

			return;
		}

		/**
		 * @brief �A�j���[�V�����X�e�[�g���X�V
		*/
		void CPlayerModelAnimation::UpdateAnimationState()
		{
			// �f�t�H���g�̃A�j���[�V�����⊮���Ԃɂ���
			m_AnimInterpolateTime = kDefaultAnimInterpolateTime;
			// �f�t�H���g�̃A�j���[�V�����X�s�[�h�ɂ���
			m_aninSpeed = kDefaultAnimSpeed;

			// �X�C���O���̃A�j���[�V�����X�e�[�g���X�V
			if (UpdateSwingAnimationState())
			{
				return;
			}

			// �R�}���h���͂����s�������̃A�j���[�V�����X�e�[�g���X�V
			if (UpdateCommandFailed())
			{
				return;
			}

			// �G�̏�ɏ���Ă�Ƃ��ɃA�j���[�V�����X�e�[�g���X�V
			if (UpdateOnEnemyAnimationState())
			{
				return;
			}

			// �ŏ��̗����̃A�j���[�V�����̍X�V
			if (UpdateStartFall())
			{
				return;
			}

			// �W�����v�Ƌ󒆂ł̃A�j���[�V�����̍X�V
			if (UpdateJumpAndAir())
			{
				return;
			}

			// �A�C�h����Ԃƕ����Ƒ���̃A�j���[�V�����̍X�V
			if (UpdateIdleWalkAndRun())
			{
				return;
			}


			return;
		}

		/**
		 * @brief �X�C���O���̃A�j���[�V�����X�e�[�g���X�V
		 * @param �������^�[�����s�����H
		*/
		bool CPlayerModelAnimation::UpdateSwingAnimationState()
		{
			if (m_playerRef->GetState() != nsPlayerConstData::enSwing)
			{
				// �X�C���O���ł͂Ȃ��̂ŁA�������^�[�����Ȃ��悤�ɂ���B
				return false;
			}

			// �X�C���O���̂̃X�e�[�g�ɉ����āA������U�蕪����
			switch (m_swingAnimSate)
			{
			case enSwingAnim_swingStart:
				if (m_playerModel->IsAnimationPlaying() != true && m_animState == enAnim_swingStart)
				{
					// �X�C���O�X�^�[�g�̃A�j���[�V�������I��������

					// �X�C���O���̃A�j���[�V�����X�e�[�g���X�C���O���ɑJ��
					m_swingAnimSate = enSwingAnim_swing;
				}

				// �X�C���O�X�^�[�g�̃A�j���[�V�������Đ�
				m_animState = enAnim_swingStart;

				break;
			case enSwingAnim_swing:
				// �X�C���O���̃A�j���[�V�������Đ�
				m_animState = enAnim_swinging;

				break;
			case enSwingAnim_swingRaiseLeg:
				// �����グ��X�C���O�̃A�j���[�V�������Đ�
				m_animState = enAnim_swingRaiseLeg;
				m_aninSpeed = 1.5f;
				break;
			case enSwingAnim_swingRoll:
				if (m_playerModel->IsAnimationPlaying() != true && m_animState == enAnim_swingRoll)
				{
					// �X�C���O���[���̃A�j���[�V�������I��������

					// �X�C���O���̃X�e�[�g���X�C���O��̋󒆏�Ԃ֑J��
					m_swingAnimSate = enSwingAnim_airAfterSwing;
				}
				// �X�C���O���[���̃A�j���[�V�������Đ�
				m_animState = enAnim_swingRoll;
				m_AnimInterpolateTime = 0.0f;
				m_aninSpeed = 2.0f;
				break;
			case enSwingAnim_airAfterSwing:
				// �󒆂̃A�C�h����Ԃ̃A�j���[�V�������Đ�
				m_animState = enAnim_airIdle;
				break;
			}

			// �X�C���O���̂��߁A�������^�[��������悤�ɂ���B
			return true;
		}


		/**
		 * @brief �R�}���h���͂����s�������̃A�j���[�V�����X�e�[�g���X�V
		 * @return �������^�[�����s�����H
		*/
		bool CPlayerModelAnimation::UpdateCommandFailed()
		{
			if (m_playerRef->GetCatchEnemy().GetCatchEnemyState() ==
				nsPlayerConstData::nsCatchEnemyConstData::enCE_FailureCommand)
			{
				// �R�}���h���͎��s��

				// �m�b�N�_�E���̃A�j���[�V�����̊J�n
				m_animState = enAnim_knockDown;
				m_AnimInterpolateTime = 0.5f;
			}

			if (m_animState != enAnim_knockDown && m_animState != enAnim_standUp)
			{
				// �m�b�N�_�E�����ł��A�N���オ�蒆�ł��Ȃ��Ƃ�
				// �������^�[�����s��Ȃ��悤�ɂ���
				return false;
			}

			// �m�b�N�_�E�����A�܂��́A�N���オ�蒆�̎�

			if (m_playerModel->IsAnimationPlaying() == true)
			{
				// �A�j���[�V�������Đ����Ȃ�A���̂܂܍Đ����Ă����B�i�m�b�N�_�E�����j
				// �������^�[�����s���悤�ɂ���B
				return true;
			}
			else
			{
				// �A�j���[�V�����̍Đ����I��������
				if (m_animState == enAnim_knockDown)
				{
					// �m�b�N�_�E���̃A�j���[�V�������I��������

					// �N���オ�蒆�̃A�j���[�V�������Đ�
					m_animState = enAnim_standUp;
					m_AnimInterpolateTime = 1.0f;
					return true;
				}
				else
				{
					// ���̑��̃A�j���[�V�����i�N���オ�蒆�j���I��������

					// �A�C�h����Ԃ̃A�j���[�V�������Đ�
					m_animState = enAnim_idle;
					m_AnimInterpolateTime = 0.5f;
				}

			}

			// �������^�[�����s���悤�ɂ���
			return true;
		}


		/**
		 * @brief �G�̏�ɏ���Ă�Ƃ��̃A�j���[�V�����X�e�[�g���X�V
		 * @return �������^�[�����s�����H
		*/
		bool CPlayerModelAnimation::UpdateOnEnemyAnimationState()
		{
			if (m_playerRef->GetPlayerMovement().GetPlayerOnEnemy().GetOnEnemyState() ==
				nsPlayerConstData::nsOnEnemyConstData::enOnEnemy)
			{
				// �G�̏�ɏ���Ă��鎞

				// �E�҃A�C�h����Ԃ̃A�j���[�V�������Đ�
				m_animState = enAnim_ninjaIdle;

				// �������^�[�����s���悤�ɂ���
				return true;
			}
			else if (m_playerRef->GetPlayerMovement().GetPlayerOnEnemy().GetOnEnemyState() ==
				nsPlayerConstData::nsOnEnemyConstData::enGoOnEnemy)
			{
				// �G�̏�Ɍ������Ă��鎞

				if (m_animState == enAnim_goOnEnemy && m_playerModel->IsAnimationPlaying() != true)
				{
					// �A�j���[�V�������A�G�̏�Ɍ������Ă���A�j���[�V�����Ƃ��A����
					// �A�j���[�V�����̍Đ����I�����Ă���Ƃ�

					// �G�̏�Ɍ������A�j���[�V����2���Đ�
					m_animState = enAnim_goOnEnemy2;
					m_aninSpeed = 2.0f;
				}
				else if (m_animState != enAnim_goOnEnemy2)
				{
					// �A�j���[�V�������A�G�̏�Ɍ������A�j���[�V����2�ł͂Ȃ��Ƃ�

					// �G�̏�Ɍ������A�j���[�V�������Đ�
					m_animState = enAnim_goOnEnemy;
					m_aninSpeed = 2.0f;
					m_AnimInterpolateTime = 0.5f;
				}

				// �������^�[�����s���悤�ɂ���
				return true;
			}

			// �������^�[�����s��Ȃ��悤�ɂ���
			return false;
		}

		/**
		 * @brief �ŏ��̗����̃A�j���[�V����
		 * @return �������^�[�����s�����H
		*/
		bool CPlayerModelAnimation::UpdateStartFall()
		{
			if (m_playerRef->GetState() != nsPlayerConstData::enStartFall ||
				m_playerRef->GetPlayerMovement().IsAir() == true)
			{
				// �ŏ��̗����̎��ł͂Ȃ��A�܂��́A�󒆂ɂ���Ƃ�
				// �������^�[�����s��Ȃ��悤�ɂ���
				return false;
			}


			// �ŏ��̗����̂Ƃ��A���A�󒆂ɂ��Ȃ����i���n���j

			if (m_animState == enAnim_landing && m_playerModel->IsAnimationPlaying() != true)
			{
				// �A�j���[�V���������n�̃A�j���[�V�����A���A�A�j���[�V�����̍Đ����I�����Ă���Ƃ�

				// �A�C�h����Ԃ̃A�j���[�V�������Đ�
				m_animState = enAnim_idle;
				m_AnimInterpolateTime = 0.8f;
			}
			else if (m_animState != enAnim_idle)
			{
				// �A�j���[�V�������A�C�h����Ԃł͂Ȃ���

				// ���n�̃A�j���[�V�������Đ�
				m_animState = enAnim_landing;
				m_AnimInterpolateTime = 0.2f;
			}

			// �������^�[�����s���悤�ɂ���
			return true;

		}


		/**
		 * @brief �W�����v�Ƌ󒆂ł̃A�j���[�V����
		 * @return �������^�[�����s�����H
		*/
		bool CPlayerModelAnimation::UpdateJumpAndAir()
		{
			if (m_playerRef->GetPlayerMovement().IsAir() != true &&
				m_playerRef->GetPlayerMovement().GetPlayerWalkAudRun().IsJumpUp() != true)
			{
				// �󒆂ɂ��Ȃ��Ƃ��A���A�W�����v�̏㏸���ł͂Ȃ��Ƃ�
				// �������^�[�����s��Ȃ��悤�ɂ���
				return false;
			}

			// �󒆂ɂ���Ƃ��A�܂��́A�W�����v�̏㏸���ƂƂ�

			if (m_animState != enAnim_airIdle)
			{
				// �A�j���[�V�������󒆃A�C�h����Ԃł͂Ȃ�

				if (m_playerModel->IsAnimationPlaying() == true)
				{
					// �A�j���[�V�������Đ����ƂƂ�

					// �W�����v�̏㏸���̃A�j���[�V�������Đ�
					m_animState = enAnim_jumpUp;
					m_AnimInterpolateTime = 0.2f;
					m_aninSpeed = 2.0f;

				}
				else
				{
					// �A�j���[�V�����̍Đ����I�������Ƃ�

					// �󒆃A�C�h����Ԃ̃A�j���[�V�������Đ�
					m_animState = enAnim_airIdle;
				}
			}

			// �������^�[�����s���悤�ɂ���
			return true;
		}

		/**
		 * @brief �A�C�h����Ԃƕ����Ƒ���̃A�j���[�V�����̍X�V
		 * @return �������^�[�����s�����H
		*/
		bool CPlayerModelAnimation::UpdateIdleWalkAndRun()
		{
			// �v���C���[�̑��x
			const float playerVelocity = m_playerRef->GetPlayerMovement().GetXZSpeed();

			if (playerVelocity <= nsPlayerConstData::nsWalkAndRunConstData::kMinSpeed + 10.0f)
			{
				// ���x���Œᑬ�x�ȉ�

				// �A�C�h����Ԃ̃A�j���[�V�������Đ�
				m_animState = enAnim_idle;

				// �������^�[�����s���悤�ɂ���
				return true;
			}
			else if (playerVelocity <= nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed + 50.0f)
			{
				// ���x�������̍ő呬�x�ȉ�

				// �����̃A�j���[�V�������Đ�
				m_animState = enAnim_walk;

				// �������^�[�����s���悤�ɂ���
				return true;
			}
			else
			{
				// ���x�������ő呬�x��葬��

				// ����̃A�j���[�V�������Đ�
				m_animState = enAnim_run;
				// ���x�ɉ����āA1.0f�`1.5f�̑����ŃA�j���[�V�������Đ�
				m_aninSpeed = 1.0f + 0.5f * playerVelocity / nsPlayerConstData::nsWalkAndRunConstData::kRunMaxSpeed;

				// �������^�[�����s���悤�ɂ���
				return true;
			}

			// �������^�[�����s��Ȃ��悤�ɂ���
			return false;
		}

	}
}