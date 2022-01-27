#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		/**
		 * @brief �Q�[�����C��UI�N���X�̒萔�f�[�^
		*/
		namespace nsGameMainUIConstData
		{
			//!< �^�C�}�[�̃t�H���g�̃e�L�X�g�̐擪
			constexpr const wchar_t* const kTimerFontTextHead = L"Time:";
			//!< �^�C�}�[�̃t�H���g�̃e�L�X�g�̖���
			constexpr const wchar_t* const kTimerFontTextEnd = L"%02d:%02d";
			//!< �^�C�}�[�̃t�H���g�̍��W
			static const Vector2 kTimerFontPosition = { -600.0f,300.0f };
			//!< �^�C�}�[�̃t�H���g�̃J���[
			static const Vector4 kTimerFontColor = { 1.0f,1.0f,1.0f,1.0f };
			//!< �^�C�}�[�̃t�H���g�̃V���h�E�I�t�Z�b�g
			constexpr float kTimerFontShadowOffset = 2.0f;
			//!< �^�C�}�[�̃t�H���g�̃V���h�E�J���[
			static const Vector4 kTimerFontShadowColor = { 0.0f,0.0f,0.0f,1.0f };

			//!< �S�[���̃X�v���C�g�t�@�C���p�X
			constexpr const char* const kGoalSpriteFilePath = "Assets/Image/sample.dds";
			constexpr int kGoalSpriteWidth = 256;	//!< �S�[���̃X�v���C�g�̕�
			constexpr int kGoalSpriteHeight = 256;	//!< �S�[���̃X�v���C�g�̍���
			//!< �S�[���̃X�v���C�g�̍��W
			static const Vector3 kGoalSpritePosition = { 0.0f,200.0f,0.0f };

		}

		/**
		 * @brief �~�j�}�b�v�̒萔�f�[�^
		*/
		namespace nsMiniMapConstData
		{
			constexpr float kStageWidth = 70000.0f;				//!< �X�e�[�W�̕�
			constexpr float kStageHeight = 70000.0f;			//!< �X�e�[�W�̍���
			constexpr float kPlayerCenterBuffWidth = 35000.0f;	//!< �v���C���[���Z���^�[�Ɋ񂹂邽�߂̕�
			constexpr float kPlayerCenterBuffHeight = 2800.0f;	//!< �v���C���[���Z���^�[�Ɋ񂹂邽�߂̍���

			//!< �~�j�}�b�v�̔w�i�p�X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kMiniMapBackSpriteFilePath = "Assets/Image/miniMap/map_back.DDS";
			//!< �~�j�}�b�v�̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kMiniMapSpriteFilePath = "Assets/Image/miniMap/map.DDS";
			//!< �~�j�}�b�v��FX�t�@�C���p�X
			constexpr const char* const kMiniMapFxFilePath = "Assets/shader/miniMap.fx";
			// ���̉摜�T�C�Y * �傫���{��
			constexpr int kMiniMapSpriteWidth = static_cast<int>(720 * 0.8f);	//!< �~�j�}�b�v�̕�
			constexpr int kMiniMapSpriteHeight = static_cast<int>(720 * 0.8f);	//!< �~�j�}�b�v�̍���
			// ���̉摜�T�C�Y * �A�X�y�N�g�� * �傫���{��
			constexpr int kMiniMapDrawWidth = static_cast<int>(720 * 0.4f * 0.7f);	//!< �~�j�}�b�v�̕�
			constexpr int kMiniMapDrawHeight = static_cast<int>(720 * 0.3f * 0.7f);	//!< �~�j�}�b�v�̍���
			//!< �~�j�}�b�v�̃X�v���C�g�̍��W
			static const Vector2 kMiniMapSpritePosition = { 500.0f,-250.0f };

			// �~�j�}�b�v�̘g�p�̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kMiniMapFrameSpriteFilePath = "Assets/Image/miniMap/map_waku.DDS";
			constexpr int kMiniMapFrameSpriteWidth = static_cast<int>(400 * 0.62f);	//!< �~�j�}�b�v�̘g�̕�
			constexpr int kMiniMapFrameSpriteHeight = static_cast<int>(300 * 0.62f);	//!< �~�j�}�b�v�̘g�̍���
			//!< �~�j�}�b�v�̘g�p�̃X�v���C�g�̍��W
			static const Vector2 kMiniMapFrameSpritePosition =
			{ kMiniMapSpritePosition.x + 10.0f, kMiniMapSpritePosition.y + 8.0f };

			//!< �v���C���[�̃A�C�R���̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kPlayerIconSpriteFilePath = "Assets/Image/miniMap/playerArrow.DDS";
			constexpr int kPlayerIconSpriteWidth = static_cast<int>(64 * 0.5f);	//! �v���C���[�A�C�R���̕�
			constexpr int kPlayerIconSpriteHeight = static_cast<int>(64 * 0.5f);	//! �v���C���[�A�C�R���̍���
			// �v���C���[�̃A�C�R���̃X�v���C�g�̍��W
			static const Vector2 kPlayerIconSpritePosition = kMiniMapSpritePosition;

			//!<  �Ԃ̃A�C�R���̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kCarIconSpriteFilePath = "Assets/Image/miniMap/carIcon.DDS";
			constexpr int kCarIconSpriteWidth = static_cast<int>(512 * 0.08f);	//! �ԃA�C�R���̕�
			constexpr int kCarIconSpriteHeight = static_cast<int>(512 * 0.08f);	//! �ԃA�C�R���̍���
			constexpr float kCarIconSpriteWorldPosYBuff = 100.0f;	// �Ԃ̃��[���h���W��Y���W�����炷��
			constexpr float kDisplayCarIconTime = 5.0f;	//!< �Ԃ̃A�C�R����\�����Ă�������
			//!< �ԃA�C�R���̃f�t�H���g�J���[
			static const Vector3 kCarIconDefaultColor = { 1.0f,0.0f,0.0f };
			//!< �ԃA�C�R���͈̔͊O�J���[�A�߂��o�[�W����
			static const Vector3 kCarIconOutColorNear = { 1.0f,0.3f,0.0f };
			//!< �ԃA�C�R���͈̔͊O�J���[�A�����o�[�W����
			static const Vector3 kCarIconOutColorFar = { 1.0f,1.0f,0.0f };
			constexpr float kCarIconOutMaxRage = 300.0f;	//!< �ԃA�C�R���͈̔͊O�̍ő勗��


			//!<  �Ԃ̃~�j�A�C�R���̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kCarMiniIconSpriteFilePath = "Assets/Image/miniMap/carMiniIcon.DDS";
			constexpr int kCarMiniIconSpriteWidth = static_cast<int>(64 * 0.5f);	//! �ԃ~�j�A�C�R���̕�
			constexpr int kCarMiniIconSpriteHeight = static_cast<int>(64 * 0.5f);	//! �ԃ~�j�A�C�R���̍���

			//!<  �Ԃ̃~�j�A�C�R���̉�ʂ݂͂����p�̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kCarMiniIconOutSpriteFilePath = "Assets/Image/miniMap/outOfView.DDS";
			constexpr int kCarMiniIconOutSpriteWidth = static_cast<int>(64 * 0.25f);	//! �ԃ~�j�A�C�R���̕�
			constexpr int kCarMiniIconOutSpriteHeight = static_cast<int>(64 * 0.25f);	//! �ԃ~�j�A�C�R���̍���
			constexpr float kCarMiniIconOutBuff = 10.0f;	//!< ���ʂ̎ԃA�C�R���Ƃ��炷��

			// �~�j�}�b�v��4���_
			static const Vector2 kMiniMapVert[4] =
			{
				// ����
				{
					kPlayerIconSpritePosition.x - kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y + kMiniMapDrawHeight * 0.5f
				},
				// �E��
				{
					kPlayerIconSpritePosition.x + kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y + kMiniMapDrawHeight * 0.5f
				},
				// ����
				{
					kPlayerIconSpritePosition.x - kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y - kMiniMapDrawHeight * 0.5f
				},
				// �E��
				{
					kPlayerIconSpritePosition.x + kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y - kMiniMapDrawHeight * 0.5f
				},

			};

			// �~�j�}�b�v�̕ӂ̃x�N�g��
			static Vector2 kMiniMapEdge[4] =
			{
				// ���ォ��E��B�n�_vert0�B
				{ kMiniMapVert[1].x - kMiniMapVert[0].x, kMiniMapVert[1].y - kMiniMapVert[0].y },
				// �E�ォ��E���B�n�_vert1�B
				{ kMiniMapVert[3].x - kMiniMapVert[1].x, kMiniMapVert[3].y - kMiniMapVert[1].y },
				// �������獶��B�n�_vert2�B
				{ kMiniMapVert[0].x - kMiniMapVert[2].x, kMiniMapVert[0].y - kMiniMapVert[2].y },
				// �E�����獶���B�n�_vert3�B
				{ kMiniMapVert[2].x - kMiniMapVert[3].x, kMiniMapVert[2].y - kMiniMapVert[3].y },
			};

		}

		/**
		 * @brief �G�l�~�[��߂܂��鏈���̒萔�f�[�^
		*/
		namespace nsEnemyCatchUIConstData
		{

			enum EnCatchEnemySpriteType
			{
				enCanCatchEnemyMain,
				enCanCatchEnemyFrame1,
				enCanCatchEnemyFrame2,
				enCanCatchEnemySpriteNum
			};
			//!< �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kCanCatchEnemySpriteFilePath[enCanCatchEnemySpriteNum] = 
			{
				"Assets/Image/qte/y.DDS",
				"Assets/Image/qte/canCatch_frame1.DDS",
				"Assets/Image/qte/canCatch_frame2.DDS"
			};

			//!< �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̕�
			constexpr int kCanCatchEnemySpriteWidht[enCanCatchEnemySpriteNum] = 
			{
				static_cast<int>(256 * 0.2f),
				static_cast<int>(512 * 0.2f),
				static_cast<int>(512 * 0.2f)
			};
			//!< �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̍���
			constexpr int kCanCatchEnemySpriteHeight[enCanCatchEnemySpriteNum] =
			{
				static_cast<int>(256 * 0.2f),
				static_cast<int>(512 * 0.2f),
				static_cast<int>(512 * 0.2f)
			};


			constexpr float kCanCatchTime1 = 1.5f;		//!< �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̃^�C��1
			constexpr float kCanCatchTime2 = 0.5f;		//!< �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̃^�C��2
			constexpr float kCanCatchMaxScale = 1.5f;	//!< �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̍ő�g�嗦


			//!< �^�[�Q�b�g�������X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kTargetSpriteFilePath = "Assets/Image/qte/target.DDS";
			//!< �^�[�Q�b�g�������X�v���C�g�̕�
			constexpr int kTargetSpriteWidth = static_cast<int>(300 * 0.3f);
			//!< �^�[�Q�b�g�������X�v���C�g�̍���
			constexpr int kTargetSpriteHeight = static_cast<int>(400 * 0.3f);

			constexpr float kTargetSpriteMaxScale = 1.0f;	//!< �^�[�Q�b�g�������X�v���C�g�̍ő�g�嗦
			constexpr float kTargetSpriteMinScale = 0.3f;	//!< �^�[�Q�b�g�������X�v���C�g�̍ŏ��g�嗦
			constexpr float kTargetSpritePowPower = 3.0f;	//!< �^�[�Q�b�g�������X�v���C�g�̊g�嗦�̕ω��̎w��


			//!< �^�[�Q�b�g�Ƃ̋����������t�H���g�����_���[�̍��W�̃o�b�t�@
			static const Vector2 kTargetLengthFontRenderPosBuff = { -30.0f, 10.0f };
		}

		/**
		 * @brief �~�b�V������UI�̒萔�f�[�^
		*/
		namespace nsMissionUIConstData
		{
			/**
			 * @brief �~�b�V�����̎��
			*/
			enum EnMissionType
			{
				enMT_carSilver,
				enMT_carRed,
				enMT_carBlue,
				enMT_clearTime,
				enMT_noCommandMiss,
				enMissionTypeNum
			};

			/**
			 * @brief �~�b�V�����̃��U���g�̎��
			*/
			enum EnMissionResultType
			{
				enMRT_clear,
				enMRT_great,
				enMRT_perfect,
				enMissionResultTypeNum
			};

			//!< �~�b�V�����̃��x��2D�̃t�@�C���p�X
			constexpr const char* const kMissionLevel2DFilePath = "Assets/level2DData/mission.casl";
			constexpr const char* const kMissionLevelObjName = "mission";	//!< �~�b�V�����̃��x���I�u�W�F�N�g�l�[��
			//!< �`�F�b�N�}�[�N�̃��x���I�u�W�F�N�g�l�[��
			constexpr const char* const kCheckMarkLevelObjNames[enMissionTypeNum] =
			{
				"mission_checkMark_carSilver",
				"mission_checkMark_carRed",
				"mission_checkMark_carBlue",
				"mission_checkMark_clearTime",
				"mission_checkMark_noCommandMiss"
			};

			//!< �~�b�V�����̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kMissionSpriteFilePath = "Assets/Image/mission/mission.DDS";
			//!< �`�F�b�N�}�[�N�̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kCheckMarkSpriteFilePath = "Assets/Image/mission/mission_checkMark.DDS";

			//!< �~�b�V�������U���g�̘g�̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kMissionResultFrameFilePath = "Assets/Image/mission/mission_result_frame.DDS";
			//!< �~�b�V�����U���g�̘g�̃X�v���C�g�̕�
			constexpr int kMissionResultFrameSpriteWidth = static_cast<int>(512.0f * 1.0f);
			//!< �~�b�V�����U���g�̘g�̃X�v���C�g�̍���
			constexpr int kMissionResultFrameSpriteHeight = static_cast<int>(512.0f * 1.0f);
			//!< �~�b�V�������U���g�̃e�L�X�g�̃��x���I�u�W�F�N�g�l�[��
			constexpr const char* const kMissionResultTextSpriteFilepath[enMissionResultTypeNum] =
			{
				"Assets/Image/mission/mission_result_clear.DDS",
				"Assets/Image/mission/mission_result_great.DDS",
				"Assets/Image/mission/mission_result_perfect.DDS"
			};
			//!< �~�b�V�����U���g�̃e�L�X�g�̃X�v���C�g�̕�
			constexpr int kMissionResultTextSpriteWidth = static_cast<int>(512.0f * 1.0f);
			//!< �~�b�V�����U���g�̃e�L�X�g�̃X�v���C�g�̍���
			constexpr int kMissionResultTextSpriteHeight = static_cast<int>(512.0f * 1.0f);
		}
	}
}