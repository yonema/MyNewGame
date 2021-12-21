#pragma once

namespace nsMyGame
{
	/**
	 * @brief AI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAI
	{
		/**
		 * @brief AI�֘A�̒萔�f�[�^
		*/
		namespace nsAIConstData
		{
			//!< �i�r���b�V���̃t�@�C���p�X
			constexpr const char* const kNaviMeshFilePath = "Assets/naviMesh/NM_Ground.tkn";
			//!< �i�r���b�V���̃^�[�Q�b�g�|�C���g�p�̃��x���̃t�@�C���p�X
			constexpr const char* const kNaviMeshTargetPointLevelFilePath = "Assets/naviMesh/NMTP.tkl";
			// �E�F�C�|�C���g�̐�
			constexpr int kWayPointNum = 346;
			// �E�F�C�|�C���g�̃t�@�C���p�X
			constexpr const char* const kWayPointFilePath = "Assets/levelData/AI/WayPoint.tkl";
		}
	}
}