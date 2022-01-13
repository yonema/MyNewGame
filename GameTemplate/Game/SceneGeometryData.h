#pragma once
#include "Noncopyable.h"

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsGeometry { class CGeometryData; }

	/**
	 * @brief �W�I���g���֘A�̃l�[���X�y�[�X
	*/
	namespace nsGeometry
	{
		/**
		 * @brief �V�[���̃W�I���g�����
		*/
		class CSceneGeometryData : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CSceneGeometryData() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CSceneGeometryData() = default;

		public:		// �����o�֐�


            /**
             * @brief �W�I���g���f�[�^��o�^
             * @param[in,out] geomData �o�^����W�I���g���f�[�^
            */
            void RegisterGeometryData(CGeometryData* geomData)
            {
                m_geometryDatas.push_back(geomData);
            }

            /**
             * @brief �W�I���g���f�[�^�̓o�^������
             * @param[in] geomData �o�^����������W�I���g���f�[�^
            */
            void UnregisterGeometryData(const CGeometryData* geomData)
            {
                auto it = std::find(
                    m_geometryDatas.begin(),
                    m_geometryDatas.end(),
                    geomData
                );
                if (it != m_geometryDatas.end()) {
                    m_geometryDatas.erase(it);
                }
            }

            /**
             * @brief �W�I���g���f�[�^���X�V
            */
            void Update();

            /**
             * @brief �r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ő���W���擾
             * @return �r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ő���W
            */
            const Vector3& GetShadowCasterMaxPositionInViewFrustum() const
            {
                return m_shadowCasterMaxPositionInViewFrustum;
            }

            /**
             * @brief �r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ŏ����W���擾
             * @return �r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ŏ����W
            */
            const Vector3& GetShadowCasterMinPositionInViewFrustum() const
            {
                return m_shadowCasterMinPositionInViewFrustum;
            }

            /**
             * @brief �V���h�E�L���X�^�[�Ɋւ���W�I���g�������\�z�ł����H���擾
             * @return �V���h�E�L���X�^�[�Ɋւ���W�I���g�������\�z�ł����H
            */
            bool IsBuildshadowCasterGeometryData() const
            {
                return m_isBuiltShadowCasterGeometryData;
            }


		private:	// �f�[�^�����o
			//!< �r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ő���W
			Vector3 m_shadowCasterMaxPositionInViewFrustum;
			//!< �r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ŏ����W
			Vector3 m_shadowCasterMinPositionInViewFrustum;
			//!< �V���h�E�L���X�^�[�Ɋւ���W�I���g�������\�z�ł����H
			bool m_isBuiltShadowCasterGeometryData = false;
			std::list<CGeometryData*> m_geometryDatas;	//!< �V�[�����\������W�I���g�����̃��X�g

		};

	}
}