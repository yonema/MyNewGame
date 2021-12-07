#include "stdafx.h"
#include "SceneGeometryData.h"
#include "GeometryData.h"

namespace nsMyGame
{


	/**
	 * @brief �W�I���g���֘A�̃l�[���X�y�[�X
	*/
	namespace nsGeometry
	{
        /**
         * @brief �W�I���g���f�[�^���X�V
        */
        void CSceneGeometryData::Update()
        {
            // �p�����[�^�����Z�b�g����
            m_isBuiltShadowCasterGeometryData = false;
            m_shadowCasterMaxPositionInViewFrustum = { -FLT_MIN, -FLT_MIN, -FLT_MIN };
            m_shadowCasterMinPositionInViewFrustum = { FLT_MAX,  FLT_MAX,  FLT_MAX };

            // �V�[���̃W�I���g�������X�V�B
            for (auto& geomData : m_geometryDatas) 
            {
                // �W�I���g���̏����X�V
                geomData->Update();

                // �r���[�t���X�^���ɓ����Ă���V���h�E�L���X�^�[��AABB�̍ő���W�ƍŏ����W���X�V
                if (geomData->IsShadowCaster() && geomData->IsInViewFrustum())
                {
                    m_shadowCasterMaxPositionInViewFrustum.Max(geomData->GetAabbMaxPositionInWorld());
                    m_shadowCasterMinPositionInViewFrustum.Min(geomData->GetAabbMinPositionInWorld());
                    m_isBuiltShadowCasterGeometryData = true;
                }
            }

            return;
        }

	}
}