#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"
#include "ModelRender.h"


namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic
	{
		namespace nsModel { class CModelRender; }	//���f�������_���[�N���X
	}

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		/**
		 * @brief �v���C���[���f���N���X
		*/
		class CPlayerModel : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerModel() = default;
			/**
			* @brief �f�X�g���N�^
			*/
			~CPlayerModel();

		public:		// �����o�֐�

			/**
			 * @brief ������
			 * @param[in] startPos �X�^�[�g���W
			 * @param[in] startRot �X�^�[�g��]
			*/
			void Init(const Vector3& startPos, const Quaternion& startRot);

			/**
			 * @brief �v���C���[���f���N���X�̃A�b�v�f�[�g�����s����
			*/
			void ExecuteUpdate();


		private:	// private�ȃ����o�֐�

			/**
			 * @brief �A�j���[�V����������
			*/
			void InitAnimation();

			/**
			 * @brief ���f��������
			 * @param[in] startPos �X�^�[�g���W
			 * @param[in] startRot �X�^�[�g��]
			*/
			void InitModel(const Vector3& startPos, const Quaternion& startRot);

			/**
			 * @brief ���f�����
			*/
			void ReleaseModel();

			/**
			 * @brief �g�����X�t�H�[�����X�V
			*/
			void UpdateTransform();

			/**
			 * @brief �A�j���[�V�����J�ڂ��X�V
			*/
			void UpdateAnimationTransition();

		private:	// �f�[�^�����o
			nsGraphic::nsModel::CModelRender* m_playerModel = nullptr;	//!< �v���C���[�̃��f�������_���[
			//!< �A�j���[�V�����N���b�v�X�N���X
			AnimationClip m_animationClips[	nsPlayerModelRenderConstData::enAnim_num];

			//!< �A�j���[�V�����X�e�[�g
			nsPlayerModelRenderConstData::EnAnimationClips m_animState =
				nsPlayerModelRenderConstData::enAnim_idle;

			//!< �A�j���[�V�����⊮����
			float m_AnimInterpolateTime = nsPlayerModelRenderConstData::kDefaultAnimInterpolateTime;

			const Vector3* m_playerPosition = nullptr;		//!< �v���C���[�̍��W�̎Q��
			const Quaternion* m_playerRotation = nullptr;	//!< �v���C���[�̉�]�̎Q��

		};

	}
}
