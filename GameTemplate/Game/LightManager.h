#pragma once
#include "LightData.h"
#include "Noncopyable.h"

namespace nsMyGame
{
	/**
	 * @brief ���C�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsLight
	{
		// �O���錾
		class CDirectionalLight;	// �f�B���N�V�������C�g
		class CPointLight;			// �|�C���g���C�g
		class CSpotLight;			// �X�|�b�g���C�g

		/**
		 * @brief ���C�g�Ǘ��N���X
		 * @note �V���O���g���p�^�[�����g�p
		*/
		class CLightManager : private nsUtil::Noncopyable
		{
		private:	// �R���X�g���N�^�ƃf�X�g���N�^�B
					// �V���O���g���p�^�[���̂���private�ɉB��
			/**
			 * @brief �R���X�g���N�^
			*/
			CLightManager();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CLightManager();

		public:		// �����o�֐�

			/**
			 * @brief �A�b�v�f�[�g�֐��B�Q�[�����[�v�ŌĂ�ł��������B
			*/
			void Update();

			/**
			 * @brief �f�B���N�V�������C�g��ǉ�����
			 * @param[in,out] light �ǉ�����f�B���N�V�������C�g
			*/
			void AddLight(CDirectionalLight* light);

			/**
			 * @brief �|�C���g���C�g��ǉ�����
			 * @param[in,out] light �ǉ�����|�C���g���C�g
			*/
			void AddLight(CPointLight* light);

			/**
			 * @brief �X�|�b�g���C�g��ǉ�����
			 * @param[in,out] light �ǉ�����X�|�b�g���C�g
			*/
			void AddLight(CSpotLight* light);

			/**
			 * @brief �f�B���N�V�������C�g��j������
			 * @param[in] light �j������f�B���N�V�������C�g
			*/
			void RemoveLight(const CDirectionalLight* light);

			/**
			 * @brief �|�C���g���C�g��j������
			 * @param[in] light �j������|�C���g���C�g
			*/
			void RemoveLight(const CPointLight* light);

			/**
			 * @brief �X�|�b�g���C�g��j������
			 * @param[in] light �j������X�|�b�g���C�g
			*/
			void RemoveLight(const CSpotLight* light);

			/**
			 * @brief ���C�g�B�̃f�[�^�̎Q�Ƃ𓾂�
			 * @return ���C�g�B�̃f�[�^
			*/
			nsLightData::SLightData& GetLightData()
			{
				return m_lightData;
			}

		public:		// static�ȃ����o�֐�
			/**
			 * @brief �B��̃C���X�^���X�𐶐�����
			 * @note �V���O���g���p�^�[��
			*/
			static void CreateInstance()
			{
				m_instance = new CLightManager;
			}

			/**
			 * @brief �B��̃C���X�^���X��j������
			 * @note �V���O���g���p�^�[��
			*/
			static void DeleteInstance()
			{
				if (m_instance)
					delete m_instance;
			}

			/**
			 * @brief �B��̃C���X�^���X�̎Q�Ƃ𓾂�
			 * @return �B��̃C���X�^���X�̎Q��
			 * @note �V���O���g���p�^�[��
			*/
			static CLightManager* GetInstance()
			{
				return m_instance;
			}

		private:	// �f�[�^�����o
			nsLightData::SLightData m_lightData;	//!< ���C�g�B�̃f�[�^
			CDirectionalLight* m_directionalLightPtrs[nsLightConstData::kMaxDirectionalLightNum] = {};	//!< ���ꂼ��̃f�B���N�V�������C�g�ւ̃|�C���^
			CPointLight* m_pointLightPtrs[nsLightConstData::kMaxPointLightNum] = {};	//!< ���ꂼ��̃|�C���g���C�g�ւ̃|�C���^
			CSpotLight* m_spotLightPtrs[nsLightConstData::kMaxSpotLightNum] = {};		//!< ���ꂼ��̃X�|�b�g���C�g�ւ̃|�C���^

		private:	// static�ȃf�[�^�����o
			static CLightManager* m_instance;		//!< �B��̃C���X�^���X
		};
	}
}
