#include "stdafx.h"
#include "AABB.h"
#include "../../MiniEngine/tkFile/TkmFile.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �W�I���g���֘A�̃l�[���X�y�[�X
	*/
	namespace nsGeometry
	{
		/**
		 * @brief AABB�̏���������
		 * @param[in] vMax �ő�l
		 * @param[in] vMin �ŏ��l
		*/
		void CAABB::Init(const Vector3& vMax, const Vector3& vMin)
		{
			// �ő�l�ƍŏ��l��ێ�
			m_max = vMax;
			m_min = vMin;

			// AABB�̔����̃T�C�Y
			Vector3 halfSize = (m_max - m_min) * 0.5f;
			// AABB�̒��S���W
			Vector3 centerPos = (vMax + vMin) * 0.5f;

			// 8���_�̍��W���v�Z

			// �i���Ӂj���_�͍���nZUp�Ȃ̂ŁA����nYUp�ɒu��������

			// �����艜�̏ꏊ�̒��_
			m_vertexPosition[enVertPos_X0Y0Z1] = centerPos;
			m_vertexPosition[enVertPos_X0Y0Z1].x -= halfSize.x;
			m_vertexPosition[enVertPos_X0Y0Z1].y -= halfSize.y;
			m_vertexPosition[enVertPos_X0Y0Z1].z -= halfSize.z;

			// �E�����̏ꏊ�̒��_
			m_vertexPosition[enVertPos_X1Y0Z1] = centerPos;
			m_vertexPosition[enVertPos_X1Y0Z1].x += halfSize.x;
			m_vertexPosition[enVertPos_X1Y0Z1].y -= halfSize.y;
			m_vertexPosition[enVertPos_X1Y0Z1].z -= halfSize.z;

			// ������O�̏ꏊ�̒��_
			m_vertexPosition[enVertPos_X0Y0Z0] = centerPos;
			m_vertexPosition[enVertPos_X0Y0Z0].x -= halfSize.x;
			m_vertexPosition[enVertPos_X0Y0Z0].y += halfSize.y;
			m_vertexPosition[enVertPos_X0Y0Z0].z -= halfSize.z;

			// �E����O�̏ꏊ�̒��_
			m_vertexPosition[enVertPos_X1Y0Z0] = centerPos;
			m_vertexPosition[enVertPos_X1Y0Z0].x += halfSize.x;
			m_vertexPosition[enVertPos_X1Y0Z0].y += halfSize.y;
			m_vertexPosition[enVertPos_X1Y0Z0].z -= halfSize.z;

			// ���㉜�̏ꏊ�̒��_
			m_vertexPosition[enVertPos_X0Y1Z1] = centerPos;
			m_vertexPosition[enVertPos_X0Y1Z1].x -= halfSize.x;
			m_vertexPosition[enVertPos_X0Y1Z1].y -= halfSize.y;
			m_vertexPosition[enVertPos_X0Y1Z1].z += halfSize.z;

			// �E�㉜�̏ꏊ�̒��_
			m_vertexPosition[enVertPos_X1Y1Z1] = centerPos;
			m_vertexPosition[enVertPos_X1Y1Z1].x += halfSize.x;
			m_vertexPosition[enVertPos_X1Y1Z1].y -= halfSize.y;
			m_vertexPosition[enVertPos_X1Y1Z1].z += halfSize.z;

			// �����O�̏ꏊ�̒��_
			m_vertexPosition[enVertPos_X0Y1Z0] = centerPos;
			m_vertexPosition[enVertPos_X0Y1Z0].x -= halfSize.x;
			m_vertexPosition[enVertPos_X0Y1Z0].y += halfSize.y;
			m_vertexPosition[enVertPos_X0Y1Z0].z += halfSize.z;

			// �E���O�̏ꏊ�̒��_
			m_vertexPosition[enVertPos_X1Y1Z0] = centerPos;
			m_vertexPosition[enVertPos_X1Y1Z0].x += halfSize.x;
			m_vertexPosition[enVertPos_X1Y1Z0].y += halfSize.y;
			m_vertexPosition[enVertPos_X1Y1Z0].z += halfSize.z;

			return;
		}

		/**
		 * @brief tkm�t�@�C������AABB�̏���������
		 * @param[in] tkmFile tkm�t�@�C��
		*/
		void CAABB::InitFromTkmFile(const TkmFile& tkmFile)
		{
			// �ő���W
			Vector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			// �ŏ����W
			Vector3 vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };

			// ���b�V���p�[�c�ɑ΂��ăN�G�����s��
			tkmFile.QueryMeshParts(
				[&](const TkmFile::SMesh& mesh)
				{
					// ���_��S����
					for (const auto& vertex : mesh.vertexBuffer)
					{
						vMax.Max(vertex.pos);	// �ő���W��ݒ�
						vMin.Min(vertex.pos);	// �ŏ����W��ݒ�
					}

					return;
				}
			);

			// AABB��������
			Init(vMax, vMin);

			return;
		}

		/**
		 * @brief AABB���\������8���_�̃��[���h��Ԃł̍��W���v�Z����
		 * @param[out] pos_out 8���_�̍��W���i�[
		 * @param[in] mWorld ���[���h�s��
		*/
		void CAABB::CalcVertexPositions(Vector3 pos_out[enVertNum], const Matrix& mWorld)
		{
			// 8���_�S�Ē��ׂ�
			for (int vertNo = 0; vertNo < enVertNum; vertNo++)
			{
				// ���_�̍��W���i�[
				pos_out[vertNo] = m_vertexPosition[vertNo];
				// ���[���h�s�񂩂烏�[���h���W���v�Z
				mWorld.Apply(pos_out[vertNo]);
			}

			return;
		}
	}
}