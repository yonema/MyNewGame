#pragma once
#include "ModelRenderConstData.h"
#include "ModelRenderData.h"
#include "Render.h"
#include "LightConstData.h"
#include "ShadowConstDatah.h"
#include "GeometryData.h"

namespace nsNinjaAttract
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
				using ModelPtr = std::unique_ptr<Model>;		// ���f���N���X�̃��j�[�N�|�C���^
				using SkeletonPtr = std::unique_ptr<Skeleton>;	// �X�P���g���N���X�̃��j�[�N�|�C���^
				using AnimPtr = std::unique_ptr<Animation>;		// �A�j���[�V�����N���X�Ƀ��j�[�N�|�C���^
				// �W�I���g���f�[�^�̃��j�[�N�|�C���^
				using GeometryDataPtr = std::unique_ptr <nsGeometry::CGeometryData>;

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
				 * @param[in] animationClips �A�j���[�V�����N���b�v
				 * @param[in] numAnimationClip �A�j���[�V�����N���b�v�̐�
				 * @param[in] �C���X�^���X�̍ő吔
				 * @param[in] modelUpAxis ���f����UP��
				 * @param[in] shadowCullFront �e���f�����t�����g�J�����O�ɂ��邩�H
				*/
				void Init(
					const char* filePath,
					AnimationClip* animationClips = nullptr,
					const int numAnimationClip = 0,
					const int maxInstance = 1,
					const EnModelUpAxis modelUpAxis = EnModelUpAxis::enModelUpAxisZ,
					const bool shadowCullFront = false
					);

				/**
				 * @brief �������`��p�̏������֐�
				 * @param[in] filePath ���f���̃t�@�C���p�X
				 * @param[in] animationClips �A�j���[�V�����N���b�v
				 * @param[in] numAnimationClip �A�j���[�V�����N���b�v�̐�
				 * @param[in] �C���X�^���X�̍ő吔
				 * @param[in] modelUpAxis ���f����UP��
				*/
				void IniTranslucent(
					const char* filePath,
					AnimationClip* animationClips = nullptr,
					const int numAnimationClip = 0,
					const int maxInstance = 1,
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ
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
				void SetRotation(const Quaternion& rot)
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
				 * @brief �A�j���[�V�������Đ������H�𓾂�
				 * @return �A�j���[�V�������Đ������H
				*/
				bool IsAnimationPlaying() const
				{
					return m_animationPtr->IsPlaying();
				}

				/**
				 * @brief �A�j���[�V�����̎c�莞�ԁi�䗦�j�𓾂�
				 * @return �A�j���[�V�����̎c�莞�ԁi�䗦�j
				*/
				float GetAnimationTimeRemaining() const
				{
					return m_animationPtr->GetTimeRemaining();
				}

				/**
				 * @brief �A�j���[�V�����̃X�s�[�h��ݒ�
				 * @param[in] animSpeed �A�j���[�V�����̃X�s�[�h
				*/
				void SetAnimationSpeed(const float animSpeed)
				{
					m_animationSpeed = animSpeed;
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

				/**
				 * @brief �V���h�E���V�[�o�[���H��ݒ�Btrue�ŉe���f��Afalse�ŉe���f��Ȃ��B
				 * @param[in] isShadowReciever �V���h�E���V�[�o�[���H
				*/
				void SetIsShadowReciever(const bool isShadowReciever)
				{
					m_modelExCB.isShadowReciever = isShadowReciever;
				}

				/**
				 * @brief �v���C���[��p�̃V���h�E�L���X�^�[��ݒ�
				 * @param[in] isPlayerShaodwCaster �v���C���[��p�̃V���h�E�L���X�^�[���H
				*/
				void SetIsPlayerShadowCaster(bool isPlayerShaodwCaster);

				/**
				 * @brief �V���h�E�L���X�^�[���H���擾
				 * @return �V���h�E�L���X�^�[���H
				*/
				bool IsShadowCaster() const
				{
					return m_isShadowCaster;
				}

				/**
				 * @brief �C���X�^���V���O�`����s�����H���擾
				 * @return �C���X�^���V���O�`����s�����H
				*/
				bool IsInstancingDraw() const
				{
					return m_isEnableInstancingDraw;
				}

				/**
				 * @brief ���f���̎Q�Ƃ𓾂�
				 * @return ���f���̎Q��
				*/
				Model& GetModel() const
				{
					return *m_model.get();
				}

				/**
				 * @brief �C���X�^���XID���w�肵�āA���[���h�s��𓾂�
				 * @param instanceId �C���X�^���XID
				 * @return ���[���h�s��
				*/
				const Matrix& GetWorldMatrix(int instanceId) const
				{
					if (IsInstancingDraw()) {
						return m_worldMatrixArray[instanceId];
					}
					return m_model->GetWorldMatrix();
				}

				/**
				 * @brief �C���X�^���V���O�f�[�^�̍X�V
				 * @param[in] pos ���W
				 * @param[in] rot ��]
				 * @param[in] scale �g�嗦
				*/
				void UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

				/**
				 * @brief LOD�p�̃��f���̏�����
				 * @param[in] filePath LOD�p�̃��f���̃t�@�C���p�X
				*/
				void InitLODModel(const char* filePath);

				/**
				 * @brief LOD�͗L�����H��ݒ肷��
				 * @param[in] isEnableLOD LOD�͗L�����H
				*/
				void SetIsEnableLOD(const bool isEnableLOD)
				{
					m_isEnableLOD = isEnableLOD;
				}

				/**
				 * @brief LOD�̋�����ݒ�
				 * @param[in] distanceLOD LOD�̋���
				*/
				void SetDistanceLOD(float distanceLOD)
				{
					m_distanceLOD = distanceLOD;
				}

				/**
				 * @brief �{�[���̖��O����{�[���ԍ�������
				 * @param[in] boneName �{�[���̖��O
				 * @return �{�[���ԍ��B������Ȃ������ꍇ��-1���Ԃ��Ă��܂��B
				*/
				int FindBoneID(const wchar_t* boneName) const
				{
					if (m_skeletonPtr)
					{
						return m_skeletonPtr->FindBoneID(boneName);
					}
					return -1;
				}

				/**
				 * @brief �{�[���ԍ�����{�[�����擾�B
				 * @param[in] boneNo �{�[���ԍ�
				 * @return �{�[��
				*/
				const Bone* GetBone(int boneNo) const
				{
					if (m_skeletonPtr)
					{
						return m_skeletonPtr->GetBone(boneNo);
					}
					return nullptr;
				}


			private:	// private�ȃ����o�֐�

				/**
				 * @brief �����������̃��C���R�A
				 * @param[in] animationClips �A�j���[�V�����N���b�v
				 * @param[in] numAnimationClips �A�j���[�V�����N���b�v�̐�
				 * @param[in] maxInstance �C���X�^���X��
				 * @param[in] isDefferdRender �f�B�t�@�[�h�����_�����O�ŕ`�悷�邩�H
				*/
				void InitMainCore(
					AnimationClip* animationClips,
					const int numAnimationClips,
					const int maxInstance,
					const bool isDefferdRender
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
				 * @brief �W�I���g������������
				 * @param maxInstance �C���X�^���X��
				*/
				void InitGeometryDatas(const int maxInstance);

				/**
				 * @brief ���f���̏������f�[�^�̋��ʕ����̐ݒ�
				 * @param[in] tkmFilePath ���f����tkm�t�@�C���p�X
				 * @param[in] maxInstance �ő�C���X�^���X��
				 * @param[in] fxFilePath �V�F�[�_�[��fx�t�@�C���p�X
				 * @param[in] modelUpAxis ���f����UP��
				*/
				void SetCommonModelInitData(
					const char* tkmFilePath,
					const int maxInstance,
					const EnModelUpAxis modelUpAxis = enModelUpAxisZ,
					const char* fxFilePath = nsModelConstData::kDefaultFxFilePath
				);

				/**
				 * @brief GBuffer�ɕ`�悷�郂�f���̒萔�o�b�t�@���Z�b�g
				*/
				void SetRenderToGBufferModelCB();

				/**
				 * @brief �������`��p���f���̒萔�o�b�t�@���Z�b�g
				*/
				void SetTranslucentModelCB();

				/**
				 * @brief GBuffer�ɕ`�悷�郂�f���̃V�F�[�_�[���\�[�X�r���[���Z�b�g
				*/
				void SetRenderToGBufferShaderResourceView();

				/**
				 * @brief �������`��p���f���̃V�F�[�_�[���\�[�X�r���[���Z�b�g
				*/
				void SetTranslucentModelShaderResourceView();

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
				 * @brief �C���X�^���V���O�`��p�̏���������
				 * @param[in] maxInstance �ő�C���X�^���X��
				*/
				void InitInstancingDraw(int maxInstance);

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

				/**
				 * @brief �v���C���[��p�̃V���h�E�}�b�v�ɕ`�悷�郂�f����`�悷��
				 * @param[in] rc �����_�����O�R���e�L�X�g
				 * @param[in] ligNo ���C�g�̔ԍ�
				 * @param[in] lvpMatrix ���C�g�r���[�v���W�F�N�V�����s��
				*/
				void PlayerShadowModelRender(RenderContext& rc,const int ligNo, const Matrix& lvpMatrix);

				/**
				 * @brief �C���X�^���V���O�`�掞�̕`��֐�
				 * @param[in] rc �����_�����O�R���e�L�X�g
				*/
				void InstancingModelRender(RenderContext& rc);

			private:	// �f�[�^�����o
				ModelPtr m_model;							//!< ���f���N���X
				//@todo�f�B���N�V�������C�g�̐������V���h�E���f�����������d�������̂ŁA�C���X�^���X�`�������܂ł̓��C�g1���̂�
				//!< �V���h�E�}�b�v�`��p���f���N���X
				ModelPtr m_shadowModels[1/*nsLight::nsLightConstData::kMaxDirectionalLightNum*/][nsShadow::nsShadowConstData::enShadowMapArea_num];
				ModelPtr m_lodModel;						//!< LOD�p�̃��f��
				//!< LOD�p�̃V���h�E���f��
				ModelPtr m_lodShadowModel[1/*nsLight::nsLightConstData::kMaxDirectionalLightNum*/][nsShadow::nsShadowConstData::enShadowMapArea_num];
				ModelInitData m_modelInitData;				//!< ���f���̏������f�[�^

				CRender m_render;							//!< �����_���[�N���X
				SkeletonPtr m_skeletonPtr;					//!< �X�P���g���N���X
				AnimPtr m_animationPtr;						//!< �A�j���[�V�����N���X
				float m_animationSpeed = 1.0f;				//!< �A�j���[�V�����̃X�s�[�h
				nsModelData::SExpandConstantBuffer m_modelExCB;	//!< ���f���̊g���萔�o�b�t�@

				Vector3 m_position = Vector3::Zero;				//!< ���W
				Quaternion m_rotation = Quaternion::Identity;	//!< ��]
				Vector3 m_scale = Vector3::One;					//!< �g�嗦

				int	m_numInstance = 0;						//!< �C���X�^���X�̐��B
				int	m_maxInstance = 1;						//!< �ő�C���X�^���X���B
				int	m_fixNumInstanceOnFrame = 0;			//!< ���̃t���[���ɕ`�悷��C���X�^���X�̐��̊m�萔�B
				int	m_fixNumInstanceOnFrameLOD = 0;			//!< LOD�p�̂��̃t���[���ɕ`�悷��C���X�^���X�̐��̊m�萔�B
				int	m_shadowFixNumInstanceOnFrame = 0;		//!< �e�p�̂��̃t���[���ɕ`�悷��C���X�^���X�̐��̊m�萔�B
				int	m_shadowFixNumInstanceOnFrameLOD = 0;	//!< �e�p��LOD�p�̂��̃t���[���ɕ`�悷��C���X�^���X�̐��̊m�萔�B
				std::unique_ptr<Matrix[]> m_worldMatrixArray;	//!< ���[���h�s��̔z��B
				std::unique_ptr<Matrix[]> m_worldMatrixArrayBuffer;	//!< �J�����O��̃��[���h�s��̔z��B
				std::unique_ptr<Matrix[]> m_worldMatrixArrayBufferLOD;	//!< LOD�p�̃J�����O��̃��[���h�s��̔z��B
				std::unique_ptr<Matrix[]> m_shadowWorldMatrixArrayBuffer;	//!< �e���f���p�J�����O��̃��[���h�s��̔z��B
				std::unique_ptr<Matrix[]> m_shadowWorldMatrixArrayBufferLOD;	//!< �e���f���pLOD�p�̃J�����O��̃��[���h�s��̔z��B
				StructuredBuffer m_worldMatrixArraySB;		//!< ���[���h�s��̔z��̃X�g���N�`���[�h�o�b�t�@�B
				StructuredBuffer m_worldMatrixArraySBLOD;	//!< LOD�p�́[���h�s��̔z��̃X�g���N�`���[�h�o�b�t�@�B
				StructuredBuffer m_shadowWorldMatrixArraySB;	//!< �e���f���p���[���h�s��̔z��̃X�g���N�`���[�h�o�b�t�@�B
				StructuredBuffer m_shadowWorldMatrixArraySBLOD;	//!< �e���f���pLOD�p�́[���h�s��̔z��̃X�g���N�`���[�h�o�b�t�@�B

				std::vector<GeometryDataPtr> m_geometryDatas;	//!< �W�I���g�����̃R���e�i
				float m_distanceLOD = nsModelConstData::kDefaultDistanceLOD;	//!< LOD�̋���

				bool m_isEnableInstancingDraw = false;		//!< �C���X�^���V���O�`�悪�L�����H
				bool m_isInited = false;					//!< �������ς݂��H
				bool m_isShadowCaster = false;				//!< �V���h�E�L���X�^�[���H
				bool m_isEnableLOD = false;					//!< LOD�͗L�����H
				bool m_isShadowCullFront = false;			//!< �e���f�����t�����g�J�����O�ɂ��邩�H
			};

		}
	}
}