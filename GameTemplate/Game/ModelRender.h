#pragma once
#include "ModelRenderConstData.h"

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief ���f���֘A�̃l�[���X�y�[�X
		*/
		namespace nsModel
		{
			/**
			 * @brief ���f���`��N���X
			*/
			class CModelRender : public IGameObject
			{
			public:		//�R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CModelRender() = default;

				/**
				 * @brief �f�X�g���N�^
				*/
				~CModelRender() = default;

			public:		//�I�[�o�[���C�h���������o�֐�
				/**
				 * @brief �X�^�[�g�֐�
				 * @return �A�b�v�f�[�g���s�����H
				*/
				bool Start() override final { return true; };

				/**
				 * @brief �j�����ꂽ�Ƃ��̏���
				*/
				void OnDestroy() override final;

				/**
				 * @brief ��ɌĂ΂��A�b�v�f�[�g�֐�
				*/
				void AlwaysUpdate() override final;

				/**
				 * @brief �`�揈���֐�
				 * @param[in] rc �����_�[�R���e�L�X�g
				*/
				void Render(RenderContext& rc) override final;

			public:		//�����o�֐�

				/**
				 * @brief �������֐�
				 * @param[in] filePath ���f���̃t�@�C���p�X
				 * @param[in] modelUpAxis ���f����UP��
				 * @param[in] animationClips �A�j���[�V�����N���b�v
				 * @param[in] numAnimationClip �A�j���[�V�����N���b�v�̐�
				*/
				void Init(
					const char* filePath,
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ,
					AnimationClip* animationClips = nullptr,
					int numAnimationClip = 0
					);


			private:	//private�ȃ����o�֐�

				/**
				 * @brief �����������̃��C���R�A
				 * @param[in] modelInitData ���f���̏������f�[�^
				 * @param[in] animationClips �A�j���[�V�����N���b�v
				 * @param[in] numAnimationClips �A�j���[�V�����N���b�v�̐�
				*/
				void InitMainCore(
					ModelInitData& modelInitData,
					AnimationClip* animationClips,
					const int numAnimationClips
				);

				/**
				 * @brief �X�P���g���̏�����
				*/
				void InitSkelton();

				/**
				 * @brief �A�j���[�V�����̏�����
				 * @param[in] animationClips �A�j���[�V�����N���b�v
				 * @param[in] numAnimationClips �A�j���[�V�����N���b�v�̐�
				*/
				void InitAnimation(AnimationClip* animationClips, const int numAnimationClips);

				/**
				 * @brief ���f���̏������f�[�^�̋��ʕ����̐ݒ�
				 * @param[out] modelInitData ���f���̏������f�[�^
				 * @param[in] tkmFilePath ���f����tkm�t�@�C���p�X
				 * @param[in] fxFilePath �V�F�[�_�[��fx�t�@�C���p�X
				 * @param[in] modelUpAxis ���f����UP��
				 * @param[in] vsEntryPointFunc ���_�V�F�[�_�[�̃G���g���[�|�C���g
				 * @param[in] vsSkinEntryPointFunc �X�L������̒��_�V�F�[�_�[�̃G���g���[�|�C���g
				 * @param[in] psEntryPointFunc �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
				*/
				void SetCommonModelInitData(
					ModelInitData* modelInitData,
					const char* tkmFilePath,
					const char* fxFilePath = nsModelConstData::kDefaultFxFilePath,
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ,
					const char* vsEntryPointFunc = nsModelConstData::kVsEntryPointFunc,
					const char* vsSkinEntryPointFunc = nsModelConstData::kVsSkinEntryPointFunc,
					const char* psEntryPointFunc = nsModelConstData::kPsEntryPointFunc
				);

			private:	//�f�[�^�����o
				Model m_model;								//!< ���f���N���X
				const char* m_tkmFilePath = nullptr;		//!< tkm�t�@�C���̃t�@�C���p�X
				std::unique_ptr<Skeleton> m_skeletonPtr;	//!< �X�P���g���N���X
				std::unique_ptr<Animation> m_animationPtr;	//!< �A�j���[�V�����N���X

				Vector3 m_position = Vector3::Zero;				//!< ���W
				Quaternion m_rotation = Quaternion::Identity;	//!< ��]
				Vector3 m_scale = Vector3::One;					//!< �g�嗦

				bool m_isInited = false;					//!< �������ς݂��H


			};

		}
	}
}