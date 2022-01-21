#include "stdafx.h"
#include "SkyCube.h"
#include "RenderingEngine.h"

namespace nsNinjaAttract
{
	/**
	 * @brief ���R�֘A�̃l�[���X�y�[�X
	*/
	namespace nsNature
	{
		// �X�J�C�L���[�u�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsSkyCubeConstData;

		/**
		 * @brief �X�^�[�g�֐�
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CSkyCube::Start()
		{


			return true;
		}

		/**
		 * @brief �j�����ꂽ�Ƃ��ɌĂ΂��֐�
		*/
		void CSkyCube::OnDestroy()
		{
			DeleteGO(m_skyModelRender);
			return;
		}

		/**
		 * @brief �������֐�
		 * @attention �I�u�W�F�N�g�𐶐����Ă����ɌĂ΂Ȃ��ƌ��ʂ�����܂���B
		 * @param[in] skyCubeType �X�J�C�L���[�u�̃^�C�v
		 * @param[in] isIBL IBL���s�����H
		*/
		void CSkyCube::Init(const nsSkyCubeConstData::EnSkyCubeType skyCubeType, const bool isIBL)
		{
			// �w�肵���X�J�C�L���[�u�̃^�C�v�Ńe�N�X�`����������
			m_skyTexture.InitFromDDSFile(GetSkyCubeTextureDDSFilePath(skyCubeType));

			nsMyEngine::CRenderingEngine::GetInstance()->ReInitIBL(GetSkyCubeTextureDDSFilePath(skyCubeType), 1.0f,isIBL);

			// �X�J�C�̃��f�������_���[�̐���
			m_skyModelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			// �e�N�X�`���̏�����
			InitTexture();

			// ���f�������_���[�̏�����
			InitModelRender();
		}

		/**
		 * @brief �^�C�g����ʂ��H��ݒ�
		 * @param[in] isTitle �^�C�g����ʂ��H
		*/
		void CSkyCube::SetIsTitle(const bool isTitle)
		{
			// ��̃��f���̏�Z�J���[
			float mul = 1.0f;

			if (isTitle)
			{
				// �^�C�g����ʂł͏����Â�����
				mul = 0.9f;
			}

			// ��Z�J���[��ݒ�
			m_skyModelRender->SetMulColor({ mul ,mul ,mul ,1.0f });

			return;
		}

		/**
		 * @brief �e�N�X�`���̏������֐�
		*/
		void CSkyCube::InitTexture()
		{
			// �X�J�C�L���[�u�̃e�N�X�`��������������Ă��Ȃ����H
			if (!m_skyTexture.IsValid())
			{
				// ����������Ă��Ȃ�
				// �f�t�H���g�̃X�J�C�L���[�u�̃e�N�X�`���ŏ���������
				m_skyTexture.InitFromDDSFile(kDefaultDDSPath);
			}

			return;
		}

		/**
		 * @brief ���f�������_���[�̏������֐�
		*/
		void CSkyCube::InitModelRender()
		{
			// ���f���̏������f�[�^
			ModelInitData modelInitData;
			modelInitData.m_tkmFilePath = kTkmFilePath;	// tkm�t�@�C���p�X��ݒ�
			modelInitData.m_fxFilePath = kFxFilePath;	// fx�t�@�C���p�X��ݒ�
			// SRV�ɃX�J�C�L���[�u�}�b�v�̃e�N�X�`����ݒ�
			modelInitData.m_expandShaderResoruceView[0] = &m_skyTexture;
			// ���C�������_�����O�^�[�Q�b�g�̂���t�H�[�}�b�g�ɍ��킹��
			modelInitData.m_colorBufferFormat[0] =
				nsMyEngine::CRenderingEngine::GetInstance()->GetMainRenderTargetFormat();

			// �X�J�C�̃��f�������_���[���t�H���[�h�����_�����O�ŕ`�悷��悤�ɏ�����
			m_skyModelRender->InitForwardRendering(modelInitData);

			// ���W�Ɗg�嗦��ݒ肷��
			m_skyModelRender->SetPosition(m_position);
			m_skyModelRender->SetScale(m_scale);

			return;
		}
	}
}