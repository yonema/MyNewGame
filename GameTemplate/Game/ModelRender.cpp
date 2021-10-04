#include "stdafx.h"
#include "ModelRender.h"
#include "GameTime.h"
#include "LightManager.h"
#include "RenderingEngine.h"
#include "RenderingEngineConstData.h"

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
			// ���f���̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsModelConstData;

			/**
			 * @brief �j�����ꂽ�Ƃ��̏���
			*/
			void CModelRender::OnDestroy()
			{

				return;
			}

			/**
			 * @brief ��ɌĂ΂��A�b�v�f�[�g�֐�
			*/
			void CModelRender::AlwaysUpdate()
			{
				// �������ς݂��H
				if (m_isInited != true)
					return;	// ���������Ă��Ȃ��A�������^�[��

				// �X�P���g��������������Ă��邩�H
				if (m_skeletonPtr)
					// �X�P���g��������������Ă�����A�X�P���g�����X�V�B
					m_skeletonPtr->Update(m_model->GetWorldMatrix());
				// �A�j���[�V����������������Ă��邩�H
				if (m_animationPtr)	// �A�j���[�V����������������Ă�����A�A�j���[�V������i�߂�B
					m_animationPtr->Progress(nsTimer::GameTime().GetFrameDeltaTime());

				// �V���h�E�}�b�v�`��p���f��������������Ă��邩�H
				if (m_shadowModels[0][0])
				{
					// ����������Ă����烂�f���̍��W�X�V
					for (auto& shadowModels : m_shadowModels)
					{
						for (auto& shadowModel : shadowModels)
						{
							shadowModel->UpdateWorldMatrix(m_position, m_rotation, m_scale);
						}
					}
				}

				// ���f���̍��W�X�V
				m_model->UpdateWorldMatrix(m_position, m_rotation, m_scale);

				return;
			}

			/**
			 * @brief �`��I�u�W�F�N�g�o�^�̓���
			*/
			void CModelRender::AddRenderEntrance()
			{
				// �������ς݂��H
				if (m_isInited != true)
					return;	// ���������Ă��Ȃ��A�������^�[��

				// �`��I�u�W�F�N�g�ɓo�^����
				nsMyEngine::CRenderingEngine::GetInstance()->AddRenderObject(&m_render);

				return;
			}


			/**
			 * @brief �������֐�
			 * @param filePath ���f���̃t�@�C���p�X
			 * @param animationClips �A�j���[�V�����N���b�v
			 * @param numAnimationClip �A�j���[�V�����N���b�v�̐�
			 * @param modelUpAxis ���f����UP��
			*/
			void CModelRender::Init(
				const char* filePath,
				AnimationClip* animationClips,
				const int numAnimationClip,
				const EnModelUpAxis modelUpAxis
			)
			{
				// ���f���̏������f�[�^�̋��ʕ����̐ݒ�
				SetCommonModelInitData(filePath, modelUpAxis);

				// GBuffer�̃J���[�t�H�[�}�b�g�ɍ��킹��
				for (int i = 0; i < nsMyEngine::nsRenderingEngineConstData::enGBufferNum; i++)
				{
					m_modelInitData.m_colorBufferFormat[i] = 
						nsMyEngine::CRenderingEngine::GetInstance()->GetGBufferColorFormat(
						static_cast<nsMyEngine::nsRenderingEngineConstData::EnGBuffer>(i)
					);
				}

				// GBuffer�ɕ`�悷�郂�f���̒萔�o�b�t�@���Z�b�g
				SetRenderToGBufferModelCB();

				// �����������̃��C���R�A
				InitMainCore(animationClips, numAnimationClip);

				return;
			}
			/**
			 * @brief �������`��p�̏������֐�
			 * @param[in] filePath ���f���̃t�@�C���p�X
			 * @param[in] animationClips �A�j���[�V�����N���b�v
			 * @param[in] numAnimationClip �A�j���[�V�����N���b�v�̐�
			 * @param[in] modelUpAxis ���f����UP��
			*/
			void CModelRender::IniTranslucent(
				const char* filePath,
				AnimationClip* animationClips,
				const int numAnimationClip,
				const EnModelUpAxis modelUpAxis
			)
			{

				// ���f���̏������f�[�^�̋��ʕ����̐ݒ�
				// �������`��p�̃V�F�[�_�[���w�肷��
				SetCommonModelInitData(filePath, modelUpAxis, kTranslucentModelFxFilePath);

				// ���C�������_�����O�^�[�Q�b�g�̂���t�H�[�}�b�g�ɍ��킹��
				m_modelInitData.m_colorBufferFormat[0] =
					nsMyEngine::CRenderingEngine::GetInstance()->GetMainRenderTargetFormat();

				// �������`��p���f���̒萔�o�b�t�@���Z�b�g
				SetTranslucentModelCB();

				// �f�t�H���g�̃V�F�[�_�[���\�[�X�r���[���Z�b�g
				SetDefaultShaderResourceView();

				// �����������̃��C���R�A
				// �f�B�t�@�\�h�ł͂Ȃ��A�t�H���[�h�����_�����O�ŕ`�悷��悤�Ɏw�肷��
				InitMainCore(animationClips, numAnimationClip, false);

				return;
			}

			/**
			 * @brief �t�H���[�h�����_�����O�p�̏������֐��B����ȃV�F�[�f�B���O���s�������Ƃ��p�B
			 * @param[in] modelInitData ���f���̏������f�[�^
			 * @param[in] animationClips �A�j���[�V�����N���b�v
			 * @param[in] numAnimationClip �A�j���[�V�����N���b�v�̐�
			*/
			void CModelRender::InitForwardRendering(
				ModelInitData& modelInitData,
				AnimationClip* animationClips,
				int numAnimationClip
			)
			{
				// ���f���̏������f�[�^��ێ�
				m_modelInitData = modelInitData;

				// �����������̃��C���R�A
				// �f�B�t�@�\�h�ł͂Ȃ��A�t�H���[�h�����_�����O�ŕ`�悷��悤�Ɏw�肷��
				InitMainCore(animationClips, numAnimationClip, false);

				return;
			}

			/**
			 * @brief �����������̃��C���R�A
			 * @param[in] animationClips �A�j���[�V�����N���b�v
			 * @param[in] numAnimationClips �A�j���[�V�����N���b�v�̐�
			 * @param[in] isDefferdRender �f�B�t�@�[�h�����_�����O�ŕ`�悷�邩�H
			*/
			void CModelRender::InitMainCore(
				AnimationClip* animationClips,
				const int numAnimationClips,
				const bool isDefferdRender
			)
			{
				// �X�P���g���̏�����
				InitSkelton();
				// �X�P���g�������݂��邩�H
				if (m_skeletonPtr)
					// ���݂���B�X�P���g���̐ݒ�
					m_modelInitData.m_skeleton = m_skeletonPtr.get();

				// ���f���̏�����
				m_model.reset(new Model);
				m_model->Init(m_modelInitData);

				// �A�j���[�V�����̏�����
				InitAnimation(animationClips, numAnimationClips);

				// �����_���[�̏�����
				InitRender(isDefferdRender);

				// ����������
				m_isInited = true;

				return;
			}


			/**
			 * @brief �X�P���g���̏�����
			*/
			void CModelRender::InitSkelton()
			{
				// tkm�t�@�C����tks�t�@�C���ɕϊ�����
				std::string skeletonFilePath = m_modelInitData.m_tkmFilePath;
				// �t�@�C���p�X�̕����񂩂�A�g���q�̏ꏊ��T��
				int pos = (int)skeletonFilePath.find(kTkmFileExtensinon);
				// tkm�̃t�@�C���p�X����tks�̃t�@�C���p�X�ɓ���ւ���
				skeletonFilePath.replace(pos, kFileExtensionLength, kTksFileExtensinon);

				// �X�P���g���̃��\�[�X�̊m��
				m_skeletonPtr.reset(new Skeleton);
				// �X�P���g���̃f�[�^��ǂݍ��݁B
				bool isInited = m_skeletonPtr->Init(skeletonFilePath.c_str());

				// �������Ɏ��s�������H
				if (isInited != true)
				{
					// ���s������X�P���g���̃��\�[�X���J������
					m_skeletonPtr.reset();
				}

				return;
			}

			/**
			 * @brief �A�j���[�V�����̏�����
			 * @param animationClips �A�j���[�V�����N���b�v
			 * @param numAnimationClips �A�j���[�V�����N���b�v�̐�
			*/
			void CModelRender::InitAnimation(AnimationClip* animationClips, const int numAnimationClips)
			{
				// �A�j���[�V�����N���b�v���o�^����Ă��邩
				if (animationClips == nullptr)
					return;	// ����Ă��Ȃ��ꍇ�͉������Ȃ�

				// �A�j���[�V�����̃��\�[�X�̊m��
				m_animationPtr.reset(new Animation);
				// �A�j���[�V�������������B
				m_animationPtr->Init(
					*m_skeletonPtr,		// �A�j���[�V�����𗬂����ރX�P���g���B
					animationClips,		// �A�j���[�V�����N���b�v�B
					numAnimationClips	// �A�j���[�V�����̐��B
				);

				return;
			}

			/**
			 * @brief ���f���̏������f�[�^�̋��ʕ����̐ݒ�
			 * @param[in] tkmFilePath ���f����tkm�t�@�C���p�X
			 * @param[in] fxFilePath �V�F�[�_�[��fx�t�@�C���p�X
			 * @param[in] modelUpAxis ���f����UP��
			 * @param[in] vsEntryPointFunc ���_�V�F�[�_�[�̃G���g���[�|�C���g
			 * @param[in] vsSkinEntryPointFunc �X�L������̒��_�V�F�[�_�[�̃G���g���[�|�C���g
			 * @param[in] psEntryPointFunc �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
			*/
			void CModelRender::SetCommonModelInitData(
				const char* tkmFilePath,
				const EnModelUpAxis modelUpAxis,
				const char* fxFilePath,
				const char* vsEntryPointFunc,
				const char* vsSkinEntryPointFunc,
				const char* psEntryPointFunc
			)
			{
				// ���f���̃t�@�C���p�X�̐ݒ�
				m_modelInitData.m_tkmFilePath = tkmFilePath;

				// ���f����UP���̐ݒ�
				m_modelInitData.m_modelUpAxis = modelUpAxis;

				// �V�F�[�_�[�̃t�@�C���p�X�̐ݒ�
				m_modelInitData.m_fxFilePath = fxFilePath;
				// ���_�V�F�[�_�[�̃G���g���[�|�C���g�̐ݒ�
				m_modelInitData.m_vsEntryPointFunc = vsEntryPointFunc;
				// �X�L������̒��_�V�F�[�_�[�̃G���g���[�|�C���g�̐ݒ�
				m_modelInitData.m_vsSkinEntryPointFunc = vsSkinEntryPointFunc;
				// �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̐ݒ�
				m_modelInitData.m_psEntryPointFunc = psEntryPointFunc;

				return;

			}

			/**
			 * @brief GBuffer�ɕ`�悷�郂�f���̒萔�o�b�t�@���Z�b�g
			*/
			void CModelRender::SetRenderToGBufferModelCB()
			{
				// ���f���̊g���萔�o�b�t�@�̐ݒ�
				m_modelInitData.m_expandConstantBuffer[0] = &m_modelExCB;
				m_modelInitData.m_expandConstantBufferSize[0] = sizeof(m_modelExCB);

				return;
			}

			/**
			 * @brief �f�t�H���g�̒萔�o�b�t�@���Z�b�g
			 * @param modelInitData ���f���̏������f�[�^
			*/
			void CModelRender::SetTranslucentModelCB()
			{
				// ���C�g�̏���萔�o�b�t�@�ɐݒ�
				m_modelInitData.m_expandConstantBuffer[0] =
					&nsLight::CLightManager::GetInstance()->GetLightData();
				m_modelInitData.m_expandConstantBufferSize[0] =
					sizeof(nsLight::CLightManager::GetInstance()->GetLightData());

				// ���f���̊g���萔�o�b�t�@�̐ݒ�
				m_modelInitData.m_expandConstantBuffer[1] = &m_modelExCB;
				m_modelInitData.m_expandConstantBufferSize[1] = sizeof(m_modelExCB);

				// �t�H���[�h�����_�����O�����ǁA�f�B�t�@�\�h�p�̒萔�o�b�t�@�������Ă���B
				m_modelInitData.m_expandConstantBuffer[2] =
					&nsMyEngine::CRenderingEngine::GetInstance()->GetIBLCB();
				m_modelInitData.m_expandConstantBufferSize[2] =
					sizeof(nsMyEngine::CRenderingEngine::GetInstance()->GetIBLCB());

				return;
			}

			/**
			 * @brief �f�t�H���g�̃V�F�[�_�[���\�[�X�r���[���Z�b�g
			*/
			void CModelRender::SetDefaultShaderResourceView()
			{
				// �V�F�[�_�[���\�[�X�r���[�̔ԍ�
				int SRVNo = 0;
				// �V���h�E�}�b�v�̃e�N�X�`����ݒ肷��
				for (int i = 0; i < nsLight::nsLightConstData::kMaxDirectionalLightNum; i++)
				{
					for (int areaNo = 0; areaNo < nsGraphic::nsShadow::nsShadowConstData::enShadowMapArea_num; areaNo++)
					{
						m_modelInitData.m_expandShaderResoruceView[SRVNo++] =
							&nsMyEngine::CRenderingEngine::GetInstance()->
							GetShadowMapRenderTargetTexture(i, areaNo);
					}
				}
				// �V�F�[�_�[���\�[�X�r���[�̐ݒ�
				m_modelInitData.m_expandShaderResoruceView[SRVNo++] =
					&nsMyEngine::CRenderingEngine::GetInstance()->GetIBLTexture();

				return;
			}

			/**
			 * @brief �����_���[������������
			 * @param[in] isDefferdRender �f�B�t�@�[�h�����_�����O�ŕ`�悷�邩�H
			*/
			void CModelRender::InitRender(const bool isDefferdRender)
			{
				if (isDefferdRender)
				{
					m_render.SetOnRenderToBGuuferFunc([&](RenderContext& rc) { this->ModelRender(rc); });
				}
				else
				{
					m_render.SetOnForwardRenderFunc([&](RenderContext& rc) { this->ModelRender(rc); });
				}

				return;
			}

			/**
			 * @brief �V���h�E�L���X�^�[���H��ݒ�Btrue�ŉe�𐶐�����Afalse�ŉe�𐶐����Ȃ��B
			 * @param[in] isShadowCaster �V���h�E�L���X�^�[���H
			*/
			void CModelRender::SetIsShadowCaster(const bool isShadowCaster)
			{
				// �V���h�E�L���X�^�[���H
				if (isShadowCaster)
				{
					// �V���h�E�L���X�^�[

					// �V���h�E�}�b�v�ɕ`�悷�郂�f�����L�����H
					if (m_shadowModels[0][0])
					{
						// �L���Ȃ�A�������^�[��
						return;
					}

					// �����Ȃ珉��������
					InitShadowModel();
					// �V���h�E�}�b�v�ɕ`�悷��֐���ݒ肷��
					m_render.SetOnShadwMapRender(
						[&](RenderContext& rc,const int ligNo, const int shadowMapNo, const Matrix& vlpMatrix)
						{
							this->ShadowModelRender(rc, ligNo, shadowMapNo, vlpMatrix);
						}
					);
				}
				else
				{
					// �V���h�E�L���X�^�[�ł͂Ȃ�

					// �V���h�E�}�b�v�ɕ`�悷�郂�f�����������H
					if (!m_shadowModels[0][0])
					{
						// �����Ȃ�A�������^�[��
						return;
					}

					// �L���Ȃ�j������
					for (auto& shadowModels : m_shadowModels)
					{
						for (auto& shadowModel : shadowModels)
						{
							shadowModel.reset();
						}
					}
					// �V���h�E�}�b�v�ɕ`�悷��֐����A�������Ȃ��悤�ɂ���
					m_render.SetOnShadwMapRender(
						[&](RenderContext&, const int, const int, const Matrix&)
						{
							return;
						}
				);
				}

				return;
			}

			/**
			 * @brief �V���h�E�}�b�v�ɕ`�悷�郂�f���̏�����
			*/
			void CModelRender::InitShadowModel()
			{
				// �V���h�E���f���̏������f�[�^
				ModelInitData shadowModelInitData;
				// �ʏ�̃��f���̏������f�[�^����K�v�ȏ�������Ă���
				shadowModelInitData.m_tkmFilePath = m_modelInitData.m_tkmFilePath;
				shadowModelInitData.m_modelUpAxis = m_modelInitData.m_modelUpAxis;
				shadowModelInitData.m_vsEntryPointFunc = m_modelInitData.m_vsEntryPointFunc;
				shadowModelInitData.m_vsSkinEntryPointFunc = m_modelInitData.m_vsSkinEntryPointFunc;
				shadowModelInitData.m_skeleton = m_modelInitData.m_skeleton;
				// fx�t�@�C���p�X��ݒ肷��
				shadowModelInitData.m_fxFilePath = kDrawShadowMapFxFilePath;
				// �J���[�t�H�[�}�b�g��ݒ肷��
				shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32_FLOAT;

				// �V���h�E�}�b�v�`��p�̃��f���𐶐����ď���������
				for (auto& shadowModels : m_shadowModels)
				{
					for (auto& shadowModel : shadowModels)
					{
						shadowModel.reset(new Model);
						shadowModel->Init(shadowModelInitData);
					}
				}

				return;
			}


			/**
			 * @brief ���f����`�悷��
			 * @param[in] rc �����_�����O�R���e�L�X�g
			*/
			void CModelRender::ModelRender(RenderContext& rc)
			{
				// ���f����`��
				m_model->Draw(rc);

				return;
			}

			/**
			 * @brief �V���h�E�}�b�v�ɕ`�悷�郂�f����`�悷��
			 * @param[in] rc �����_�����O�R���e�L�X�g
			 * @param[in] ligNo ���C�g�ԍ�
			 * @param[in] shadowMapNo �V���h�E�}�b�v�̔ԍ�
			 * @param[in] lvpMatrix ���C�g�r���[�v���W�F�N�V�����s��
			*/
			void CModelRender::ShadowModelRender(
				RenderContext& rc,
				const int ligNo,
				const int shadowMapNo,
				const Matrix& lvpMatrix
			)
			{
				if (m_shadowModels[0][0])
				{
					//@todo�f�B���N�V�������C�g�̐������V���h�E���f�����������d�������̂ŁA�C���X�^���X�`�������܂ł̓��C�g1���̂�
					if (ligNo != 0)
						return;

					// �V���h�E�}�b�v�ɕ`�悷�郂�f����`��
					m_shadowModels[ligNo][shadowMapNo]->Draw(rc, Matrix::Identity, lvpMatrix);
				}

				return;
			}


		}
	}
}