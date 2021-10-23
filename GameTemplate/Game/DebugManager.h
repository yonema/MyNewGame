#pragma once
#include "VectorRender.h"
#include "DebugConstData.h"
#include "TextPanelRender.h"
#include "Noncopyable.h"

namespace nsMyGame
{
	/**
	 * @brief �f�o�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsDebug
	{

		/**
		 * @brief �f�o�b�N�̃}�l�[�W���[�B
		 * @details �f�o�b�N�Ŏg�p����@�\���Ǘ�����N���X�BRelease�r���h�ł͓��삵�Ȃ��B
		 * @note �V���O���g���p�^�[�����g�p
		*/
		class CDebugManager : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CDebugManager();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CDebugManager();

		public:		// static�ȃ����o�֐�

			/**
			 * @brief �C���X�^���X�𐶐�
			*/
			static void CreateInstance()
			{
				m_instance = new CDebugManager;
			}

			/**
			 * @brief �C���X�^���X��j��
			*/
			static void DeleteInstance()
			{
				if (m_instance)
					delete m_instance;
			}

			/**
			 * @brief �C���X�^���X���擾
			 * @return �C���X�^���X
			*/
			static CDebugManager* GetInstance()
			{
				return m_instance;
			}

		private:	// static�ȃf�[�^�����o
			static CDebugManager* m_instance;		//!< �C���X�^���X

		public:		// �����o�֐�

			/**
			 * @brief �`�悷��x�N�g����ǉ�����
			 * @param[in] vectorRenderData �x�N�g���̕`��f�[�^
			*/
			void AddVector(const CVectorRender::SVectorRenderData& vectorRenderData)
			{
#ifdef MY_DEBUG
				m_vectorRender->AddVector(vectorRenderData);
#endif
			}

			/**
			 * @brief �x�N�g���̃t�H���g�̊g�嗦��ݒ�
			 * @param[in] scale �g�嗦
			*/
			void SetVectorFontScale(const float scale)
			{
#ifdef MY_DEBUG
				m_vectorRender->SetFontScale(scale);
#endif
			}

			/**
			 * @brief �`�悷��e�L�X�g���e�L�X�g�p�l���ɒǉ�
			 * @param[in] text �e�L�X�g
			 * @param[in] headText �擪�ɕt����e�L�X�g
			*/
			void AddTextPanel(const std::wstring& text, const std::wstring& headText)
			{
#ifdef MY_DEBUG
				m_textPanelRender->AddTextPanel(text, headText);
#endif
			}


			/**
			 * @brief ���O�̔j������
			*/
			void PreDelete();

		private:	// �f�[�^�����o
			CVectorRender* m_vectorRender = nullptr;		//!< �x�N�g���`��N���X
			CTextPanelRender* m_textPanelRender = nullptr;	//!< �e�L�X�g�p�l���`��N���X

		};


		/**
		 * @brief �x�N�g����`�悷��i�x�N�g���`��f�[�^����j
		 * @param[in] vectorRenderData �x�N�g���`��f�[�^
		*/
		static inline void DrawVector(const CVectorRender::SVectorRenderData& vectorRenderData)
		{
			CDebugManager::GetInstance()->AddVector(vectorRenderData);
		}

		/**
		 * @brief �x�N�g����`�悷��i��_�A�x�N�g���A���O����j
		 * @param[in] origin ��_
		 * @param[in] vector �x�N�g��
		 * @param[in] name �x�N�g�������O
		*/
		static inline void DrawVector(
			const Vector3 origin,
			const Vector3 vector,
			const std::string name = nsDebugConstData::kDefaultVectorName
		)
		{
			CVectorRender::SVectorRenderData vectorRenderData =
			{ name,vector,origin, };

			CDebugManager::GetInstance()->AddVector(vectorRenderData);
		}


		/**
		 * @brief �`�悷��e�L�X�g���e�L�X�g�p�l���ɒǉ�
		 * @param[in] text �e�L�X�g
		 * @param[in] headText �擪�ɕt����e�L�X�g
		*/
		static inline void DrawTextPanel(const std::wstring& text, const std::wstring& headText = L"")
		{
			CDebugManager::GetInstance()->AddTextPanel(text, headText);
		}

		/**
		 * @brief �`�悷��e�L�X�g���e�L�X�g�p�l���ɒǉ��ifloat�j
		 * @param[in] floatText float�̃e�L�X�g
		 * @param[in] headText �擪�ɕt����e�L�X�g
		*/
		static inline void DrawTextPanel(const float floatText, const std::wstring& headText = L"")
		{
			CDebugManager::GetInstance()->AddTextPanel(std::to_wstring(floatText), headText);
		}

		/**
		 * @brief �`�悷��e�L�X�g���e�L�X�g�p�l���ɒǉ��iVector3�j
		 * @param[in] vectorText �x�N�g���e�L�X�g
		 * @param[in] headText �擪�ɕt����e�L�X�g
		*/
		static inline void DrawTextPanel(const Vector3& vectorText, const std::wstring& headText = L"")
		{
			wchar_t text[128];
			swprintf_s(text, L"(%2.2f,%2.2f,%2.2f)", vectorText.x, vectorText.y, vectorText.z);
			CDebugManager::GetInstance()->AddTextPanel(text, headText);
		}

		/**
		 * @brief �`�悷��e�L�X�g���e�L�X�g�p�l���ɒǉ��ibool�j
		 * @param[in] boolText �u�[���l
		 * @param[in] headText �擪�ɕt����e�L�X�g
		*/
		static inline void DrawTextPanelBool(const bool boolText, const std::wstring& headText = L"")
		{
			CDebugManager::GetInstance()->AddTextPanel(boolText ? L"true" : L"false", headText);
		}

	}
}