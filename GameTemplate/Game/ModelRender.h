#pragma once
#include "ModelRenderConstData.h"
#include "Render.h"

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
			private:	// �G�C���A�X�錾
				using ModelPtr = std::unique_ptr<Model>;		//!< ���f���N���X�̃��j�[�N�|�C���^
				using SkeletonPtr = std::unique_ptr<Skeleton>;	//!< �X�P���g���N���X�̃��j�[�N�|�C���^
				using AnimPtr = std::unique_ptr<Animation>;		//!< �A�j���[�V�����N���X�Ƀ��j�[�N�|�C���^

			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CModelRender() = default;

				/**
				 * @brief �f�X�g���N�^
				*/
				~CModelRender() = default;

			public:		// �I�[�o�[���C�h���������o�֐�
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
				 * @brief �`��I�u�W�F�N�g�o�^�̓���
				*/
				void AddRenderEntrance() override final;

			public:		// �����o�֐�

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

				/**
				 * @brief ���W��ݒ肷��
				 * @param[in] position ���W
				*/
				void SetPosition(const Vector3& pos)
				{
					m_position = pos;
				}

				/**
				 * @brief ���W�𓾂�
				 * @return[in] ���W
				*/
				const Vector3& GetPosition() const
				{
					return m_position;
				}

				/**
				 * @brief ��]��ݒ肷��
				 * @param[in] rot ��]�𓾂�
				*/
				void SetRotatioin(const Quaternion& rot)
				{
					m_rotation = rot;
				}

				/**
				 * @brief ��]�𓾂�
				 * @return ��]
				*/
				const Quaternion& GetRotation() const
				{
					return m_rotation;
				}

				/**
				 * @brief �g�嗦��ݒ肷��
				 * @param[in] scale �g�嗦
				*/
				void SetScale(const Vector3 scale)
				{
					m_scale = scale;
				}

				/**
				 * @brief �g�嗦�𓾂�
				 * @return �g�嗦
				*/
				const Vector3& GetScale() const
				{
					return m_scale;
				}

				void PlayAnimation(
					int animNo,
					float interpolateTime = nsModelConstData::kAnimationInterpolateTime
				)
				{
					m_animationPtr->Play(animNo, interpolateTime);
					return;
				}


			private:	// private�ȃ����o�֐�

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
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ,
					const char* fxFilePath = nsModelConstData::kDefaultFxFilePath,
					const char* vsEntryPointFunc = nsModelConstData::kVsEntryPointFunc,
					const char* vsSkinEntryPointFunc = nsModelConstData::kVsSkinEntryPointFunc,
					const char* psEntryPointFunc = nsModelConstData::kPsEntryPointFunc
				);

				/**
				 * @brief �f�t�H���g�̒萔�o�b�t�@���Z�b�g
				 * @param modelInitData ���f���̏������f�[�^
				*/
				void SetDefaultConstantBuffer(ModelInitData* modelInitData);

				/**
				 * @brief �����_���[������������
				*/
				void InitRender();

				/**
				 * @brief GBuffer�ɏ������ފ֐������s
				 * @param rc 
				*/
				void OnRenderToGBuffer(RenderContext& rc);

			private:	// �f�[�^�����o
				ModelPtr m_model;							//!< ���f���N���X
				CRender m_render;							//!< �����_���[�N���X
				SkeletonPtr m_skeletonPtr;					//!< �X�P���g���N���X
				AnimPtr m_animationPtr;						//!< �A�j���[�V�����N���X
				const char* m_tkmFilePath = nullptr;		//!< tkm�t�@�C���̃t�@�C���p�X

				Vector3 m_position = Vector3::Zero;				//!< ���W
				Quaternion m_rotation = Quaternion::Identity;	//!< ��]
				Vector3 m_scale = Vector3::One;					//!< �g�嗦

				bool m_isInited = false;					//!< �������ς݂��H


			};

		}
	}
}