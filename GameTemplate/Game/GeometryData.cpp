#include "stdafx.h"
#include "GeometryData.h"
#include "ModelRender.h"
#include "RenderingEngine.h"

namespace nsMyGame
{

	/**
	 * @brief �W�I���g���֘A�̃l�[���X�y�[�X
	*/
	namespace nsGeometry
	{
		/**
		 * @brief ������
		 * @param[in] modelRender ���f�������_���[
		 * @param[in] instanceId �C���X�^���XID
		*/
		void CGeometryData::Init(const nsGraphic::nsModel::CModelRender* modelRender, int instanceId)
		{
			// ���̃W�I���g�����Ɗ֘A�Â��Ă��郂�f�������_���[��ێ�
			m_modelRender = modelRender;
			// �C���X�^���XID��ێ�
			m_instanceId = instanceId;

			// tkm�t�@�C�����烂�f���̍ŏ����W�ƍő���W�𒲂ׂ�B
			Vector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			Vector3 vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };

			const auto& tkmFile = modelRender->GetModel().GetTkmFile();
			tkmFile->QueryMeshParts([&](const TkmFile::SMesh& mesh) {
				for (const auto& vertex : mesh.vertexBuffer) {
					vMax.Max(vertex.pos);
					vMin.Min(vertex.pos);
				}
				});

			// �ő���W�ƍŏ����W����AABB��������
			m_aabb.Init(vMax, vMin);
		}

		/**
		 * @brief �W�I���g�������X�V
		*/
		void CGeometryData::Update()
		{
			// �C���X�^���XID���w�肵�āA���[���h�s��𓾂�
			Matrix worldMatrix = m_modelRender->GetWorldMatrix(m_instanceId);
			// AABB���\������8���_�̃��[���h��Ԃł̍��W���v�Z����
			m_aabb.CalcVertexPositions(
				m_aabbWorldVertexPositions,
				worldMatrix
			);
			
			// �r���[�J�����O�̂��߂̃r���[�v���W�F�N�V�����s����擾
			const auto& viewProjMatrix = nsMyEngine::CRenderingEngine::GetInstance()->GetViewProjectionMatrixForViewCulling();

			// AABB��Ԃł̍ő�l�ƍŏ��l
			m_aabbMax = Vector3::MinV;
			m_aabbMin = Vector3::MaxV;

			// �r���[�v���W�F�N�V������Ԃł̍ő�l�ƍŏ��l
			Vector4 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };
			Vector4 vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX, FLT_MAX };

			// �r���[�t���X�^���̒��ɉf���Ă��邩�H
			m_isInViewFrustum = false;

			// AABB�̒��_�𒲂ׂ�
			for (Vector4 v : m_aabbWorldVertexPositions) {

				// AABB��Ԃł̍ő�l�ƍŏ��l�𒲂ׂ�
				m_aabbMax.Max({ v.x, v.y, v.z });
				m_aabbMin.Min({ v.x, v.y, v.z });

				// �r���[�v���W�F�N�V������Ԃł̍ő�l�ƍŏ��l�𒲂ׂ�
				// w�Ŋ����Đ��K������
				viewProjMatrix.Apply(v);
				v.x /= fabsf(v.w);
				v.y /= fabsf(v.w);
				v.z /= fabsf(v.w);

				vMax.Max(v);
				vMin.Min(v);
			}

			// �r���[�t���X�^���͈͓̔��Ɉڂ��Ă��邩���ׂ�
			if (vMax.x > -1.0f
				&& vMax.y > -1.0f
				&& vMax.z > 0.0f
				&& vMin.x < 1.0f
				&& vMin.y < 1.0f
				&& vMin.z < 1.0f
				) 
			{
				//��ʂɉf��B
				m_isInViewFrustum = true;
			}

			return;
		}

		/**
		 * @brief �V���h�E�L���X�^�[�̊􉽊w�f�[�^�H���擾
		 * @return �V���h�E�L���X�^�[�̊􉽊w�f�[�^�H
		*/
		bool CGeometryData::IsShadowCaster() const
		{
			return m_modelRender->IsShadowCaster();
		}


	}
}