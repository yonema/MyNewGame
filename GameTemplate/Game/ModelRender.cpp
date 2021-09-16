#include "stdafx.h"
#include "ModelRender.h"

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
					m_skeletonPtr->Update(m_model.GetWorldMatrix());
				// �A�j���[�V����������������Ă��邩�H
				if (m_animationPtr)	// �A�j���[�V����������������Ă�����A�A�j���[�V������i�߂�B
					m_animationPtr->Progress(1.0f / 60.0f);

				// ���f���̍��W�X�V
				m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

				return;
			}

			/**
			 * @brief �`�揈���֐�
			 * @param rc �����_�[�R���e�L�X�g
			*/
			void CModelRender::Render(RenderContext& rc)
			{
				// �������ς݂��H
				if (m_isInited != true)
					return;	// ���������Ă��Ȃ��A�������^�[��

				// ���f����`��
				m_model.Draw(rc);

				return;
			}


			/**
			 * @brief �������֐�
			 * @param filePath ���f���̃t�@�C���p�X
			 * @param modelUpAxis ���f����UP��
			 * @param animationClips �A�j���[�V�����N���b�v
			 * @param numAnimationClip �A�j���[�V�����N���b�v�̐�
			*/
			void CModelRender::Init(
				const char* filePath,
				const EnModelUpAxis modelUpAxis,
				AnimationClip* animationClips,
				int numAnimationClip
			)
			{
				// ���f���̏������f�[�^
				ModelInitData modelInitData;

				// ���f���̏������f�[�^�̋��ʕ����̐ݒ�
				SetCommonModelInitData(&modelInitData, filePath);

				// �����������̃��C���R�A
				InitMainCore(modelInitData, animationClips, numAnimationClip);

				return;
			}

			/**
			 * @brief �����������̃��C���R�A
			 * @param modelInitData ���f���̏������f�[�^
			 * @param animationClips �A�j���[�V�����N���b�v
			 * @param numAnimationClips �A�j���[�V�����N���b�v�̐�
			*/
			void CModelRender::InitMainCore(
				ModelInitData& modelInitData,
				AnimationClip* animationClips,
				const int numAnimationClips
			)
			{
				// tkm�t�@�C���p�X��ێ�����
				m_tkmFilePath = modelInitData.m_tkmFilePath;

				// �X�P���g���̏�����
				InitSkelton();
				// �X�P���g�������݂��邩�H
				if (m_skeletonPtr)
					// ���݂���B�X�P���g���̐ݒ�
					modelInitData.m_skeleton = m_skeletonPtr.get();

				// ���f���̏�����
				m_model.Init(modelInitData);

				// �A�j���[�V�����̏�����
				InitAnimation(animationClips, numAnimationClips);

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
				std::string skeletonFilePath = m_tkmFilePath;
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
			 * @param[out] modelInitData ���f���̏������f�[�^
			 * @param[in] tkmFilePath ���f����tkm�t�@�C���p�X
			 * @param[in] fxFilePath �V�F�[�_�[��fx�t�@�C���p�X
			 * @param[in] modelUpAxis ���f����UP��
			 * @param[in] vsEntryPointFunc ���_�V�F�[�_�[�̃G���g���[�|�C���g
			 * @param[in] vsSkinEntryPointFunc �X�L������̒��_�V�F�[�_�[�̃G���g���[�|�C���g
			 * @param[in] psEntryPointFunc �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
			*/
			void CModelRender::SetCommonModelInitData(
				ModelInitData* modelInitData,
				const char* tkmFilePath,
				const char* fxFilePath,
				const EnModelUpAxis modelUpAxis,
				const char* vsEntryPointFunc,
				const char* vsSkinEntryPointFunc,
				const char* psEntryPointFunc
			)
			{
				// ���f���̃t�@�C���p�X�̐ݒ�
				modelInitData->m_tkmFilePath = tkmFilePath;

				// ���f����UP���̐ݒ�
				modelInitData->m_modelUpAxis = modelUpAxis;

				// �V�F�[�_�[�̃t�@�C���p�X�̐ݒ�
				modelInitData->m_fxFilePath = fxFilePath;
				// ���_�V�F�[�_�[�̃G���g���[�|�C���g�̐ݒ�
				modelInitData->m_vsEntryPointFunc = vsEntryPointFunc;
				// �X�L������̒��_�V�F�[�_�[�̃G���g���[�|�C���g�̐ݒ�
				modelInitData->m_vsSkinEntryPointFunc = vsSkinEntryPointFunc;
				// �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̐ݒ�
				modelInitData->m_psEntryPointFunc = psEntryPointFunc;

				return;

			}


		}
	}
}