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
		using namespace nsPlayerConstData::nsPlayerModelRenderConstData;

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
			//m_playerModel->Init(kPlayerModelFilePath, m_animationClips, enAnim_num, enModelUpAxisY);
			m_playerModel->IniTranslucent(
				kPlayerModelFilePath,
				m_animationClips,
				enAnim_num
				);
			// �V���h�E�L���X�^�[��L���ɂ���
			m_playerModel->SetIsShadowCaster(true);
			m_playerModel->SetAnimationSpeed(2.0f);

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
			m_animState = enAnim_idle;

			//switch (m_playerRef->GetState())
			{
				//case nsPlayerConstData::enWalkAndRun:

					if (m_playerRef->GetPlayerMovement().GetVelocity() <= 10.0f)
						m_animState = enAnim_idle;
					else if (m_playerRef->GetPlayerMovement().GetVelocity() <= 800.0f)
						m_animState = enAnim_walk;
					else
						m_animState = enAnim_run;

					//if (m_playerRef->GetPlayerMovement().IsAir())
					//	m_animState = enAnim_jump;


				//	break;
				//case nsPlayerConstData::enSwing:
					//m_animState = enAnim_swingStart;

				//	break;
			}


			// �A�j���[�V�����Đ�
			// �O�t���[���Ɠ����A�j���[�V������Play�����ꍇ�A�Ȃɂ����Ȃ�
			m_playerModel->PlayAnimation(m_animState, m_AnimInterpolateTime);

			return;
		}


	}
}