#pragma once
#include "Render.h"

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
		 * @brief �x�N�g���`��N���X
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
			 * @param renderData 
			*/
			void AddVector(const SVectorRenderData& renderData)
			{
				m_vectorRenderDatas.emplace_back(std::move(renderData));
			}

			/**
			 * @brief 
			*/
			void ResetVectorRenderDatas()
			{
				m_vectorRenderDatas.clear();
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief 2D�`��֐�
			 * @param rc �����_�����O�R���e�L�X�g
			*/
			void Render2D(RenderContext& rc);

		private:	// �f�[�^�����o
			std::vector<SVectorRenderData> m_vectorRenderDatas;
			nsGraphic::CRender m_render;										//!< �����_���[�N���X
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;
			Font m_font;

		};

	}
}