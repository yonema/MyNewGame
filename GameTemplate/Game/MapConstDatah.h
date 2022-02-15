#pragma once

namespace nsNinjaAttract
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
				enLevelAlpha2,	//!< �A���t�@2�p���x��
				enLevelPlateAndBuildings,	//!< �n�ʂƌ����p���x��
				enLevelPlayer,	//!< �v���C���[�p���x��
				enLevelCar,		//!< �ԗp���x��
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
				"Assets/levelData/level_alpha.tkl",
				"Assets/levelData/level_alpha2.tkl",
				"Assets/levelData/plateAndBuildings.tkl",
				"Assets/levelData/playerLevel.tkl",
				"Assets/levelData/carLevel.tkl"
			};

			/**
			 * @brief �����̎��
			*/
			enum EnPropsType
			{
				enPropsStreetLight,			//!< �X���p�̃��x��
				enPropsTrafficLight,		//!< �M���@�p�̃��x��
				enPropsPedestrianLight,		//!< ���s�ҐM���@�p�̃��x��
				enPropsStreetTree,			//!< �X�H���p�̃��x��
				enPropsStreetTreeBranch,	//!< �X�H���̎}�p�̃��x��
				enPropsTypeNum,				//!< �����̎�ނ̐�
			};

			/**
			 * @brief �����̃��x���̃t�@�C���p�X
			*/
			constexpr const char* const kPropsLevelFilePath[enPropsTypeNum] =
			{
				"Assets/levelData/level_StreetLight.tkl",
				"Assets/levelData/level_TrafficLight.tkl",
				"Assets/levelData/level_PedestrianLight.tkl",
				"Assets/levelData/level_StreetTree.tkl",
				"Assets/levelData/level_StreetTree_Branch.tkl"
			};

			//!< ������LOD�p�̃��f���̃t�@�C���p�X
			constexpr const char* const kPropsLODModelFilePath[enPropsTypeNum] =
			{
				"Assets/modelData/levelSource/StreetLight_LOD.tkm",
				"Assets/modelData/levelSource/TrafficLight_LOD.tkm",
				nullptr,
				"Assets/modelData/levelSource/StreetTree_LOD.tkm",
				"Assets/modelData/levelSource/StreetTree_Branch_LOD.tkm"
			};

			//!< ������LOD�p�̋���
			constexpr float kPropsLODDistance[enPropsTypeNum] =
			{
				1000.0f,
				1000.0f,
				-1.0f,
				-1.0f,
				10000.0f
			};

			//!< �}�b�v�`�b�v�̗\�񐔃e�[�u��
			constexpr int kNumMapChipReserveTbl[enPropsTypeNum] =
			{
				604,
				146,
				143,
				458,
				458
			};

			//!< �u���b�L���O�{�����[���̍��W
			static const Vector3 kBlockingVolumePosition[4]
			{
				{0.0f,0.0f,-3300.0f},
				{35500.0f,0.0,32200.0f},
				{0.0f,0.0f,67700.0f},
				{-35000.0f,0.0,32200.0f},
			};

			//!< �u���b�L���O�{�����[���̊g�嗦
			static const Vector3 kBlockingVolumeScale = { 70000.0f + 1000.0f,20000.0f,1000.0f };

			//!< �^�C�g�����̃v���C���[�̃��x����̖��O
			constexpr const char* const kPlayerLevelNameInTitle = "player_inTitle";
			//!< �Q�[�����̃v���C���[�̃��x����̖��O
			constexpr const char* const kPlayerLevelNameInGame = "player_inGame";

			//!< ���艹�̃T�E���h�̃t�@�C���p�X
			constexpr const wchar_t* const kDecisionSoundFilePath = L"Assets/sound/decision.wav";

			//!< �^�C�g���̃X�v���C�g�̃t�@�C�X�p�X
			constexpr const char* const kTitleSpriteFilePath = "Assets/Image/title/title.DDS";
			//!< �X�^�[�g�̃X�v���C�g�̃t�@�C�X�p�X
			constexpr const char* const kTitleStartSpriteFilePath = "Assets/Image/title/title_start.DDS";
			//!< �^�C�g���̃X�v���C�g�̕�
			constexpr int kTitleSpriteWidth = static_cast<int>(512.0f + 1.5f);
			//!< �^�C�g���̃X�v���C�g�̍���
			constexpr int kTitleSpriteHeight = static_cast<int>(512.0f + 1.5f);
			//!< �X�^�[�g�̃X�v���C�g�̕�
			constexpr int kTitleStartSpriteWidth = static_cast<int>(512.0f + 0.4f);
			//!< �X�^�[�g�̃X�v���C�g�̍���
			constexpr int kTitleStartSpriteHeight = static_cast<int>(512.0f + 0.4f);
			//!< �^�C�g���̃X�v���C�g�̍��W
			static const Vector2 kTitleSpritePosition = { -200.0f,100.0f };
			//!< �X�^�[�g�̃X�v���C�g�̍��W
			static const Vector2 kTitleStartSpritePosition = { kTitleSpritePosition.x,kTitleSpritePosition .y - 100.0f};
			//!< �^�C�g���̃X�v���C�g�̍ŏ��̍���
			constexpr float kTitleSpriteInitialHeight = 50.0f;

			/**
			 * @brief �^�C�g���̃X�e�[�g
			*/
			enum EnTitleState
			{
				enTS_beforeFadeIn,
				enTS_fadeIn,
				enTS_titleIn,
				enTS_titleIdle,
				enTS_titleOut,
				enTS_swingDirecting,
				enTS_fadeOut
			};

			constexpr float kTitleInTime = 1.0f;		//!< �^�C�g���C���̃^�C��
			constexpr float kTitleIdleLoopTime = 5.0f;	//!< �^�C�g���A�C�h���̃��[�v�^�C��
			constexpr float kTitleOutTime = 1.0f;		//!< �^�C�g���A�E�g�̃^�C��

			constexpr float kStartDirectingTime = 4.0f;				//!< �J�n���o�̃^�C��
			constexpr float kFadeOutToClearDirectingTime = 0.5f;	//!< �N���A���o�̑O�̃t�F�[�h�A�E�g�̃^�C��
			constexpr float kFadeInToClearDirectingTime = 0.5f;		//!< �N���A���o�̑O�̃t�F�[�h�C���̃^�C��
			constexpr float kFadeOutGoTitleMinActiveTime = 1.5f;	//!< �^�C�g���֑J�ڂ��邽�߂̃t�F�[�h�A�E�g�̍ŏ��L���^�C��
			constexpr float kFadeOutGoTitleMaxActiveTime = 1.7f;	//!< �^�C�g���֑J�ڂ��邽�߂̃t�F�[�h�A�E�g�̍ő�L���^�C��
			constexpr float kFadeOutGoTitleTime = 0.5f;				//!< �^�C�g���֑J�ڂ��邽�߂̃t�F�[�h�A�E�g�̃^�C��
			constexpr float kGoTitleDirectingTime = 2.5f;			//< �^�C�g���֍s�����߂̉��o�̃^�C��
		}
	}
}