#pragma once
#include "Render.h"
#include "DebugConstData.h"

namespace nsMyGame
{
	namespace nsGraphic
	{
		namespace nsModel { class CModelRender; }	// ���f�������_���[�N���X
	}

	/**
	 * @brief �f�o�b�N�֘A�̃l�[���X�y�[�X
	*/	
	namespace nsDebug
	{
		/**
		 * @brief �f�o�b�N�p�̃x�N�g���`��N���X
		*/
		class CVectorRender : public IGameObject
		{
		public:		// �\����

			/**
			 * @brief �x�N�g���`��f�[�^
			*/
			struct SVectorRenderData {
				std::string name = "vector";		//!< ���O
				Vector3 vector = Vector3::One;		//!< �x�N�g���B
				Vector3 origin = Vector3::Zero;		//!< ��_�B
			};

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CVectorRender() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CVectorRender() = default;

		public:		// �I�[�o�[���C�h���������o�֐�

			/**
			 * @brief Update�̒��O�ŌĂ΂��J�n����
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool Start() override final;

			/**
			 * @brief ��������鎞�ɌĂ΂�鏈��
			*/
			void OnDestroy() override final;

			/**
			 * @brief ��ɌĂ΂��X�V����
			*/
			void AlwaysUpdate() override final;

			/**
			 * @brief �`��I�u�W�F�N�g�o�^�̓���
			*/
			void AddRenderEntrance() override final;
			
		public:		// �����o�֐�

			/**
			 * @brief �`�悷��x�N�g����ǉ�
			 * @param[in] renderData �x�N�g���`��f�[�^
			*/
			void AddVector(const SVectorRenderData& renderData)
			{
				m_vectorRenderDatas.emplace_back(std::move(renderData));
			}

			/**
			 * @brief �x�N�g���̃t�H���g�̊g�嗦��ݒ肷��
			 * @param[in] scale �g�嗦
			*/
			void SetFontScale(const float scale)
			{
				m_fontScale = scale;
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief 2D�`��֐�
			 * @param[in,out] rc �����_�����O�R���e�L�X�g
			*/
			void Render2D(RenderContext& rc);

		private:	// �f�[�^�����o
			std::vector<SVectorRenderData> m_vectorRenderDatas;			//!< �x�N�g�������_���[�̃f�[�^�W
			nsGraphic::CRender m_render;								//!< �����_���[�N���X
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< ���f�������_���[
			Font m_font;												//!< �t�H���g
			float m_fontScale = nsDebugConstData::kVectorFontScale;		//!< �x�N�g���̃t�H���g�̊g�嗦

		};

	}
}