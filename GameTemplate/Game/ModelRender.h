#pragma once
#include "ModelRenderConstData.h"
#include "Render.h"
#include "LightConstData.h"
#include "ShadowConstDatah.h"

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
				 * @brief �������`��p�̏������֐�
				 * @param[in] filePath ���f���̃t�@�C���p�X
				 * @param[in] modelUpAxis ���f����UP��
				 * @param[in] animationClips �A�j���[�V�����N���b�v
				 * @param[in] numAnimationClip �A�j���[�V�����N���b�v�̐�
				*/
				void IniTranslucent(
					const char* filePath,
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ,
					AnimationClip* animationClips = nullptr,
					int numAnimationClip = 0
				);

				/**
				 * @brief �t�H���[�h�����_�����O�p�̏������֐��B����ȃV�F�[�f�B���O���s�������Ƃ��p�B
				 * @param[in] modelInitData ���f���̏������f�[�^
				 * @param[in] animationClips �A�j���[�V�����N���b�v
				 * @param[in] numAnimationClip �A�j���[�V�����N���b�v�̐�
				*/
				void InitForwardRendering(
					ModelInitData& modelInitData,
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

				/**
				 * @brief �A�j���[�V�������Đ�����
				 * @param[in] animNo �A�j���[�V�����N���b�v�̔ԍ�
				 * @param[in] interpolateTime �⊮���ԁi�P�ʁF�b�j
				*/
				void PlayAnimation(
					const int animNo,
					const float interpolateTime = nsModelConstData::kAnimationInterpolateTime
				)
				{
					m_animationPtr->Play(animNo, interpolateTime);
					return;
				}

				/**
				 * @brief ���Ȕ����J���[��ݒ�
				 * @param[in] emmisonColor ���Ȕ����J���[
				*/
				void SetEmmisonColor(const Vector4& emmisonColor)
				{
					m_model->SetEmmisonColor(emmisonColor);
				}

				/**
				 * @brief ��Z�J���[��ݒ�
				 * @param[in] mulColor ��Z�J���[
				*/
				void SetMulColor(const Vector4& mulColor)
				{
					m_model->SetMulColor(mulColor);
				}

				/**
				 * @brief ���f���̃A���t�@�l��ݒ�
				 * @param[in] alphaValue �A���t�@�l
				*/
				void SetAlphaValue(const float alphaValue)
				{
					m_model->SetAlphaValue(alphaValue);
				}

				/**
				 * @brief �V���h�E�L���X�^�[���H��ݒ�Btrue�ŉe�𐶐�����Afalse�ŉe�𐶐����Ȃ��B
				 * @param[in] isShadowCaster �V���h�E�L���X�^�[���H
				*/
				void SetIsShadowCaster(const bool isShadowCaster);


			private:	// private�ȃ����o�֐�

				/**
				 * @brief �����������̃��C���R�A
				 * @param[in] animationClips �A�j���[�V�����N���b�v
				 * @param[in] numAnimationClips �A�j���[�V�����N���b�v�̐�
				 * @param[in] isDefferdRender �f�B�t�@�[�h�����_�����O�ŕ`�悷�邩�H
				*/
				void InitMainCore(
					AnimationClip* animationClips,
					const int numAnimationClips,
					const bool isDefferdRender = true
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
				 * @param[in] tkmFilePath ���f����tkm�t�@�C���p�X
				 * @param[in] fxFilePath �V�F�[�_�[��fx�t�@�C���p�X
				 * @param[in] modelUpAxis ���f����UP��
				 * @param[in] vsEntryPointFunc ���_�V�F�[�_�[�̃G���g���[�|�C���g
				 * @param[in] vsSkinEntryPointFunc �X�L������̒��_�V�F�[�_�[�̃G���g���[�|�C���g
				 * @param[in] psEntryPointFunc �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
				*/
				void SetCommonModelInitData(
					const char* tkmFilePath,
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ,
					const char* fxFilePath = nsModelConstData::kDefaultFxFilePath,
					const char* vsEntryPointFunc = nsModelConstData::kVsEntryPointFunc,
					const char* vsSkinEntryPointFunc = nsModelConstData::kVsSkinEntryPointFunc,
					const char* psEntryPointFunc = nsModelConstData::kPsEntryPointFunc
				);

				/**
				 * @brief �f�t�H���g�̒萔�o�b�t�@���Z�b�g
				*/
				void SetDefaultConstantBuffer();

				/**
				 * @brief �����_���[������������
				 * @param[in] isDefferdRender �f�B�t�@�[�h�����_�����O�ŕ`�悷�邩�H
				*/
				void InitRender(const bool isDefferdRender);

				/**
				 * @brief �V���h�E�}�b�v�ɕ`�悷�郂�f���̏�����
				*/
				void InitShadowModel();

				/**
				 * @brief ���f����`�悷��
				 * @param[in] rc �����_�����O�R���e�L�X�g
				*/
				void ModelRender(RenderContext& rc);

				/**
				 * @brief �V���h�E�}�b�v�ɕ`�悷�郂�f����`�悷��
				 * @param[in] rc �����_�����O�R���e�L�X�g
				 * @param[in] ligNo ���C�g�ԍ�
				 * @param[in] shadowMapNo �V���h�E�}�b�v�̔ԍ�
				 * @param[in] lvpMatrix ���C�g�r���[�v���W�F�N�V�����s��
				*/
				void ShadowModelRender(RenderContext& rc,const int ligNo, const int shadowMapNo, const Matrix& lvpMatrix);

			private:	// �f�[�^�����o
				ModelPtr m_model;							//!< ���f���N���X
				//!< �V���h�E�}�b�v�`��p���f���N���X
				ModelPtr m_shadowModels[nsLight::nsLightConstData::kMaxDirectionalLightNum][nsShadow::nsShadowConstData::enShadowMapArea_num];
				ModelInitData m_modelInitData;				//!< ���f���̏������f�[�^
				CRender m_render;							//!< �����_���[�N���X
				SkeletonPtr m_skeletonPtr;					//!< �X�P���g���N���X
				AnimPtr m_animationPtr;						//!< �A�j���[�V�����N���X

				Vector3 m_position = Vector3::Zero;				//!< ���W
				Quaternion m_rotation = Quaternion::Identity;	//!< ��]
				Vector3 m_scale = Vector3::One;					//!< �g�嗦

				bool m_isInited = false;					//!< �������ς݂��H


			};

		}
	}
}