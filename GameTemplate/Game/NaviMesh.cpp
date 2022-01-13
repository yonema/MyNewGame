#include "stdafx.h"
#include "NaviMesh.h"

#include "TknFile.h"

namespace nsNinjaAttract
{
	/**
	 * @brief AI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAI
	{
		/**
		 * @brief tkn�t�@�C������i�r�Q�[�V�������b�V�����\�z����
		 * @param tknFilePath tkn�t�@�C���̃t�@�C���p�X
		*/
		void CNaviMesh::Init(const char* tknFilePath)
		{
			CTknFile tknFile;
			tknFile.Load(tknFilePath);
			// tkn�t�@�C������Z�������\�z����B
			int numCell = tknFile.GetNumCell();
			m_cellArray.resize(numCell);
			// �Z���̔z����\�z����B
			for (int cellNo = 0; cellNo < numCell; cellNo++) {
				const auto& cellLow = tknFile.GetCell(cellNo);
				// ���_�f�[�^��ݒ肷��B
				m_cellArray[cellNo].SetVertexPosition(0, cellLow.vertexPosition[0]);
				m_cellArray[cellNo].SetVertexPosition(1, cellLow.vertexPosition[1]);
				m_cellArray[cellNo].SetVertexPosition(2, cellLow.vertexPosition[2]);
				// �@����ݒ肷��B
				m_cellArray[cellNo].SetNormal(cellLow.normal);
				// ���S���W���v�Z����B
				auto centerPos = cellLow.vertexPosition[0]
					+ cellLow.vertexPosition[1]
					+ cellLow.vertexPosition[2];
				centerPos /= 3.0f;
				m_cellArray[cellNo].SetCenterPosition(centerPos);
				// �אڃZ������ݒ肷��B
				for (int linkNo = 0; linkNo < 3; linkNo++) {
					if (cellLow.linkCellNo[linkNo] != -1) {
						m_cellArray[cellNo].SetLinkCell(linkNo, &m_cellArray[cellLow.linkCellNo[linkNo]]);
					}
					else {
						m_cellArray[cellNo].SetLinkCell(linkNo, nullptr);
					}
				}
				m_cellArray[cellNo].SetCellNo(cellNo);
			}
			// �Z���̒��S���W�𗘗p����BSP�c���[���\�z����B
			for (auto& cell : m_cellArray) {
				// ���[�t��ǉ��B
				m_cellCenterPosBSP.AddLeaf(
					cell.GetCenterPosition(),
					&cell
				);
			}
			m_cellCenterPosBSP.Build();
		}

		/**
		 * @brief �w�肵�����W�ɍł��߂��Z������������
		 * @param[in] pos �w����W
		 * @return �ł��߂��Z��
		*/
		const CCell& CNaviMesh::FindNearestCell(const Vector3& pos) const
		{
			const CCell* nearestCell = nullptr;

			float dist = FLT_MAX;
			m_cellCenterPosBSP.WalkTree(pos, [&](nsGeometry::CBSP::SLeaf* leaf) {
				CCell* cell = static_cast<CCell*>(leaf->extraData);
				auto distTmp = (cell->GetCenterPosition() - pos).Length();
				if (distTmp < dist) {
					//������̕����߂��B
					dist = distTmp;
					nearestCell = cell;
				}
				});
			return *nearestCell;
		}
	}
}