#pragma once

namespace nsMyGame
{
	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		/**
		 * @brief �}�b�v�̒萔�f�[�^
		*/
		namespace nsMapConstData
		{
			constexpr const char* const kPlayerName = "player";				//!< �v���C���[�̖��O
			constexpr const char* const kBuildingName = "testBuilding";		//!< �����̖��O
			constexpr const char* const kGoalName = "goal";					//!< �S�[���̖��O

			/**
			 * @brief ���x���̎��
			*/
			enum EnLevelType
			{
				enLevelTest,	//!< �e�X�g�p���x��
				enLevelProto,	//!< �v���g�p���x��
				enLevelTypeNum	//!< ���x���̎�ނ̐�
			};

			/**
			 * @brief ���x���̃t�@�C���p�X
			*/
			constexpr const char* const kLevelFilePath[enLevelTypeNum]
			{
				"Assets/levelData/testLevel.tkl",
				"Assets/levelData/level_proto.tkl"
			};

		}
	}
}