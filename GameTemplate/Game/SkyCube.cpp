#include "stdafx.h"
#include "SkyCube.h"

namespace nsMyGame
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
			// �e�N�X�`���̏�����
			InitTexture();

			// ���f�������_���[�̏�����
			InitModelRender();

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
		*/
		void CSkyCube::Init(const nsSkyCubeConstData::EnSkyCubeType skyCubeType)
		{
			// �w�肵���X�J�C�L���[�u�̃^�C�v�Ńe�N�X�`����������
			m_skyTexture.InitFromDDSFile(GetSkyCubeTextureDDSFilePath(skyCubeType));
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
			// �X�J�C�̃��f�������_���[�̏�����
			m_skyModelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			// ���f���̏������f�[�^
			ModelInitData modelInitData;
			modelInitData.m_tkmFilePath = kTkmFilePath;	// tkm�t�@�C���p�X��ݒ�
			modelInitData.m_fxFilePath = kFxFilePath;	// fx�t�@�C���p�X��ݒ�
			// SRV�ɃX�J�C�L���[�u�}�b�v�̃e�N�X�`����ݒ�
			modelInitData.m_expandShaderResoruceView[0] = &m_skyTexture;

			// �X�J�C�̃��f�������_���[���t�H���[�h�����_�����O�ŕ`�悷��悤�ɏ�����
			m_skyModelRender->InitForwardRendering(modelInitData);

			// ���W�Ɗg�嗦��ݒ肷��
			m_skyModelRender->SetPosition(m_position);
			m_skyModelRender->SetScale(m_scale);

			return;
		}
	}
}