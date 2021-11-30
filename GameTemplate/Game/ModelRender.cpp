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
				{
					// �X�P���g��������������Ă�����A�X�P���g�����X�V�B

					if (m_isEnableInstancingDraw)
					{
						// �C���X�^���V���O�`�掞
						// �e�C���X�^���X�̃��[���h��Ԃւ̕ϊ��́A
						// �C���X�^���X���Ƃɍs���K�v������̂ŁA���_�V�F�[�_�[�ōs���B
						// �Ȃ̂ŁA�P�ʍs���n���āA���f����ԂŃ{�[���s����\�z����B
						m_skeletonPtr->Update(g_matIdentity);
					}
					else
					{
						// �ʏ�`�掞
						m_skeletonPtr->Update(m_model->GetWorldMatrix());
					}
				}
				// �A�j���[�V����������������Ă��邩�H
				if (m_animationPtr)	// �A�j���[�V����������������Ă�����A�A�j���[�V������i�߂�B
					m_animationPtr->Progress(nsTimer::GameTime().GetFrameDeltaTime() * m_animationSpeed);

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
			 * @param[in] filePath ���f���̃t�@�C���p�X
			 * @param[in] animationClips �A�j���[�V�����N���b�v
			 * @param[in] numAnimationClip �A�j���[�V�����N���b�v�̐�
			 * @param[in] �C���X�^���X�̍ő吔
			 * @param[in] modelUpAxis ���f����UP��
			*/
			void CModelRender::Init(
				const char* filePath,
				AnimationClip* animationClips,
				const int numAnimationClip,
				const int maxInstance,
				const EnModelUpAxis modelUpAxis
			)
			{
				// ���f���̏������f�[�^�̋��ʕ����̐ݒ�
				SetCommonModelInitData(filePath, maxInstance, modelUpAxis);

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

				// GBuffer�ɕ`�悷�郂�f���̃V�F�[�_�[���\�[�X�r���[���Z�b�g
				SetRenderToGBufferShaderResourceView();

				// �����������̃��C���R�A
				InitMainCore(animationClips, numAnimationClip);

				return;
			}
			/**
			 * @brief �������`��p�̏������֐�
			 * @param[in] filePath ���f���̃t�@�C���p�X
			 * @param[in] animationClips �A�j���[�V�����N���b�v
			 * @param[in] numAnimationClip �A�j���[�V�����N���b�v�̐�
			 * @param[in] �C���X�^���X�̍ő吔
			 * @param[in] modelUpAxis ���f����UP��
			*/
			void CModelRender::IniTranslucent(
				const char* filePath,
				AnimationClip* animationClips,
				const int numAnimationClip,
				const int maxInstance,
				const EnModelUpAxis modelUpAxis
			)
			{

				// ���f���̏������f�[�^�̋��ʕ����̐ݒ�
				// �������`��p�̃V�F�[�_�[���w�肷��
				SetCommonModelInitData(filePath, maxInstance, modelUpAxis, kTranslucentModelFxFilePath);

				// ���C�������_�����O�^�[�Q�b�g�̂���t�H�[�}�b�g�ɍ��킹��
				m_modelInitData.m_colorBufferFormat[0] =
					nsMyEngine::CRenderingEngine::GetInstance()->GetMainRenderTargetFormat();

				// �������`��p���f���̒萔�o�b�t�@���Z�b�g
				SetTranslucentModelCB();

				// �������`��p���f���̃V�F�[�_�[���\�[�X�r���[���Z�b�g
				SetTranslucentModelShaderResourceView();

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
				// ���f���̍��W�X�V
				m_model->UpdateWorldMatrix(m_position, m_rotation, m_scale);

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
			 * @param[in] maxInstance �ő�C���X�^���X��
			 * @param[in] modelUpAxis ���f����UP��
			 * @param[in] fxFilePath �V�F�[�_�[��fx�t�@�C���p�X
			*/
			void CModelRender::SetCommonModelInitData(
				const char* tkmFilePath,
				const int maxInstance,
				const EnModelUpAxis modelUpAxis,
				const char* fxFilePath
			)
			{
				// ���f���̃t�@�C���p�X�̐ݒ�
				m_modelInitData.m_tkmFilePath = tkmFilePath;
				// �V�F�[�_�[�̃t�@�C���p�X�̐ݒ�
				m_modelInitData.m_fxFilePath = fxFilePath;
				// ���f����UP���̐ݒ�
				m_modelInitData.m_modelUpAxis = modelUpAxis;
				// �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̐ݒ�
				m_modelInitData.m_psEntryPointFunc = kPsEntryPointFunc;

				// �C���X�^���V���O�`��p�̏���������
				InitInstancingDraw(maxInstance);

				if (m_isEnableInstancingDraw)
				{
					// �C���X�^���V���O�`��p
					
					// �C���X�^���V���O�`��p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�̐ݒ�
					m_modelInitData.m_vsEntryPointFunc = kVsInstancingEntryPointFunc;
					//�X�L������A�C���X�^���V���O�`��p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�̐ݒ�
					m_modelInitData.m_vsSkinEntryPointFunc = kVsSkinInstancingEntryPointFunc;
				}
				else
				{
					// �ʏ�`��p
					
					// ���_�V�F�[�_�[�̃G���g���[�|�C���g�̐ݒ�
					m_modelInitData.m_vsEntryPointFunc = kVsEntryPointFunc;
					// �X�L������̒��_�V�F�[�_�[�̃G���g���[�|�C���g�̐ݒ�
					m_modelInitData.m_vsSkinEntryPointFunc = kVsSkinEntryPointFunc;
				}

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

				// IBL�p�̃f�[�^��ݒ�
				m_modelInitData.m_expandConstantBuffer[2] =
					&nsMyEngine::CRenderingEngine::GetInstance()->GetIBLCB();
				m_modelInitData.m_expandConstantBufferSize[2] =
					sizeof(nsMyEngine::CRenderingEngine::GetInstance()->GetIBLCB());

				return;
			}

			/**
			 * @brief GBuffer�ɕ`�悷�郂�f���̃V�F�[�_�[���\�[�X�r���[���Z�b�g
			*/
			void CModelRender::SetRenderToGBufferShaderResourceView()
			{
				// �C���X�^���V���O�`����s���ꍇ�́A�C���X�^���V���O�`��p�̃f�[�^��ݒ�
				if (m_isEnableInstancingDraw)
				{
					m_modelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
				}

				return;
			}

			/**
			 * @brief �������`��p���f���̃V�F�[�_�[���\�[�X�r���[���Z�b�g
			*/
			void CModelRender::SetTranslucentModelShaderResourceView()
			{
				// �V�F�[�_�[���\�[�X�r���[�̔ԍ�
				int SRVNo = 0;

				// �C���X�^���V���O�`����s���ꍇ�́A�C���X�^���V���O�`��p�̃f�[�^��ݒ�
				if (m_isEnableInstancingDraw)
				{
					m_modelInitData.m_expandShaderResoruceView[SRVNo] = &m_worldMatrixArraySB;
				}
				SRVNo++;

				// IBL�p�̃f�[�^��ݒ�
				m_modelInitData.m_expandShaderResoruceView[SRVNo++] =
					&nsMyEngine::CRenderingEngine::GetInstance()->GetIBLTexture();

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
			 * @brief �C���X�^���V���O�f�[�^�̍X�V
			 * @param[in] pos ���W
			 * @param[in] rot ��]
			 * @param[in] scale �g�嗦
			*/
			void CModelRender::UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
			{
				if (m_numInstance > m_maxInstance)
				{
					MessageBoxA(nullptr, "�C���X�^���X�̐����������ł�", "�x��", MB_OK);
				}

				if (!m_isEnableInstancingDraw) {
					return;
				}

				Matrix worldMatrix = m_model->CalcWorldMatrix(pos, rot, scale);

				// �C���X�^���V���O�`����s���B
				m_worldMatrixArray[m_numInstance] = worldMatrix;

				m_numInstance++;

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

				// �C���X�^���V���O�`����s���ꍇ�́A�C���X�^���V���O�`��p�̃f�[�^��ݒ�
				if (m_isEnableInstancingDraw)
				{
					m_modelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
				}

				// �V���h�E�}�b�v�`��p�̃��f���𐶐����ď���������
				for (auto& shadowModels : m_shadowModels)
				{
					for (auto& shadowModel : shadowModels)
					{
						shadowModel.reset(new Model);
						shadowModel->Init(shadowModelInitData);
						shadowModel->UpdateWorldMatrix(m_position, m_rotation, m_scale);
					}
				}

				return;
			}

			/**
			 * @brief �C���X�^���V���O�`��p�̏���������
			 * @param[in] maxInstance �ő�C���X�^���X��
			*/
			void CModelRender::InitInstancingDraw(int maxInstance)
			{
				m_maxInstance = maxInstance;
				if (m_maxInstance > 1) {
					//�C���X�^���V���O�`����s���̂�
					//����p�̃f�[�^���\�z����
					//���[���h�s��̔z��̃��������m�ۂ���
					m_worldMatrixArray = std::make_unique<Matrix[]>(m_maxInstance);
					//���[���h�s���GPU�ɓ]�����邽�߂̃X�g���N�`���[�h�o�b�t�@���m��
					m_worldMatrixArraySB.Init(
						sizeof(Matrix),
						m_maxInstance,
						nullptr
					);
					m_isEnableInstancingDraw = true;
				}
			}


			/**
			 * @brief ���f����`�悷��
			 * @param[in] rc �����_�����O�R���e�L�X�g
			*/
			void CModelRender::ModelRender(RenderContext& rc)
			{
				if (m_isEnableInstancingDraw)
				{
					// �C���X�^���V���O�`�掞

					// ���̂����A�t���X�^���J�����O����邩��A���̎��̏����͂����ɓ����
					m_fixNumInstanceOnFrame = m_numInstance;
					//// �r���[�t���X�^���Ɋ܂܂�Ă���C���X�^���X�̂ݕ`�悷��B
					//for (int instanceId = 0; instanceId < m_numInstance; instanceId++) {
					//	if (m_geometryDatas[instanceId].IsInViewFrustum()) {
					//		// �r���[�t���X�^���Ɋ܂܂�Ă���B
					//		m_worldMatrixArray[m_fixNumInstanceOnFrame] = m_worldMatrixArray[instanceId];
					//		m_fixNumInstanceOnFrame++;
					//	}
					//}

					if (m_fixNumInstanceOnFrame != 0)
					{
						// �X�g���N�`���[�h�o�b�t�@���X�V
						m_worldMatrixArraySB.Update(m_worldMatrixArray.get());
						// ���f����`��
						m_model->Draw(rc, m_fixNumInstanceOnFrame);
					}

				}
				else
				{
					// �ʏ�`�掞
					// ���f����`��
					m_model->Draw(rc);
				}

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
					m_shadowModels[ligNo][shadowMapNo]->Draw(rc, Matrix::Identity, lvpMatrix, m_fixNumInstanceOnFrame);
				}

				return;
			}


		}
	}
}