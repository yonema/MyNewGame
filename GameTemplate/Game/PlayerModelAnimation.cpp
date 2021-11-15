#include "stdafx.h"
#include "PlayerModelAnimation.h"
#include "Player.h"
#include "ModelRender.h"

namespace nsMyGame
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
			m_playerModel->Init(kPlayerModelFilePath, m_animationClips, enAnim_num);
			//m_playerModel->IniTranslucent(
			//	kPlayerModelFilePath,
			//	m_animationClips,
			//	enAnim_num
			//	);
			// �V���h�E�L���X�^�[��L���ɂ���
			m_playerModel->SetIsShadowCaster(true);

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
			m_AnimInterpolateTime = 0.2f;
			if (m_playerRef->GetState() == nsPlayerConstData::enSwing)
			{
				// �X�C���O���̃A�j���[�V�����X�e�[�g���X�V
				UpdateSwingAnimationState();
				return;
			}

			// �v���C���[�̑��x

			if (m_playerRef->GetPlayerMovement().IsAir())
			{
				if (m_animState != enAnim_airIdle)
				{
					m_playerModel->SetAnimationSpeed(2.0f);
					if (m_playerModel->IsAnimationPlaying() == true)
					{
						m_AnimInterpolateTime = 0.0f;
						m_animState = enAnim_jumpUp;
					}
					else
					{
						m_animState = enAnim_airIdle;
					}
				}

				return;
			}
			const float playerVelocity = m_playerRef->GetPlayerMovement().GetXZSpeed();

			if (playerVelocity <= 10.0f)
			{
				m_animState = enAnim_idle;
				m_playerModel->SetAnimationSpeed(2.0f);
			}
			else if (playerVelocity <= nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed + 50.0f)
			{
				m_animState = enAnim_walk;
				m_playerModel->SetAnimationSpeed(
					1.0f + 1.0f * playerVelocity / nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed
				);
			}
			else
			{
				m_animState = enAnim_run;
				m_playerModel->SetAnimationSpeed(
					2.0f * playerVelocity / nsPlayerConstData::nsWalkAndRunConstData::kRunMaxSpeed
				);
			}

			return;
		}

		/**
		 * @brief �X�C���O���̃A�j���[�V�����X�e�[�g���X�V
		*/
		void CPlayerModelAnimation::UpdateSwingAnimationState()
		{
			switch (m_swingAnimSate)
			{
			case enSwingAnim_swingStart:
				if (m_playerModel->IsAnimationPlaying() != true && m_animState == enAnim_swingStart)
				{
					m_swingAnimSate = enSwingAnim_swing;
				}
				m_animState = enAnim_swingStart;
				m_playerModel->SetAnimationSpeed(1.0f);

				break;
			case enSwingAnim_swing:
				m_playerModel->SetAnimationSpeed(1.0f);
				m_animState = enAnim_swinging;
				break;
			case enSwingAnim_swingRaiseLeg:
				m_playerModel->SetAnimationSpeed(1.5f);
				m_animState = enAnim_swingRaiseLeg;
				break;
			case enSwingAnim_swingRoll:
				if (m_playerModel->IsAnimationPlaying() != true && m_animState == enAnim_swingRoll)
				{
					m_swingAnimSate = enSwingAnim_airAfterSwing;
				}
				m_animState = enAnim_swingRoll;
				m_AnimInterpolateTime = 0.0f;
				m_playerModel->SetAnimationSpeed(2.0f);
				break;
			case enSwingAnim_airAfterSwing:
				m_animState = enAnim_airIdle;
				m_playerModel->SetAnimationSpeed(1.0f);
				break;
			}

			return;
		}

	}
}