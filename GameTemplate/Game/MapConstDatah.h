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
			constexpr const char* const kGoalName = "goal";					//!< �S�[���̖��O

			/**
			 * @brief ���x���̎��
			*/
			enum EnLevelType
			{
				enLevelProto,	//!< �v���g�p���x��
				enLevelProto2,	//!< �v���g�p���x��2
				enLevelProto3,	//!< �v���g�p���x��3
				enLevelAlpha,	//!< �A���t�@�p���x��
				enLevelTypeNum	//!< ���x���̎�ނ̐�
			};

			/**
			 * @brief ���x���̃t�@�C���p�X
			*/
			constexpr const char* const kLevelFilePath[enLevelTypeNum]
			{
				"Assets/levelData/level_proto.tkl",
				"Assets/levelData/level_proto2.tkl",
				"Assets/levelData/level_proto3.tkl",
				"Assets/levelData/level_alpha.tkl"
			};

		}
	}
}