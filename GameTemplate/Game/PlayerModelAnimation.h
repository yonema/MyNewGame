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
		// �O���錾
		class CPlayer;	// �v���C���[�N���X

		/**
		 * @brief �v���C���[���f���A�j���[�V�����N���X
		*/
		class CPlayerModelAnimation : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerModelAnimation() = default;
			/**
			* @brief �f�X�g���N�^
			*/
			~CPlayerModelAnimation();

		public:		// �����o�֐�

			/**
			 * @brief ������
			 * @param[in] player �v���C���[�̎Q��
			*/
			void Init(const CPlayer& player);

			/**
			 * @brief �v���C���[���f���N���X�̃A�b�v�f�[�g�����s����
			*/
			void ExecuteUpdate();

			/**
			 * @brief �X�C���O���̃A�j���[�V�����X�e�[�g��ݒ�
			 * @param[in] state �X�e�[�g
			*/
			void SetSwingAnimState(const nsPlayerConstData::nsModelAnimationConstData::EnSwingAnimState state)
			{
				m_swingAnimSate = state;
			}



		private:	// private�ȃ����o�֐�

			/**
			 * @brief �A�j���[�V����������
			*/
			void InitAnimation();

			/**
			 * @brief ���f��������
			*/
			void InitModel();

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

			/**
			 * @brief �A�j���[�V�����X�e�[�g���X�V
			*/
			void UpdateAnimationState();

			/**
			 * @brief �X�C���O���̃A�j���[�V�����X�e�[�g���X�V
			*/
			void UpdateSwingAnimationState();



		private:	// �f�[�^�����o
			const CPlayer* m_playerRef = nullptr;						//!< �v���C���[�̎Q��
			nsGraphic::nsModel::CModelRender* m_playerModel = nullptr;	//!< �v���C���[�̃��f�������_���[

			//!< �A�j���[�V�����N���b�v�X�N���X
			AnimationClip m_animationClips[nsPlayerConstData::nsModelAnimationConstData::enAnim_num];
			//!< �A�j���[�V�����X�e�[�g
			nsPlayerConstData::nsModelAnimationConstData::EnAnimationClips m_animState =
				nsPlayerConstData::nsModelAnimationConstData::enAnim_idle;
			//!< �A�j���[�V�����⊮����
			float m_AnimInterpolateTime = 
				nsPlayerConstData::nsModelAnimationConstData::kDefaultAnimInterpolateTime;

			bool m_walkFlag = false;
			bool m_runFlag = false;
			bool m_jumpFlag = false;
			bool m_swingFlag = false;

			//!< �X�C���O���̃A�j���[�V�����X�e�[�g
			nsPlayerConstData::nsModelAnimationConstData::EnSwingAnimState m_swingAnimSate =
				nsPlayerConstData::nsModelAnimationConstData::enSwingAnim_swing;
		};

	}
}
