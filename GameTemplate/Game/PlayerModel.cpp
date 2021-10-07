#include "stdafx.h"
#include "PlayerModel.h"
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
		using namespace nsPlayerModelRenderConstData;

		/**
		* @brief �f�X�g���N�^
		*/
		CPlayerModel::~CPlayerModel()
		{
			// ���f�����
			ReleaseModel();

			return;
		}

		/**
		 * @brief ������
		 * @param startPos �X�^�[�g���W
		 * @param startRot �X�^�[�g��]
		*/
		void CPlayerModel::Init(const Vector3& startPos, const Quaternion& startRot)
		{
			// �A�j���[�V����������
			InitAnimation();
			// ���f��������
			InitModel(startPos, startRot);

			return;
		}

		/**
		 * @brief �v���C���[���f���N���X�̃A�b�v�f�[�g�����s����
		*/
		void CPlayerModel::ExecuteUpdate()
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
		void CPlayerModel::InitAnimation()
		{
			// �A�j���[�V�����̏�����

			m_animationClips[enAnim_idle].Load(kAnimationFilePath_idle);
			m_animationClips[enAnim_idle].SetLoopFlag(true);
			m_animationClips[enAnim_walk].Load(kAnimationFilePath_walk);
			m_animationClips[enAnim_walk].SetLoopFlag(true);


			return;
		}

		/**
		 * @brief ���f��������
		 * @param[in] startPos �X�^�[�g���W
		 * @param[in] startRot �X�^�[�g��]
		*/
		void CPlayerModel::InitModel(const Vector3& startPos, const Quaternion& startRot)
		{
			// �v���C���[���f�������_���[�̐���
			m_playerModel = NewGO<nsGraphic::nsModel::CModelRender>(enPriorityFirst);

			// �v���C���[�̍��W�Ɖ�]�̎Q�Ƃ��󂯎��
			m_playerPosition = &startPos;
			m_playerRotation = &startRot;

			// ���W�Ɖ�]��ݒ�
			m_playerModel->SetPosition(*m_playerPosition);
			m_playerModel->SetRotation(*m_playerRotation);

			// �v���C���[���f�������_���[�̏�����
			m_playerModel->Init(kPlayerModelFilePath, m_animationClips, enAnim_num, enModelUpAxisY);

			// �V���h�E�L���X�^�[��L���ɂ���
			m_playerModel->SetIsShadowCaster(true);

			return;
		}

		/**
		* @brief ���f�����
		*/
		void CPlayerModel::ReleaseModel()
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
		void CPlayerModel::UpdateTransform()
		{
			// ���W�Ɖ�]���X�V
			m_playerModel->SetPosition(*m_playerPosition);
			m_playerModel->SetRotation(*m_playerRotation);

			return;
		}


		/**
		 * @brief �A�j���[�V�����J�ڂ��X�V
		*/
		void CPlayerModel::UpdateAnimationTransition()
		{
			// �A�j���[�V�����Đ�
			// �O�t���[���Ɠ����A�j���[�V������Play�����ꍇ�A�Ȃɂ����Ȃ�
			m_playerModel->PlayAnimation(m_animState, m_AnimInterpolateTime);

			return;
		}


	}
}